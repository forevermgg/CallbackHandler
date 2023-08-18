package com.mgg.callbackhandler

import android.content.pm.PackageInfo

interface AppDataSource {

  fun getApplicationList(): List<PackageInfo>

  fun getApplicationMap(): Map<String, PackageInfo>
}
