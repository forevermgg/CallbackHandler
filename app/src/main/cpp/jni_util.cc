// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "jni_util.h"

#include <android/log.h>
#include <sys/prctl.h>

#include <string>

#include "log/logging.h"
#include "thread_local.h"

static JavaVM* g_jvm = nullptr;
// 替换为 std::mutex
static std::mutex g_mutex;

struct JNIDetach {
  ~JNIDetach() { DetachFromVM(); }
};

// Thread-local object that will detach from JNI during thread shutdown;
THREAD_LOCAL ThreadLocalUniquePtr<JNIDetach> tls_jni_detach;

void InitJavaVM(JavaVM* vm) {
  std::lock_guard<std::mutex> lock(g_mutex);
  // FML_DCHECK(g_jvm == nullptr);
  FML_LOG(ERROR) << "InitJavaVM";
  g_jvm = vm;
}

JNIEnv* AttachCurrentThread() {
  std::lock_guard<std::mutex> lock(g_mutex);
  FML_DCHECK(g_jvm != nullptr)
      << "Trying to attach to current thread without calling InitJavaVM first.";

  JNIEnv* env = nullptr;
  if (g_jvm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_4) ==
      JNI_OK) {
    return env;
  }

  JavaVMAttachArgs args;
  args.version = JNI_VERSION_1_4;
  args.group = nullptr;
  // 16 is the maximum size for thread names on Android.
  char thread_name[16];
  int err = prctl(PR_GET_NAME, thread_name);
  if (err < 0) {
    args.name = nullptr;
  } else {
    args.name = thread_name;
  }
  [[maybe_unused]] jint ret = g_jvm->AttachCurrentThread(&env, &args);
  FML_DCHECK(JNI_OK == ret);

  FML_DCHECK(tls_jni_detach.get() == nullptr);
  tls_jni_detach.reset(new JNIDetach());

  return env;
}

void DetachFromVM() {
  std::lock_guard<std::mutex> lock(g_mutex);
  if (g_jvm) {
    g_jvm->DetachCurrentThread();
    __android_log_print(ANDROID_LOG_ERROR, "DetachCurrentThread",
                        "DetachFromVM DetachCurrentThread");
  }
}

class JvmRefBase {
 protected:
  JvmRefBase(JavaVM* vm) { process_level_jvm_.store(vm); }
  ~JvmRefBase() { process_level_jvm_.store(nullptr); }

  static JavaVM* GetJavaVm() { return process_level_jvm_.load(); }
  static void SetJavaVm(JavaVM* jvm) { process_level_jvm_.store(jvm); }

  static inline std::atomic<JavaVM*> process_level_jvm_ = nullptr;
};

class JvmThread {
 public:
  explicit JvmThread(JavaVM* jvm) {
    jvm_ = jvm;
    attached_ = false;
    jni_env_ = nullptr;
    int get_env_stat =
        jvm_->GetEnv(reinterpret_cast<void**>(&jni_env_), JNI_VERSION_1_6);
    // TODO: report the error back to Java layer.
    switch (get_env_stat) {
      case JNI_OK:
        break;
      case JNI_EDETACHED:
        FML_LOG(ERROR) << "GetEnv: not attached";
        if (jvm_->AttachCurrentThread(
#ifdef __ANDROID__
                &jni_env_,
#else
                reinterpret_cast<void**>(&jni_env_),
#endif  // __ANDROID__
                nullptr) != 0) {
          FML_LOG(ERROR) << "Failed to attach to java thread.";
          break;
        }
        attached_ = true;
        break;
      case JNI_EVERSION:
        FML_LOG(ERROR) << "GetEnv: jni version not supported.";
        break;
      default:
        FML_LOG(ERROR) << "GetEnv: unknown status.";
        break;
    }
  }

  ~JvmThread() {
    if (attached_) {
      jvm_->DetachCurrentThread();
    }
  }

  JNIEnv* GetEnv() const { return jni_env_; }

 private:
  bool attached_;
  JavaVM* jvm_;
  JNIEnv* jni_env_;
};

// Since current android abi doesn't have pthread_local, we have to rely on
// pthread functions to achieve the detachment of java thread when native thread
// exits (see: http://developer.android.com/training/articles/perf-jni.html).
static pthread_key_t jvm_thread_key;
static pthread_once_t key_once = PTHREAD_ONCE_INIT;

static void ThreadExitCallback(void* key_value) {
  JvmThread* jvm_thread = reinterpret_cast<JvmThread*>(key_value);
  // Detach the thread when thread exits.
  FML_LOG(ERROR) << "Exiting thread. Detach thread.";
  delete jvm_thread;
}

void MakeKey() { pthread_key_create(&jvm_thread_key, ThreadExitCallback); }

// Returns the global Java VM instance.
JavaVM* GetJavaVM() {
  std::lock_guard<std::mutex> lock(g_mutex);
  return g_jvm;
}

bool HasJavaVM() {
  std::lock_guard<std::mutex> lock(g_mutex);
  return g_jvm != nullptr;
}

bool SetJavaVM(JNIEnv* env) {
    std::lock_guard<std::mutex> lock(g_mutex);
  if (!g_jvm) {
    if (env->GetJavaVM(&g_jvm) != JNI_OK) {
      FML_LOG(ERROR) << "Can not get the Java VM instance!";
      g_jvm = nullptr;
      return false;
    }
  }
  return true;
}

JNIEnv* GetJNIEnv() {
  pthread_once(&key_once, MakeKey);
  JvmThread* jvm_thread =
      reinterpret_cast<JvmThread*>(pthread_getspecific(jvm_thread_key));
  if (jvm_thread == nullptr) {
    jvm_thread = new JvmThread(GetJavaVM());
    pthread_setspecific(jvm_thread_key, jvm_thread);
  }
  return jvm_thread->GetEnv();
}