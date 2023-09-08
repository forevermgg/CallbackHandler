#ifndef FOREVER_PLATFORM_ANDROID_JNI_UTIL_H_
#define FOREVER_PLATFORM_ANDROID_JNI_UTIL_H_

#include <jni.h>

#include <vector>

#include "scoped_java_ref.h"

namespace FOREVER {
namespace JNI {

// Returns a JNI environment for the current thread.
// Attaches the thread to JNI if needed.
JNIEnv* AttachCurrentThread();

// Same to AttachCurrentThread except that thread name will be set to
// |thread_name| if it is the first call. Otherwise, thread_name won't be
// changed. AttachCurrentThread() doesn't regard underlying platform thread
// name, but just resets it to "Thread-???". This function should be called
// right after new thread is created if it is important to keep thread name.
JNIEnv* AttachCurrentThreadWithName(const std::string& thread_name);

// Detaches the current thread from VM if it is attached.
void DetachFromVM();

// Initializes the global JVM.
void InitJavaVM(JavaVM* vm);

// Returns true if the global JVM has been initialized.
bool IsVMInitialized();

// Returns the global JVM, or nullptr if it has not been initialized.
JavaVM* GetVM();

// Initializes the global ClassLoader used by the GetClass and LazyGetClass
// methods. This is needed because JNI will use the base ClassLoader when there
// is no Java code on the stack. The base ClassLoader doesn't know about any of
// the application classes and will fail to lookup anything other than system
// classes.
void InitGlobalClassLoader(JNIEnv* env);

// Sets the global Java VM instance, if it is not set yet.
// Returns true on success.
bool SetJavaVM(JNIEnv* env);

// Determines if the global Java VM instance is available.
bool HasJavaVM();

// Returns the current JNI environment.
JNIEnv* GetJNIEnv();

std::string JavaStringToString(JNIEnv* env, jstring string);

ScopedJavaLocalRef<jstring> StringToJavaString(JNIEnv* env,
                                               const std::string& str);

std::vector<std::string> StringArrayToVector(JNIEnv* env, jobjectArray jargs);

std::vector<std::string> StringListToVector(JNIEnv* env, jobject list);

ScopedJavaLocalRef<jobjectArray> VectorToStringArray(
    JNIEnv* env, const std::vector<std::string>& vector);

ScopedJavaLocalRef<jobjectArray> VectorToBufferArray(
    JNIEnv* env, const std::vector<std::vector<uint8_t>>& vector);

bool HasException(JNIEnv* env);

bool ClearException(JNIEnv* env, bool silent = false);

bool CheckException(JNIEnv* env);
std::string GetJavaExceptionInfo(JNIEnv* env, jthrowable java_throwable);

}  // namespace JNI
}  // namespace FOREVER

#endif  // FOREVER_PLATFORM_ANDROID_JNI_UTIL_H_
