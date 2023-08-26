#include <errno.h>
#include <pthread.h>

#include "mutex.h"
#include "../log/logging.h"

namespace Forever {

Mutex::Mutex(Mode mode) {
  pthread_mutexattr_t attr;
  int ret = pthread_mutexattr_init(&attr);
  FML_CHECK(ret == 0);
  if (mode & kModeRecursive) {
    ret = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    FML_CHECK(ret == 0);
  }
  ret = pthread_mutex_init(&mutex_, &attr);
  FML_CHECK(ret == 0);
  ret = pthread_mutexattr_destroy(&attr);
  FML_CHECK(ret == 0);
}

Mutex::~Mutex() {
  int ret = pthread_mutex_destroy(&mutex_);
  FML_CHECK(ret == 0);
  (void)ret;
}

void Mutex::Acquire() {
  int ret = pthread_mutex_lock(&mutex_);
  if (ret == EINVAL) {
    return;
  }
  FML_CHECK(ret == 0);
  (void)ret;
}

void Mutex::Release() {
  int ret = pthread_mutex_unlock(&mutex_);
  FML_CHECK(ret == 0);
  (void)ret;
}
}  // namespace Forever
