#include <jni.h>
#include "logger.h"
#include "nativeCamera.h"

static void camera_device_on_disconnected(void *context, ACameraDevice *device) {
    LOG_INFO("Camera(id: %s) is diconnected.\n", ACameraDevice_getId(device));
}

static void camera_device_on_error(void *context, ACameraDevice *device, int error) {
    LOG_ERROR("Error(code: %d) on Camera(id: %s).\n", error, ACameraDevice_getId(device));
}

static void capture_session_on_ready(void *context, ACameraCaptureSession *session) {
    LOG_INFO("Session is ready.\n");
}

static void capture_session_on_active(void *context, ACameraCaptureSession *session) {
    LOG_INFO("Session is activated.\n");
}


void NativeCamera_openCamera(NativeCamera * NCamera) {

    if(NCamera == NULL){
        LOG_ERROR("NCamera is NULL.\n");
        return;
    }

    ACameraIdList *cameraIdList = NULL;
    ACameraMetadata *cameraMetadata = NULL;

    const char *selectedCameraId = NULL;
    camera_status_t camera_status = ACAMERA_OK;
    ACameraManager *cameraManager = ACameraManager_create();

    camera_status = ACameraManager_getCameraIdList(cameraManager, &cameraIdList);
    if(camera_status != ACAMERA_OK){
        LOG_ERROR("Failed to get camera id list: %d\n", camera_status);
        return;
    }
    NCamera->manager = cameraManager;

    if(cameraIdList->numCameras < 1){
        LOG_ERROR("No camera device been detected\n");
        return;
    }

    selectedCameraId = cameraIdList->cameraIds[0];
    NCamera->selectedCameraId = selectedCameraId;

    LOG_INFO("Trying to open camera2 (id: %s, num of camera: %d)", selectedCameraId, cameraIdList->numCameras);

    camera_status = ACameraManager_getCameraCharacteristics(cameraManager, selectedCameraId, &cameraMetadata);
    if(camera_status != ACAMERA_OK){
        LOG_ERROR("Failed to get camera meta data of ID: %s\n", selectedCameraId);
    }


    NCamera->deviceStateCallback.onDisconnected = camera_device_on_disconnected;
    NCamera->deviceStateCallback.onError = camera_device_on_error;

    camera_status = ACameraManager_openCamera(cameraManager, selectedCameraId, &NCamera->deviceStateCallback, &NCamera->cameraDevice);
    if(camera_status != ACAMERA_OK){
        LOG_ERROR("Failed to open camera device (id: %s)\n",selectedCameraId);
        return;
    }

    camera_status = ACameraDevice_createCaptureRequest(NCamera->cameraDevice, TEMPLATE_PREVIEW, &NCamera->captureRequest);
    if(camera_status != ACAMERA_OK){
        LOG_ERROR("Failed to create preview capture request (id: %s)\n",selectedCameraId);
        ACameraDevice_close(NCamera->cameraDevice);
        return;
    }

    camera_status = ACaptureSessionOutputContainer_create(&NCamera->captureSessionOutputContainer);
    if(camera_status != ACAMERA_OK){
        LOG_ERROR("Failed to create capture session output container (id: %s)\n",selectedCameraId);
        ACaptureRequest_free(NCamera->captureRequest);
        ACameraDevice_close(NCamera->cameraDevice);
        return;
    }

    ACameraMetadata_free(cameraMetadata);
    ACameraManager_deleteCameraIdList(cameraIdList);
    ACameraManager_delete(cameraManager);
}



void NativeCamera_closeCamera(NativeCamera * NCamera){
    if(NCamera == NULL){
        LOG_ERROR("NCamera is NULL.\n");
        return;
    }

    ACameraDevice *cameraDevice = NCamera->cameraDevice;
    ACaptureRequest **captureRequest = &NCamera->captureRequest;
    ACameraOutputTarget ** cameraOutputTarget = &NCamera->cameraOutputTarget;
    ACaptureSessionOutputContainer ** captureSessionOutputContainer = &NCamera->captureSessionOutputContainer;
    camera_status_t camera_status = ACAMERA_OK;

    camera_status = ACameraDevice_close(cameraDevice);
    if(camera_status != ACAMERA_OK){
        LOG_ERROR("Failed to close camera device.\n");
        return;
    }

    if(*captureRequest != NULL){
        ACaptureRequest_free(*captureRequest);
        *captureRequest = NULL;
    }

    if(*cameraOutputTarget != NULL){
        ACameraOutputTarget_free(*cameraOutputTarget);
        *cameraOutputTarget = NULL;
    }

    if(*captureSessionOutputContainer != NULL){
        ACaptureSessionOutputContainer_free(*captureSessionOutputContainer);
        *captureSessionOutputContainer = NULL;
    }

    ACameraManager_delete(NCamera->manager);

    LOG_INFO("camera closed\n");
}



void NativeCamera_shutdown(NativeCamera * NCamera){
    if(NCamera == NULL){
        LOG_ERROR("NCamera is NULL.\n");
        return;
    }
    ANativeWindow **pWindow = &NCamera->window;
    if(*pWindow != NULL){
        ANativeWindow_release(*pWindow);
        *pWindow = NULL;
    }
}

void NativeCamera_setSurface(NativeCamera * NCamera, ANativeWindow *win) {
    if(NCamera == NULL){
        LOG_ERROR("NCamera is NULL.\n");
        return;
    }

    if(win == NULL){
        LOG_ERROR("win is NULL.\n");
        return;
    }


    NCamera->window = win;
    LOG_INFO("Surface is prepared in %p.\n",win);
    if(NCamera->cameraOutputTarget != NULL){
        ACameraOutputTarget_free(NCamera->cameraOutputTarget);
        NCamera->cameraOutputTarget = NULL;
    }

    ACameraOutputTarget_create(NCamera->window, &NCamera->cameraOutputTarget);
    ACaptureRequest_addTarget(NCamera->captureRequest, NCamera->cameraOutputTarget);

    if(NCamera->sessionOutput != NULL){
        ACaptureSessionOutputContainer_remove(NCamera->captureSessionOutputContainer, NCamera->sessionOutput);
        ACaptureSessionOutput_free(NCamera->sessionOutput);
        NCamera->sessionOutput = NULL;
    }

    ACaptureSessionOutput_create(NCamera->window, &NCamera->sessionOutput);
    ACaptureSessionOutputContainer_add(NCamera->captureSessionOutputContainer, NCamera->sessionOutput);

    NCamera->cameraCaptureSessionStateCallback.onReady = capture_session_on_ready;
    NCamera->cameraCaptureSessionStateCallback.onActive = capture_session_on_active;

    ACameraDevice_createCaptureSession(NCamera->cameraDevice, NCamera->captureSessionOutputContainer, &NCamera->cameraCaptureSessionStateCallback, & NCamera->captureSession);

    ACameraCaptureSession_setRepeatingRequest(NCamera->captureSession, NULL, 1, &NCamera->captureRequest, NULL);
}

NativeCamera *NativeCamera_create(){
    NativeCamera *nativeCamera = NULL;
    nativeCamera = (NativeCamera *)malloc(sizeof(NativeCamera));
    memset(nativeCamera,0,sizeof(NativeCamera));
    return nativeCamera;
}
