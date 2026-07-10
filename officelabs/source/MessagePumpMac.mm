/* -*- Mode: ObjC++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * OfficeLabs CEF message-pump shim for macOS.
 *
 * Bridges CefBrowserProcessHandler::OnScheduleMessagePumpWork() (plain C++ in
 * OfficelabsBrowserApp.cxx) to Grand Central Dispatch on the AppKit main
 * thread. Kept in its own Objective-C++ (.mm) translation unit so that the
 * CefApp implementation stays platform-neutral C++.
 *
 * The scheduled callback |fn| is CefDoMessageLoopWork() (wrapped by a small
 * static in OfficelabsBrowserApp.cxx that clears the coalescing flag first).
 * Running it on the main queue guarantees it executes on the same thread that
 * called CefInitialize() — CEF's UI thread and LibreOffice's VCL main thread
 * are both the AppKit main thread on mac.
 */

#ifdef HAVE_FEATURE_CEF
#ifdef MACOSX

#include <dispatch/dispatch.h>
#include <cstdint>

extern "C" void officelabs_schedule_pump_on_main(int64_t delay_ms,
                                                 void (*fn)())
{
    if (fn == nullptr)
        return;

    if (delay_ms <= 0)
    {
        // Pump as soon as the main queue is free.
        dispatch_async(dispatch_get_main_queue(), ^{
            fn();
        });
    }
    else
    {
        // Pump after the requested delay.
        const dispatch_time_t when =
            dispatch_time(DISPATCH_TIME_NOW, delay_ms * NSEC_PER_MSEC);
        dispatch_after(when, dispatch_get_main_queue(), ^{
            fn();
        });
    }
}

#endif // MACOSX
#endif // HAVE_FEATURE_CEF

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
