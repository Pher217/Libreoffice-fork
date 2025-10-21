/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the OfficeLabs project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDED_OFFICELABS_AGENT_IAGENTMODULE_HXX
#define INCLUDED_OFFICELABS_AGENT_IAGENTMODULE_HXX

#include <rtl/ustring.hxx>
#include <memory>
#include <vector>
#include <map>

namespace officelabs {
namespace agent {

/**
 * Agent type enumeration
 */
enum class AgentType {
    WRITER,     // Document editor agent
    CALC,       // Spreadsheet agent
    IMPRESS,    // Presentation agent
    MACRO       // Python macro agent
};

/**
 * Document object for context
 */
struct DocumentObject {
    OUString type;          // Type: HEADING, PARAGRAPH, TABLE, CELL, SLIDE, etc.
    OUString address;       // @-address: @Para#3, @Heading("Introduction"), etc.
    OUString displayName;   // Display name: "Heading: Introduction"
    OUString preview;       // Content preview (first 100 chars)
    std::map<OUString, OUString> metadata;  // Additional metadata
};

/**
 * Agent context - information sent to the agent
 */
struct AgentContext {
    // Document info
    OUString documentTitle;
    OUString documentType;          // writer, calc, impress
    
    // Selection
    OUString selection;             // Currently selected content
    OUString selectedAddress;       // @-address of selection
    
    // Document structure
    std::vector<DocumentObject> documentObjects;
    
    // History
    std::vector<OUString> conversationHistory;
    
    // Additional context
    std::map<OUString, OUString> metadata;
};

/**
 * Patch operation types
 */
enum class PatchType {
    REPLACE,    // Replace content
    INSERT,     // Insert new content
    DELETE,     // Delete content
    FORMAT,     // Change formatting
    CREATE      // Create new object (table, heading, etc.)
};

/**
 * Agent response with patch
 */
struct AgentResponse {
    // Response message
    OUString message;
    
    // Patch information
    PatchType patchType;
    OUString patchTarget;       // @-address of target
    OUString patchOldValue;     // Old value (for REPLACE)
    OUString patchNewValue;     // New value
    OUString patchDiff;         // Diff preview
    
    // Metadata
    OUString model;             // LLM model used
    sal_Int32 tokens;           // Tokens used
    sal_Int32 latencyMs;        // Response latency in ms
    
    // Status
    bool success;
    OUString errorMessage;
};

/**
 * Base agent interface
 * 
 * All agent modules (Writer, Calc, Impress, Macro) implement this interface.
 */
class IAgentModule {
public:
    virtual ~IAgentModule() = default;
    
    /**
     * Get the agent type
     */
    virtual AgentType getType() const = 0;
    
    /**
     * Send a message to the agent
     * 
     * @param message User's message
     * @param context Current document context
     * @return Agent response with patch
     */
    virtual AgentResponse sendMessage(
        const OUString& message,
        const AgentContext& context
    ) = 0;
    
    /**
     * Check if agent is connected to backend service
     */
    virtual bool isConnected() const = 0;
    
    /**
     * Connect to agent backend service
     * 
     * @param socketPath Path to Unix socket or named pipe
     * @return true if connected successfully
     */
    virtual bool connect(const OUString& socketPath) = 0;
    
    /**
     * Disconnect from agent service
     */
    virtual void disconnect() = 0;
    
    /**
     * Update the document index
     * 
     * Called when document changes to update the context
     */
    virtual void updateIndex() = 0;
    
    /**
     * Get current document objects (for UI display)
     */
    virtual std::vector<DocumentObject> getDocumentObjects() const = 0;
};

/**
 * Create agent instance for given type
 * 
 * Factory function to create the appropriate agent implementation
 */
std::shared_ptr<IAgentModule> createAgent(AgentType type);

} // namespace agent
} // namespace officelabs

#endif // INCLUDED_OFFICELABS_AGENT_IAGENTMODULE_HXX

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */

