package com.rulerbug.mycpp;

import android.app.Application;

import com.rulerbug.bugutils.nas_library_utils.Utils.NasApp;

public class App extends Application {
    @Override
    public void onCreate() {
        super.onCreate();
        NasApp.INSTANCE.init(this);
    }
}
