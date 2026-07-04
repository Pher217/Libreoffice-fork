/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * OfficeLabs CEF Initialization
 *
 * CRITICAL: multi_threaded_message_loop = true
 *           external_message_pump = false
 * This avoids conflicts with LibreOffice's VCL event loop.
 */

#ifdef HAVE_FEATURE_CEF

#ifdef _WIN32
#include <prewin.h>
#include <windows.h>
#include <postwin.h>
#endif

#include <officelabs/CefInit.hxx>
#include <officelabs/WebViewPanel.hxx>

#include <include/cef_app.h>
#include <include/cef_browser.h>

#include <sal/log.hxx>
#include <osl/file.hxx>
#include <osl/module.hxx>
#include <rtl/bootstrap.hxx>

#include <com/sun/star/frame/Desktop.hpp>
#include <com/sun/star/frame/XDesktop2.hpp>
#include <com/sun/star/frame/XTerminateListener.hpp>
#include <com/sun/star/lang/EventObject.hpp>
#include <com/sun/star/uno/Reference.hxx>
#include <com/sun/star/uno/XComponentContext.hpp>
#include <comphelper/processfactory.hxx>
#include <cppuhelper/implbase.hxx>

#include <cstdlib>
#include <string>

namespace {

// Drives CefShutdown() on the main thread during normal LibreOffice
// termination. CEF is initialized with multi_threaded_message_loop = true,
// so CefShutdown() must run on the thread that called CefInitialize, while
// the process is still alive. Leaving it to the CefInit static destructor
// runs it during CRT teardown after main() returns, racing CEF's worker
// threads and firing the libcef int3 (BREAKPOINT_80000003) on close. This
// listener fires synchronously on the main thread from Desktop::terminate(),
// before static destruction. The static-dtor shutdown() stays as an
// idempotent fallback (guarded by m_bInitialized).
class CefTerminateListener
    : public cppu::WeakImplHelper<css::frame::XTerminateListener>
{
public:
    // XTerminateListener — never veto termination
    void SAL_CALL queryTermination(const css::lang::EventObject&) override {}

    void SAL_CALL notifyTermination(const css::lang::EventObject&) override
    {
        officelabs::CefInit::instance().shutdown();
    }

    // XEventListener
    void SAL_CALL disposing(const css::lang::EventObject&) override {}
};

} // anonymous namespace

namespace officelabs {

CefInit& CefInit::instance()
{
    static CefInit sInstance;
    return sInstance;
}

CefInit::CefInit()
    : m_bInitialized(false)
{
}

CefInit::~CefInit()
{
    shutdown();
}

bool CefInit::initialize()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_bInitialized)
        return true;

#ifdef _WIN32
    CefMainArgs main_args(GetModuleHandle(nullptr));
#else
    CefMainArgs main_args(0, nullptr);
#endif

    CefSettings settings;

    // CRITICAL: Use multi-threaded message loop so CEF doesn't block VCL
    settings.multi_threaded_message_loop = true;
    settings.external_message_pump = false;

    // No sandbox - LibreOffice doesn't support CEF's sandbox model
    settings.no_sandbox = true;

    // Remote debugging for Chrome DevTools
    settings.remote_debugging_port = 9222;

    // Disable windowless rendering - we use a real HWND
    settings.windowless_rendering_enabled = false;

    // Set subprocess path
    OUString subprocessPath = getSubprocessPath();
    OString utf8Path = OUStringToOString(subprocessPath, RTL_TEXTENCODING_UTF8);
    CefString(&settings.browser_subprocess_path).FromASCII(utf8Path.getStr());

    // Persistent cache — enables localStorage across CEF browser restarts.
    // Without this, CEF runs in "incognito mode" and localStorage is lost
    // when the sidebar panel is recreated (e.g. during OLE chart activation).
#ifdef _WIN32
    {
        const char* localAppData = std::getenv("LOCALAPPDATA");
        if (localAppData)
        {
            std::string rootCache = std::string(localAppData) + "\\OfficeLabs\\cef_data";
            std::string profileCache = rootCache + "\\Default";
            CefString(&settings.root_cache_path).FromASCII(rootCache.c_str());
            CefString(&settings.cache_path).FromASCII(profileCache.c_str());
            SAL_INFO("officelabs.cef", "CEF cache: " << rootCache);
        }
    }
#endif

    // Log settings
    settings.log_severity = LOGSEVERITY_INFO;
    CefString(&settings.log_file).FromASCII("officelabs_cef.log");

    SAL_INFO("officelabs.cef", "Initializing CEF with subprocess: " << utf8Path);

    if (!CefInitialize(main_args, settings, nullptr, nullptr))
    {
        SAL_WARN("officelabs.cef", "CefInitialize() FAILED");
        return false;
    }

    m_bInitialized = true;
    SAL_INFO("officelabs.cef", "CEF initialized successfully (debug port 9222)");

    // Register a terminate listener so CefShutdown() runs on the main thread
    // during normal LO termination, before static destructors (see comment on
    // CefTerminateListener). Runs once — initialize() early-returns when
    // already initialized.
    try
    {
        css::uno::Reference<css::uno::XComponentContext> xContext(
            ::comphelper::getProcessComponentContext());
        if (xContext.is())
        {
            css::uno::Reference<css::frame::XDesktop2> xDesktop(
                css::frame::Desktop::create(xContext));
            xDesktop->addTerminateListener(new CefTerminateListener());
            SAL_INFO("officelabs.cef", "Registered CEF terminate listener");
        }
    }
    catch (const css::uno::Exception&)
    {
        // Non-fatal: fall back to the static-destructor shutdown() path.
        SAL_WARN("officelabs.cef",
                 "Failed to register CEF terminate listener; CefShutdown will "
                 "fall back to static destructor");
    }

    return true;
}

void CefInit::shutdown()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_bInitialized)
        return;

    SAL_INFO("officelabs.cef", "Shutting down CEF...");

    // Release persistent browser/popup/router BEFORE CefShutdown().
    // Static CefRefPtrs in WebViewPanel.cxx must be cleared while CEF
    // is still alive, otherwise their destructors touch freed CEF state.
    WebViewPanel::cleanupPersistentBrowser();

    CefShutdown();
    m_bInitialized = false;
    SAL_INFO("officelabs.cef", "CEF shutdown complete");
}

OUString CefInit::getSubprocessPath() const
{
    // The subprocess exe lives next to soffice.exe in instdir/program/
    OUString sInstDir(u"$BRAND_BASE_DIR/$BRAND_SHARE_SUBDIR/.."_ustr);
    rtl::Bootstrap::expandMacros(sInstDir);

    OUString sFileUrl;
    osl::FileBase::getAbsoluteFileURL(OUString(), sInstDir, sFileUrl);

    // Convert file:// URL to system path
    OUString sSystemPath;
    osl::FileBase::getSystemPathFromFileURL(sFileUrl, sSystemPath);

#ifdef _WIN32
    return sSystemPath + "\\program\\officelabs_cef_subprocess.exe";
#else
    return sSystemPath + "/program/officelabs_cef_subprocess";
#endif
}

} // namespace officelabs

#endif // HAVE_FEATURE_CEF

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
