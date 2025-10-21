# OfficeLabs UI Integration

**Native AI Chat Panel Implementation**

---

## 🎯 Overview

This document describes how to integrate the AI chat panel **natively** into LibreOffice's UI using VCL (Visual Class Library), the native widget toolkit.

**Key Principle:** The AI experience is not a plugin or extension—it's **built into the core UI**.

---

## 🏗️ UI Architecture

```
LibreOffice Window
├── Menu Bar
├── Toolbar
├── Main Content Area
│   ├── Document Editor
│   └── AI Chat Panel (dockable)     ← Our integration
├── Status Bar
└── Sidebar (optional)
```

### Integration Approach

**NOT:**
- ❌ Browser-based extension
- ❌ External window
- ❌ Sidebar-only panel

**YES:**
- ✅ Native VCL widget
- ✅ Dockable panel (like Find & Replace)
- ✅ Keyboard-driven (Ctrl+K)
- ✅ Inline ghost text
- ✅ Command palette

---

## 🎨 Design Specifications

### Visual Design

**Theme:** Glacier Blue on White (Swiss minimalism)

**Colors:**
```cpp
// Color palette
const Color COLOR_PRIMARY      = Color(0x4A90E2);  // Glacier Blue
const Color COLOR_SECONDARY    = Color(0x2C3E50);  // Deep Navy
const Color COLOR_ACCENT       = Color(0x00D9FF);  // Electric Teal
const Color COLOR_BACKGROUND   = Color(0xFFFFFF);  // Pure White
const Color COLOR_TEXT         = Color(0x333333);  // Charcoal
const Color COLOR_BORDER       = Color(0xE0E0E0);  // Light Gray
const Color COLOR_GHOST        = Color(0xCCCCCC);  // Ghost text
```

**Typography:**
```cpp
// Fonts
const Font FONT_UI       = Font("Inter", 12);      // UI text
const Font FONT_CODE     = Font("JetBrains Mono", 11);  // Code/diffs
const Font FONT_CHAT     = Font("System Default", 13);  // Chat messages
```

### Layout Dimensions

```cpp
// Panel dimensions
const int PANEL_MIN_WIDTH  = 300;  // px
const int PANEL_DEFAULT    = 400;  // px
const int PANEL_MAX_WIDTH  = 800;  // px

const int PANEL_MIN_HEIGHT = 200;  // px
const int PANEL_DEFAULT_H  = 600;  // px

// Element spacing
const int MARGIN     = 16;   // px
const int PADDING    = 12;   // px
const int GAP        = 8;    // px
const int BORDER_R   = 8;    // px (border radius)
```

---

## 🧱 VCL Implementation

### ChatPanel Widget

**File:** `vcl/source/ai/ChatPanel.hxx`

```cpp
#ifndef INCLUDED_VCL_AI_CHATPANEL_HXX
#define INCLUDED_VCL_AI_CHATPANEL_HXX

#include <vcl/dockwin.hxx>
#include <vcl/button.hxx>
#include <vcl/edit.hxx>
#include <vcl/fixed.hxx>
#include <vcl/scrollbar.hxx>
#include <vcl/lstbox.hxx>
#include <memory>
#include <vector>

namespace officelabs {

struct ChatMessage {
    enum class Role { USER, ASSISTANT };
    
    Role role;
    OUString content;
    std::optional<Patch> patch;
    tools::Time timestamp;
};

struct Patch {
    enum class Type { REPLACE, INSERT, DELETE };
    
    Type type;
    OUString target;        // @-address
    OUString oldValue;
    OUString newValue;
    OUString diff;          // Unified diff
};

class ChatPanel : public DockingWindow {
public:
    explicit ChatPanel(vcl::Window* pParent);
    virtual ~ChatPanel() override;
    
    // Lifecycle
    virtual void StateChanged(StateChangedType nType) override;
    virtual void DataChanged(const DataChangedEvent& rDCEvt) override;
    
    // Chat operations
    void sendMessage(const OUString& message);
    void displayResponse(const ChatMessage& message);
    void showPatchPreview(const Patch& patch);
    void clearChat();
    
    // Status
    void setConnectionStatus(bool connected);
    void setProcessing(bool processing);
    
    // Context
    void setDocumentContext(const OUString& title, const OUString& type);
    void setSelection(const OUString& selection);
    
private:
    // UI components
    VclPtr<FixedText>     m_pStatusLabel;
    VclPtr<FixedText>     m_pContextLabel;
    VclPtr<Control>       m_pChatHistory;      // Custom chat display
    VclPtr<Edit>          m_pInputField;
    VclPtr<PushButton>    m_pSendButton;
    VclPtr<PushButton>    m_pClearButton;
    VclPtr<MenuButton>    m_pOptionsButton;
    
    // Patch preview
    VclPtr<Control>       m_pPatchPreview;     // Custom diff viewer
    VclPtr<PushButton>    m_pAcceptButton;
    VclPtr<PushButton>    m_pRejectButton;
    VclPtr<PushButton>    m_pRegenerateButton;
    
    // State
    std::vector<ChatMessage> m_messages;
    std::optional<Patch>     m_currentPatch;
    bool                     m_connected;
    bool                     m_processing;
    
    // Layout
    void layout();
    void layoutChatHistory();
    void layoutInput();
    void layoutPatchPreview();
    
    // Rendering
    void renderMessage(vcl::RenderContext& rRenderContext, 
                      const ChatMessage& message,
                      const tools::Rectangle& rRect);
    void renderPatch(vcl::RenderContext& rRenderContext,
                    const Patch& patch,
                    const tools::Rectangle& rRect);
    
    // Event handlers
    DECL_LINK(SendClickHdl, Button*, void);
    DECL_LINK(ClearClickHdl, Button*, void);
    DECL_LINK(AcceptClickHdl, Button*, void);
    DECL_LINK(RejectClickHdl, Button*, void);
    DECL_LINK(RegenerateClickHdl, Button*, void);
    DECL_LINK(InputKeyHdl, KeyEvent&, bool);
    
    // IPC
    void sendToAgent(const OUString& message);
    void receiveFromAgent();
};

} // namespace officelabs

#endif
```

