#include <jni.h>
#include <android/hardware_buffer_jni.h>
#include <dlfcn.h>
#include "CallbackUtils.h"
#include "Test.h"
#include "VirtualMachineEnv.h"
#include "jni_android.h"
#include "logging.h"
#include "string_conversion.h"

extern "C" JNIEXPORT void JNICALL Java_com_mgg_callbackhandler_Test_nConsumer(
    JNIEnv* env, jclass clazz, jlong nativeTest, jobject handler,
    jobject internalCallback) {
  // jniState will be initialized the first time this method is called
  FOREVER_LOG(ERROR) << "nConsumer";
  static const struct JniState {
    jclass internalOnPickCallbackClass;
    jfieldID codeFieldId;
    jfieldID msgFieldId;
    explicit JniState(JNIEnv* env) noexcept {
      internalOnPickCallbackClass =
          env->FindClass("com/mgg/callbackhandler/Test$InternalOnPickCallback");
      codeFieldId = env->GetFieldID(internalOnPickCallbackClass, "code", "I");
      msgFieldId = env->GetFieldID(internalOnPickCallbackClass, "msg",
                                   "Ljava/lang/String;");
    }
  } jniState(env);
  Test* test = (Test*)nativeTest;
  JniCallback* callback = JniCallback::make(env, handler, internalCallback);
  test->Consumer(
      [callback](Test::PickingQueryResult const& result) {
        FOREVER_LOG(ERROR) << "Consumer JNICallBack";
        // this is executed on the backend/service thread
        jobject obj = callback->getCallbackObject();
        JNIEnv* env = VirtualMachineEnv::get().getEnvironment();
        env->SetIntField(obj, jniState.codeFieldId, (jint)result.code);
        env->SetObjectField(
            obj, jniState.msgFieldId,
            FOREVER::JNI::StringToJavaString(env, result.msg.c_str()).obj());
        FOREVER_LOG(ERROR) << "postToJavaAndDestroy";
        JniCallback::postToJavaAndDestroy(callback);
      },
      callback->getHandler());
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_mgg_callbackhandler_Test_nCreateTest(JNIEnv* env, jclass clazz) {
  Test* test = new Test();
  FOREVER_LOG(ERROR) << "CreateTest";
  return (jlong)test;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_mgg_callbackhandler_Test_nDestroyTest(JNIEnv* env, jclass clazz,
                                               jlong nativeTest) {
  Test* test = (Test*)nativeTest;
  if (test) {
    FOREVER_LOG(ERROR) << "DestroyTest";
    delete test;
  }
  // Fatal signal 5 (SIGTRAP), code 1 (TRAP_BRKPT), fault addr 0x795fd8c3b8 in
  // tid 10529 (FinalizerDaemon), pid 10520 (callbackhandler)
  return true;
}

extern "C" JNIEXPORT void JNICALL Java_com_mgg_callbackhandler_Test_nProducer(
    JNIEnv* env, jclass clazz, jlong nativeTest) {
  Test* test = (Test*)nativeTest;
  if (test) {
    FOREVER_LOG(ERROR) << "Producer";
    test->Producer();
  }
}
extern "C" JNIEXPORT void JNICALL
Java_com_mgg_callbackhandler_Test_nSetCompletedCallback(JNIEnv* env,
                                                        jclass clazz,
                                                        jlong nativeTest,
                                                        jobject handler,
                                                        jobject runnable) {
  Test* test = (Test*)nativeTest;
  auto* callback = JniCallback::make(env, handler, runnable);
  test->setCompletedCallback(nullptr, [callback](Test* swapChain) {
    JniCallback::postToJavaAndDestroy(callback);
  });
}

using PFN_FROMHARDWAREBUFFER = AHardwareBuffer* (*)(JNIEnv*, jobject);
static PFN_FROMHARDWAREBUFFER AHardwareBuffer_fromHardwareBuffer_fn = nullptr;
static bool sHardwareBufferSupported = true;

extern "C" JNIEXPORT void JNICALL
Java_com_mgg_callbackhandler_Test_nSetAcquiredImage(JNIEnv* env, jclass,
                                                    jlong nativeTest,
                                                    jobject hwbuffer,
                                                    jobject handler,
                                                    jobject runnable) {
  Test* test = (Test*)nativeTest;
  // This function is not available before NDK 15 or before Android 8.
  if (UTILS_UNLIKELY(!AHardwareBuffer_fromHardwareBuffer_fn)) {
    if (!sHardwareBufferSupported) {
      return;
    }
    AHardwareBuffer_fromHardwareBuffer_fn = (PFN_FROMHARDWAREBUFFER)dlsym(
        RTLD_DEFAULT, "AHardwareBuffer_fromHardwareBuffer");
    if (!AHardwareBuffer_fromHardwareBuffer_fn) {
      __android_log_print(
          ANDROID_LOG_WARN, "Filament",
          "AHardwareBuffer_fromHardwareBuffer is not available.");
      sHardwareBufferSupported = false;
      return;
    }
  }

  AHardwareBuffer* nativeBuffer =
      AHardwareBuffer_fromHardwareBuffer_fn(env, hwbuffer);
  if (!nativeBuffer) {
    __android_log_print(ANDROID_LOG_INFO, "Filament",
                        "Unable to obtain native HardwareBuffer.");
    return;
  }

  auto* callback = JniImageCallback::make(env, handler, runnable,
                                          (long)nativeBuffer);
  test->setAcquiredImage((void*) nativeBuffer,
                         callback->getHandler(), &JniImageCallback::postToJavaAndDestroy, callback);
}