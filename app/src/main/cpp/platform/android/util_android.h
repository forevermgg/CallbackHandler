#include <jni.h>

#include <string>

#include "embedded_file.h"
#include "log.h"
// To ensure that Proguard doesn't strip the classes you're using, place this
// string directly before the JNI class string in your METHOD_LOOKUP_DEFINITION
// macro invocation.
#define PROGUARD_KEEP_CLASS "%PG%"

namespace FOREVER {
typedef ::JNINativeMethod JNINativeMethod;
// Do nothing. Caller does not actually own the result but can call the no-op
// CleanUpConvertedJNINativeMethod() to match the other branch.
inline const ::JNINativeMethod* ConvertJNINativeMethod(
    const JNINativeMethod* source, size_t number_of_native_methods) {
  // We avoid copying and use the source directly.
  return source;
}
inline void CleanUpConvertedJNINativeMethod(const ::JNINativeMethod* target) {
  // Dothing, as caller does not own it.
  return;
}

namespace UTIL {
// Type of a Java method.
enum MethodType {
  kMethodTypeInstance,  // Called on an instance of a class.
  kMethodTypeStatic,    // Static method of a class.
};

// Whether a Java method (or field) is required or optional.
enum MethodRequirement {
  kMethodRequired,  // The method is required, an assert will occur if you try
  // to look it up and it doesn't exist.
  kMethodOptional,  // The method is optional, no error will occur if you try
                    // to look it up and it doesn't exist.
};

// Name and signature of a class method.
struct MethodNameSignature {
  const char* name;       // Name of the method e.g java/lang/Object
  const char* signature;  // JNI signature of the method e.g "()I"
  const MethodType type;
  const MethodRequirement optional;  // Whether lookup of this method should
                                     // fail silently.
};

// Whether this is instance or static field in a class.
enum FieldType {
  kFieldTypeInstance,  // Field on an instance of a class.
  kFieldTypeStatic,    // Static field of a class.
};

// Name and type of a class field.
struct FieldDescriptor {
  const char* name;       // Name of the field.
  const char* signature;  // JNI string that describes the type.
  const FieldType type;
  const MethodRequirement optional;  // Whether lookup of this field should fail
                                     // silently.
};

// Whether a Java class is required or optional.
enum ClassRequirement {
  kClassRequired,  // The class is required, an error will be logged if you
  // try to look it up and it doesn't exist.
  kClassOptional,  // The class is optional, no error be logged if you try
                   // to look it up and it doesn't exist.
};

// Lookup method IDs specified by the method_name_signatures array and store
// in method_ids.  Used by METHOD_LOOKUP_DECLARATION.
bool LookupMethodIds(JNIEnv* env, jclass clazz,
                     const MethodNameSignature* method_name_signatures,
                     size_t number_of_method_name_signatures,
                     jmethodID* method_ids, const char* class_name);

// Lookup field IDs specified by the field_descriptors array and store
// in field_ids.  Used by FIELD_LOOKUP_DECLARATION.
bool LookupFieldIds(JNIEnv* env, jclass clazz,
                    const FieldDescriptor* field_descriptors,
                    size_t number_of_field_descriptors, jfieldID* field_ids,
                    const char* class_name);

// Used to make METHOD_ID and METHOD_NAME_SIGNATURE macros variadic.
#define METHOD_NAME_INFO_EXPANDER(arg0, arg1, arg2, arg3, arg4, arg5, \
                                  function, ...)                      \
  function

// Used to populate an array of MethodNameSignature.
#define METHOD_NAME_SIGNATURE_5(id, name, signature, method_type, optional) \
  { name, signature, method_type, optional }
#define METHOD_NAME_SIGNATURE_4(id, name, signature, method_type) \
  METHOD_NAME_SIGNATURE_5(id, name, signature, method_type,       \
                          ::FOREVER::UTIL::MethodRequirement::kMethodRequired)
#define METHOD_NAME_SIGNATURE_3(id, name, signature) \
  METHOD_NAME_SIGNATURE_4(id, name, signature,       \
                          ::FOREVER::UTIL::kMethodTypeInstance)
#define METHOD_NAME_SIGNATURE(...)                                        \
  METHOD_NAME_INFO_EXPANDER(, ##__VA_ARGS__,                              \
                            METHOD_NAME_SIGNATURE_5(__VA_ARGS__),         \
                            METHOD_NAME_SIGNATURE_4(__VA_ARGS__),         \
                            METHOD_NAME_SIGNATURE_3(__VA_ARGS__),         \
                            numargs2(__VA_ARGS__), numargs1(__VA_ARGS__), \
                            /* Generate nothing with no args. */)

// Used to populate an enum of method identifiers.
#define METHOD_ID_3(id, name, signature) k##id
#define METHOD_ID_4(id, name, signature, method_type) k##id
#define METHOD_ID_5(id, name, signature, method_type, optional) k##id
#define METHOD_ID(...)                                                        \
  METHOD_NAME_INFO_EXPANDER(                                                  \
      , ##__VA_ARGS__, METHOD_ID_5(__VA_ARGS__), METHOD_ID_4(__VA_ARGS__),    \
      METHOD_ID_3(__VA_ARGS__), numargs2(__VA_ARGS__), numargs1(__VA_ARGS__), \
      /* Generate nothing with no args. */)

// Used to populate FieldDescriptor
#define FIELD_DESCRIPTOR(...) METHOD_NAME_SIGNATURE(__VA_ARGS__)
// Used to populate an enum of field identifiers.
#define FIELD_ID(...) METHOD_ID(__VA_ARGS__)

// Used with METHOD_LOOKUP_DECLARATION to generate no method lookups.
#define METHOD_LOOKUP_NONE(X)                       \
  X(InvalidMethod, nullptr, nullptr,                \
    ::FOREVER::UTIL::MethodType::kMethodTypeStatic, \
    ::FOREVER::UTIL::MethodRequirement::kMethodOptional)

// Used with METHOD_LOOKUP_DECLARATION to generate no field lookups.
#define FIELD_LOOKUP_NONE(X)                      \
  X(InvalidField, nullptr, nullptr,               \
    ::FOREVER::UTIL::FieldType::kFieldTypeStatic, \
    ::FOREVER::UTIL::MethodRequirement::kMethodOptional)

// Declares a namespace which caches class method IDs.
// To make cached method IDs available in to other files or projects, this macro
// must be defined in a header file. If the method IDs are local to a specific
// file however, this can be placed in the source file. Regardless of whether
// this is placed in the header or not, you must also add
// METHOD_LOOKUP_DEFINITION to the source file.
// clang-format off
#define METHOD_LOOKUP_DECLARATION_3(namespace_identifier,                     \
                                    method_descriptor_macro,                  \
                                    field_descriptor_macro)                   \
  namespace namespace_identifier {                                            \
                                                                              \
  enum Method {                                                               \
    method_descriptor_macro(METHOD_ID),                                       \
    kMethodCount                                                              \
  };                                                                          \
                                                                              \
  enum Field {                                                                \
    field_descriptor_macro(FIELD_ID),                                         \
    kFieldCount                                                               \
  };                                                                          \
                                                                              \
  /* Find and hold a reference to this namespace's class, optionally */       \
  /* searching a set of files for the class. */                               \
  /* This specified file list must have previously been cached using */       \
  /* CacheEmbeddedFiles(). */                                                 \
  jclass CacheClassFromFiles(                                                 \
      JNIEnv *env,                                                            \
      const std::vector<::FOREVER::INTERNAL::EmbeddedFile>*                   \
          embedded_files);                                                    \
                                                                              \
  /* Find and hold a reference to this namespace's class. */                  \
  jclass CacheClass(JNIEnv *env);                                             \
                                                                              \
  /* Get the cached class associated with this namespace. */                  \
  jclass GetClass();                                                          \
                                                                              \
  /* Register native methods on the class associated with this namespace. */  \
  bool RegisterNatives(JNIEnv* env, const JNINativeMethod* native_methods,    \
                       size_t number_of_native_methods);                      \
                                                                              \
  /* Release the cached class reference. */                                   \
  void ReleaseClass(JNIEnv *env);                                             \
                                                                              \
  /* See LookupMethodIds() */                                                 \
  bool CacheMethodIds(JNIEnv *env);                                           \
                                                                              \
  /* Lookup a method ID using a Method enum value. */                         \
  jmethodID GetMethodId(Method method);                                       \
                                                                              \
  /* See LookupFieldIds() */                                                  \
  bool CacheFieldIds(JNIEnv *env);                                            \
                                                                              \
  /* Lookup a field ID using a Field enum value. */                           \
  jfieldID GetFieldId(Field field);                                           \
                                                                              \
  }  // namespace namespace_identifier

