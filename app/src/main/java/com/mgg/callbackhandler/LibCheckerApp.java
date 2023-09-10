package com.mgg.callbackhandler;

import android.app.Application;

import timber.log.Timber;

public class LibCheckerApp extends Application {
    static LibCheckerApp instancesLibCheckerApp = null;

    public static LibCheckerApp getInstance() {
        if (instancesLibCheckerApp == null) {
            throw new IllegalStateException("LibCheckerApp does not exist");
        }
        return instancesLibCheckerApp;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        JNIUtils.setClassLoader(this.getClassLoader());
        Timber.plant(Timber.asTree());
        instancesLibCheckerApp = this;
    }
}
