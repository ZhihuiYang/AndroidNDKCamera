#include <media/NdkMediaError.h>
#include <media/NdkImageReader.h>
#include "nativeImageReader.h"
#ifdef DEBUG_MEM
#include "include/mymalloc.h"
#else
#include <malloc.h>
#endif
#include <string.h>
#include "logger.h"



ImageReader *ImageReader_create(int imageWidth, int imageHeight, AIMAGE_FORMATS format){
    ImageReader *imageReader = (ImageReader *)malloc(sizeof(ImageReader));
    memset(imageReader,0, sizeof(ImageReader));
    media_status_t status = AImageReader_new(imageWidth,imageHeight, format, 2, &imageReader->reader);
    if(imageReader->reader == NULL || status != AMEDIA_OK){
        LOG_ERROR("Failed to create AImageReader.\n");
        free(imageReader);
        return NULL;
    }

    int max_buffer_size = imageWidth * imageHeight *4;
    imageReader->buffer = (uint8_t *)malloc(max_buffer_size);
    memset(imageReader->buffer, 0 , max_buffer_size);
    return imageReader;
}

void ImageReader_registerImageCallback(ImageReader *imageReader, ImageCallback imageCallback){
    if(imageReader == NULL || imageCallback == NULL){
        LOG_ERROR("parameter invalid.\n");
        return;
    }
    AImageReader_ImageListener listener;
    listener.context = imageReader;
    listener.onImageAvailable = imageCallback;
    AImageReader_setImageListener(imageReader->reader, &listener);
}

ANativeWindow *ImageReader_getNativeWindow(ImageReader * imageReader){
    ANativeWindow *nativeWindow = NULL;
    AImageReader_getWindow(imageReader->reader, &nativeWindow);
    return nativeWindow;
}

void ImageReader_release(ImageReader *imageReader){
    if(imageReader == NULL){
        return;
    }

    if(imageReader->buffer != NULL){
        free(imageReader->buffer);
    }

    if(imageReader->reader != NULL){
        AImageReader_delete(imageReader->reader);
    }

    free(imageReader);
}