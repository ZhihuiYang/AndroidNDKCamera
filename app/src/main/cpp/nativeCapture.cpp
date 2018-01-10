#include "nativeCapture.h"

Capture * gCapture = NULL;

void onSurfaceCreated(int width, int height){
    if (gCapture == NULL) {
        gCapture = Capture_create();
    }
    Capture_open(gCapture, width, height);
}


void onDrawFrame(){
    AVFrame * pFrame = NULL;
    if(gCapture == NULL){
        return;
    }

    if(gCapture->frameQueue != NULL) {
        gCapture->frameQueue->deQueue(gCapture->frameQueue, (void **) &pFrame);
        if (pFrame != NULL) {
            if(gCapture->yuvRender != NULL) {
                gCapture->yuvRender->renderFrame(gCapture->yuvRender, pFrame);
            }
            freeAVFrame(&pFrame);
        }
    }
}

void OnSurfaceDestroyed(){
    if(gCapture == NULL){
        return;
    }

    Capture_close(gCapture);
    gCapture = NULL;

}

void OnImageCallback(void *ctx, AImageReader *reader){
    ImageReader *imageReader = (ImageReader *)ctx;

    int format;
    media_status_t status = AImageReader_getFormat(reader, &format);
    if(status != AMEDIA_OK){
        LOG_ERROR("get image format failed.\n");
        return;
    }

    if(format == AIMAGE_FORMAT_YUV_420_888) {
        AImage *image = NULL;
        status = AImageReader_acquireLatestImage(reader, &image);
        if (status != AMEDIA_OK) {
            LOG_ERROR("acquire next image failed.\n");
            return;
        }

        int width = 0, height = 0;
        int64_t pts;
        AImage_getHeight(image, &height);
        AImage_getWidth(image,&width);

        //render yuv to surfaceView. use openGL es
        AImage_getPlaneData(image, 0, &imageReader->buffer, &imageReader->yLen);
        imageReader->y = imageReader->buffer;
        imageReader->u = imageReader->y + imageReader->yLen;
        AImage_getPlaneData(image, 1, &imageReader->u, &imageReader->uLen);
        imageReader->v = imageReader->u + imageReader->uLen+1;
        AImage_getPlaneData(image, 2, &imageReader->v , &imageReader->vLen);
        AImage_getTimestamp(image, &pts);



        AVFrame * frame = (AVFrame *) malloc(sizeof(AVFrame));
        memset(frame, 0, sizeof(AVFrame));
        frame->data[0] = (unsigned char *) malloc(width * height * 3 / 2);
        memset(frame->data[0], 0, width * height * 3 / 2);
        frame->width = width;
        frame->height = height;

        memcpy(frame->data[0],imageReader->y, imageReader->yLen);
        frame->dataSize[0] = imageReader->yLen;
        frame->data[1]  = frame->data[0] + imageReader->yLen;
        memcpy(frame->data[1],imageReader->v, imageReader->yLen/2);
        frame->dataSize[1]  = frame->dataSize[0]/2;

        if(frame != NULL){
            gCapture->frameQueue->enQueue(gCapture->frameQueue, frame);
        }
        AImage_delete(image);
    }
}

Capture *Capture_create(){
    Capture * capture = NULL;
    capture = (Capture *)malloc(sizeof(Capture));
    memset(capture, 0 , sizeof(Capture));

    capture->camera = NativeCamera_create();
    if(capture->camera == NULL){
        LOG_ERROR("Failed to create native camera.\n");
    }

    capture->yuvRender = createYUVRender();
    capture->yuvRender->initialize(capture->yuvRender);

    capture->frameQueue = generateLinkedQueue();
    gCapture = capture;
    return capture;
}

void Capture_open(Capture *capture, int width, int height) {
    if (capture == NULL) {
        LOG_ERROR("capture is NULL, please create it first.\n");
        return;
    }
    ImageFormat viewFormat;
    viewFormat.width = width;
    viewFormat.height = height;
    NativeCamera *camera = capture->camera;
    ImageReader *imageReader;
    ANativeWindow *readerWindow;

    imageReader = ImageReader_create(viewFormat.width, viewFormat.height, AIMAGE_FORMAT_YUV_420_888);
    if(!imageReader){
        LOG_ERROR("Failed to create imagereader\n");
        return;
    }
    capture->imageReader = imageReader;

    ImageReader_registerImageCallback(imageReader, OnImageCallback);
    AImageReader_getWindow(imageReader->reader,&readerWindow);


    NativeCamera_openCamera(camera);
    NativeCamera_setSurface(camera, readerWindow);
}

void Capture_close(Capture *capture){
    if(capture == NULL){
        return;
    }

    NativeCamera *camera = capture->camera;
    if(camera != NULL){
        NativeCamera_closeCamera(camera);
        NativeCamera_shutdown(camera);
        free(camera);
    }

    ImageReader *imageReader = capture->imageReader;
    if(imageReader != NULL){
        ImageReader_release(imageReader);
        free(imageReader);
    }

    YUVRender *yuvRender = capture->yuvRender;
    if(yuvRender != NULL){
        releaseYUVRender(&yuvRender);
    }


}