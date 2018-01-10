
#ifndef HILIVE_NATIVEIMAGEREADER_H
#define HILIVE_NATIVEIMAGEREADER_H

#include <media/NdkImageReader.h>
#include <media/NdkImage.h>

typedef void (*ImageCallback)(void *ctx, AImageReader *reader);

typedef struct _ImageReader{
    int prsentRotation;
    AImageReader *reader;
    int imageWidth;
    int imageHeight;
    uint8_t * buffer;
    uint8_t *y;
    uint8_t *u;
    uint8_t *v;
    int yLen; int uLen; int vLen;
    int yStride; int  uStride; int vStride;
}ImageReader, pImageReader;

ImageReader *ImageReader_create(int imageWidth, int imageHeight, AIMAGE_FORMATS formats);

void ImageReader_registerImageCallback(ImageReader *imageReader, ImageCallback imageCallback);

ANativeWindow *ImageReader_getNativeWindow(ImageReader * imageReader);

void ImageReader_release(ImageReader *imageReader);


#endif //HILIVE_NATIVEIMAGEREADER_H
