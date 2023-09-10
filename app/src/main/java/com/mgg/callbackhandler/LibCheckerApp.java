package com.mgg.callbackhandler;

import android.app.Application;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

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
        Timber.plant(new Timber.Tree() {
            @Override
            protected void log(int i, @Nullable String s, @NonNull String s1, @Nullable Throwable throwable) {
            }
        });
        instancesLibCheckerApp = this;
    }
}
