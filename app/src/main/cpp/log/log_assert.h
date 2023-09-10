#ifndef FOREVER_ASSERT_H_
#define FOREVER_ASSERT_H_

#include "log.h"

#define FOREVER_EXPAND_STRINGIFY_(X) #X
#define FOREVER_EXPAND_STRINGIFY(X) FOREVER_EXPAND_STRINGIFY_(X)

// Only include file and line number in debug build assert messages.
#if defined(NDEBUG)
#define FOREVER_ASSERT_MESSAGE_PREFIX
#else
#define FOREVER_ASSERT_MESSAGE_PREFIX \
  __FILE__ "(" FOREVER_EXPAND_STRINGIFY(__LINE__) "): "
#endif  // defined(NDEBUG)

// FOREVER_ASSERT_* macros are not compiled out of release builds. They should
// be used for assertions that need to be propagated to end-users of SDKs.
// FOREVER_DEV_ASSERT_* macros are compiled out of release builds, similar to
// the C assert() macro. They should be used for internal assertions that are
// only shown to SDK developers.

// Assert condition is true, if it's false log an assert with the specified
// expression as a string.
#define FOREVER_ASSERT_WITH_EXPRESSION(condition, expression)      \
  do {                                                              \
    if (!(condition)) {                                             \
      FOREVER::LogAssert(                                          \
          FOREVER_ASSERT_MESSAGE_PREFIX FOREVER_EXPAND_STRINGIFY( \
              expression));                                         \
    }                                                               \
  } while (false)

// Assert condition is true, if it's false log an assert with the specified
// expression as a string. Compiled out of release builds.
#if !defined(NDEBUG)
#define FOREVER_DEV_ASSERT_WITH_EXPRESSION(condition, expression) \
  FOREVER_ASSERT_WITH_EXPRESSION(condition, expression)
#else
#define FOREVER_DEV_ASSERT_WITH_EXPRESSION(condition, expression) \
  (void)(condition)
#endif  // !defined(NDEBUG)

// Custom assert() implementation that is not compiled out in release builds.
#define FOREVER_ASSERT(expression) \
  FOREVER_ASSERT_WITH_EXPRESSION(expression, expression)

// Custom assert() implementation that is compiled out in release builds.
// Compiled out of release builds.
#define FOREVER_DEV_ASSERT(expression) \
  FOREVER_DEV_ASSERT_WITH_EXPRESSION(expression, expression)

// Assert that returns a value if the log operation doesn't abort the program.
#define FOREVER_ASSERT_RETURN(return_value, expression)    \
  {                                                         \
    bool condition = !(!(expression));                      \
    FOREVER_ASSERT_WITH_EXPRESSION(condition, expression); \
    if (!condition) return (return_value);                  \
  }

// Assert that returns a value if the log operation doesn't abort the program.
// Compiled out of release builds.
#if !defined(NDEBUG)
#define FOREVER_DEV_ASSERT_RETURN(return_value, expression) \
  FOREVER_ASSERT_RETURN(return_value, expression)
#else
#define FOREVER_DEV_ASSERT_RETURN(return_value, expression) \
  { (void)(expression); }
#endif  // !defined(NDEBUG)

// Assert that returns if the log operation doesn't abort the program.
#define FOREVER_ASSERT_RETURN_VOID(expression)             \
  {                                                         \
    bool condition = !(!(expression));                      \
    FOREVER_ASSERT_WITH_EXPRESSION(condition, expression); \
    if (!condition) return;                                 \
  }

// Assert that returns if the log operation doesn't abort the program.
// Compiled out of release builds.
#if !defined(NDEBUG)
#define FOREVER_DEV_ASSERT_RETURN_VOID(expression) \
  FOREVER_ASSERT_RETURN_VOID(expression)
#else
#define FOREVER_DEV_ASSERT_RETURN_VOID(expression) \
  { (void)(expression); }
#endif  // !defined(NDEBUG)

// Assert condition is true otherwise display the specified expression,
// message and abort.
#define FOREVER_ASSERT_MESSAGE_WITH_EXPRESSION(condition, expression, ...) \
  do {                                                                      \
    if (!(condition)) {                                                     \
      FOREVER::LogError(                                                   \
          FOREVER_ASSERT_MESSAGE_PREFIX FOREVER_EXPAND_STRINGIFY(         \
              expression));                                                 \
      FOREVER::LogAssert(__VA_ARGS__);                                     \
    }                                                                       \
  } while (false)

// Assert condition is true otherwise display the specified expression,
// message and abort. Compiled out of release builds.
#if !defined(NDEBUG)
#define FOREVER_DEV_ASSERT_MESSAGE_WITH_EXPRESSION(condition, expression, \
                                                    ...)                   \
  FOREVER_ASSERT_MESSAGE_WITH_EXPRESSION(condition, expression, __VA_ARGS__)
#else
#define FOREVER_DEV_ASSERT_MESSAGE_WITH_EXPRESSION(condition, expression, \
                                                    ...)                   \
  { (void)(condition); }
#endif  // !defined(NDEBUG)

// Assert expression is true otherwise display the specified message and
// abort.
#define FOREVER_ASSERT_MESSAGE(expression, ...) \
  FOREVER_ASSERT_MESSAGE_WITH_EXPRESSION(expression, expression, __VA_ARGS__)

// Assert expression is true otherwise display the specified message and
// abort. Compiled out of release builds.
#define FOREVER_DEV_ASSERT_MESSAGE(expression, ...)                  \
  FOREVER_DEV_ASSERT_MESSAGE_WITH_EXPRESSION(expression, expression, \
                                              __VA_ARGS__)

// Assert expression is true otherwise display the specified message and
// abort or return a value if the log operation doesn't abort.
#define FOREVER_ASSERT_MESSAGE_RETURN(return_value, expression, ...) \
  {                                                                   \
    bool condition = !(!(expression));                                \
    FOREVER_ASSERT_MESSAGE_WITH_EXPRESSION(condition, expression,    \
                                            __VA_ARGS__);             \
    if (!condition) return (return_value);                            \
  }

// Assert expression is true otherwise display the specified message and
// abort or return a value if the log operation doesn't abort. Compiled out of
// release builds.
#if !defined(NDEBUG)
#define FOREVER_DEV_ASSERT_MESSAGE_RETURN(return_value, expression, ...) \
  FOREVER_ASSERT_MESSAGE_RETURN(return_value, expression, __VA_ARGS__)
#else
#define FOREVER_DEV_ASSERT_MESSAGE_RETURN(return_value, expression, ...) \
  { (void)(expression); }
#endif  // !defined(NDEBUG)

// Assert expression is true otherwise display the specified message and
// abort or return if the log operation doesn't abort.
#define FOREVER_ASSERT_MESSAGE_RETURN_VOID(expression, ...)       \
  {                                                                \
    bool condition = !(!(expression));                             \
    FOREVER_ASSERT_MESSAGE_WITH_EXPRESSION(condition, expression, \
                                            __VA_ARGS__);          \
    if (!condition) return;                                        \
  }

// Assert expression is true otherwise display the specified message and
// abort or return if the log operation doesn't abort. Compiled out of release
// builds.
#if !defined(NDEBUG)
#define FOREVER_DEV_ASSERT_MESSAGE_RETURN_VOID(expression, ...) \
  FOREVER_ASSERT_MESSAGE_RETURN_VOID(expression, __VA_ARGS__)
#else
#define FOREVER_DEV_ASSERT_MESSAGE_RETURN_VOID(expression, ...) \
  { (void)(expression); }
#endif  // !defined(NDEBUG)

#endif  // FOREVER_ASSERT_H_
