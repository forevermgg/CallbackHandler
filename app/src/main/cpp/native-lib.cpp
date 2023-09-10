#include <android/hardware_buffer_jni.h>
#include <android/log.h>
#include <dlfcn.h>
#include <jni.h>

#include <string>

#include "VirtualMachineEnv.h"
#include "debug.h"
#include "platform/android/jni_android.h"
#include "platform/android/util_android.h"
#include "log/logging.h"
#include "memory/mutex.h"
#include "generate.h"

// Find an Android SDK FirebaseApp instance by name.
// Returns a local jobject reference if successful, nullptr otherwise.
static jobject GetPlatformAppByName(JNIEnv* jni_env, const char* name) {
  jobject platform_app;
  jobject name_string = jni_env->NewStringUTF(name);
  platform_app = jni_env->CallStaticObjectMethod(
          app::GetClass(), app::GetMethodId(app::kGetInstanceByName),
          name_string);
  jni_env->DeleteLocalRef(name_string);
  if (jni_env->ExceptionCheck()) {
    // Explicitly set `platform_app` to `NULL` if an exception was thrown
    // because on KitKat (API 19) `CallStaticObjectMethod()` may return garbage
    // instead of `NULL` if an exception was thrown, and callers of this
    // function expect `NULL` to be returned if the app was not found.
    platform_app = NULL;  // NOLINT
  }
  jni_env->ExceptionClear();
  return platform_app;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_mgg_callbackhandler_MainActivity_stringFromJNI(JNIEnv* env,
                                                        jobject /* this */) {
  FOREVER::UTIL::Initialize(env);
  FOREVER::JNI::SetJavaVM(env);
  FOREVER::UTIL::CheckAndClearJniExceptions(env);
  std::string hello = "Hello from C++";
  auto test_env = VirtualMachineEnv::get().getEnvironment();
  test_env = FOREVER::JNI::AttachCurrentThread();
  test_env = FOREVER::JNI::GetJNIEnv();
  FOREVER::UTIL::ReleaseClasses(env);
  return test_env->NewStringUTF(hello.c_str());
}
