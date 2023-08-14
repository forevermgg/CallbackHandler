// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "thread_local.h"

#include <cstring>

ThreadLocalPointer::ThreadLocalPointer(void (*destroy)(void*)) {
  (pthread_key_create(&key_, destroy) == 0);
}

ThreadLocalPointer::~ThreadLocalPointer() { (pthread_key_delete(key_) == 0); }

void* ThreadLocalPointer::get() const { return pthread_getspecific(key_); }

void* ThreadLocalPointer::swap(void* ptr) {
  void* old_ptr = get();
  pthread_setspecific(key_, ptr);
  return old_ptr;
}
