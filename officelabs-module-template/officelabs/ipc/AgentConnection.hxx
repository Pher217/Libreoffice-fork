/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the OfficeLabs project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDED_OFFICELABS_IPC_AGENTCONNECTION_HXX
#define INCLUDED_OFFICELABS_IPC_AGENTCONNECTION_HXX

#include <rtl/ustring.hxx>
#include <memory>

namespace officelabs {
namespace ipc {

/**
 * IPC connection to OfficeLabs agent service
 * 
 * Handles communication between LibreOffice (C++) and the agent service (Python)
 * using Unix sockets (Linux/Mac) or named pipes (Windows).
 * 
 * Protocol: JSON-RPC 2.0
 */
class AgentConnection {
public:
    AgentConnection();
    ~AgentConnection();
    
    /**
     * Connect to agent service
     * 
     * @param socketPath Unix socket path (e.g., "/tmp/officelabs-agent.sock")
     *                   or named pipe (e.g., "\\\\.\\pipe\\officelabs-agent")
     * @return true if connected successfully
     */
    bool connect(const OUString& socketPath);
    
    /**
     * Disconnect from agent service
     */
    void disconnect();
    
    /**
     * Check if connected
     */
    bool isConnected() const;
    
    /**
     * Send JSON-RPC request to agent service
     * 
     * @param method JSON-RPC method name (e.g., "chat", "index.update")
     * @param params JSON parameters as string
     * @return JSON response as string
     * 
     * Example:
     *   response = sendRequest("chat", "{"message": "Make @Para#3 more concise", ...}")
     */
    OUString sendRequest(
        const OUString& method,
        const OUString& params
    );
    
    /**
     * Send async request (non-blocking)
     * 
     * Returns immediately, callback is called when response is ready
     */
    void sendRequestAsync(
        const OUString& method,
        const OUString& params,
        std::function<void(const OUString&)> callback
    );
    
    /**
     * Get last error message
     */
    OUString getLastError() const;
    
private:
    struct Impl;
    std::unique_ptr<Impl> mpImpl;
    
    // Helper methods
    OUString createRequestId();
    bool sendData(const char* data, size_t size);
    OUString receiveData();
};

} // namespace ipc
} // namespace officelabs

#endif // INCLUDED_OFFICELABS_IPC_AGENTCONNECTION_HXX

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */

