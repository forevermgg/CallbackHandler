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

extern "C" JNIEXPORT jstring JNICALL
Java_com_mgg_callbackhandler_MainActivity_stringFromJNI(JNIEnv* env,
                                                        jobject /* this */) {
  FOREVER::UTIL::Initialize(env);
  FOREVER::JNI::SetJavaVM(env);
  FOREVER::UTIL::GetPlatformAppByName(env);
  FOREVER::UTIL::CheckAndClearJniExceptions(env);
  std::string hello = "Hello from C++";
  auto test_env = VirtualMachineEnv::get().getEnvironment();
  test_env = FOREVER::JNI::AttachCurrentThread();
  test_env = FOREVER::JNI::GetJNIEnv();
  FOREVER::UTIL::ReleaseClasses(env);
  return test_env->NewStringUTF(hello.c_str());
}