**File:** `vcl/source/ai/ChatPanel.cxx`

```cpp
#include <vcl/ai/ChatPanel.hxx>
#include <vcl/svapp.hxx>
#include <vcl/settings.hxx>
#include <officelabs/ipc/AgentConnection.hxx>

namespace officelabs {

ChatPanel::ChatPanel(vcl::Window* pParent)
    : DockingWindow(pParent, "ChatPanel", "vcl/ui/chatpanel.ui")
    , m_connected(false)
    , m_processing(false)
{
    // Initialize UI components
    get(m_pStatusLabel, "status_label");
    get(m_pContextLabel, "context_label");
    get(m_pChatHistory, "chat_history");
    get(m_pInputField, "input_field");
    get(m_pSendButton, "send_button");
    get(m_pClearButton, "clear_button");
    get(m_pOptionsButton, "options_button");
    
    get(m_pPatchPreview, "patch_preview");
    get(m_pAcceptButton, "accept_button");
    get(m_pRejectButton, "reject_button");
    get(m_pRegenerateButton, "regenerate_button");
    
    // Set up event handlers
    m_pSendButton->SetClickHdl(LINK(this, ChatPanel, SendClickHdl));
    m_pClearButton->SetClickHdl(LINK(this, ChatPanel, ClearClickHdl));
    m_pAcceptButton->SetClickHdl(LINK(this, ChatPanel, AcceptClickHdl));
    m_pRejectButton->SetClickHdl(LINK(this, ChatPanel, RejectClickHdl));
    m_pRegenerateButton->SetClickHdl(LINK(this, ChatPanel, RegenerateClickHdl));
    
    m_pInputField->SetKeyInputHdl(LINK(this, ChatPanel, InputKeyHdl));
    
    // Initial layout
    layout();
    
    // Initially hide patch preview
    m_pPatchPreview->Hide();
    m_pAcceptButton->Hide();
    m_pRejectButton->Hide();
    m_pRegenerateButton->Hide();
}

ChatPanel::~ChatPanel() {
    disposeOnce();
}

void ChatPanel::layout() {
    Size aSize = GetSizePixel();
    Point aPos(MARGIN, MARGIN);
    
    // Status label at top
    m_pStatusLabel->SetPosSizePixel(
        aPos,
        Size(aSize.Width() - 2 * MARGIN, 30)
    );
    aPos.AdjustY(30 + GAP);
    
    // Context label
    m_pContextLabel->SetPosSizePixel(
        aPos,
        Size(aSize.Width() - 2 * MARGIN, 25)
    );
    aPos.AdjustY(25 + GAP);
    
    // Chat history (takes remaining space minus input area)
    int inputHeight = 100;
    int historyHeight = aSize.Height() - aPos.Y() - inputHeight - 3 * MARGIN;
    
    m_pChatHistory->SetPosSizePixel(
        aPos,
        Size(aSize.Width() - 2 * MARGIN, historyHeight)
    );
    aPos.AdjustY(historyHeight + GAP);
    
    // Input field
    int buttonWidth = 80;
    m_pInputField->SetPosSizePixel(
        aPos,
        Size(aSize.Width() - 2 * MARGIN - buttonWidth - GAP, 40)
    );
    
    // Send button
    m_pSendButton->SetPosSizePixel(
        Point(aPos.X() + aSize.Width() - 2 * MARGIN - buttonWidth, aPos.Y()),
        Size(buttonWidth, 40)
    );
}

void ChatPanel::sendMessage(const OUString& message) {
    // Add user message to history
    ChatMessage userMsg;
    userMsg.role = ChatMessage::Role::USER;
    userMsg.content = message;
    userMsg.timestamp = tools::Time::GetCurrentTime();
    m_messages.push_back(userMsg);
    
    // Update UI
    m_pChatHistory->Invalidate();
    
    // Clear input
    m_pInputField->SetText("");
    
    // Show processing
    setProcessing(true);
    
    // Send to agent
    sendToAgent(message);
}

void ChatPanel::displayResponse(const ChatMessage& message) {
    // Add to history
    m_messages.push_back(message);
    
    // Update UI
    m_pChatHistory->Invalidate();
    
    // Show patch preview if present
    if (message.patch.has_value()) {
        showPatchPreview(message.patch.value());
    }
    
    // Stop processing
    setProcessing(false);
}

void ChatPanel::showPatchPreview(const Patch& patch) {
    m_currentPatch = patch;
    
    // Show patch preview UI
    m_pPatchPreview->Show();
    m_pAcceptButton->Show();
    m_pRejectButton->Show();
    m_pRegenerateButton->Show();
    
    // Render patch
    m_pPatchPreview->Invalidate();
}

void ChatPanel::renderMessage(
    vcl::RenderContext& rRenderContext,
    const ChatMessage& message,
    const tools::Rectangle& rRect
) {
    // Set colors based on role
    Color bgColor = (message.role == ChatMessage::Role::USER) 
                   ? COLOR_PRIMARY : Color(0xF5F5F5);
    Color textColor = (message.role == ChatMessage::Role::USER)
                     ? COL_WHITE : COLOR_TEXT;
    
    // Draw message bubble
    rRenderContext.SetFillColor(bgColor);
    rRenderContext.SetLineColor(COLOR_BORDER);
    rRenderContext.DrawRect(rRect, BORDER_R, BORDER_R);
    
    // Draw text
    rRenderContext.SetTextColor(textColor);
    rRenderContext.DrawText(
        Point(rRect.Left() + PADDING, rRect.Top() + PADDING),
        message.content
    );
    
    // Draw timestamp (small, gray)
    OUString timeStr = message.timestamp.GetTimeInFormat(
        tools::Time::TimeFormat::Time24
    );
    rRenderContext.SetTextColor(Color(0x999999));
    Font smallFont = rRenderContext.GetFont();
    smallFont.SetFontHeight(10);
    rRenderContext.SetFont(smallFont);
    rRenderContext.DrawText(
        Point(rRect.Right() - 50, rRect.Bottom() - 15),
        timeStr
    );
}

void ChatPanel::renderPatch(
    vcl::RenderContext& rRenderContext,
    const Patch& patch,
    const tools::Rectangle& rRect
) {
    // Draw patch header
    rRenderContext.SetFillColor(Color(0xFFF8DC));  // Light yellow
    rRenderContext.SetLineColor(COLOR_BORDER);
    tools::Rectangle headerRect(
        rRect.Left(), rRect.Top(),
        rRect.Right(), rRect.Top() + 30
    );
    rRenderContext.DrawRect(headerRect);
    
    // Draw patch type and target
    rRenderContext.SetTextColor(COLOR_TEXT);
    Font boldFont = rRenderContext.GetFont();
    boldFont.SetWeight(WEIGHT_BOLD);
    rRenderContext.SetFont(boldFont);
    
    OUString header = OUString::Format(
        "%s %s",
        patch.type == Patch::Type::REPLACE ? "Replace" :
        patch.type == Patch::Type::INSERT ? "Insert" : "Delete",
        patch.target.getStr()
    );
    rRenderContext.DrawText(
        Point(headerRect.Left() + PADDING, headerRect.Top() + 10),
        header
    );
    
    // Draw diff
    tools::Rectangle diffRect(
        rRect.Left(), headerRect.Bottom() + GAP,
        rRect.Right(), rRect.Bottom()
    );
    
    rRenderContext.SetFont(FONT_CODE);
    
    // Parse and render diff lines
    sal_Int32 lineY = diffRect.Top() + PADDING;
    sal_Int32 index = 0;
    
    while (index < patch.diff.getLength()) {
        sal_Int32 newline = patch.diff.indexOf('\n', index);
        if (newline == -1) newline = patch.diff.getLength();
        
        OUString line = patch.diff.copy(index, newline - index);
        
        // Set color based on diff prefix
        if (line.startsWith("-")) {
            rRenderContext.SetTextColor(Color(0xFF0000));  // Red
        } else if (line.startsWith("+")) {
            rRenderContext.SetTextColor(Color(0x00AA00));  // Green
        } else {
            rRenderContext.SetTextColor(Color(0x666666));  // Gray
        }
        
        rRenderContext.DrawText(
            Point(diffRect.Left() + PADDING, lineY),
            line
        );
        
        lineY += 18;  // Line height
        index = newline + 1;
    }
}

IMPL_LINK_NOARG(ChatPanel, SendClickHdl, Button*, void) {
    OUString message = m_pInputField->GetText();
    if (!message.isEmpty()) {
        sendMessage(message);
    }
}

IMPL_LINK(ChatPanel, InputKeyHdl, KeyEvent&, rKEvt, bool) {
    // Send on Ctrl+Enter
    const KeyCode& rKeyCode = rKEvt.GetKeyCode();
    if (rKeyCode.GetCode() == KEY_RETURN && rKeyCode.IsMod1()) {
        SendClickHdl(nullptr);
        return true;
    }
    return false;
}

IMPL_LINK_NOARG(ChatPanel, AcceptClickHdl, Button*, void) {
    if (m_currentPatch.has_value()) {
        // Apply patch
        // TODO: Send to patch applier
        
        // Hide preview
        m_pPatchPreview->Hide();
        m_pAcceptButton->Hide();
        m_pRejectButton->Hide();
        m_pRegenerateButton->Hide();
        
        m_currentPatch.reset();
    }
}

IMPL_LINK_NOARG(ChatPanel, RejectClickHdl, Button*, void) {
    // Just hide preview
    m_pPatchPreview->Hide();
    m_pAcceptButton->Hide();
    m_pRejectButton->Hide();
    m_pRegenerateButton->Hide();
    
    m_currentPatch.reset();
}

} // namespace officelabs
```

