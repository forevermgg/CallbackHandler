#include <assert.h>
#include <jni.h>

#include <string>

#include "log.h"
#include "jni_android.h"

extern "C" JNIEXPORT void JNICALL Java_com_mgg_callbackhandler_Log_nativeLog(
    JNIEnv* env, jobject instance, jint priority, jstring tag, jstring msg) {
  std::string ctag = FOREVER::JNI::JavaStringToString(env, tag);
  std::string cmsg = FOREVER::JNI::JavaStringToString(env, msg);
  static const FOREVER::LogLevel kLogPriorityToLogLevel[] = {
      FOREVER::LogLevel::kLogLevelDebug,    // 0 = undocumented
      FOREVER::LogLevel::kLogLevelDebug,    // 1 = undocumented
      FOREVER::LogLevel::kLogLevelVerbose,  // 2 = android.util.Log.VERBOSE
      FOREVER::LogLevel::kLogLevelDebug,    // 3 = android.util.Log.DEBUG
      FOREVER::LogLevel::kLogLevelInfo,     // 4 = android.util.Log.INFO
      FOREVER::LogLevel::kLogLevelWarning,  // 5 = android.util.Log.WARN
      FOREVER::LogLevel::kLogLevelError,    // 6 = android.util.Log.ERROR
      FOREVER::LogLevel::kLogLevelAssert,   // 7 = android.util.Log.ASSERT
  };
  LogSetLevel(FOREVER::LogLevel::kLogLevelDebug);
  assert(priority < FOREVER_ARRAYSIZE(kLogPriorityToLogLevel));
  assert(priority >= 0);
  LogMessage(kLogPriorityToLogLevel[priority], "(%s) %s", ctag.c_str(),
             cmsg.c_str());
}
