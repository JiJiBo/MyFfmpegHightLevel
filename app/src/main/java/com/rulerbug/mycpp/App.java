package com.rulerbug.mycpp;

import android.app.Application;
import android.os.Handler;

import com.rulerbug.bugutils.Utils.BugApp;

public class App extends Application {
    @Override
    public void onCreate() {
        super.onCreate();
        BugApp.init(getApplicationContext(), new Handler(), android.os.Process.myTid());
    }
}
