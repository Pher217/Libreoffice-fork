/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * OfficeLabs CEF browser-process CefApp implementation.
 *
 * See OfficelabsBrowserApp.hxx for the design rationale. The mac message-pump
 * hop is done through a tiny Objective-C++ shim (MessagePumpMac.mm) so that
 * this translation unit stays plain C++ and can be compiled on every platform.
 */

#ifdef HAVE_FEATURE_CEF

#include <officelabs/OfficelabsBrowserApp.hxx>

#include <include/cef_app.h>

#ifdef MACOSX
#include <atomic>

// Implemented in MessagePumpMac.mm. Schedules |fn| to run on the AppKit main
// (NSApplication) thread after |delay_ms| milliseconds. delay_ms <= 0 means
// "as soon as possible" (dispatch_async); delay_ms > 0 uses dispatch_after.
extern "C" void officelabs_schedule_pump_on_main(int64_t delay_ms,
                                                 void (*fn)());

namespace {

// Coalescing guard. Set true while a pump block is already queued so CEF's
// frequent OnScheduleMessagePumpWork() calls do not enqueue thousands of
// redundant CefDoMessageLoopWork() blocks. There is exactly one
// browser-process app instance, so file-static state is correct here.
std::atomic<bool> g_bPumpScheduled{ false };

// Runs on the AppKit main thread. Clears the guard *before* pumping so that
// any OnScheduleMessagePumpWork() calls issued by CEF while
// CefDoMessageLoopWork() runs will schedule the next pump.
void officelabsPumpOnMain()
{
    g_bPumpScheduled.store(false, std::memory_order_release);
    CefDoMessageLoopWork();
}

} // anonymous namespace
#endif // MACOSX

namespace officelabs {

void OfficelabsBrowserApp::OnScheduleMessagePumpWork(int64_t delay_ms)
{
#ifdef MACOSX
    // Coalesce: if a pump is already queued, do nothing. The queued block runs
    // CefDoMessageLoopWork(), after which CEF re-schedules if work remains.
    // A pending delayed pump is intentionally not shortened when a later
    // 0-delay request arrives — see the FLAG for Opus in the port report.
    bool bExpected = false;
    if (!g_bPumpScheduled.compare_exchange_strong(bExpected, true,
                                                  std::memory_order_acq_rel))
        return;

    officelabs_schedule_pump_on_main(delay_ms, &officelabsPumpOnMain);
#else
    // Windows uses multi_threaded_message_loop = true, so CEF never calls this.
    (void)delay_ms;
#endif
}

} // namespace officelabs

#endif // HAVE_FEATURE_CEF

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
