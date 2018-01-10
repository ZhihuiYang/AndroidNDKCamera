package com.yzh.hilive;

import android.content.Context;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

/**
 * Created by eyngzui on 12/11/2017.
 */

public class NdkCamera2 {
    static final String TAG = "NdkCamera2";
//    private native void openCamera();
//    private native void closeCamera();
//    private native void setSurface(Surface surface);
//    private native void shutdown();

    private SurfaceView surfaceView;
    private SurfaceHolder surfaceHolder;
    private Context context;
    private Surface surface;

    public NdkCamera2(Context context){
        this.context = context;
    }

    public void setSurfaceView(SurfaceView surfaceView){
        this.surfaceView = surfaceView;
        this.surfaceHolder = surfaceView.getHolder();
        this.surface = surfaceHolder.getSurface();
    }

    public void open(){
//        openCamera();
//        setSurface(this.surface);
    }

    public void close(){
//        closeCamera();
//        shutdown();
    }
}
