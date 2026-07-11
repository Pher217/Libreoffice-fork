/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * OfficeLabs CEF Initialization Singleton
 *
 * Manages the CEF lifecycle: initialize once at first panel creation,
 * shut down cleanly when LibreOffice exits.
 *
 * Key settings:
 *   no_sandbox = true
 *   remote_debugging_port = 9222
 *   Message loop: Windows uses multi_threaded_message_loop (CEF owns its own
 *     thread); macOS uses external_message_pump instead (does not exist on
 *     mac) and pumps CefDoMessageLoopWork() from OfficelabsBrowserApp.
 */

#ifndef INCLUDED_OFFICELABS_CEFINIT_HXX
#define INCLUDED_OFFICELABS_CEFINIT_HXX

#ifdef HAVE_FEATURE_CEF

#include <officelabs/officelabsdllapi.h>
#include <rtl/ustring.hxx>
#include <mutex>

namespace officelabs {

class OFFICELABS_DLLPUBLIC CefInit final
{
public:
    static CefInit& instance();

    bool isInitialized() const { return m_bInitialized; }

    // Called lazily on first WebViewPanel creation
    bool initialize();

    // Called at LibreOffice shutdown
    void shutdown();

    // Path to the CEF subprocess: officelabs_cef_subprocess(.exe) on
    // Windows/Linux, the "OfficeLabs Helper" .app bundle executable on macOS.
    OUString getSubprocessPath() const;

    int getRemoteDebuggingPort() const { return 9222; }

private:
    CefInit();
    ~CefInit();
    CefInit(const CefInit&) = delete;
    CefInit& operator=(const CefInit&) = delete;

    bool m_bInitialized;
    std::mutex m_mutex;
};

} // namespace officelabs

#endif // HAVE_FEATURE_CEF
#endif // INCLUDED_OFFICELABS_CEFINIT_HXX

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
