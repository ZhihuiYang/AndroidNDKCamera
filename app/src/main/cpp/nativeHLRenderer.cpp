#include <jni.h>
#include <string>
#include "YUVRender.h"
#include "nativeCapture.h"



static Capture *publisher = NULL;

extern "C" {
JNIEXPORT void JNICALL Java_com_yzh_hilive_HLRenderer_ndkOnSurfaceCreated(
        JNIEnv *env,
        jobject object, jint width, jint height) {
    onSurfaceCreated(width, height);
}

JNIEXPORT void JNICALL Java_com_yzh_hilive_HLRenderer_ndkOnSurfaceChanged(
        JNIEnv *env,
jobject /* this */, jint width, jint height) {
    glViewport(0,0, width, height);
}


JNIEXPORT void JNICALL Java_com_yzh_hilive_HLRenderer_ndkOnDrawFrame(
        JNIEnv *env,
        jobject /* this */) {

    onDrawFrame();
}


JNIEXPORT void JNICALL Java_com_yzh_hilive_MainActivity_ndkOnSurfaceDestroyed(
        JNIEnv *env,
        jobject /* this */) {

    OnSurfaceDestroyed();
}

}//extern "C"