#define METHOD_LOOKUP_DECLARATION_2(namespace_identifier,                     \
                                    method_descriptor_macro)                  \
  METHOD_LOOKUP_DECLARATION_3(namespace_identifier, method_descriptor_macro,  \
                              FIELD_LOOKUP_NONE)

// Used to make METHOD_LOOKUP_DECLARATION variadic.
#define METHOD_LOOKUP_DECLARATION_EXPANDER(arg0, arg1, arg2, arg3,            \
                                           function, ...)                     \
  function

#define METHOD_LOOKUP_DECLARATION(...)                                        \
  METHOD_LOOKUP_DECLARATION_EXPANDER(                                         \
      , ##__VA_ARGS__, METHOD_LOOKUP_DECLARATION_3(__VA_ARGS__),              \
      METHOD_LOOKUP_DECLARATION_2(__VA_ARGS__), numargs1(__VA_ARGS__),        \
      numargs0(__VA_ARGS__))

// Defines a namespace which caches class method IDs.
// To cache class method IDs, you must first declare them in either the header
// or source file with METHOD_LOOKUP_DECLARATION. Regardless of whether the
// declaration is in a header or source file, this macro must be called from a
// source file.
// clang-format off
#define METHOD_LOOKUP_DEFINITION_4(namespace_identifier,                      \
                                   class_name, method_descriptor_macro,       \
                                   field_descriptor_macro)                    \
  namespace namespace_identifier {                                            \
                                                                              \
  /* Skip optional "%PG%" at the beginning of the string, if present. */      \
  static const char* kClassName =                                             \
      class_name[0] == '%' ? &class_name[4] : class_name;                     \
                                                                              \
  static const ::FOREVER::UTIL::MethodNameSignature                           \
      kMethodSignatures[] = {                                                 \
    method_descriptor_macro(METHOD_NAME_SIGNATURE),                           \
  };                                                                          \
                                                                              \
  static const ::FOREVER::UTIL::FieldDescriptor                               \
      kFieldDescriptors[] = {                                                 \
    field_descriptor_macro(FIELD_DESCRIPTOR),                                 \
  };                                                                          \
                                                                              \
  static jmethodID g_method_ids[kMethodCount];                                \
  static jfieldID g_field_ids[kFieldCount];                                   \
                                                                              \
  static jclass g_class = nullptr;                                            \
  static bool g_registered_natives = false;                                   \
                                                                              \
  /* Find and hold a reference to this namespace's class, optionally */       \
  /* searching a set of files for the class. */                               \
  /* This specified file list must have previously been cached using */       \
  /* CacheEmbeddedFiles(). */                                                 \
  /* If optional == kClassOptional, no errors will be emitted if the class */ \
  /* does not exist. */                                                       \
  jclass CacheClassFromFiles(                                                 \
      JNIEnv *env,                                                            \
      const std::vector<::FOREVER::INTERNAL::EmbeddedFile>*                   \
          embedded_files,                                                     \
      ::FOREVER::UTIL::ClassRequirement optional) {                           \
    if (!g_class) {                                                           \
      g_class = ::FOREVER::UTIL::FindClassGlobal(                             \
          env, embedded_files, kClassName, optional);                         \
    }                                                                         \
    return g_class;                                                           \
  }                                                                           \
                                                                              \
  jclass CacheClassFromFiles(                                                 \
      JNIEnv *env,                                                            \
      const std::vector<::FOREVER::INTERNAL::EmbeddedFile>*                   \
          embedded_files) {                                                   \
    return CacheClassFromFiles(                                               \
        env, embedded_files,                                                  \
        ::FOREVER::UTIL::ClassRequirement::kClassRequired);                   \
  }                                                                           \
                                                                              \
  /* Find and hold a reference to this namespace's class. */                  \
  jclass CacheClass(JNIEnv* env,                      \
                    ::FOREVER::UTIL::ClassRequirement optional) {  \
    return CacheClassFromFiles(env, nullptr, optional);      \
  }                                                                           \
                                                                              \
  /* Find and hold a reference to this namespace's class. */                  \
  jclass CacheClass(JNIEnv* env) {                   \
    return CacheClassFromFiles(                                               \
        env, nullptr,                                        \
        ::FOREVER::UTIL::ClassRequirement::kClassRequired);        \
  }                                                                           \
                                                                              \
  /* Get the cached class associated with this namespace. */                  \
  jclass GetClass() { return g_class; }                                       \
                                                                              \
  bool RegisterNatives(JNIEnv* env, const JNINativeMethod* native_methods,    \
                       size_t number_of_native_methods) {                     \
    if (g_registered_natives) return false;                                   \
    const ::JNINativeMethod* true_native_methods =                            \
        FOREVER::ConvertJNINativeMethod(native_methods,            \
                                         number_of_native_methods);           \
    const jint register_status = env->RegisterNatives(                        \
      GetClass(), true_native_methods, number_of_native_methods);             \
    FOREVER::CleanUpConvertedJNINativeMethod(true_native_methods); \
    FOREVER::UTIL::CheckAndClearJniExceptions(env);                \
    g_registered_natives = register_status == JNI_OK;                         \
    return g_registered_natives;                                              \
  }                                                                           \
                                                                              \
  /* Release the cached class reference. */                                   \
  void ReleaseClass(JNIEnv* env) {                                            \
    if (g_class) {                                                            \
      if (g_registered_natives) {                                             \
        env->UnregisterNatives(g_class);                                      \
        g_registered_natives = false;                                         \
      }                                                                       \
      FOREVER::UTIL::CheckAndClearJniExceptions(env);              \
      env->DeleteGlobalRef(g_class);                                          \
      g_class = nullptr;                                                      \
    }                                                                         \
  }                                                                           \
                                                                              \
  /* See LookupMethodIds() */                                                 \
  /* If the class is being loaded from an embedded file use */                \
  /* CacheClassFromFiles() before calling this function to cache method */    \
  /* IDs. */                                                                  \
  bool CacheMethodIds(JNIEnv* env) {                 \
    return ::FOREVER::UTIL::LookupMethodIds(                       \
        env, CacheClass(env), kMethodSignatures,             \
        kMethodCount, g_method_ids, kClassName);                              \
  }                                                                           \
                                                                              \
  /* Lookup a method ID using a Method enum value. */                         \
  jmethodID GetMethodId(Method method) {                                      \
    FOREVER_ASSERT(method < kMethodCount);                                   \
    jmethodID method_id = g_method_ids[method];                               \
    return method_id;                                                         \
  }                                                                           \
                                                                              \
  /* See LookupFieldIds() */                                                  \
  /* If the class is being loaded from an embedded file use */                \
  /* CacheClassFromFiles() before calling this function to cache field */     \
  /* IDs. */                                                                  \
  bool CacheFieldIds(JNIEnv* env) {                  \
    return ::FOREVER::UTIL::LookupFieldIds(                        \
        env, CacheClass(env),                                \
        kFieldDescriptors, kFieldCount,                                       \
        g_field_ids, kClassName);                                             \
  }                                                                           \
                                                                              \
  /* Lookup a field ID using a Field enum value. */                           \
  jfieldID GetFieldId(Field field) {                                          \
    FOREVER_ASSERT(field < kFieldCount);                                     \
    jfieldID field_id = g_field_ids[field];                                   \
    return field_id;                                                          \
  }                                                                           \
                                                                              \
  }  // namespace namespace_identifier
