package com.mgg.callbackhandler

import android.content.pm.PackageManager

object SystemServices {
  val packageManager: PackageManager by lazy { LibCheckerApp.app.packageManager }
}
