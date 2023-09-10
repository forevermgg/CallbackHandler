#include "log.h"

#include <stdarg.h>
#include <stdio.h>

#include <cstdlib>
#include <mutex>

#include "mutex.h"
#include "../memory/mutex.h"

namespace FOREVER {
namespace LOG {
// Default log callback.
static void DefaultLogCallback(LogLevel log_level, const char* message,
                               void* /*callback_data*/);

const LogLevel kDefaultLogLevel = kLogLevelInfo;

LogLevel g_log_level = kDefaultLogLevel;
LogCallback g_log_callback = DefaultLogCallback;
void* g_log_callback_data = nullptr;
// Mutex which controls access to the log buffer used to format callback
// log messages.
Mutex* g_log_mutex = nullptr;

// Initialize g_log_mutex when static constructors are called.
// This class makes sure g_log_mutex is initialized typically initialized
// before the first log function is called.
class InitializeLogMutex {
 public:
  InitializeLogMutex() {
    if (!g_log_mutex) g_log_mutex = new Mutex();
  }
};
InitializeLogMutex g_log_mutex_initializer;

// Forward a log message to LogMessageV().
static void InternalLogMessage(LogLevel log_level, const char* message, ...) {
  va_list list;
  va_start(list, message);
  LogMessageV(log_level, "%s", list);
  va_end(list);
}

// Default log callback.  Halts the application after logging assert messages.
static void DefaultLogCallback(LogLevel log_level, const char* message,
                               void* /*callback_data*/) {
  InternalLogMessage(log_level, "%s", message);
  if (log_level == kLogLevelAssert) {
    abort();
  }
}

// Log a firebase message (implemented by the platform specific logger).
void LogMessageWithCallbackV(LogLevel log_level, const char* format,
                             va_list args) {
  // We create the mutex on the heap as this can be called before the C++
  // runtime is initialized on iOS.  This ensures the Mutex class is
  // constructed before we attempt to use it.  Of course, this isn't thread
  // safe but the first time this is called on any platform it will be from
  // a single thread to initialize the API.
  if (!g_log_mutex) g_log_mutex = new Mutex();
  MutexLock lock(*g_log_mutex);

  LogInitialize();
  if (log_level < GetLogLevel()) return;

  static char log_buffer[512] = {0};
  vsnprintf(log_buffer, sizeof(log_buffer) - 1, format, args);
  g_log_callback(log_level, log_buffer, g_log_callback_data);
}

void SetLogLevel(LogLevel level) {
  g_log_level = level;
  LogSetPlatformLevel(level);
}

LogLevel GetLogLevel() { return g_log_level; }

void LogSetLevel(LogLevel level) { SetLogLevel(level); }

LogLevel LogGetLevel() { return GetLogLevel(); }

// Log a debug message to the system log.
void LogDebug(const char* format, ...) {
  va_list list;
  va_start(list, format);
  LogMessageWithCallbackV(kLogLevelDebug, format, list);
  va_end(list);
}

// Log an info message to the system log.
void LogInfo(const char* format, ...) {
  va_list list;
  va_start(list, format);
  LogMessageWithCallbackV(kLogLevelInfo, format, list);
  va_end(list);
}

// Log a warning to the system log.
void LogWarning(const char* format, ...) {
  va_list list;
  va_start(list, format);
  LogMessageWithCallbackV(kLogLevelWarning, format, list);
  va_end(list);
}

// Log an error message to the system log.
void LogError(const char* format, ...) {
  va_list list;
  va_start(list, format);
  LogMessageWithCallbackV(kLogLevelError, format, list);
  va_end(list);
}

// Log an assert message to the system log.
// NOTE: The default callback will stop the application.
void LogAssert(const char* format, ...) {
  va_list list;
  va_start(list, format);
  LogMessageWithCallbackV(kLogLevelAssert, format, list);
  va_end(list);
}

// Log a firebase message via LogMessageV().
void LogMessage(LogLevel log_level, const char* format, ...) {
  va_list args;
  va_start(args, format);
  LogMessageWithCallbackV(log_level, format, args);
  va_end(args);
}

// Set the log callback.
void LogSetCallback(LogCallback callback, void* callback_data) {
  g_log_callback = callback ? callback : DefaultLogCallback;
  g_log_callback_data = callback_data;
}

// Get the log callback.
LogCallback LogGetCallback(void** callback_data) {
  // FOREVER_ASSERT(callback_data);
  *callback_data = g_log_callback_data;
  return g_log_callback;
}
}  // namespace LOG
}  // namespace FOREVER
