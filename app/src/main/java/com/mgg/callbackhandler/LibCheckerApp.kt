package com.mgg.callbackhandler

import android.app.Application
import timber.log.Timber

class LibCheckerApp : Application() {

    override fun onCreate() {
        super.onCreate()
        JNIUtils.setClassLoader(this.classLoader)
        Timber.plant(Timber.DebugTree())
        app = this
    }

    companion object {
        //noinspection StaticFieldLeak
        lateinit var app: Application
    }
}