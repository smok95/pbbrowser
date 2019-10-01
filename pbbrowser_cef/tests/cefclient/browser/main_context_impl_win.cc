// Copyright (c) 2015 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "tests/cefclient/browser/main_context_impl.h"

#include <direct.h>
#include <shlobj.h>

namespace client {

std::wstring MainContextImpl::GetDownloadPath(const std::wstring& file_name) {
  
  PWSTR pStr = NULL;
  std::wstring path;
  if (SHGetKnownFolderPath(FOLDERID_Downloads, 0, NULL, &pStr) == S_OK) {
	  path = CefString(pStr);
	  path += L"\\";
	  path += file_name;
  }
  else {
	  wchar_t szFolderPath[MAX_PATH];
	  // Save the file in the user's "My Documents" folder.
	  if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PERSONAL | CSIDL_FLAG_CREATE, NULL,
		  0, szFolderPath))) {
		  path = CefString(szFolderPath);
		  path += L"\\";
		  path += file_name;
	  }
  }
  return path;
}

std::string MainContextImpl::GetAppWorkingDirectory() {
  char szWorkingDir[MAX_PATH + 1];
  if (_getcwd(szWorkingDir, MAX_PATH) == NULL) {
    szWorkingDir[0] = 0;
  } else {
    // Add trailing path separator.
    size_t len = strlen(szWorkingDir);
    szWorkingDir[len] = '\\';
    szWorkingDir[len + 1] = 0;
  }
  return szWorkingDir;
}

}  // namespace client
