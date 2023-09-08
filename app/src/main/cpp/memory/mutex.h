#ifndef FOREVER_MUTEX_H_
#define FOREVER_MUTEX_H_

#include <pthread.h>
#include <memory>

namespace FOREVER {

/// @brief A simple synchronization lock. Only one thread at a time can Acquire.
class Mutex {
 public:
  // Bitfield that describes the mutex configuration.
  enum Mode {
    kModeNonRecursive = (0 << 0),
    kModeRecursive = (1 << 0),
  };

  Mutex() : Mutex(kModeRecursive) {}

  explicit Mutex(Mode mode);

  ~Mutex();

  // Acquires the lock for this mutex, blocking until it is available.
  void Acquire();

  // Releases the lock for this mutex acquired by a previous `Acquire()` call.
  void Release();


  pthread_mutex_t* native_handle() { return &mutex_; }

 private:
  Mutex(const Mutex&) = delete;
  Mutex& operator=(const Mutex&) = delete;

  pthread_mutex_t mutex_;
};

/// @brief Acquire and hold a /ref Mutex, while in scope.
///
/// Example usage:
///   \code{.cpp}
///   Mutex syncronization_mutex;
///   void MyFunctionThatRequiresSynchronization() {
///     MutexLock lock(syncronization_mutex);
///     // ... logic ...
///   }
///   \endcode
class MutexLock {
 public:
  explicit MutexLock(Mutex& mutex) : mutex_(&mutex) { mutex_->Acquire(); }
  ~MutexLock() { mutex_->Release(); }

 private:
  // Copy is disallowed.
  MutexLock(const MutexLock& rhs);  // NOLINT
  MutexLock& operator=(const MutexLock& rhs);

  Mutex* mutex_;
};
}  // namespace FOREVER

#endif  // FOREVER_MUTEX_H_
