package com.mgg.callbackhandler

import android.app.Application

class LibCheckerApp : Application() {

    override fun onCreate() {
        super.onCreate()
        JNIUtils.setClassLoader(this.classLoader)
        app = this
    }

    companion object {
        //noinspection StaticFieldLeak
        lateinit var app: Application
    }
}