package com.rulerbug.myldemo

class Demo {
    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    companion object {
        // Used to load the 'native-lib' library on application startup.
        init {

            // Used to load the 'native-lib' library on application startup.

//            System.loadLibrary("native-lib");
//            System.loadLibrary("avcodec-57");
//            System.loadLibrary("avdevice-57");
//            System.loadLibrary("avfilter-6");
//            System.loadLibrary("avformat-57");
//            System.loadLibrary("avutil-55");
//            System.loadLibrary("postproc-54");
//            System.loadLibrary("swresample-2");
//            System.loadLibrary("swscale-4");
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