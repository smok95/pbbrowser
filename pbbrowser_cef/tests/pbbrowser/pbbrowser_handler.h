// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_
#define CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_

#include "include/cef_client.h"

#include <list>

class PBBrowserHandler : public CefClient,
                      public CefDisplayHandler,
                      public CefLifeSpanHandler,
                      public CefLoadHandler,
						public CefJSDialogHandler{
 public:
  explicit PBBrowserHandler(bool use_views);
  ~PBBrowserHandler();

  // Provide access to the single global instance of this object.
  static PBBrowserHandler* GetInstance();

  // CefClient methods:
  virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE {
    return this;
  }
  virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE {
    return this;
  }
  virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE { return this; }

  // CefDisplayHandler methods:
  virtual void OnTitleChange(CefRefPtr<CefBrowser> browser,
                             const CefString& title) OVERRIDE;

  // CefLifeSpanHandler methods:
  virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
  virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
  virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

  // CefLoadHandler methods:
  virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame,
                           ErrorCode errorCode,
                           const CefString& errorText,
                           const CefString& failedUrl) OVERRIDE;

  // CefJSDialogHandler methods:
  virtual CefRefPtr<CefJSDialogHandler> GetJSDialogHandler() OVERRIDE { return this; }
  virtual bool OnJSDialog(CefRefPtr<CefBrowser> browser,
	  const CefString& origin_url,
	  JSDialogType dialog_type,
	  const CefString& message_text,
	  const CefString& default_prompt_text,
	  CefRefPtr<CefJSDialogCallback> callback,
	  bool& suppress_message) OVERRIDE;

  virtual bool OnBeforeUnloadDialog(CefRefPtr<CefBrowser> browser,
	  const CefString& message_text,
	  bool is_reload,
	  CefRefPtr<CefJSDialogCallback> callback) OVERRIDE;

  // Request that all existing browser windows close.
  void CloseAllBrowsers(bool force_close);

  bool IsClosing() const { return is_closing_; }

 private:
  // Platform-specific implementation.
  void PlatformTitleChange(CefRefPtr<CefBrowser> browser,
                           const CefString& title);

  // True if the application is using the Views framework.
  const bool use_views_;

  // List of existing browser windows. Only accessed on the CEF UI thread.
  typedef std::list<CefRefPtr<CefBrowser>> BrowserList;
  BrowserList browser_list_;

  bool is_closing_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(PBBrowserHandler);
};

#endif  // CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_
