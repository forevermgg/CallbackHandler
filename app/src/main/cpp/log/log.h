#ifndef FOREVER_LOG_H_
#define FOREVER_LOG_H_

/// @brief Namespace that encompasses all Firebase APIs.
#include <cstdarg>

namespace FOREVER {

/// @brief Levels used when logging messages.
enum LogLevel {
  /// Verbose Log Level
  kLogLevelVerbose = 0,
  /// Debug Log Level
  kLogLevelDebug,
  /// Info Log Level
  kLogLevelInfo,
  /// Warning Log Level
  kLogLevelWarning,
  /// Error Log Level
  kLogLevelError,
  /// Assert Log Level
  kLogLevelAssert,
};

/// @brief Sets the logging verbosity.
/// All log messages at or above the specific log level.
///
/// @param[in] level Log level to display, by default this is set to
/// kLogLevelInfo.
void SetLogLevel(LogLevel level);

/// @brief Gets the logging verbosity.
///
/// @return Get the currently configured logging verbosity.
LogLevel GetLogLevel();

void LogSetLevel(LogLevel level);
// Use firebase::GetLogLevel() instead.
LogLevel LogGetLevel();
// Set the platform specific SDK log level.
// This is called internally by LogSetLevel().
void LogSetPlatformLevel(LogLevel level);
// Log a debug message to the system log.
void LogDebug(const char* format, ...);
// Log an info message to the system log.
void LogInfo(const char* format, ...);
// Log a warning to the system log.
void LogWarning(const char* format, ...);
// Log an error to the system log.
void LogError(const char* format, ...);
// Log an assert and stop the application.
void LogAssert(const char* format, ...);
// Log a firebase message (implemented by the platform specific logger).
void LogMessageV(LogLevel log_level, const char* format, va_list args);
// Log a firebase message via LogMessageWithCallbackV().
void LogMessage(LogLevel log_level, const char* format, ...);
// Log a firebase message through log callback.
void LogMessageWithCallbackV(LogLevel log_level, const char* format,
                             va_list args);

// Callback which can be used to override message logging.
typedef void (*LogCallback)(LogLevel log_level, const char* log_message,
                            void* callback_data);
// Set the log callback.
void LogSetCallback(LogCallback callback, void* callback_data);
// Get the log callback.
LogCallback LogGetCallback(void** callback_data);

// Initializes the logging module (implemented by the platform specific logger).
void LogInitialize();

#define FOREVER_ARRAYSIZE(x) (sizeof(x) / sizeof((x)[0]))
}  // namespace FOREVER

#endif  // FOREVER_LOG_H_
