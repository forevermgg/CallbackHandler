#include <jni.h>

#include "VirtualMachineEnv.h"
#include "platform/android/jni_android.h"
#include "log/log_level.h"
#include "log/log_settings.h"
#include "log/logging.h"

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
  JNIEnv *env;
  if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
    return -1;
  }
  FOREVER::LogSettings log_settings;
  log_settings.min_log_level = FOREVER::LOG_INFO;
  FOREVER::SetLogSettings(log_settings);
  // This must be called when the library is loaded. We need this to get a
  // reference to the global VM
  FOREVER::JNI::InitJavaVM(vm);
  VirtualMachineEnv::JNI_OnLoad(vm);
  return JNI_VERSION_1_6;
}
