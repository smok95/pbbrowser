// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "pbbrowser_handler.h"

#include <sstream>
#include <string>

#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"

namespace {

PBBrowserHandler* g_instance = NULL;

}  // namespace

PBBrowserHandler::PBBrowserHandler(bool use_views)
    : use_views_(use_views), is_closing_(false) {
  DCHECK(!g_instance);
  g_instance = this;
}

PBBrowserHandler::~PBBrowserHandler() {
  g_instance = NULL;
}

// static
PBBrowserHandler* PBBrowserHandler::GetInstance() {
  return g_instance;
}

void PBBrowserHandler::OnTitleChange(CefRefPtr<CefBrowser> browser,
                                  const CefString& title) {
  CEF_REQUIRE_UI_THREAD();

  if (use_views_) {
    // Set the title of the window using the Views framework.
    CefRefPtr<CefBrowserView> browser_view =
        CefBrowserView::GetForBrowser(browser);
    if (browser_view) {
      CefRefPtr<CefWindow> window = browser_view->GetWindow();
      if (window)
        window->SetTitle(title);
    }
  } else {
    // Set the title of the window using platform APIs.
    PlatformTitleChange(browser, title);
  }
}

void PBBrowserHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();

  // Add to the list of existing browsers.
  browser_list_.push_back(browser);
}

bool PBBrowserHandler::DoClose(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();

  // Closing the main window requires special handling. See the DoClose()
  // documentation in the CEF header for a detailed destription of this
  // process.
  if (browser_list_.size() == 1) {
    // Set a flag to indicate that the window close should be allowed.
    is_closing_ = true;
  }

  // Allow the close. For windowed browsers this will result in the OS close
  // event being sent.
  return false;
}

void PBBrowserHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();

  // Remove from the list of existing browsers.
  BrowserList::iterator bit = browser_list_.begin();
  for (; bit != browser_list_.end(); ++bit) {
    if ((*bit)->IsSame(browser)) {
      browser_list_.erase(bit);
      break;
    }
  }

  if (browser_list_.empty()) {
    // All browser windows have closed. Quit the application message loop.
    CefQuitMessageLoop();
  }
}

void PBBrowserHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                ErrorCode errorCode,
                                const CefString& errorText,
                                const CefString& failedUrl) {
  CEF_REQUIRE_UI_THREAD();

  // Don't display an error for downloaded files.
  if (errorCode == ERR_ABORTED)
    return;

  // Display a load error message.
  std::stringstream ss;
  ss << "<html><body bgcolor=\"white\">"
        "<h2>Failed to load URL "
     << std::string(failedUrl) << " with error " << std::string(errorText)
     << " (" << errorCode << ").</h2></body></html>";
  frame->LoadString(ss.str(), failedUrl);
}

bool PBBrowserHandler::OnJSDialog(CefRefPtr<CefBrowser> browser,
	const CefString& origin_url,
	JSDialogType dialog_type,
	const CefString& message_text,
	const CefString& default_prompt_text,
	CefRefPtr<CefJSDialogCallback> callback,
	bool& suppress_message) {
	
	CEF_REQUIRE_UI_THREAD()
	switch (dialog_type)
	{
	case JSDIALOGTYPE_ALERT:
	{
		MessageBox(browser->GetHost()->GetWindowHandle(), message_text.c_str(), L"PBBrowser", MB_OK);		
		callback->Continue(true, "");

		/*	2019.09.27 kim,jk
		MessageBox사용시 alert표시 후 창을 닫으면 웹페이지 화면내의 input컨트롤이 입력이 되지 않는 문제가 발생함.
		다른 프로세스창으로 포커스를 줬다가 다시 돌아오면 정상적으로 동작하는 것으로 확인되었음.
		아래는 편법을 이용한 코드로 작업표시줄로 포커스를 설정했다가 다시 브라우저로 포커스를 설정한다.
		*/
		if (HWND hwndTaskbar = FindWindow(L"Shell_TrayWnd", NULL)) {
			SetFocus(hwndTaskbar);

			if (HWND hParent = GetAncestor(browser->GetHost()->GetWindowHandle(), GA_ROOT))
				SetFocus(hParent);
		}
		
		return true;
	}
	}

	return false;
}

bool PBBrowserHandler::OnBeforeUnloadDialog(CefRefPtr<CefBrowser> browser, const CefString& message_text, bool is_reload, CefRefPtr<CefJSDialogCallback> callback) {	
	callback->Continue(true, "");
	return true;
}

void PBBrowserHandler::CloseAllBrowsers(bool force_close) {
  if (!CefCurrentlyOn(TID_UI)) {
    // Execute on the UI thread.
    CefPostTask(TID_UI, base::Bind(&PBBrowserHandler::CloseAllBrowsers, this,
                                   force_close));
    return;
  }

  if (browser_list_.empty())
    return;

  BrowserList::const_iterator it = browser_list_.begin();
  for (; it != browser_list_.end(); ++it)
    (*it)->GetHost()->CloseBrowser(force_close);
}
