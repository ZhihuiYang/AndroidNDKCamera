package com.yzh.hilive;


import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.SurfaceHolder;

public class HLGLSurfaceView extends GLSurfaceView{
    private HLRenderer hlRenderer;

    public HLGLSurfaceView(Context context){
        super(context);
        hlRenderer = new HLRenderer();
        setEGLContextClientVersion(2);
        setRenderer(hlRenderer);
        setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
        SurfaceHolder surfaceHolder = getHolder();
        surfaceHolder.addCallback(surfaceHolderCallback);
    }

    private SurfaceHolder.Callback surfaceHolderCallback = new SurfaceHolder.Callback() {
        @Override
        public void surfaceCreated(SurfaceHolder holder) {

        }

        @Override
        public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

        }

        @Override
        public void surfaceDestroyed(SurfaceHolder holder) {

        }
    };

}