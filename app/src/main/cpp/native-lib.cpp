#include <android/hardware_buffer_jni.h>
#include <android/log.h>
#include <dlfcn.h>
#include <jni.h>

#include <string>

#include "VirtualMachineEnv.h"
#include "debug.h"
#include "platform/android/jni_android.h"
#include "log/logging.h"
#include "memory/mutex.h"

using PFN_FROMHARDWAREBUFFER = AHardwareBuffer* (*)(JNIEnv*, jobject);
static PFN_FROMHARDWAREBUFFER AHardwareBuffer_fromHardwareBuffer_fn = nullptr;
static bool sHardwareBufferSupported = true;
extern "C" JNIEXPORT jstring JNICALL
Java_com_mgg_callbackhandler_MainActivity_stringFromJNI(JNIEnv* env,
                                                        jobject /* this */) {
  if (!FOREVER::JNI::SetJavaVM(env)) {
    FOREVER_LOG(ERROR) << "env error";
  }
  std::string hello = "Hello from C++";
  // This function is not available before NDK 15 or before Android 8.
  if (UTILS_UNLIKELY(!AHardwareBuffer_fromHardwareBuffer_fn)) {
    if (!sHardwareBufferSupported) {
      hello = "sHardwareBufferSupported == false";
      return env->NewStringUTF(hello.c_str());
    }
    AHardwareBuffer_fromHardwareBuffer_fn = (PFN_FROMHARDWAREBUFFER)dlsym(
        RTLD_DEFAULT, "AHardwareBuffer_fromHardwareBuffer");
    if (!AHardwareBuffer_fromHardwareBuffer_fn) {
      __android_log_print(
          ANDROID_LOG_WARN, "Filament",
          "AHardwareBuffer_fromHardwareBuffer is not available.");
      sHardwareBufferSupported = false;
      hello = "AHardwareBuffer_fromHardwareBuffer is not available.";
      return env->NewStringUTF(hello.c_str());
    }
  }
  auto test_env = VirtualMachineEnv::get().getEnvironment();
  test_env = FOREVER::JNI::AttachCurrentThread();
  if (!test_env) {
    FOREVER_LOG(ERROR) << "env error";
  }
  test_env = FOREVER::JNI::GetJNIEnv();
  if (!test_env) {
    FOREVER_LOG(ERROR) << "env error";
  }
  return test_env->NewStringUTF(hello.c_str());
}