// clang-format on

#define METHOD_LOOKUP_DEFINITION_3(namespace_identifier, class_name, \
                                   method_descriptor_macro)          \
  METHOD_LOOKUP_DEFINITION_4(namespace_identifier, class_name,       \
                             method_descriptor_macro, FIELD_LOOKUP_NONE)

// Used to make METHOD_LOOKUP_DEFINITION variadic.
#define METHOD_LOOKUP_DEFINITION_EXPANDER(arg0, arg1, arg2, arg3, arg4, \
                                          function, ...)                \
  function

#define METHOD_LOOKUP_DEFINITION(...)                                 \
  METHOD_LOOKUP_DEFINITION_EXPANDER(                                  \
      , ##__VA_ARGS__, METHOD_LOOKUP_DEFINITION_4(__VA_ARGS__),       \
      METHOD_LOOKUP_DEFINITION_3(__VA_ARGS__), numargs2(__VA_ARGS__), \
      numargs1(__VA_ARGS__), numargs0(__VA_ARGS))

// Used to call android.app.Activity methods.
// clang-format off
#define ACTIVITY_BASE_METHODS(X)                                        \
  X(GetApplicationContext, "getApplicationContext",                     \
  "()Landroid/content/Context;"),                                       \
  X(GetCacheDir, "getCacheDir", "()Ljava/io/File;"),                    \
  X(GetClassLoader, "getClassLoader", "()Ljava/lang/ClassLoader;"),     \
  X(GetIntent, "getIntent", "()Landroid/content/Intent;"),              \
  X(GetPackageName, "getPackageName", "()Ljava/lang/String;"),          \
  X(GetResources, "getResources", "()Landroid/content/res/Resources;"), \
  X(Finish, "finish", "()V"),                                           \
  X(GetContentResolver, "getContentResolver",                           \
    "()Landroid/content/ContentResolver;"),                             \
  X(GetString, "getString", "(I)Ljava/lang/String;")