---

## ⌨️ Command Palette

**File:** `vcl/source/ai/CommandPalette.hxx`

```cpp
#ifndef INCLUDED_VCL_AI_COMMANDPALETTE_HXX
#define INCLUDED_VCL_AI_COMMANDPALETTE_HXX

#include <vcl/floatwin.hxx>
#include <vcl/edit.hxx>
#include <vcl/lstbox.hxx>

namespace officelabs {

struct Command {
    OUString id;
    OUString label;
    OUString icon;
    OUString description;
    std::function<void()> action;
};

class CommandPalette : public FloatingWindow {
public:
    CommandPalette(vcl::Window* pParent);
    virtual ~CommandPalette() override;
    
    void show();
    void hide();
    
    void registerCommand(const Command& cmd);
    void filterCommands(const OUString& query);
    
private:
    VclPtr<Edit> m_pSearchField;
    VclPtr<ListBox> m_pCommandList;
    
    std::vector<Command> m_allCommands;
    std::vector<Command> m_filteredCommands;
    
    void initDefaultCommands();
    void updateList();
    
    DECL_LINK(SearchModifyHdl, Edit&, void);
    DECL_LINK(ListSelectHdl, ListBox&, void);
    DECL_LINK(ListDoubleClickHdl, ListBox&, void);
};

} // namespace officelabs

#endif
```

