package com.mgg.callbackhandler

import android.content.pm.PackageInfo
import android.content.pm.PackageManager
import androidx.core.content.pm.PackageInfoCompat

/**
 * Get version code of an app
 * @return version code as Long Integer
 */
fun PackageInfo.getVersionCode(): Long {
  return PackageInfoCompat.getLongVersionCode(this)
}

/**
 * Get version string of an app ( 1.0.0(1) )
 * @return version code as String
 */
fun PackageInfo.getVersionString(): String {
  return try {
    "${versionName ?: "<unknown>"} (${getVersionCode()})"
  } catch (e: PackageManager.NameNotFoundException) {
    "Unknown"
  }
}

/**
 * Get target api string of an app ( API 30 )
 * @return version code as String
 */
fun PackageInfo.getTargetApiString(): String {
  return runCatching {
    applicationInfo.targetSdkVersion.toString()
  }.getOrDefault("?")
}

private const val compileSdkVersion = "compileSdkVersion"

/**
 * Get permissions list of an app
 * @return Permissions list
 */
fun PackageInfo.getPermissionsList(): List<String> {
  return requestedPermissions?.toList() ?: emptyList()
}

/**
 * Check if an app uses split apks
 * @return true if it uses split apks
 */
fun PackageInfo.isSplitsApk(): Boolean {
  return !applicationInfo.splitSourceDirs.isNullOrEmpty()
}

private const val AGP_KEYWORD = "androidGradlePluginVersion"
private const val AGP_KEYWORD2 = "Created-By: Android Gradle "

/**
 * Check if an app is a Xposed module
 * @return True if is a Xposed module
 */
fun PackageInfo.isXposedModule(): Boolean {
  return applicationInfo.metaData?.getBoolean("xposedmodule") == true ||
          applicationInfo.metaData?.containsKey("xposedminversion") == true
}

/**
 * Check if an app contains Play App Signing
 * @return True if contains Play App Signing
 */
fun PackageInfo.isPlayAppSigning(): Boolean {
  return applicationInfo.metaData?.getString("com.android.stamp.type") == "STAMP_TYPE_DISTRIBUTION_APK" &&
          applicationInfo.metaData?.getString("com.android.stamp.source") == "https://play.google.com/store"
}

/**
 * Check if an app is PWA
 * @return True if is PWA
 */
fun PackageInfo.isPWA(): Boolean {
  return applicationInfo.metaData?.keySet()
    ?.any { it.startsWith("org.chromium.webapk.shell_apk") } == true
}

fun PackageInfo.getAppName(): String? =
  applicationInfo?.loadLabel(SystemServices.packageManager)?.toString()

const val PREINSTALLED_TIMESTAMP = 1230768000000 // 2009-01-01 08:00:00 GMT+8

fun PackageInfo.isPreinstalled(): Boolean {
  return lastUpdateTime <= PREINSTALLED_TIMESTAMP
}
