package com.mgg.callbackhandler

import android.content.pm.PackageInfo
import android.content.pm.PackageManager
import timber.log.Timber

object LocalAppDataSource : AppDataSource {

  override fun getApplicationList(): List<PackageInfo> {
    Timber.d("getApplicationList start")
    val list =
      PackageManagerCompat.getInstalledPackages(PackageManager.GET_META_DATA or PackageManager.GET_PERMISSIONS)
    Timber.d("getApplicationList end, apps count: ${list.size}")
    return list
  }

  override fun getApplicationMap(): Map<String, PackageInfo> {
    return getApplicationList().asSequence()
      .filter { it.applicationInfo.sourceDir != null || it.applicationInfo.publicSourceDir != null }
      .map { it.packageName to it }
      .toMap()
  }

  private val runtime by lazy { Runtime.getRuntime() }

  private fun getAppListByShell(): List<String> {
    try {
      val process = runtime.exec("pm list packages")
      val packages = process.inputStream.reader().useLines { lines ->
        lines.mapNotNull { line ->
          if (line.startsWith("package:")) {
            line.removePrefix("package:").trim().takeIf { it.isNotEmpty() }
          } else {
            null
          }
        }.toList()
      }

      return packages
    } catch (t: Throwable) {
      Timber.w(t)
      return emptyList()
    }
  }
}
