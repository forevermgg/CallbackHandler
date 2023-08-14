/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <jni.h>

#include "VirtualMachineEnv.h"
#include "jni_util.h"
#include "log/log_settings.h"
#include "log/logging.h"
#include "log/log_level.h"

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
  JNIEnv* env;
  if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
    return -1;
  }
  fml::LogSettings log_settings;
  log_settings.min_log_level = fml::LOG_INFO;
  fml::SetLogSettings(log_settings);
  // This must be called when the library is loaded. We need this to get a
  // reference to the global VM
  InitJavaVM(vm);
  VirtualMachineEnv::JNI_OnLoad(vm);
  return JNI_VERSION_1_6;
}