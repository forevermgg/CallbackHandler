// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_FML_PLATFORM_ANDROID_JNI_UTIL_H_
#define FLUTTER_FML_PLATFORM_ANDROID_JNI_UTIL_H_

#include <jni.h>

#include <vector>

void InitJavaVM(JavaVM* vm);

// Returns a JNI environment for the current thread.
// Attaches the thread to JNI if needed.
JNIEnv* AttachCurrentThread();

void DetachFromVM();

// Sets the global Java VM instance, if it is not set yet.
// Returns true on success.
bool SetJavaVM(JNIEnv* env);

// Determines if the global Java VM instance is available.
bool HasJavaVM();

// Returns the current JNI environment.
JNIEnv* GetJNIEnv();

#endif  // FLUTTER_FML_PLATFORM_ANDROID_JNI_UTIL_H_
