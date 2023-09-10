#ifndef FOREVER_LOG_LEVEL_H_
#define FOREVER_LOG_LEVEL_H_

namespace FOREVER {

typedef int LogSeverity;

// Default log levels. Negative values can be used for verbose log levels.
constexpr LogSeverity LOG_VERBOSE = 0;
constexpr LogSeverity LOG_DEBUG = 1;
constexpr LogSeverity LOG_INFO = 2;
constexpr LogSeverity LOG_WARNING = 3;
constexpr LogSeverity LOG_ERROR = 4;
constexpr LogSeverity LOG_FATAL = 5;
constexpr LogSeverity LOG_NUM_SEVERITIES = 4;

// One of the Windows headers defines ERROR to 0. This makes the token
// concatenation in FML_LOG(ERROR) to resolve to LOG_0. We define this back to
// the appropriate log level.

// LOG_DFATAL is LOG_FATAL in debug mode, ERROR in normal mode
#ifdef NDEBUG
const LogSeverity LOG_DFATAL = LOG_ERROR;
#else
const LogSeverity LOG_DFATAL = LOG_FATAL;
#endif

}  // namespace FOREVER

#endif  // FOREVER_LOG_LEVEL_H_
