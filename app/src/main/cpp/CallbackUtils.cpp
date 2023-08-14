#include "CallbackUtils.h"

#include "VirtualMachineEnv.h"

void acquireCallbackJni(JNIEnv* env, CallbackJni& callbackUtils) {
#ifdef __ANDROID__
  callbackUtils.handlerClass = env->FindClass("android/os/Handler");
  callbackUtils.handlerClass =
      (jclass)env->NewGlobalRef(callbackUtils.handlerClass);
  callbackUtils.post = env->GetMethodID(callbackUtils.handlerClass, "post",
                                        "(Ljava/lang/Runnable;)Z");
#endif

  callbackUtils.executorClass = env->FindClass("java/util/concurrent/Executor");
  callbackUtils.executorClass =
      (jclass)env->NewGlobalRef(callbackUtils.executorClass);
  callbackUtils.execute = env->GetMethodID(
      callbackUtils.executorClass, "execute", "(Ljava/lang/Runnable;)V");
}

void releaseCallbackJni(JNIEnv* env, CallbackJni callbackUtils, jobject handler,
                        jobject callback) {
  if (handler && callback) {
#ifdef __ANDROID__
    if (env->IsInstanceOf(handler, callbackUtils.handlerClass)) {
      env->CallBooleanMethod(handler, callbackUtils.post, callback);
    }
#endif
    if (env->IsInstanceOf(handler, callbackUtils.executorClass)) {
      env->CallVoidMethod(handler, callbackUtils.execute, callback);
    }
  }
  env->DeleteGlobalRef(handler);
  env->DeleteGlobalRef(callback);
#ifdef __ANDROID__
  env->DeleteGlobalRef(callbackUtils.handlerClass);
#endif
  env->DeleteGlobalRef(callbackUtils.executorClass);
}

// -----------------------------------------------------------------------------------------------

JniCallback* JniCallback::make(JNIEnv* env, jobject handler, jobject callback) {
  return new JniCallback(env, handler, callback);
}

JniCallback::JniCallback(JNIEnv* env, jobject handler, jobject callback)
    : mHandler(env->NewGlobalRef(handler)),
      mCallback(env->NewGlobalRef(callback)) {
  acquireCallbackJni(env, mCallbackUtils);
}

JniCallback::~JniCallback() = default;

void JniCallback::post(void* user, CallbackHandler::Callback callback) {
  callback(user);
}

void JniCallback::postToJavaAndDestroy(JniCallback* callback) {
  JNIEnv* env = VirtualMachineEnv::get().getEnvironment();
  releaseCallbackJni(env, callback->mCallbackUtils, callback->mHandler,
                     callback->mCallback);
  delete callback;
}
