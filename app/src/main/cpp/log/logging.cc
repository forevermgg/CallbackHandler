// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "logging.h"

#include <android/log.h>

#include <algorithm>
#include <cstring>
#include <iostream>

#include "log_settings.h"

namespace fml {

namespace {
const char* const kLogSeverityNames[LOG_NUM_SEVERITIES] = {"INFO", "WARNING",
                                                           "ERROR", "FATAL"};

const char* GetNameForLogSeverity(LogSeverity severity) {
  if (severity >= LOG_INFO && severity < LOG_NUM_SEVERITIES) {
    return kLogSeverityNames[severity];
  }
  return "UNKNOWN";
}

const char* StripDots(const char* path) {
  while (strncmp(path, "../", 3) == 0) {
    path += 3;
  }
  return path;
}

const char* StripPath(const char* path) {
  auto* p = strrchr(path, '/');
  if (p) {
    return p + 1;
  }
  return path;
}
}  // namespace

LogMessage::LogMessage(LogSeverity severity, const char* file, int line,
                       const char* condition)
    : severity_(severity), file_(file), line_(line) {
  stream_ << "[";
  if (severity >= LOG_INFO) {
    stream_ << GetNameForLogSeverity(severity);
  } else {
    stream_ << "VERBOSE" << -severity;
  }
  stream_ << ":" << (severity > LOG_INFO ? StripDots(file_) : StripPath(file_))
          << "(" << line_ << ")] ";
  if (condition) {
    stream_ << "Check failed: " << condition << ". ";
  }
}

// static
thread_local std::ostringstream* LogMessage::capture_next_log_stream_ = nullptr;

namespace testing {

LogCapture::LogCapture() { fml::LogMessage::CaptureNextLog(&stream_); }

LogCapture::~LogCapture() { fml::LogMessage::CaptureNextLog(nullptr); }

std::string LogCapture::str() const { return stream_.str(); }

}  // namespace testing

// static
void LogMessage::CaptureNextLog(std::ostringstream* stream) {
  LogMessage::capture_next_log_stream_ = stream;
}

LogMessage::~LogMessage() {
  stream_ << std::endl;
  if (capture_next_log_stream_) {
    *capture_next_log_stream_ << stream_.str();
    capture_next_log_stream_ = nullptr;
  } else {
    android_LogPriority priority =
        (severity_ < 0) ? ANDROID_LOG_VERBOSE : ANDROID_LOG_UNKNOWN;
    switch (severity_) {
      case LOG_INFO:
        priority = ANDROID_LOG_INFO;
        break;
      case LOG_WARNING:
        priority = ANDROID_LOG_WARN;
        break;
      case LOG_ERROR:
        priority = ANDROID_LOG_ERROR;
        break;
      case LOG_FATAL:
        priority = ANDROID_LOG_FATAL;
        break;
    }
    __android_log_write(priority, "Forever", stream_.str().c_str());
  }

  if (severity_ >= LOG_FATAL) {
    KillProcess();
  }
}

int GetVlogVerbosity() { return std::max(-1, LOG_INFO - GetMinLogLevel()); }

bool ShouldCreateLogMessage(LogSeverity severity) {
  return severity >= GetMinLogLevel();
}

void KillProcess() { abort(); }

}  // namespace fml
