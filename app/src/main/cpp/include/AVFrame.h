

#ifndef AVFRAME_H_
#define AVFRAME_H_

#define DATA_NUMBER 3
typedef struct _AvFrame
{
    unsigned long pts;
    unsigned long dts;
    unsigned char * data[DATA_NUMBER];
    int dataSize[DATA_NUMBER];
    int width;
    int height;
    int stride[DATA_NUMBER];
    int format;        //PixelFormat
    int key_frame;
    int targetbitrate;
    int maxbitrate;
}AVFrame;

AVFrame * createFrame(int size_y, unsigned char * data_y, int size_u,
        unsigned char * data_u, int size_v, unsigned char * data_v, int width,
        int height, int stride_y, int stride_u, int stride_v);

void freeAVFrame(AVFrame ** ppFrame);

#endif /* AVFRAME_H_ */
