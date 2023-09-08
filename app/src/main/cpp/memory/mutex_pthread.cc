#include <errno.h>
#include <pthread.h>

#include "mutex.h"
#include "logging.h"

namespace FOREVER {

Mutex::Mutex(Mode mode) {
  pthread_mutexattr_t attr;
  int ret = pthread_mutexattr_init(&attr);
  FOREVER_CHECK(ret == 0);
  if (mode & kModeRecursive) {
    ret = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    FOREVER_CHECK(ret == 0);
  }
  ret = pthread_mutex_init(&mutex_, &attr);
  FOREVER_CHECK(ret == 0);
  ret = pthread_mutexattr_destroy(&attr);
  FOREVER_CHECK(ret == 0);
}

Mutex::~Mutex() {
  int ret = pthread_mutex_destroy(&mutex_);
  FOREVER_CHECK(ret == 0);
  (void)ret;
}

void Mutex::Acquire() {
  int ret = pthread_mutex_lock(&mutex_);
  if (ret == EINVAL) {
    return;
  }
  FOREVER_CHECK(ret == 0);
  (void)ret;
}

void Mutex::Release() {
  int ret = pthread_mutex_unlock(&mutex_);
  FOREVER_CHECK(ret == 0);
  (void)ret;
}
}  // namespace FOREVER
