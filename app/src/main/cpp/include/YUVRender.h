#ifndef VIDEORENDER2_H_
#define VIDEORENDER2_H_

#include <pthread.h>
#include <android/native_window.h>
#include <android/asset_manager_jni.h>
#include <android/asset_manager.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>



typedef struct _yuvRender
{
    pthread_mutex_t mMutex;
    EGLDisplay mDisplay;
    EGLSurface mSurface;
    EGLContext mContext;
    GLuint vShader;
    GLuint fShader;
    GLuint programObject;
    GLint YsamplerLoc;
    GLint UVsamplerLoc;
    GLint YtextureId;
    GLint UVtextureId;
    GLuint VBO, VAO, EBO;

    int (*renderFrame)(struct _yuvRender * render, void * frame);
    int (*initialize)(struct _yuvRender * render);        //Initialize egl-* context
    int (*uninitialize)(struct _yuvRender * render);
}YUVRender, *pYUVRender;

YUVRender *createYUVRender();

void releaseYUVRender(YUVRender **);


#endif /* VIDEORENDER2_H_ */
