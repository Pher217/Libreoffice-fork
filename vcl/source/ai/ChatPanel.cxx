/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the OfficeLabs project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ChatPanel.hxx"

#include <vcl/commandevent.hxx>
#include <vcl/menu.hxx>
#include <vcl/event.hxx>
#include <vcl/layout.hxx>
#include <vcl/svapp.hxx>
#include <vcl/settings.hxx>
#include <officecfg/Office/Common.hxx>
#include <rtl/ustrbuf.hxx>

namespace officelabs {
namespace ui {

namespace {

OUString GetRoleLabel(ChatMessage::Role eRole)
{
    switch (eRole)
    {
        case ChatMessage::Role::User:
            return OUString("You");
        case ChatMessage::Role::Assistant:
            return OUString("Assistant");
        case ChatMessage::Role::System:
            return OUString("System");
    }
    return OUString("Unknown");
}

OUString FormatTimestamp(const tools::Time& rTime)
{
    return OUString::number(rTime.GetHour()) + ":" + OUString::number(rTime.GetMin(), 2);
}

} // anonymous namespace

ChatPanel::ChatPanel(vcl::Window* pParent)
    : DockingWindow(pParent, WB_STDWORK)
    , mpStatusLabel(nullptr)
    , mpContextLabel(nullptr)
    , mpChatHistory(nullptr)
    , mpInputField(nullptr)
    , mpSendButton(nullptr)
    , mpClearButton(nullptr)
    , mpOptionsButton(nullptr)
    , mpPatchPreview(nullptr)
    , mpAcceptButton(nullptr)
    , mpRejectButton(nullptr)
    , mpRegenerateButton(nullptr)
    , maLayoutIdle("officelabs_ui_chatpanel_layout")
    , mbConnected(false)
    , mbProcessing(false)
{
    Initialize();
}

ChatPanel::~ChatPanel()
{
}

void ChatPanel::Initialize()
{
    maLayoutIdle.SetPriority(TaskPriority::LOWEST);
    maLayoutIdle.SetInvokeHandler(LINK(this, ChatPanel, LayoutIdleHdl));

    mpStatusLabel = VclPtr<FixedText>::Create(this);
    mpStatusLabel->SetText("Disconnected");

    mpContextLabel = VclPtr<FixedText>::Create(this);
    mpContextLabel->SetWrap(true);

    mpChatHistory = VclPtr<Control>::Create(this);
    mpChatHistory->SetPaintTransparent(true);

    mpInputField = VclPtr<Edit>::Create(this);
    mpInputField->SetQuickHelpText("Ask the agent anything about your document");
    mpInputField->SetKeyPressHdl(LINK(this, ChatPanel, InputKeyHdl));

    mpSendButton = VclPtr<PushButton>::Create(this);
    mpSendButton->SetText("Send");
    mpSendButton->SetClickHdl(LINK(this, ChatPanel, SendClickHdl));

    mpClearButton = VclPtr<PushButton>::Create(this);
    mpClearButton->SetText("Clear");
    mpClearButton->SetClickHdl(LINK(this, ChatPanel, ClearClickHdl));

    mpOptionsButton = VclPtr<MenuButton>::Create(this);
    mpOptionsButton->SetText("Options");

    mpPatchPreview = VclPtr<Control>::Create(this);
    mpPatchPreview->Hide();

    mpAcceptButton = VclPtr<PushButton>::Create(this);
    mpAcceptButton->SetText("Apply");
    mpAcceptButton->SetClickHdl(LINK(this, ChatPanel, AcceptClickHdl));
    mpAcceptButton->Hide();

    mpRejectButton = VclPtr<PushButton>::Create(this);
    mpRejectButton->SetText("Dismiss");
    mpRejectButton->SetClickHdl(LINK(this, ChatPanel, RejectClickHdl));
    mpRejectButton->Hide();

    mpRegenerateButton = VclPtr<PushButton>::Create(this);
    mpRegenerateButton->SetText("Retry");
    mpRegenerateButton->SetClickHdl(LINK(this, ChatPanel, RegenerateClickHdl));
    mpRegenerateButton->Hide();

    CreateLayout();
    LayoutWidgets();
}

void ChatPanel::CreateLayout()
{
    // TODO: Replace with actual AutoLayout implementation once integrated
}

void ChatPanel::LayoutWidgets()
{
    Size aSize = GetOutputSizePixel();
    const long nPadding = 12;
    const long nButtonWidth = 80;
    const long nButtonHeight = 26;

    Point aStatusPos(nPadding, nPadding);
    mpStatusLabel->SetPosPixel(aStatusPos);

    Point aContextPos(nPadding, aStatusPos.Y() + mpStatusLabel->GetSizePixel().Height() + 4);
    Size aContextSize(aSize.Width() - 2 * nPadding, 48);
    mpContextLabel->SetPosSizePixel(aContextPos, aContextSize);

    Point aChatPos(nPadding, aContextPos.Y() + aContextSize.Height() + nPadding);
    Size aChatSize(aSize.Width() - 2 * nPadding, aSize.Height() - (4 * nPadding + 48 + nButtonHeight * 2));
    mpChatHistory->SetPosSizePixel(aChatPos, aChatSize);

    Point aInputPos(nPadding, aChatPos.Y() + aChatSize.Height() + nPadding);
    Size aInputSize(aSize.Width() - 3 * nPadding - nButtonWidth * 2, nButtonHeight);
    mpInputField->SetPosSizePixel(aInputPos, aInputSize);

    Point aSendPos(aInputPos.X() + aInputSize.Width() + nPadding, aInputPos.Y());
    mpSendButton->SetPosSizePixel(aSendPos, Size(nButtonWidth, nButtonHeight));

    Point aClearPos(aSendPos.X() + nButtonWidth + nPadding, aSendPos.Y());
    mpClearButton->SetPosSizePixel(aClearPos, Size(nButtonWidth, nButtonHeight));

    Point aPatchPos(nPadding, aInputPos.Y() + nButtonHeight + nPadding);
    Size aPatchSize(aSize.Width() - 2 * nPadding, 120);
    mpPatchPreview->SetPosSizePixel(aPatchPos, aPatchSize);

    Point aAcceptPos(aPatchPos.X(), aPatchPos.Y() + aPatchSize.Height() + 4);
    mpAcceptButton->SetPosSizePixel(aAcceptPos, Size(nButtonWidth, nButtonHeight));

    Point aRejectPos(aAcceptPos.X() + nButtonWidth + nPadding, aAcceptPos.Y());
    mpRejectButton->SetPosSizePixel(aRejectPos, Size(nButtonWidth, nButtonHeight));

    Point aRegeneratePos(aRejectPos.X() + nButtonWidth + nPadding, aRejectPos.Y());
    mpRegenerateButton->SetPosSizePixel(aRegeneratePos, Size(nButtonWidth, nButtonHeight));
}

void ChatPanel::StateChanged(StateChangedType nType)
{
    DockingWindow::StateChanged(nType);

    if (nType == StateChangedType::Resize)
    {
        LayoutWidgets();
    }
}

void ChatPanel::DataChanged(const DataChangedEvent& rDCEvt)
{
    DockingWindow::DataChanged(rDCEvt);
}

void ChatPanel::SendMessage(const OUString& rMessage)
{
    if (!rMessage.getLength())
        return;

    ChatMessage aMessage;
    aMessage.role = ChatMessage::Role::User;
    aMessage.content = rMessage;
    aMessage.timestamp = tools::Time();
    maMessages.push_back(aMessage);

    RenderMessages(*mpChatHistory);
    DispatchToAgent(rMessage);
    mpInputField->SetText(OUString());
}

void ChatPanel::DisplayResponse(const ChatMessage& rMessage)
{
    maMessages.push_back(rMessage);
    maCurrentPatch = rMessage.patch;

    RenderMessages(*mpChatHistory);

    if (maCurrentPatch.has_value())
    {
        RenderPatch(*mpPatchPreview);
        mpPatchPreview->Show();
        mpAcceptButton->Show();
        mpRejectButton->Show();
        mpRegenerateButton->Show();
    }

    SetProcessing(false);
}

void ChatPanel::ShowPatchPreview(const ChatPatch& rPatch)
{
    maCurrentPatch = rPatch;
    RenderPatch(*mpPatchPreview);
    mpPatchPreview->Show();
    mpAcceptButton->Show();
    mpRejectButton->Show();
    mpRegenerateButton->Show();
}

void ChatPanel::ClearChat()
{
    maMessages.clear();
    maCurrentPatch.reset();
    mpPatchPreview->Hide();
    mpAcceptButton->Hide();
    mpRejectButton->Hide();
    mpRegenerateButton->Hide();
    RenderMessages(*mpChatHistory);
}

void ChatPanel::SetConnectionStatus(bool bConnected)
{
    mbConnected = bConnected;
    UpdateStatusLabel();
}

void ChatPanel::SetProcessing(bool bProcessing)
{
    mbProcessing = bProcessing;
    UpdateStatusLabel();
    UpdateButtons();
}

void ChatPanel::SetDocumentContext(const OUString& rTitle, const OUString& rType)
{
    maDocumentTitle = rTitle;
    maDocumentType = rType;
    UpdateContextLabel();
}

void ChatPanel::SetSelection(const OUString& rSelection)
{
    maSelection = rSelection;
    UpdateContextLabel();
}

void ChatPanel::UpdateStatusLabel()
{
    OUStringBuffer aBuffer;
    aBuffer.append(mbConnected ? "Connected" : "Disconnected");
    if (mbProcessing)
        aBuffer.append(" \u2022 Working...");
    mpStatusLabel->SetText(aBuffer.makeStringAndClear());
}

void ChatPanel::UpdateContextLabel()
{
    OUStringBuffer aBuffer;
    if (!maDocumentTitle.isEmpty())
    {
        aBuffer.append("Document: ");
        aBuffer.append(maDocumentTitle);
        if (!maDocumentType.isEmpty())
        {
            aBuffer.append(" (");
            aBuffer.append(maDocumentType);
            aBuffer.append(")");
        }
    }

    if (!maSelection.isEmpty())
    {
        if (aBuffer.getLength())
            aBuffer.append("\n");
        aBuffer.append("Selection: ");
        aBuffer.append(maSelection);
    }

    mpContextLabel->SetText(aBuffer.makeStringAndClear());
}

void ChatPanel::UpdateButtons()
{
    mpSendButton->Enable(!mbProcessing);
    mpInputField->Enable(!mbProcessing);
}

void ChatPanel::RenderMessages(OutputDevice& rDevice)
{
    VirtualDevice aVirDev(rDevice);
    Size aSize = mpChatHistory->GetOutputSizePixel();
    aVirDev.SetOutputSizePixel(aSize);

    aVirDev.SetBackground(Wallpaper(Color(0xFFFFFF)));
    aVirDev.Erase();

    long nY = 0;
    const long nLineHeight = 18;
    const long nPadding = 8;

    for (const auto& rMessage : maMessages)
    {
        OUString aPrefix = GetRoleLabel(rMessage.role) + " " + FormatTimestamp(rMessage.timestamp) + ": ";
        OUString aLine = aPrefix + rMessage.content;
        aVirDev.DrawText(Point(nPadding, nY + nPadding), aLine);
        nY += nLineHeight + nPadding;
    }

    rDevice.DrawOutDev(Point(0, 0), aSize, Point(0, 0), aSize, aVirDev);
}

void ChatPanel::RenderPatch(OutputDevice& rDevice)
{
    if (!maCurrentPatch.has_value())
        return;

    VirtualDevice aVirDev(rDevice);
    Size aSize = mpPatchPreview->GetOutputSizePixel();
    aVirDev.SetOutputSizePixel(aSize);

    aVirDev.SetBackground(Wallpaper(Color(0xF7F7F7)));
    aVirDev.Erase();

    const ChatPatch& rPatch = *maCurrentPatch;
    OUStringBuffer aBuffer;
    aBuffer.append("Patch preview (");
    switch (rPatch.type)
    {
        case ChatPatch::Type::Insert:
            aBuffer.append("Insert");
            break;
        case ChatPatch::Type::Replace:
            aBuffer.append("Replace");
            break;
        case ChatPatch::Type::Delete:
            aBuffer.append("Delete");
            break;
    }
    aBuffer.append(")\n\n");
    aBuffer.append(rPatch.diff);

    aVirDev.DrawText(Point(8, 8), aBuffer.makeStringAndClear());
    rDevice.DrawOutDev(Point(0, 0), aSize, Point(0, 0), aSize, aVirDev);
}

IMPL_LINK(ChatPanel, SendClickHdl, Button*, /*pButton*/, void)
{
    SendMessage(mpInputField->GetText());
}

IMPL_LINK(ChatPanel, ClearClickHdl, Button*, /*pButton*/, void)
{
    ClearChat();
}

IMPL_LINK(ChatPanel, AcceptClickHdl, Button*, /*pButton*/, void)
{
    if (!maCurrentPatch.has_value())
        return;
    // TODO: Apply patch to document via command dispatch
    mpPatchPreview->Hide();
    mpAcceptButton->Hide();
    mpRejectButton->Hide();
    mpRegenerateButton->Hide();
}

IMPL_LINK(ChatPanel, RejectClickHdl, Button*, /*pButton*/, void)
{
    maCurrentPatch.reset();
    mpPatchPreview->Hide();
    mpAcceptButton->Hide();
    mpRejectButton->Hide();
    mpRegenerateButton->Hide();
}

IMPL_LINK(ChatPanel, RegenerateClickHdl, Button*, /*pButton*/, void)
{
    if (maMessages.empty())
        return;
    const OUString aLastUserPrompt = maMessages.back().content;
    DispatchToAgent(aLastUserPrompt);
}

IMPL_LINK(ChatPanel, InputKeyHdl, KeyEvent&, rEvent, bool)
{
    if (rEvent.GetKeyCode().GetCode() == KEY_RETURN)
    {
        SendMessage(mpInputField->GetText());
        return true;
    }
    return false;
}

IMPL_LINK(ChatPanel, LayoutIdleHdl, Idle*, /*pIdle*/, void)
{
    maLayoutIdle.Stop();
    LayoutWidgets();
}

void ChatPanel::DispatchToAgent(const OUString& rMessage)
{
    (void)rMessage;
    NotifyAssistantTyping();
}

void ChatPanel::NotifyAssistantTyping()
{
    SetProcessing(true);
    maLayoutIdle.Start();
}

} // namespace ui
} // namespace officelabs

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