---

## 👻 Inline Ghost Text

**File:** `sw/source/ai/GhostText.hxx`

```cpp
#ifndef INCLUDED_SW_AI_GHOSTTEXT_HXX
#define INCLUDED_SW_AI_GHOSTTEXT_HXX

#include <rtl/ustring.hxx>
#include <optional>

namespace sw {

class SwView;
class SwWrtShell;

class GhostTextManager {
public:
    explicit GhostTextManager(SwView* pView);
    
    void showSuggestion(const OUString& suggestion);
    void hideSuggestion();
    void acceptSuggestion();
    void rejectSuggestion();
    
    bool hasSuggestion() const;
    
private:
    SwView* m_pView;
    SwWrtShell* m_pWrtShell;
    std::optional<OUString> m_suggestion;
    
    void renderGhostText();
    void clearGhostText();
};

} // namespace sw

#endif
```

**Usage:**
```cpp
// Show ghost text suggestion
ghostTextMgr->showSuggestion("and its impact on productivity...");

// User presses Tab → accept
ghostTextMgr->acceptSuggestion();

// User presses Esc → reject
ghostTextMgr->rejectSuggestion();
```

---

## 🔧 Integration Points

### Registering the Chat Panel

**File:** `sw/source/uibase/app/appenv.cxx`

