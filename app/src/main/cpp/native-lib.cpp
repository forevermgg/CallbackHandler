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
#include "absl/strings/string_view.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_mgg_callbackhandler_MainActivity_stringFromJNI(JNIEnv* env,
                                                        jobject /* this */) {
  FOREVER::UTIL::Initialize(env);
  FOREVER::JNI::SetJavaVM(env);
  FOREVER::UTIL::GetPlatformAppByName(env);
  FOREVER::UTIL::CheckAndClearJniExceptions(env);
  auto test_env = VirtualMachineEnv::get().getEnvironment();
  test_env = FOREVER::JNI::AttachCurrentThread();
  test_env = FOREVER::JNI::GetJNIEnv();
  FOREVER::UTIL::ReleaseClasses(env);
  constexpr absl::string_view hello = "Hello from C++";
  return test_env->NewStringUTF(std::string(hello).c_str());
}
