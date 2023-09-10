#include "util_android.h"

#include <assert.h>
#include <jni.h>

#include <thread>

#include "log_assert.h"
namespace FOREVER {
namespace UTIL {
// Global references to class loaders used to find classes and load embedded
// classes.
static std::vector<jobject>* g_class_loaders;
// LINT.IfChange
static const char kMissingJavaClassError[] =
    "Java class %s not found.  "
    "Please verify the AAR which contains the %s class is included "
    "in your app.";

static const char kMissingJavaMethodFieldError[] =
    "Unable to find %s.  "
    "Please verify the AAR which contains the %s class is included "
    "in your app.";

// clang-format off
#define CLASS_LOADER_METHODS(X) \
  X(LoadClass, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;"), \
  X(FindLoadedClass, "findLoadedClass",                               \
    "(Ljava/lang/String;)Ljava/lang/Class;")
// clang-format on
METHOD_LOOKUP_DECLARATION(class_loader, CLASS_LOADER_METHODS)
// ClassLoader比较常用的分为两种，PathClassLoader和DexClassLoader，虽然两者继承于BaseDexClassLoader，
// BaseDexClassLoader继承于ClassLoader，但是前者只能加载已安装的Apk里面的dex文件，后者则支持加载apk、
// dex以及jar，也可以从SD卡里面加载。
METHOD_LOOKUP_DEFINITION(class_loader, "java/lang/ClassLoader",
                         CLASS_LOADER_METHODS)

// clang-format off
#define DEX_CLASS_LOADER_METHODS(X) \
  X(Constructor, "<init>", \
    "(Ljava/lang/String;Ljava/lang/String;" \
    "Ljava/lang/String;Ljava/lang/ClassLoader;)V"), \
  X(LoadClass, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;")
// clang-format on
METHOD_LOOKUP_DECLARATION(dex_class_loader, DEX_CLASS_LOADER_METHODS)
METHOD_LOOKUP_DEFINITION(dex_class_loader,
                         PROGUARD_KEEP_CLASS "dalvik/system/DexClassLoader",
                         DEX_CLASS_LOADER_METHODS)
// clang-format off
#define THROWABLE_METHODS(X)               \
    X(GetLocalizedMessage, "getLocalizedMessage", "()Ljava/lang/String;"), \
    X(GetMessage, "getMessage", "()Ljava/lang/String;"),                   \
    X(ToString, "toString", "()Ljava/lang/String;")
// clang-format on
METHOD_LOOKUP_DECLARATION(throwable, THROWABLE_METHODS);
METHOD_LOOKUP_DEFINITION(throwable, "java/lang/Throwable", THROWABLE_METHODS)

// clang-format off
#define FOREVER_APP_METHODS(X)                                                \
  X(GetInstance, "getInstance", "()Lcom/mgg/callbackhandler/LibCheckerApp;",         \
    FOREVER::UTIL::kMethodTypeStatic)
// clang-format on

METHOD_LOOKUP_DECLARATION(app, FOREVER_APP_METHODS)
METHOD_LOOKUP_DEFINITION(app,
                         PROGUARD_KEEP_CLASS
                         "com/mgg/callbackhandler/LibCheckerApp",
                         FOREVER_APP_METHODS)

METHOD_LOOKUP_DEFINITION(activity, "android/app/Activity", ACTIVITY_METHODS)

bool LookupMethodIds(JNIEnv* env, jclass clazz,
                     const MethodNameSignature* method_name_signatures,
                     size_t number_of_method_name_signatures,
                     jmethodID* method_ids, const char* class_name) {
  FOREVER_ASSERT(method_name_signatures);
  FOREVER_ASSERT(number_of_method_name_signatures > 0);
  FOREVER_ASSERT(method_ids);
  FOREVER_ASSERT_MESSAGE_RETURN(false, clazz, kMissingJavaClassError,
                                class_name, class_name);
  FOREVER::LOG::LogDebug("Looking up methods for %s", class_name);
  for (size_t i = 0; i < number_of_method_name_signatures; ++i) {
    const MethodNameSignature& method = method_name_signatures[i];
    if (method.optional == kMethodOptional && method.name == nullptr) {
      continue;
    }
    switch (method.type) {
      case kMethodTypeInstance:
        method_ids[i] = env->GetMethodID(clazz, method.name, method.signature);
        break;
      case kMethodTypeStatic:
        method_ids[i] =
            env->GetStaticMethodID(clazz, method.name, method.signature);
        break;
    }
    if (CheckAndClearJniExceptions(env)) {
      method_ids[i] = 0;
    }
    char method_message[256];
    snprintf(method_message, sizeof(method_message),
             "Method %s.%s (signature '%s', %s)", class_name, method.name,
             method.signature,
             method.type == kMethodTypeInstance ? "instance" : "static");
    FOREVER::LOG::LogDebug(
        "%s (optional %d) 0x%08x%s", method_message,
        (method.optional == kMethodOptional) ? 1 : 0,
        static_cast<int>(reinterpret_cast<intptr_t>(method_ids[i])),
        method_ids[i] ? "" : " (not found)");
    FOREVER_ASSERT_MESSAGE_RETURN(
        false, method_ids[i] || (method.optional == kMethodOptional),
        kMissingJavaMethodFieldError, method_message, class_name);
  }
  return true;
}

// Lookup field IDs specified by the field_descriptors array and store
// in field_ids.  Used by FIELD_LOOKUP_DECLARATION.
bool LookupFieldIds(JNIEnv* env, jclass clazz,
                    const FieldDescriptor* field_descriptors,
                    size_t number_of_field_descriptors, jfieldID* field_ids,
                    const char* class_name) {
  FOREVER_ASSERT(field_descriptors);
  FOREVER_ASSERT(number_of_field_descriptors > 0);
  FOREVER_ASSERT(field_ids);
  FOREVER_ASSERT_MESSAGE_RETURN(false, clazz, kMissingJavaClassError,
                                class_name, class_name);
  FOREVER::LOG::LogDebug("Looking up fields for %s", class_name);
  for (size_t i = 0; i < number_of_field_descriptors; ++i) {
    const FieldDescriptor& field = field_descriptors[i];
    if (field.optional == kMethodOptional && field.name == nullptr) {
      continue;
    }
    switch (field.type) {
      case kFieldTypeInstance:
        field_ids[i] = env->GetFieldID(clazz, field.name, field.signature);
        break;
      case kFieldTypeStatic:
        field_ids[i] =
            env->GetStaticFieldID(clazz, field.name, field.signature);
        break;
    }
    if (CheckAndClearJniExceptions(env)) {
      field_ids[i] = 0;
    }
    char field_message[256];
    snprintf(field_message, sizeof(field_message),
             "Field %s.%s (signature '%s', %s)", class_name, field.name,
             field.signature,
             field.type == kFieldTypeInstance ? "instance" : "static");
    FOREVER::LOG::LogDebug(
        "%s (optional %d) 0x%08x%s", field_message,
        (field.optional == kMethodOptional) ? 1 : 0,
        static_cast<int>(reinterpret_cast<intptr_t>(field_ids[i])),
        field_ids[i] ? "" : " (not found)");
    FOREVER_ASSERT_MESSAGE_RETURN(
        false, field_ids[i] || (field.optional == kMethodOptional),
        kMissingJavaMethodFieldError, field_message, class_name);
  }
  return true;
}

// Find a class and retrieve a global reference to it.
// NOTE: This method will assert if the class isn't found.
jclass FindClassGlobal(
    JNIEnv* env, const std::vector<INTERNAL::EmbeddedFile>* embedded_files,
    const char* class_name, ClassRequirement optional) {
  FOREVER::LOG::LogDebug("Looking up class %s", class_name);
  jclass local_class = FindClass(env, class_name);
  FOREVER::LOG::LogDebug(
      "Class %s, lref 0x%08x", class_name,
      static_cast<int>(reinterpret_cast<intptr_t>(local_class)));
  if (!local_class) {
    if (optional == kClassRequired) {
      FOREVER::LOG::LogError(kMissingJavaClassError, class_name, class_name);
    }
    return nullptr;
  }
  jclass global_class = static_cast<jclass>(env->NewGlobalRef(local_class));
  env->DeleteLocalRef(local_class);
  FOREVER::LOG::LogDebug(
      "Class %s, gref 0x%08x", class_name,
      static_cast<int>(reinterpret_cast<intptr_t>(global_class)));
  CheckAndClearJniExceptions(env);
  if (!global_class) {
    if (optional == kClassRequired) {
      FOREVER::LOG::LogError(kMissingJavaClassError, class_name, class_name);
    }
    return nullptr;
  }
  return global_class;
}

// Try to find a class in the list of class loaders.
static jclass FindOrLoadClassFromLoaders(JNIEnv* env, const char* class_name) {
  assert(g_class_loaders);
  static const class_loader::Method kFindLoadClassMethods[] = {
      class_loader::kFindLoadedClass, class_loader::kLoadClass};
  jstring class_name_object = env->NewStringUTF(class_name);
  jclass class_object = nullptr;
  for (size_t i = 0; i < FOREVER_ARRAYSIZE(kFindLoadClassMethods); ++i) {
    for (auto it = g_class_loaders->begin();
         !class_object && it != g_class_loaders->end(); ++it) {
      class_object = static_cast<jclass>(env->CallObjectMethod(
          *it, class_loader::GetMethodId(kFindLoadClassMethods[i]),
          class_name_object));
      if (env->ExceptionCheck()) {
        env->ExceptionClear();
        class_object = nullptr;
      }
    }
  }
  env->DeleteLocalRef(class_name_object);
  return class_object;
}

// Find a class, attempting to load the class if it's not found.
jclass FindClass(JNIEnv* env, const char* class_name) {
  jclass class_object = env->FindClass(class_name);
  if (env->ExceptionCheck()) {
    env->ExceptionClear();
    // If the class isn't found it's possible NativeActivity is being used by
    // the application which means the class path is set to only load system
    // classes.  The following falls back to loading the class using the
    // Activity and Dex classes loaders before retrieving a reference to it.
    class_object = FindOrLoadClassFromLoaders(env, class_name);
  }
  return class_object;
}

// Check for JNI exceptions, report them if any, and clear them.
bool CheckAndClearJniExceptions(JNIEnv* env) {
  if (env->ExceptionCheck()) {
    env->ExceptionDescribe();
    env->ExceptionClear();
    return true;
  }
  return false;
}

// Convert a jstring (created by the JVM e.g passed into a native method)
// into
// a std::string.  Unlike JniStringToString() this does not release the
// reference to the string_object as the caller owns the object in a native
// method.
std::string JStringToString(JNIEnv* env, jobject string_object) {
  if (string_object == nullptr) return "";
  const char* string_buffer =
      env->GetStringUTFChars(static_cast<jstring>(string_object), 0);
  std::string return_string(string_buffer);
  env->ReleaseStringUTFChars(static_cast<jstring>(string_object),
                             string_buffer);
  return return_string;
}

// Convert a jstring to a std::string, releasing the reference to the
// jstring.
std::string JniStringToString(JNIEnv* env, jobject string_object) {
  std::string return_string = JStringToString(env, string_object);
  env->DeleteLocalRef(string_object);
  return return_string;
}

std::string GetAndClearExceptionMessage(JNIEnv* env) {
  jobject exception = env->ExceptionOccurred();
  if (exception) {
    env->ExceptionClear();
    std::string exception_message_out = GetMessageFromException(env, exception);
    env->DeleteLocalRef(exception);
    return exception_message_out;
  }
  return std::string();
}

std::string GetMessageFromException(JNIEnv* env, jobject exception) {
  if (exception != nullptr) {
    jstring message = static_cast<jstring>(env->CallObjectMethod(
        exception, throwable::GetMethodId(throwable::kGetLocalizedMessage)));
    CheckAndClearJniExceptions(env);
    if (!message) {
      // If GetLocalizedMessage returns null, try GetMessage.
      message = static_cast<jstring>(env->CallObjectMethod(
          exception, throwable::GetMethodId(throwable::kGetMessage)));
      CheckAndClearJniExceptions(env);
    }
    if (!message || env->GetStringUTFLength(message) == 0) {
      // If GetMessage returns null, just use ToString.
      if (message) {
        // If it was an empty string, we'll need to free the message ref.
        env->DeleteLocalRef(message);
      }
      message = static_cast<jstring>(env->CallObjectMethod(
          exception, throwable::GetMethodId(throwable::kToString)));
      CheckAndClearJniExceptions(env);
    }
    if (message) {
      return JniStringToString(env, message);
    } else {
      return std::string("Unknown Exception.");
    }
  }
  return std::string();
}

bool LogException(JNIEnv* env, FOREVER::LOG::LogLevel log_level,
                  const char* log_fmt, ...) {
  jobject exception = env->ExceptionOccurred();
  if (exception != nullptr) {
    env->ExceptionClear();
    jobject message = env->CallObjectMethod(
        exception, throwable::GetMethodId(throwable::kGetLocalizedMessage));
    CheckAndClearJniExceptions(env);
    if (!message) {
      // If GetLocalizedMessage returns null, try GetMessage.
      message = env->CallObjectMethod(
          exception, throwable::GetMethodId(throwable::kGetMessage));
      CheckAndClearJniExceptions(env);
    }
    if (!message) {
      // If GetMessage returns null too, just use ToString.
      message = env->CallObjectMethod(
          exception, throwable::GetMethodId(throwable::kToString));
      CheckAndClearJniExceptions(env);
    }
    if (message) {
      std::string message_str = JniStringToString(env, message);
      if (log_fmt == nullptr) {
        FOREVER::LOG::LogMessage(log_level, "%s", message_str.c_str());
      } else {
        static char buf[512];
        va_list list;
        va_start(list, log_fmt);
        vsnprintf(buf, sizeof(buf) - 1, log_fmt, list);
        va_end(list);
        strncat(buf, ": ", sizeof(buf) - 1);
        strncat(buf, message_str.c_str(), sizeof(buf) - 1);
        FOREVER::LOG::LogMessage(log_level, "%s", buf);
      }
    }
    env->DeleteLocalRef(exception);
    return true;
  }
  return false;
}
// Static variables used in tracking thread initialization and cleanup.
pthread_key_t jni_env_key;
pthread_once_t pthread_key_initialized = PTHREAD_ONCE_INIT;

// The following methods are extern "C" to ensure they're called with the
// C rather than C++ linkage convention.
namespace {

extern "C" void DetachJVMThreads(void* stored_java_vm) {
  assert(stored_java_vm);
  JNIEnv* jni_env;
  JavaVM* java_vm = static_cast<JavaVM*>(stored_java_vm);
  // AttachCurrentThread does nothing if we're already attached, but
  // calling it ensures that the DetachCurrentThread doesn't fail.
  FOREVER::UTIL::AttachCurrentThread(java_vm, &jni_env);
  java_vm->DetachCurrentThread();
}

// Called the first time GetJNIEnv is invoked.
// Ensures that jni_env_key is created and that the destructor is in place.
extern "C" void SetupJvmDetachOnThreadDestruction() {
  pthread_key_create(&jni_env_key, DetachJVMThreads);
}

}  // namespace

JNIEnv* GetThreadsafeJNIEnv(JavaVM* java_vm) {
  // Set up the thread key and destructor the first time this is called:
  (void)pthread_once(&pthread_key_initialized,
                     SetupJvmDetachOnThreadDestruction);
  pthread_setspecific(jni_env_key, java_vm);

  JNIEnv* env;
  jint result = FOREVER::UTIL::AttachCurrentThread(java_vm, &env);
  return result == JNI_OK ? env : nullptr;
}

jint AttachCurrentThread(JavaVM* java_vm, JNIEnv** env) {
  return java_vm->AttachCurrentThread(env, nullptr);
}

// Find an Android SDK FirebaseApp instance by name.
// Returns a local jobject reference if successful, nullptr otherwise.
jobject GetPlatformAppByName(JNIEnv* jni_env) {
  jobject platform_app;
  platform_app = jni_env->CallStaticObjectMethod(
      app::GetClass(), app::GetMethodId(app::kGetInstance));
  if (jni_env->ExceptionCheck()) {
    // Explicitly set `platform_app` to `NULL` if an exception was thrown
    // because on KitKat (API 19) `CallStaticObjectMethod()` may return garbage
    // instead of `NULL` if an exception was thrown, and callers of this
    // function expect `NULL` to be returned if the app was not found.
    platform_app = NULL;  // NOLINT
  }
  jni_env->ExceptionClear();
  return platform_app;
}

// Release cached classes.
void ReleaseClasses(JNIEnv* env) {
  throwable::ReleaseClass(env);
  app::ReleaseClass(env);
}

bool Initialize(JNIEnv* env) {
  throwable::CacheMethodIds(env);
  app::CacheMethodIds(env);
  return true;
}
}  // namespace UTIL
// NOLINTNEXTLINE - allow namespace overridden
}  // namespace FOREVER