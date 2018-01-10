#include "include/AVFrame.h"
#include <string.h>
#ifdef DEBUG_MEM
#include "include/mymalloc.h"
#else
#include <malloc.h>
#endif

AVFrame * createFrame(int size_y, unsigned char * data_y, int size_u,
        unsigned char * data_u, int size_v, unsigned char * data_v, int width,
        int height, int stride_y, int stride_u, int stride_v)
{
    if (size_y < 1 || size_u < 1 || size_v < 1)
    {
        return NULL;
    }
    
    AVFrame * frame = (AVFrame *) malloc(sizeof(AVFrame));
    memset(frame, 0, sizeof(AVFrame));
    frame->data[0] = (unsigned char *) malloc(width * height * 3 / 2);
    memset(frame->data[0], 0, width * height * 3 / 2);
    frame->dataSize[0] = width * height;
    frame->data[1] = frame->data[0] + frame->dataSize[0];
    frame->dataSize[1] = width * height / 4;
    frame->data[2] = frame->data[1] + frame->dataSize[1];
    frame->dataSize[2] = frame->dataSize[1];
    int offset1 = 0;
    int offset2 = 0;
    for (int i = 0; i < height; i++)
    {
        memcpy(frame->data[0] + offset1, data_y + offset2, width);
        offset1 += width;
        offset2 += stride_y;
    }
    offset1 = 0;
    offset2 = 0;
    for (int i = 0; i < height / 2; i++)
    {
        memcpy(frame->data[1] + offset1, data_u + offset2, width / 2);
        offset1 += width / 2;
        offset2 += stride_u;
    }
    offset1 = 0;
    offset2 = 0;
    for (int i = 0; i < height / 2; i++)
    {
        memcpy(frame->data[2] + offset1, data_v + offset2, width / 2);
        offset1 += width / 2;
        offset2 += stride_v;
    }
    frame->width = width;
    frame->height = height;
    frame->stride[0] = width;
    frame->stride[1] = width / 2;
    frame->stride[2] = frame->stride[1];
    
    return frame;
    
}

void freeAVFrame(AVFrame ** ppFrame)
{
    if (NULL == ppFrame || NULL == *ppFrame)
    {
        return;
    }
    AVFrame * pFrame = *ppFrame;
    if (NULL != pFrame->data[0])
    {
        free(pFrame->data[0]);
    }
    free(pFrame);
    pFrame = NULL;
}
