package com.rulerbug.myldemo

import android.view.SurfaceView

class Demo {
    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */

    external fun play(s: String, sv: SurfaceView?)

    companion object {

        init {

            System.loadLibrary("native-lib");
            System.loadLibrary("avcodec");
            System.loadLibrary("avdevice");
            System.loadLibrary("avfilter");
            System.loadLibrary("avformat");
            System.loadLibrary("avutil");
            System.loadLibrary("postproc");
            System.loadLibrary("swresample");
            System.loadLibrary("swscale");

        }
    }
}