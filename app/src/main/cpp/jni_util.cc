// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "jni_util.h"

#include <android/log.h>
#include <sys/prctl.h>

#include <string>

#include "thread_local.h"
#include "log/logging.h"

static JavaVM* g_jvm = nullptr;

struct JNIDetach {
  ~JNIDetach() { DetachFromVM(); }
};

// Thread-local object that will detach from JNI during thread shutdown;
THREAD_LOCAL ThreadLocalUniquePtr<JNIDetach> tls_jni_detach;

void InitJavaVM(JavaVM* vm) {
  // FML_DCHECK(g_jvm == nullptr);
  g_jvm = vm;
}

JNIEnv* AttachCurrentThread() {
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
  if (g_jvm) {
    g_jvm->DetachCurrentThread();
    __android_log_print(ANDROID_LOG_ERROR, "DetachCurrentThread",
                        "DetachFromVM DetachCurrentThread");
  }
}
