// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

// Defines all of the command line switches used by cefclient.

#ifndef CEF_TESTS_SHARED_SHARED_COMMON_SWITCHES_H_
#define CEF_TESTS_SHARED_SHARED_COMMON_SWITCHES_H_
#pragma once

namespace client {
namespace switches {

extern const char kMultiThreadedMessageLoop[];
extern const char kExternalMessagePump[];
extern const char kCachePath[];
extern const char kUrl[];
extern const char kOffScreenRenderingEnabled[];
extern const char kOffScreenFrameRate[];
extern const char kTransparentPaintingEnabled[];
extern const char kShowUpdateRect[];
extern const char kSharedTextureEnabled[];
extern const char kExternalBeginFrameEnabled[];
extern const char kMouseCursorChangeDisabled[];
extern const char kRequestContextPerBrowser[];
extern const char kRequestContextSharedCache[];
extern const char kBackgroundColor[];
extern const char kEnableGPU[];
extern const char kFilterURL[];
extern const char kUseViews[];
extern const char kHideFrame[];
extern const char kShowControls[];
extern const char kAlwaysOnTop[];
extern const char kHideTopMenu[];
extern const char kWidevineCdmPath[];
extern const char kSslClientCertificate[];
extern const char kCRLSetsPath[];
extern const char kLoadExtension[];
extern const char kNoActivate[];
extern const char kConsoleLogFile[];	// console.log message를 파일에 기록, 2019.10.01 kim,jk
extern const char kDebugMode[];			// debug mode, 2019.10.01 kim,jk
extern const char kWidth[];				// 초기 윈도우창 너비, 2019.10.24 kim,jk
extern const char kHeight[];			// 초기 윈도우창 높이, 2019.10.24 kim,jk

}  // namespace switches
}  // namespace client

#endif  // CEF_TESTS_SHARED_SHARED_COMMON_SWITCHES_H_
