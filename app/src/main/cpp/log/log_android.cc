#include <android/log.h>

#include "log.h"

#define STR_EXPAND(x) #x
#define STR(x) STR_EXPAND(x)

#include <stdarg.h>

namespace FOREVER {

const char* kDefaultTag = "forever";

void LogInitialize() {}

// Log a firebase message.
void AndroidLogMessageV(int priority, const char* tag, const char* format,
                        va_list args) {
  __android_log_vprint(priority, tag, format, args);
}

// Set the platform specific SDK log level.
void LogSetPlatformLevel(LogLevel level) {
  // This isn't available on Android, instead logs go through the framework's
  // android.util.Log.  Some modules, like Analytics and Realtime Database,
  // have their own custom logging which are enabled via system configuration
  // variables or module-specific API calls.
  (void)level;
}

// Log a firebase message.
void LogMessageV(LogLevel log_level, const char* format, va_list args) {
  switch (log_level) {
    case kLogLevelVerbose:
      AndroidLogMessageV(ANDROID_LOG_VERBOSE, kDefaultTag, format, args);
      break;
    case kLogLevelDebug:
      AndroidLogMessageV(ANDROID_LOG_DEBUG, kDefaultTag, format, args);
      break;
    case kLogLevelInfo:
      AndroidLogMessageV(ANDROID_LOG_INFO, kDefaultTag, format, args);
      break;
    case kLogLevelWarning:
      AndroidLogMessageV(ANDROID_LOG_WARN, kDefaultTag, format, args);
      break;
    case kLogLevelError:
      AndroidLogMessageV(ANDROID_LOG_ERROR, kDefaultTag, format, args);
      break;
    case kLogLevelAssert:
      AndroidLogMessageV(ANDROID_LOG_FATAL, kDefaultTag, format, args);
      break;
  }
}
}  // namespace FOREVER