```cpp
#include <vcl/ai/ChatPanel.hxx>

void SwModule::InitInterface_Impl() {
    // ... existing code ...
    
    // Register AI Chat Panel
    RegisterChildWindow(
        officelabs::ChatPanel::GetChildWindowId(),
        SfxChildWinInfo()
    );
}
```

### Menu Integration

**File:** `sw/sdi/swriter.sdi`

```sdi
SfxVoidItem ToggleChatPanel SID_TOGGLE_CHAT_PANEL
(
    ApiName = "ToggleChatPanel",
    ExecMethod = ExecView,
    StateMethod = GetState,
    Accelerator = "Shift+F5",
    MenuConfig, AccelConfig, ToolBoxConfig
)
```

### Keyboard Shortcut

**File:** `sw/source/uibase/app/swmodul1.cxx`

```cpp
// Register Ctrl+K for command palette
pKeyBinding->insert(
    KEY_K | KEY_MOD1,
    ".uno:ShowCommandPalette"
);
```

---

## 🎨 Theme Integration

**File:** `vcl/source/ai/OfficeLabs.theme`

```css
/* OfficeLabs Theme */

.chat-panel {
    background-color: #FFFFFF;
    border: 1px solid #E0E0E0;
    border-radius: 8px;
}

.chat-message.user {
    background-color: #4A90E2;
    color: #FFFFFF;
    border-radius: 12px;
    padding: 12px;
    margin: 8px;
}

.chat-message.assistant {
    background-color: #F5F5F5;
    color: #333333;
    border-radius: 12px;
    padding: 12px;
    margin: 8px;
}

.patch-preview {
    background-color: #FFF8DC;
    border: 1px solid #FFD700;
    border-radius: 8px;
    padding: 16px;
    font-family: "JetBrains Mono", monospace;
}

.diff-line.added {
    color: #00AA00;
}

.diff-line.removed {
    color: #FF0000;
}

.command-palette {
    background-color: #FFFFFF;
    box-shadow: 0 4px 16px rgba(0, 0, 0, 0.2);
    border-radius: 8px;
}
```

---

## 📱 Responsive Layout

The chat panel adapts to different window sizes:

```cpp
void ChatPanel::layout() {
    Size aSize = GetSizePixel();
    
    // Compact mode for narrow panels
    if (aSize.Width() < 350) {
        layoutCompact();
    }
    // Normal mode
    else if (aSize.Width() < 600) {
        layoutNormal();
    }
    // Wide mode
    else {
        layoutWide();
    }
}
```

---

## 🧪 Testing the UI

### Manual Testing

```bash
# Build with UI debug enabled
./autogen.sh --enable-debug --with-officelabs

# Run with UI logging
SAL_LOG="+WARN+INFO.officelabs.ui" ./instdir/program/soffice --writer
```

### Unit Tests

```cpp
CPPUNIT_TEST_SUITE(ChatPanelTest);
CPPUNIT_TEST(testMessageDisplay);
CPPUNIT_TEST(testPatchPreview);
CPPUNIT_TEST(testKeyboardShortcuts);
CPPUNIT_TEST_SUITE_END();

void ChatPanelTest::testMessageDisplay() {
    ScopedVclPtrInstance<ChatPanel> panel(nullptr);
    
    ChatMessage msg;
    msg.role = ChatMessage::Role::USER;
    msg.content = "Test message";
    
    panel->displayMessage(msg);
    
    CPPUNIT_ASSERT_EQUAL(1u, panel->getMessageCount());
}
```

---

## 📚 References

- [VCL Documentation](https://docs.libreoffice.org/vcl/html/)
- [LibreOffice UI Guidelines](https://wiki.documentfoundation.org/Design)
- [Docking Windows](https://api.libreoffice.org/docs/idl/ref/interfacecom_1_1sun_1_1star_1_1frame_1_1XDockingAreaAcceptor.html)

---

**OfficeLabs UI** - Native, fast, beautiful

