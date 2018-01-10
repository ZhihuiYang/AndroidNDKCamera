package com.yzh.hilive;

import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by eyngzui on 12/8/2017.
 */


public class HLRenderer implements GLSurfaceView.Renderer {
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        ndkOnSurfaceCreated(640,480);
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        ndkOnSurfaceChanged(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        ndkOnDrawFrame();
    }


    private native void ndkOnSurfaceCreated(int width, int height);
    private native void ndkOnSurfaceChanged(int width, int height);
    private native void ndkOnDrawFrame();
}