package com.mgg.callbackhandler

data class LCItem(
  val packageName: String,
  val label: String,
  val versionName: String,
  val versionCode: Long,
  val installedTime: Long,
  val lastUpdatedTime: Long,
  val isSystem: Boolean,
  /*val abi: Short,
  val features: Int,*/
  val targetApi: Short,
  // val variant: Short
)