#define ACTIVITY_METHODS(X)                                             \
  ACTIVITY_BASE_METHODS(X),                                             \
  X(GetCodeCacheDir, "getCodeCacheDir", "()Ljava/io/File;",             \
    UTIL::kMethodTypeInstance, UTIL::kMethodOptional)
// clang-format on
METHOD_LOOKUP_DECLARATION(activity, ACTIVITY_METHODS)

// Find a class and retrieve a global reference to it.
// If a set of files is provided and the class isn't found in the default class
// path, the files will be searched for the class and loaded.
// NOTE: This method will log an error if the class isn't found unless
// optional == kClassOptional.
jclass FindClassGlobal(
    JNIEnv* env, const std::vector<INTERNAL::EmbeddedFile>* embedded_files,
    const char* class_name, ClassRequirement optional);

// Find a class, attempting to load the class if it's not found.
jclass FindClass(JNIEnv* env, const char* class_name);

// Convert a jstring to a std::string, releasing the reference to the jstring.
std::string JniStringToString(JNIEnv* env, jobject string_object);

// Convert a jstring (created by the JVM e.g passed into a native method) into
// a std::string.  Unlike JniStringToString() this does not release the
// reference to the string_object as the caller owns the object in a native
// method.
std::string JStringToString(JNIEnv* env, jobject string_object);

