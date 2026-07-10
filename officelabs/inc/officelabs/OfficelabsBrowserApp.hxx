/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * OfficeLabs CEF browser-process CefApp (macOS message-pump integration)
 *
 * On macOS, CEF cannot own its own UI thread the way it does on Windows
 * (multi_threaded_message_loop does not exist on mac). LibreOffice already
 * owns the NSApplication run loop, so CEF is configured with
 * external_message_pump = true and this CefApp / CefBrowserProcessHandler
 * receives OnScheduleMessagePumpWork() callbacks. Each callback schedules a
 * single CefDoMessageLoopWork() on the AppKit main thread (see
 * MessagePumpMac.mm), which is the same thread as LibreOffice's VCL main
 * thread and CEF's UI thread on mac.
 *
 * This app is only passed to CefInitialize() in the browser process; the
 * renderer/helper processes use OfficelabsRendererApp (cef_subprocess_main).
 *
 * Registered on both platforms so the object files exist, but its
 * message-pump behaviour is only meaningful under external_message_pump,
 * i.e. the macOS build. On Windows the OnScheduleMessagePumpWork() override
 * is never invoked because multi_threaded_message_loop = true there.
 */

#ifndef INCLUDED_OFFICELABS_OFFICELABSBROWSERAPP_HXX
#define INCLUDED_OFFICELABS_OFFICELABSBROWSERAPP_HXX

#ifdef HAVE_FEATURE_CEF

#include <include/cef_app.h>
#include <include/cef_browser_process_handler.h>

namespace officelabs {

// Browser-process application handler. Owns the external message-pump
// integration on macOS via OnScheduleMessagePumpWork(). The coalescing state
// is a file-static in OfficelabsBrowserApp.cxx (there is exactly one
// browser-process app instance for the process lifetime, and the mac dispatch
// shim takes a context-free callback).
class OfficelabsBrowserApp final : public CefApp,
                                   public CefBrowserProcessHandler
{
public:
    OfficelabsBrowserApp() = default;

    // CefApp
    CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override
    {
        return this;
    }

    // CefBrowserProcessHandler
    // Called by CEF (external_message_pump mode) to request that CEF work be
    // pumped after |delay_ms| milliseconds. Implemented in
    // OfficelabsBrowserApp.cxx; on mac it hops to the AppKit main thread via
    // the C shim in MessagePumpMac.mm. No-op on Windows.
    void OnScheduleMessagePumpWork(int64_t delay_ms) override;

private:
    IMPLEMENT_REFCOUNTING(OfficelabsBrowserApp);
};

} // namespace officelabs

#endif // HAVE_FEATURE_CEF
#endif // INCLUDED_OFFICELABS_OFFICELABSBROWSERAPP_HXX

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
