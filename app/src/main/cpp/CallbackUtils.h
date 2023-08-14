#pragma once

#include <jni.h>

#include "CallbackHandler.h"

struct CallbackJni {
#ifdef __ANDROID__
  jclass handlerClass = nullptr;
  jmethodID post = nullptr;
#endif
  jclass executorClass = nullptr;
  jmethodID execute = nullptr;
};

void acquireCallbackJni(JNIEnv* env, CallbackJni& callbackUtils);
void releaseCallbackJni(JNIEnv* env, CallbackJni callbackUtils, jobject handler,
                        jobject callback);

struct JniCallback : private CallbackHandler {
  JniCallback(JniCallback const&) = delete;
  JniCallback(JniCallback&&) = delete;
  JniCallback& operator=(JniCallback const&) = delete;
  JniCallback& operator=(JniCallback&&) = delete;

  // create a JniCallback
  static JniCallback* make(JNIEnv* env, jobject handler, jobject runnable);

  // execute the callback on the java thread and destroy ourselves
  static void postToJavaAndDestroy(JniCallback* callback);

  // CallbackHandler interface.
  void post(void* user, Callback callback) override;

  // Get the CallbackHandler interface
  CallbackHandler* getHandler() noexcept { return this; }

  jobject getCallbackObject() { return mCallback; }

 protected:
  JniCallback(JNIEnv* env, jobject handler, jobject runnable);
  explicit JniCallback() = default;  // this version does nothing
  virtual ~JniCallback();
  jobject mHandler{};
  jobject mCallback{};
  CallbackJni mCallbackUtils{};
};
