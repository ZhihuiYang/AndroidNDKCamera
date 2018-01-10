

#ifndef HILIVE_NATIVEPUBLISHER_H
#define HILIVE_NATIVEPUBLISHER_H

#include "nativeCamera.h"
#include "nativeImageReader.h"
#include <malloc.h>
#include <string.h>
#include "YUVRender.h"
#include "linkedQueue.h"

typedef struct _Capture{
    NativeCamera *camera;
    ImageReader *imageReader;
    YUVRender *yuvRender;
    LinkedQueue *frameQueue;
}Capture;

Capture *Capture_create();
void Capture_open(Capture *capture, int width, int height);
void Capture_close(Capture *capture);

void onSurfaceCreated(int width, int height);
void onDrawFrame();
void OnSurfaceDestroyed();
#endif //HILIVE_NATIVEPUBLISHER_H
