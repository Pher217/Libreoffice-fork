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
#include <cstdio>

// TEMP instrumentation (white-page debug): append pump-side evidence to
// /tmp/olcef.log so we can tell "CEF never schedules" apart from "the
// dispatched block never runs on the VCL main loop".
static void olPumpLog(const char* fmt, long long a, long long b)
{
    if (FILE* f = fopen("/tmp/olcef.log", "a"))
    {
        fprintf(f, fmt, a, b);
        fclose(f);
    }
}

// Implemented in MessagePumpMac.mm. Schedules |fn| to run on the AppKit main
// (NSApplication) thread after |delay_ms| milliseconds. delay_ms <= 0 means
// "as soon as possible" (dispatch_async); delay_ms > 0 uses dispatch_after.
extern "C" void officelabs_schedule_pump_on_main(int64_t delay_ms,
                                                 void (*fn)());
extern "C" void officelabs_start_pump_heartbeat(int64_t interval_ms,
                                                void (*fn)());
extern "C" void officelabs_stop_pump_heartbeat();

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
    static std::atomic<long long> s_nPump{ 0 };
    const long long n = ++s_nPump;
    if (n <= 5 || n % 500 == 0)
        olPumpLog("PumpOnMain exec #%lld\n", n, 0);
    g_bPumpScheduled.store(false, std::memory_order_release);
    CefDoMessageLoopWork();
}

// 30Hz heartbeat body (see MessagePumpMac.mm for why a heartbeat exists).
void officelabsHeartbeatPump()
{
    static std::atomic<long long> s_nBeat{ 0 };
    const long long n = ++s_nBeat;
    if (n <= 3 || n % 300 == 0)
        olPumpLog("HeartbeatPump exec #%lld\n", n, 0);
    CefDoMessageLoopWork();
}

} // anonymous namespace
#endif // MACOSX

namespace officelabs {

void OfficelabsBrowserApp::OnScheduleMessagePumpWork(int64_t delay_ms)
{
#ifdef MACOSX
    static std::atomic<long long> s_nSched{ 0 };
    const long long nCall = ++s_nSched;
    if (nCall <= 5 || nCall % 500 == 0)
        olPumpLog("SchedulePumpWork call #%lld delay=%lld\n", nCall,
                  static_cast<long long>(delay_ms));
    // Coalesce: if a pump is already queued, do nothing. The queued block runs
    // CefDoMessageLoopWork(), after which CEF re-schedules if work remains.
    // A pending delayed pump is intentionally not shortened when a later
    // 0-delay request arrives — see the FLAG for Opus in the port report.
    // FIX (Fable): cap the pump delay so urgent 0-delay requests (the renderer's
    // Mojo handshake tasks) are not starved behind a long-delay dispatch_after that
    // the coalescing guard swallows -> renderer dies at Chromium's 15s timeout ->
    // white page. Mirrors cefclient's kMaxTimerDelay = 1000/30 (~33ms).
    constexpr int64_t kMaxTimerDelayMs = 1000 / 30;
    if (delay_ms > kMaxTimerDelayMs)
        delay_ms = kMaxTimerDelayMs;

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

void StartCefPumpHeartbeat()
{
#ifdef MACOSX
    officelabs_start_pump_heartbeat(33, &officelabsHeartbeatPump);
#endif
}

void StopCefPumpHeartbeat()
{
#ifdef MACOSX
    officelabs_stop_pump_heartbeat();
#endif
}

} // namespace officelabs

#endif // HAVE_FEATURE_CEF

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
