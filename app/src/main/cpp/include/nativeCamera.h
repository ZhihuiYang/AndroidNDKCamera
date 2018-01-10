
#ifndef HILIVE_NATIVECAMERA2_H_H
#define HILIVE_NATIVECAMERA2_H_H


#include <malloc.h>
#include <string.h>
#include <android/native_window_jni.h>

#include <camera/NdkCameraDevice.h>
#include <camera/NdkCameraManager.h>
#include "util.h"

typedef struct _NativeCamera{
    ANativeWindow *window;
    ACameraDevice *cameraDevice;
    ACameraManager *manager;
    ACaptureRequest *captureRequest;
    ACameraOutputTarget *cameraOutputTarget;
    ACaptureSessionOutput *sessionOutput;
    ACaptureSessionOutputContainer * captureSessionOutputContainer;
    ACameraCaptureSession *captureSession;
    const char *selectedCameraId;
    ACameraDevice_stateCallbacks deviceStateCallback;
    ACameraCaptureSession_stateCallbacks cameraCaptureSessionStateCallback;
}NativeCamera, *pNativeCamera;


NativeCamera *NativeCamera_create();

void NativeCamera_openCamera(NativeCamera * NCamera);

void NativeCamera_closeCamera(NativeCamera * NCamera);

void NativeCamera_shutdown(NativeCamera * NCamera);

void NativeCamera_setSurface(NativeCamera * NCamera, ANativeWindow *win);


#endif //HILIVE_NATIVECAMERA2_H_H
