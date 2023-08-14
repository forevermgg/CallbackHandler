#ifndef MACROS_H_
#define MACROS_H_

#include <jni.h>


#define DISALLOW_COPY(TypeName) TypeName(const TypeName&) = delete

#define DISALLOW_ASSIGN(TypeName) \
  TypeName& operator=(const TypeName&) = delete

#define DISALLOW_MOVE(TypeName) \
  TypeName(TypeName&&) = delete;            \
  TypeName& operator=(TypeName&&) = delete

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;                  \
  TypeName& operator=(const TypeName&) = delete

#define DISALLOW_COPY_ASSIGN_AND_MOVE(TypeName) \
  TypeName(const TypeName&) = delete;                       \
  TypeName(TypeName&&) = delete;                            \
  TypeName& operator=(const TypeName&) = delete;            \
  TypeName& operator=(TypeName&&) = delete

#define DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
  TypeName() = delete;                                       \
  DISALLOW_COPY_ASSIGN_AND_MOVE(TypeName)

// true no error,false log_fault.
#define CHECK(condition)                                         \
  (condition) ? log_jni_debug << "File = " << __FILE__ << " Line = " << __LINE__ \
                          << " condition = " << #condition                   \
              : log_jni_fault << "File = " << __FILE__ << " Line = " << __LINE__ \
                          << " condition = " << #condition;

#define DCHECK(condition) CHECK(condition)

#endif  // MACROS_H_