// Check for JNI exceptions, report them if any, and clear them. Returns true
// if there was an exception, false if not.
bool CheckAndClearJniExceptions(JNIEnv* env);

// If an JNI exception occurred, this function will return the message
// and clear the exception.
// Otherwise, it will return an empty string.
std::string GetAndClearExceptionMessage(JNIEnv* env);

// Returns the message from an exception.
// This does not clear the JNI exception state.
std::string GetMessageFromException(JNIEnv* env, jobject exception);

// If an exception occurred, log its message with the given log level and an
// optional log message. If you specify a log_fmt, the given log message will
// displayed if an exception occurred, followed by a colon and then the
// exception's localized message. If you don't specify a log_fmt, the exception
// message will be displayed by itself.
//
// If an exception occurred, this function will clear it and return true.
// Otherwise it will return false.
bool LogException(JNIEnv* env, LOG::LogLevel log_level = LOG::kLogLevelError,
                  const char* log_fmt = nullptr, ...);
// Returns a pointer to the JNI environment.  Also, registers a destructor
// to automatically detach the thread from the JVM when it exits.  (Failure
// to detach threads will cause errors when exiting otherwise.)
JNIEnv* GetThreadsafeJNIEnv(JavaVM* java_vm);

// Attaches the current thread to a Java VM. Returns a JNI interface pointer in
// the JNIEnv argument. This deals with the different parameter requirements in
// JNI call, where NDK asks for JNIEnv** while JDK asks for void**.
//
// Users should use GetThreadsafeJNIEnv() instead to ensure they detach the JNI
// environment from the thread when exiting.
jint AttachCurrentThread(JavaVM* java_vm, JNIEnv** env);

void ReleaseClasses(JNIEnv* env);
bool Initialize(JNIEnv* env);
}  // namespace UTIL
// NOLINTNEXTLINE - allow namespace overridden
}  // namespace FOREVER