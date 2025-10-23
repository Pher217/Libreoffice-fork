/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the OfficeLabs project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDED_VCL_AI_CHATPANEL_HXX
#define INCLUDED_VCL_AI_CHATPANEL_HXX

#include <vcl/dockwin.hxx>
#include <vcl/button.hxx>
#include <vcl/edit.hxx>
#include <vcl/fixed.hxx>
#include <vcl/menu.hxx>
#include <vcl/ctrl.hxx>
#include <vcl/virdev.hxx>
#include <vcl/idle.hxx>
#include <rtl/ustring.hxx>
#include <tools/link.hxx>
#include <tools/time.hxx>

#include <memory>
#include <optional>
#include <vector>

namespace officelabs {
namespace ui {

struct ChatPatch {
    enum class Type { Replace, Insert, Delete };

    Type type;
    OUString target;
    OUString oldValue;
    OUString newValue;
    OUString diff;
};

struct ChatMessage {
    enum class Role { User, Assistant, System };

    Role role;
    OUString content;
    std::optional<ChatPatch> patch;
    tools::Time timestamp;
};

class ChatPanel final : public DockingWindow {
public:
    explicit ChatPanel(vcl::Window* pParent);
    virtual ~ChatPanel() override;

    virtual void StateChanged(StateChangedType nType) override;
    virtual void DataChanged(const DataChangedEvent& rDCEvt) override;

    void SendMessage(const OUString& rMessage);
    void DisplayResponse(const ChatMessage& rMessage);
    void ShowPatchPreview(const ChatPatch& rPatch);
    void ClearChat();

    void SetConnectionStatus(bool bConnected);
    void SetProcessing(bool bProcessing);

    void SetDocumentContext(const OUString& rTitle, const OUString& rType);
    void SetSelection(const OUString& rSelection);

private:
    VclPtr<FixedText> mpStatusLabel;
    VclPtr<FixedText> mpContextLabel;
    VclPtr<Control> mpChatHistory;
    VclPtr<Edit> mpInputField;
    VclPtr<PushButton> mpSendButton;
    VclPtr<PushButton> mpClearButton;
    VclPtr<MenuButton> mpOptionsButton;

    VclPtr<Control> mpPatchPreview;
    VclPtr<PushButton> mpAcceptButton;
    VclPtr<PushButton> mpRejectButton;
    VclPtr<PushButton> mpRegenerateButton;

    Idle maLayoutIdle;

    std::vector<ChatMessage> maMessages;
    std::optional<ChatPatch> maCurrentPatch;
    bool mbConnected;
    bool mbProcessing;

    OUString maDocumentTitle;
    OUString maDocumentType;
    OUString maSelection;

    void Initialize();
    void CreateLayout();
    void LayoutWidgets();
    void UpdateStatusLabel();
    void UpdateContextLabel();
    void UpdateButtons();
    void RenderMessages(OutputDevice& rDevice);
    void RenderPatch(OutputDevice& rDevice);

    DECL_LINK(SendClickHdl, Button*, void);
    DECL_LINK(ClearClickHdl, Button*, void);
    DECL_LINK(AcceptClickHdl, Button*, void);
    DECL_LINK(RejectClickHdl, Button*, void);
    DECL_LINK(RegenerateClickHdl, Button*, void);
    DECL_LINK(InputKeyHdl, KeyEvent&, bool);
    DECL_LINK(LayoutIdleHdl, Idle*, void);

    void DispatchToAgent(const OUString& rMessage);
    void NotifyAssistantTyping();
};

} // namespace ui
} // namespace officelabs

#endif // INCLUDED_VCL_AI_CHATPANEL_HXX

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
