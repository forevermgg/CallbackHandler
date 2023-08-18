@file:Suppress("DEPRECATION")

package com.mgg.callbackhandler

import android.content.Intent
import android.content.pm.ApplicationInfo
import android.content.pm.PackageInfo
import android.content.pm.PackageManager
import android.content.pm.ResolveInfo

object PackageManagerCompat {
    fun getPackageInfo(packageName: String, flags: Int): PackageInfo {
        return if (OsUtils.atLeastT()) {
            SystemServices.packageManager.getPackageInfo(
                packageName,
                PackageManager.PackageInfoFlags.of(flags.toLong())
            )
        } else {
            SystemServices.packageManager.getPackageInfo(packageName, flags)
        }
    }

    fun getPackageArchiveInfo(archiveFilePath: String, flags: Int): PackageInfo? {
        return runCatching {
            if (OsUtils.atLeastT()) {
                SystemServices.packageManager.getPackageArchiveInfo(
                    archiveFilePath,
                    PackageManager.PackageInfoFlags.of(flags.toLong())
                )
            } else {
                SystemServices.packageManager.getPackageArchiveInfo(archiveFilePath, flags)
            }
        }.getOrNull()
    }

    fun getApplicationInfo(packageName: String, flags: Int): ApplicationInfo {
        return if (OsUtils.atLeastT()) {
            SystemServices.packageManager.getApplicationInfo(
                packageName,
                PackageManager.ApplicationInfoFlags.of(flags.toLong())
            )
        } else {
            SystemServices.packageManager.getApplicationInfo(packageName, flags)
        }
    }

    fun getInstalledPackages(flags: Int): List<PackageInfo> {
        return if (OsUtils.atLeastT()) {
            SystemServices.packageManager.getInstalledPackages(
                PackageManager.PackageInfoFlags.of(
                    flags.toLong()
                )
            )
        } else {
            SystemServices.packageManager.getInstalledPackages(flags)
        }
    }

    fun queryIntentActivities(intent: Intent, flags: Int): List<ResolveInfo> {
        return if (OsUtils.atLeastT()) {
            SystemServices.packageManager.queryIntentActivities(
                intent,
                PackageManager.ResolveInfoFlags.of(flags.toLong())
            )
        } else {
            SystemServices.packageManager.queryIntentActivities(intent, flags)
        }
    }
}
