#include <jni.h>

#include "CallbackUtils.h"
#include "Test.h"
#include "VirtualMachineEnv.h"
#include "jni_android.h"
#include "string_conversion.h"
#include "logging.h"

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
      msgFieldId = env->GetFieldID(internalOnPickCallbackClass, "msg", "Ljava/lang/String;");
    }
  } jniState(env);
  Test* test = (Test*)nativeTest;
  JniCallback* callback = JniCallback::make(env, handler, internalCallback);
  test->Consumer([callback](Test::PickingQueryResult const& result) {
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
  }, callback->getHandler());
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
  // Fatal signal 5 (SIGTRAP), code 1 (TRAP_BRKPT), fault addr 0x795fd8c3b8 in tid 10529 (FinalizerDaemon), pid 10520 (callbackhandler)
  return true;
}

extern "C" JNIEXPORT void JNICALL
Java_com_mgg_callbackhandler_Test_nProducer(JNIEnv *env, jclass clazz, jlong nativeTest) {
  Test* test = (Test*)nativeTest;
  if (test) {
    FOREVER_LOG(ERROR) << "Producer";
    test->Producer();
  }
}