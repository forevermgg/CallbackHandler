#include "jni_android.h"

#include <android/log.h>
#include <sys/prctl.h>

#include <string>

#include "logging.h"
#include "string_conversion.h"
#include "thread_local.h"

namespace FOREVER {
namespace JNI {

static JavaVM *g_jvm = nullptr;
// 替换为 std::mutex
static std::mutex g_mutex;

#define ASSERT_NO_EXCEPTION() FOREVER_CHECK(env->ExceptionCheck() == JNI_FALSE);

struct JNIDetach {
  ~JNIDetach() { DetachFromVM(); }
};

// Thread-local object that will detach from JNI during thread shutdown;
THREAD_LOCAL ThreadLocalUniquePtr<JNIDetach> tls_jni_detach;

void InitJavaVM(JavaVM *vm) {
  std::lock_guard<std::mutex> lock(g_mutex);
  // FML_DCHECK(g_jvm == nullptr);
  FOREVER_LOG(ERROR) << "InitJavaVM";
  g_jvm = vm;
}

JNIEnv *AttachCurrentThread() {
  std::lock_guard<std::mutex> lock(g_mutex);
  FOREVER_DCHECK(g_jvm != nullptr)
      << "Trying to attach to current thread without calling InitJavaVM first.";

  JNIEnv *env = nullptr;
  if (g_jvm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_4) ==
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
  FOREVER_DCHECK(JNI_OK == ret);

  FOREVER_DCHECK(tls_jni_detach.get() == nullptr);
  tls_jni_detach.reset(new JNIDetach());

  return env;
}

JNIEnv *AttachCurrentThreadWithName(const std::string &thread_name) {
  FOREVER_DCHECK(g_jvm != nullptr);
  JavaVMAttachArgs args;
  args.version = JNI_VERSION_1_2;
  args.name = const_cast<char *>(thread_name.c_str());
  args.group = nullptr;
  JNIEnv *env = nullptr;
  jint ret = g_jvm->AttachCurrentThread(&env, &args);
  FOREVER_DCHECK(JNI_OK == ret);
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

bool IsVMInitialized() { return g_jvm != nullptr; }

JavaVM *GetVM() { return g_jvm; }

class JvmThread {
 public:
  explicit JvmThread(JavaVM *jvm) {
    jvm_ = jvm;
    attached_ = false;
    jni_env_ = nullptr;
    int get_env_stat =
        jvm_->GetEnv(reinterpret_cast<void **>(&jni_env_), JNI_VERSION_1_6);
    // TODO: report the error back to Java layer.
    switch (get_env_stat) {
      case JNI_OK:
        break;
      case JNI_EDETACHED:
        FOREVER_LOG(ERROR) << "GetEnv: not attached";
        if (jvm_->AttachCurrentThread(
#ifdef __ANDROID__
                &jni_env_,
#else
                reinterpret_cast<void **>(&jni_env_),
#endif  // __ANDROID__
                nullptr) != 0) {
          FOREVER_LOG(ERROR) << "Failed to attach to java thread.";
          break;
        }
        attached_ = true;
        break;
      case JNI_EVERSION:
        FOREVER_LOG(ERROR) << "GetEnv: jni version not supported.";
        break;
      default:
        FOREVER_LOG(ERROR) << "GetEnv: unknown status.";
        break;
    }
  }

  ~JvmThread() {
    if (attached_) {
      jvm_->DetachCurrentThread();
    }
  }

  JNIEnv *GetEnv() const { return jni_env_; }

 private:
  bool attached_;
  JavaVM *jvm_;
  JNIEnv *jni_env_;
};

// Since current android abi doesn't have pthread_local, we have to rely on
// pthread functions to achieve the detachment of java thread when native thread
// exits (see: http://developer.android.com/training/articles/perf-jni.html).
static pthread_key_t jvm_thread_key;
static pthread_once_t key_once = PTHREAD_ONCE_INIT;

static void ThreadExitCallback(void *key_value) {
  JvmThread *jvm_thread = reinterpret_cast<JvmThread *>(key_value);
  // Detach the thread when thread exits.
  FOREVER_LOG(ERROR) << "Exiting thread. Detach thread.";
  delete jvm_thread;
}

void MakeKey() { pthread_key_create(&jvm_thread_key, ThreadExitCallback); }

// Returns the global Java VM instance.
JavaVM *GetJavaVM() {
  std::lock_guard<std::mutex> lock(g_mutex);
  return g_jvm;
}

bool HasJavaVM() {
  std::lock_guard<std::mutex> lock(g_mutex);
  return g_jvm != nullptr;
}

bool SetJavaVM(JNIEnv *env) {
  std::lock_guard<std::mutex> lock(g_mutex);
  if (!g_jvm) {
    if (env->GetJavaVM(&g_jvm) != JNI_OK) {
      FOREVER_LOG(ERROR) << "Can not get the Java VM instance!";
      g_jvm = nullptr;
      return false;
    }
  }
  return true;
}

JNIEnv *GetJNIEnv() {
  pthread_once(&key_once, MakeKey);
  JvmThread *jvm_thread =
      reinterpret_cast<JvmThread *>(pthread_getspecific(jvm_thread_key));
  if (jvm_thread == nullptr) {
    jvm_thread = new JvmThread(GetJavaVM());
    pthread_setspecific(jvm_thread_key, jvm_thread);
  }
  return jvm_thread->GetEnv();
}

std::string JavaStringToString(JNIEnv *env, jstring str) {
  if (env == nullptr || str == nullptr) {
    return "";
  }
  const jchar *chars = env->GetStringChars(str, NULL);
  if (chars == nullptr) {
    return "";
  }
  std::u16string u16_string(reinterpret_cast<const char16_t *>(chars),
                            env->GetStringLength(str));
  std::string u8_string = Utf16ToUtf8(u16_string);
  env->ReleaseStringChars(str, chars);
  ASSERT_NO_EXCEPTION();
  return u8_string;
}

ScopedJavaLocalRef<jstring> StringToJavaString(JNIEnv *env,
                                               const std::string &u8_string) {
  std::u16string u16_string = Utf8ToUtf16(u8_string);
  auto result = ScopedJavaLocalRef<jstring>(
      env, env->NewString(reinterpret_cast<const jchar *>(u16_string.data()),
                          u16_string.length()));
  ASSERT_NO_EXCEPTION();
  return result;
}

std::vector<std::string> StringArrayToVector(JNIEnv *env, jobjectArray array) {
  std::vector<std::string> out;
  if (env == nullptr || array == nullptr) {
    return out;
  }

  jsize length = env->GetArrayLength(array);

  if (length == -1) {
    return out;
  }

  out.resize(length);
  for (jsize i = 0; i < length; ++i) {
    ScopedJavaLocalRef<jstring> java_string(
        env, static_cast<jstring>(env->GetObjectArrayElement(array, i)));
    out[i] = JavaStringToString(env, java_string.obj());
  }

  return out;
}

std::vector<std::string> StringListToVector(JNIEnv *env, jobject list) {
  std::vector<std::string> out;
  if (env == nullptr || list == nullptr) {
    return out;
  }

  ScopedJavaLocalRef<jclass> list_clazz(env, env->FindClass("java/util/List"));
  FOREVER_DCHECK(!list_clazz.is_null());

  jmethodID list_get =
      env->GetMethodID(list_clazz.obj(), "get", "(I)Ljava/lang/Object;");
  jmethodID list_size = env->GetMethodID(list_clazz.obj(), "size", "()I");

  jint size = env->CallIntMethod(list, list_size);

  if (size == 0) {
    return out;
  }

  out.resize(size);
  for (jint i = 0; i < size; ++i) {
    ScopedJavaLocalRef<jstring> java_string(
        env, static_cast<jstring>(env->CallObjectMethod(list, list_get, i)));
    out[i] = JavaStringToString(env, java_string.obj());
  }

  return out;
}

ScopedJavaLocalRef<jobjectArray> VectorToStringArray(
    JNIEnv *env, const std::vector<std::string> &vector) {
  FOREVER_DCHECK(env);
  ScopedJavaLocalRef<jclass> string_clazz(env,
                                          env->FindClass("java/lang/String"));
  FOREVER_DCHECK(!string_clazz.is_null());
  jobjectArray java_array =
      env->NewObjectArray(vector.size(), string_clazz.obj(), NULL);
  ASSERT_NO_EXCEPTION();
  for (size_t i = 0; i < vector.size(); ++i) {
    ScopedJavaLocalRef<jstring> item = StringToJavaString(env, vector[i]);
    env->SetObjectArrayElement(java_array, i, item.obj());
  }
  return ScopedJavaLocalRef<jobjectArray>(env, java_array);
}

ScopedJavaLocalRef<jobjectArray> VectorToBufferArray(
    JNIEnv *env, const std::vector<std::vector<uint8_t>> &vector) {
  FOREVER_DCHECK(env);
  ScopedJavaLocalRef<jclass> byte_buffer_clazz(
      env, env->FindClass("java/nio/ByteBuffer"));
  FOREVER_DCHECK(!byte_buffer_clazz.is_null());
  jobjectArray java_array =
      env->NewObjectArray(vector.size(), byte_buffer_clazz.obj(), NULL);
  ASSERT_NO_EXCEPTION();
  for (size_t i = 0; i < vector.size(); ++i) {
    uint8_t *data = const_cast<uint8_t *>(vector[i].data());
    ScopedJavaLocalRef<jobject> item(
        env, env->NewDirectByteBuffer(reinterpret_cast<void *>(data),
                                      vector[i].size()));
    env->SetObjectArrayElement(java_array, i, item.obj());
  }
  return ScopedJavaLocalRef<jobjectArray>(env, java_array);
}

bool HasException(JNIEnv *env) { return env->ExceptionCheck() != JNI_FALSE; }

bool ClearException(JNIEnv *env, bool silent) {
  if (!HasException(env)) {
    return false;
  }
  if (!silent) {
    env->ExceptionDescribe();
  }
  env->ExceptionClear();
  return true;
}

bool CheckException(JNIEnv *env) {
  if (!HasException(env)) {
    return true;
  }

  jthrowable exception = env->ExceptionOccurred();
  env->ExceptionClear();
  FOREVER_LOG(ERROR) << FOREVER::JNI::GetJavaExceptionInfo(env, exception);
  env->DeleteLocalRef(exception);
  return false;
}

std::string GetJavaExceptionInfo(JNIEnv *env, jthrowable java_throwable) {
  ScopedJavaLocalRef<jclass> throwable_clazz(
      env, env->FindClass("java/lang/Throwable"));
  jmethodID throwable_printstacktrace = env->GetMethodID(
      throwable_clazz.obj(), "printStackTrace", "(Ljava/io/PrintStream;)V");

  // Create an instance of ByteArrayOutputStream.
  ScopedJavaLocalRef<jclass> bytearray_output_stream_clazz(
      env, env->FindClass("java/io/ByteArrayOutputStream"));
  jmethodID bytearray_output_stream_constructor =
      env->GetMethodID(bytearray_output_stream_clazz.obj(), "<init>", "()V");
  jmethodID bytearray_output_stream_tostring = env->GetMethodID(
      bytearray_output_stream_clazz.obj(), "toString", "()Ljava/lang/String;");
  ScopedJavaLocalRef<jobject> bytearray_output_stream(
      env, env->NewObject(bytearray_output_stream_clazz.obj(),
                          bytearray_output_stream_constructor));

  // Create an instance of PrintStream.
  ScopedJavaLocalRef<jclass> printstream_clazz(
      env, env->FindClass("java/io/PrintStream"));
  jmethodID printstream_constructor = env->GetMethodID(
      printstream_clazz.obj(), "<init>", "(Ljava/io/OutputStream;)V");
  ScopedJavaLocalRef<jobject> printstream(
      env, env->NewObject(printstream_clazz.obj(), printstream_constructor,
                          bytearray_output_stream.obj()));

  // Call Throwable.printStackTrace(PrintStream)
  env->CallVoidMethod(java_throwable, throwable_printstacktrace,
                      printstream.obj());

  // Call ByteArrayOutputStream.toString()
  ScopedJavaLocalRef<jstring> exception_string(
      env,
      static_cast<jstring>(env->CallObjectMethod(
          bytearray_output_stream.obj(), bytearray_output_stream_tostring)));
  if (ClearException(env)) {
    return "Java OOM'd in exception handling, check logcat";
  }

  return JavaStringToString(env, exception_string.obj());
}

bool CheckAndClearException(JNIEnv *env) {
  if (env->ExceptionCheck()) {
    env->ExceptionDescribe();
    env->ExceptionClear();
    return true;
  }
  return false;
}

// Converts a `java.util.List<String>` to a `std::vector<std::string>`.
std::vector<std::string> JavaStringListToStdStringVector(JNIEnv *env,
                                                         jobject list) {
  jclass cls = env->FindClass("java/util/List");
  int size = env->CallIntMethod(list, env->GetMethodID(cls, "size", "()I"));
  env->DeleteLocalRef(cls);
  if (CheckAndClearException(env)) size = 0;
  std::vector<std::string> vector(size);
  for (int i = 0; i < size; i++) {
    cls = env->FindClass("java/util/List");
    jobject element = env->CallObjectMethod(
        list, env->GetMethodID(cls, "get", "(I)Ljava/lang/Object;"), i);
    bool failed = CheckAndClearException(env);
    env->DeleteLocalRef(cls);
    if (failed) break;
    vector[i] = JavaStringToStdString(env, element);
    env->DeleteLocalRef(element);
  }
  return vector;
}

// Convert a `jstring` to a `std::string`.
std::string JavaStringToStdString(JNIEnv *env, jobject string_object) {
  if (string_object == nullptr) return "";
  const char *string_buffer =
      env->GetStringUTFChars(static_cast<jstring>(string_object), nullptr);
  std::string return_string(string_buffer);
  env->ReleaseStringUTFChars(static_cast<jstring>(string_object),
                             string_buffer);
  return return_string;
}

}  // namespace JNI
}  // namespace FOREVER
