# OfficeLabs Fork Guide

Complete guide to forking LibreOffice and building OfficeLabs.

---

## 🎯 Overview

This guide covers:
1. Forking LibreOffice core
2. Setting up the build environment
3. Integrating OfficeLabs AI layer
4. Building and packaging
5. Maintaining upstream sync

---

## 📋 Prerequisites

### System Requirements

**Hardware:**
- CPU: 4+ cores (8+ recommended)
- RAM: 16GB minimum (32GB recommended)
- Disk: 50GB+ free space
- Network: Fast internet for initial clone

**Software:**

**Linux (Ubuntu/Debian):**
```bash
sudo apt-get install \
  git build-essential zip ccache \
  python3 python3-dev python3-pip \
  openjdk-11-jdk ant \
  gperf bison flex \
  libkrb5-dev libssl-dev libcups2-dev \
  libfontconfig1-dev libfreetype6-dev \
  libxrandr-dev libx11-dev libxrender-dev \
  libgl1-mesa-dev libglu1-mesa-dev \
  libjpeg-dev libpng-dev \
  autoconf automake libtool pkg-config
```

**macOS:**
```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install Homebrew
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install \
  autoconf automake libtool pkg-config \
  ccache gnu-tar python@3.11
```

**Windows:**
- Visual Studio 2022 (Community Edition)
- Git for Windows
- Cygwin or WSL2 (recommended)
- Python 3.9+

---

## 🔱 Step 1: Fork LibreOffice

### Create Your Fork

1. **Go to LibreOffice GitHub:**
   ```
   https://github.com/LibreOffice/core
   ```

2. **Click "Fork"** → Create fork under your organization:
   ```
   https://github.com/Pher217/officelabs-core
   ```

### Clone the Fork

```bash
# Clone your fork
git clone https://github.com/Pher217/officelabs-core.git
cd officelabs-core

# Add upstream remote
git remote add upstream https://github.com/LibreOffice/core.git

# Create our main branch
git checkout -b officelabs-main

# Create sync branch
git checkout -b libreoffice-sync upstream/master
```

### Branch Strategy

```
officelabs-main       ← Our integrated features (ship this)
    ↑
    │ (cherry-pick fixes)
    │
libreoffice-sync      ← Tracks upstream LibreOffice
    ↑
    │ (fetch/merge)
    │
upstream/master       ← LibreOffice official
```

---

## 🏗️ Step 2: Build LibreOffice (Baseline)

### Initial Configuration

```bash
cd officelabs-core

# Run autogen
./autogen.sh \
  --with-vendor="OfficeLabs" \
  --with-theme="officelabs" \
  --enable-python=system \
  --without-java \
  --disable-odk \
  --enable-ccache
```

**Explanation:**
- `--with-vendor="OfficeLabs"` → Branding
- `--with-theme="officelabs"` → Custom theme
- `--enable-python=system` → Use system Python
- `--without-java` → Skip Java (optional, speeds up build)
- `--enable-ccache` → Speed up rebuilds

### First Build

```bash
# Build everything (this takes 1-3 hours)
make build-nocheck

# Or with parallel jobs (faster)
make build-nocheck -j8
```

**Note:** First build is slow. Subsequent builds are much faster with ccache.

### Test the Build

```bash
# Run LibreOffice Writer
./instdir/program/soffice --writer
```

If this works, you've successfully built LibreOffice! 🎉

---

## 🤖 Step 3: Integrate OfficeLabs AI Layer

### Add OfficeLabs Modules

Create directory structure:

```bash
mkdir -p officelabs/agent
mkdir -p officelabs/context
mkdir -p officelabs/ui
mkdir -p officelabs/ipc
```

### Create Base Agent Interface

**File:** `officelabs/agent/IAgentModule.hxx`

```cpp
#ifndef INCLUDED_OFFICELABS_AGENT_IAGENTMODULE_HXX
#define INCLUDED_OFFICELABS_AGENT_IAGENTMODULE_HXX

#include <rtl/ustring.hxx>
#include <memory>

namespace officelabs {
namespace agent {

enum class AgentType {
    WRITER,
    CALC,
    IMPRESS,
    MACRO
};

struct AgentContext {
    OUString documentTitle;
    OUString documentType;
    OUString selection;
    OUString selectedAddress;  // @-address
    // More context fields...
};

struct AgentResponse {
    OUString message;
    OUString patchType;
    OUString patchTarget;
    OUString patchOldValue;
    OUString patchNewValue;
    OUString patchDiff;
};

class IAgentModule {
public:
    virtual ~IAgentModule() = default;
    
    virtual AgentType getType() const = 0;
    virtual AgentResponse sendMessage(
        const OUString& message,
        const AgentContext& context
    ) = 0;
    virtual bool isConnected() const = 0;
};

} // namespace agent
} // namespace officelabs

#endif
```

### Create IPC Connection Module

**File:** `officelabs/ipc/AgentConnection.hxx`

```cpp
#ifndef INCLUDED_OFFICELABS_IPC_AGENTCONNECTION_HXX
#define INCLUDED_OFFICELABS_IPC_AGENTCONNECTION_HXX

#include <rtl/ustring.hxx>
#include <memory>

namespace officelabs {
namespace ipc {

class AgentConnection {
public:
    AgentConnection();
    ~AgentConnection();
    
    // Connect to agent service
    bool connect(const OUString& socketPath);
    void disconnect();
    bool isConnected() const;
    
    // Send JSON-RPC request
    OUString sendRequest(
        const OUString& method,
        const OUString& params
    );
    
private:
    struct Impl;
    std::unique_ptr<Impl> mpImpl;
};

} // namespace ipc
} // namespace officelabs

#endif
```

**File:** `officelabs/ipc/AgentConnection.cxx`

```cpp
#include "AgentConnection.hxx"
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>

namespace officelabs {
namespace ipc {

struct AgentConnection::Impl {
    int socketFd = -1;
    bool connected = false;
};

AgentConnection::AgentConnection()
    : mpImpl(std::make_unique<Impl>())
{
}

AgentConnection::~AgentConnection() {
    disconnect();
}

bool AgentConnection::connect(const OUString& socketPath) {
    // Unix socket connection
    mpImpl->socketFd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (mpImpl->socketFd < 0) {
        return false;
    }
    
    struct sockaddr_un addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    
    OString path = OUStringToOString(socketPath, RTL_TEXTENCODING_UTF8);
    std::strncpy(addr.sun_path, path.getStr(), sizeof(addr.sun_path) - 1);
    
    if (::connect(mpImpl->socketFd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        close(mpImpl->socketFd);
        mpImpl->socketFd = -1;
        return false;
    }
    
    mpImpl->connected = true;
    return true;
}

void AgentConnection::disconnect() {
    if (mpImpl->socketFd >= 0) {
        close(mpImpl->socketFd);
        mpImpl->socketFd = -1;
    }
    mpImpl->connected = false;
}

bool AgentConnection::isConnected() const {
    return mpImpl->connected;
}

OUString AgentConnection::sendRequest(
    const OUString& method,
    const OUString& params
) {
    if (!mpImpl->connected) {
        return OUString();
    }
    
    // Build JSON-RPC request
    OUString request = 
        "{"
        "\"jsonrpc\":\"2.0\","
        "\"id\":\"" + createUUID() + "\","
        "\"method\":\"" + method + "\","
        "\"params\":" + params +
        "}";
    
    // Send request
    OString utf8Request = OUStringToOString(request, RTL_TEXTENCODING_UTF8);
    ssize_t sent = send(mpImpl->socketFd, utf8Request.getStr(), 
                       utf8Request.getLength(), 0);
    
    if (sent < 0) {
        return OUString();
    }
    
    // Read response
    char buffer[4096];
    ssize_t received = recv(mpImpl->socketFd, buffer, sizeof(buffer) - 1, 0);
    
    if (received <= 0) {
        return OUString();
    }
    
    buffer[received] = '\0';
    return OUString::createFromAscii(buffer);
}

} // namespace ipc
} // namespace officelabs
```

### Create AI Chat Panel Widget

**File:** `vcl/source/ai/ChatPanel.hxx`

```cpp
#ifndef INCLUDED_VCL_AI_CHATPANEL_HXX
#define INCLUDED_VCL_AI_CHATPANEL_HXX

#include <vcl/dockwin.hxx>
#include <vcl/edit.hxx>
#include <vcl/button.hxx>
#include <vcl/fixed.hxx>
#include <officelabs/agent/IAgentModule.hxx>

namespace vcl {

class ChatPanel : public DockingWindow {
public:
    ChatPanel(vcl::Window* pParent);
    virtual ~ChatPanel() override;
    
    void setAgent(std::shared_ptr<officelabs::agent::IAgentModule> agent);
    void sendMessage(const OUString& message);
    void displayResponse(const officelabs::agent::AgentResponse& response);
    
private:
    VclPtr<FixedText> mpStatusLabel;
    VclPtr<ScrollBar> mpChatHistory;
    VclPtr<Edit> mpInputField;
    VclPtr<PushButton> mpSendButton;
    VclPtr<PushButton> mpReindexButton;
    
    std::shared_ptr<officelabs::agent::IAgentModule> mpAgent;
    
    DECL_LINK(SendClickHdl, Button*, void);
    DECL_LINK(ReindexClickHdl, Button*, void);
    
    void layoutChildren();
    void appendMessage(const OUString& sender, const OUString& message);
};

} // namespace vcl

#endif
```

### Modify Build System

**File:** `officelabs/Module_officelabs.mk`

```makefile
$(eval $(call gb_Module_Module,officelabs))

$(eval $(call gb_Module_add_targets,officelabs,\
    Library_officelabs_agent \
    Library_officelabs_ipc \
))

ifneq ($(OS),WNT)
$(eval $(call gb_Module_add_targets,officelabs,\
    Package_officelabs_config \
))
endif
```

**File:** `officelabs/Library_officelabs_agent.mk`

```makefile
$(eval $(call gb_Library_Library,officelabs_agent))

$(eval $(call gb_Library_add_exception_objects,officelabs_agent,\
    officelabs/agent/IAgentModule \
    officelabs/agent/WriterAgent \
    officelabs/agent/CalcAgent \
    officelabs/agent/ImpressAgent \
    officelabs/agent/MacroAgent \
))

$(eval $(call gb_Library_use_libraries,officelabs_agent,\
    sal \
    cppu \
    cppuhelper \
    officelabs_ipc \
))
```

### Register Module in Top-Level Build

**File:** `RepositoryModule_host.mk`

Add to the module list:
```makefile
$(eval $(call gb_Module_add_moduledirs,libreoffice,\
    ...existing modules...
    officelabs \
))
```

---

## 🔧 Step 4: Build OfficeLabs

### Configure with OfficeLabs Features

```bash
./autogen.sh \
  --with-vendor="OfficeLabs" \
  --with-theme="officelabs" \
  --enable-python=system \
  --with-officelabs \
  --enable-ai-features \
  --with-agent-socket=/tmp/officelabs-agent.sock
```

### Build

```bash
make build-nocheck -j8
```

### Test AI Panel

```bash
# Start agent service first (in another terminal)
cd ../officelabs-agent
python -m src.main

# Then start OfficeLabs
cd ../officelabs-core
./instdir/program/soffice --writer
```

---

## 📦 Step 5: Packaging

### Linux (AppImage)

```bash
# Install appimagetool
wget https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage
chmod +x appimagetool-x86_64.AppImage

# Create AppDir
mkdir -p OfficeLabs.AppDir/usr
cp -r instdir/* OfficeLabs.AppDir/usr/

# Create desktop file
cat > OfficeLabs.AppDir/OfficeLabs.desktop <<EOF
[Desktop Entry]
Type=Application
Name=OfficeLabs
Exec=soffice
Icon=officelabs
Categories=Office;
EOF

# Create AppImage
./appimagetool-x86_64.AppImage OfficeLabs.AppDir OfficeLabs-x86_64.AppImage
```

### macOS (DMG)

```bash
# Build installer
cd macosx
make OfficeLabs.app

# Create DMG
hdiutil create -volname "OfficeLabs" \
  -srcfolder OfficeLabs.app \
  -ov -format UDZO \
  OfficeLabs.dmg
```

### Windows (MSI)

```bash
# Build installer
cd win32
nmake /f makefile.mk

# Output: OfficeLabs-Setup.msi
```

---

## 🔄 Step 6: Maintaining Upstream Sync

### Quarterly Sync Process

```bash
# Switch to sync branch
git checkout libreoffice-sync

# Fetch upstream
git fetch upstream

# Merge upstream changes
git merge upstream/master

# Review and resolve conflicts
# (Most conflicts will be in build files, not our code)

# Test the build
./autogen.sh
make build-nocheck

# If successful, cherry-pick fixes to main
git checkout officelabs-main
git cherry-pick <commit-hash>  # For security/bug fixes
```

### Selective Merge Strategy

**What to Merge:**
- ✅ Security fixes
- ✅ Bug fixes
- ✅ Performance improvements
- ✅ File format updates

**What NOT to Merge:**
- ❌ UI changes that conflict with our AI panel
- ❌ Features that duplicate our AI capabilities
- ❌ Build system changes that break our modules

### Patch Management

Store our changes as patches:

```bash
# Generate patches for our features
git format-patch upstream/master --stdout > officelabs-patches/ai-integration.patch

# Reapply patches after upstream sync
git apply officelabs-patches/ai-integration.patch
```

---

## 🐛 Troubleshooting

### Build Fails with "Module officelabs not found"

**Solution:**
```bash
# Clean build
make clean
./autogen.sh --with-officelabs
make build-nocheck
```

### Agent Connection Fails

**Solution:**
```bash
# Check if agent is running
ps aux | grep officelabs-agent

# Check socket file
ls -la /tmp/officelabs-agent.sock

# Test socket connection
nc -U /tmp/officelabs-agent.sock
```

### UI Panel Not Showing

**Solution:**
```bash
# Check if module is compiled
ls instdir/program/libofficelabs_agent.so

# Enable debug logging
export OFFICELABS_DEBUG=1
./instdir/program/soffice --writer
```

### Upstream Merge Conflicts

**Solution:**
```bash
# Use ours strategy for build files
git checkout --ours RepositoryModule_host.mk

# Manually resolve code conflicts
git mergetool

# Test after resolution
make build-nocheck
```

---

## 📊 Build Performance Tips

### Use ccache

```bash
# Install ccache
sudo apt-get install ccache

# Configure
./autogen.sh --enable-ccache

# Check stats
ccache -s
```

### Parallel Builds

```bash
# Use all CPU cores
make build-nocheck -j$(nproc)

# Or limit to 8 cores
make build-nocheck -j8
```

### Incremental Builds

```bash
# Only rebuild changed modules
make officelabs.build

# Or specific library
make Library_officelabs_agent
```

### Reduce Build Scope

```bash
# Skip tests
--disable-odk

# Skip Java
--without-java

# Skip help files
--without-help
```

---

## 🧪 Development Workflow

### Typical Development Cycle

1. **Make changes** to C++ code
2. **Rebuild module:**
   ```bash
   make officelabs.build
   ```
3. **Test:**
   ```bash
   ./instdir/program/soffice --writer
   ```
4. **Iterate**

### Hot Reload (Python Agent)

Agent service supports hot reload:
```bash
cd officelabs-agent
uvicorn src.main:app --reload
```

C++ changes require rebuild.

---

## 📚 Resources

- **LibreOffice Build Guide:** https://wiki.documentfoundation.org/Development/BuildingOnLinux
- **VCL Documentation:** https://docs.libreoffice.org/vcl/html/
- **UNO API:** https://api.libreoffice.org/
- **OfficeLabs Agent:** https://github.com/Pher217/officelabs-agent

---

## 🎓 Next Steps

1. Read [FORK_ARCHITECTURE.md](./FORK_ARCHITECTURE.md)
2. Set up agent service from `officelabs-agent` repo
3. Start implementing Writer Agent
4. Join OfficeLabs development discussions

---

**Happy Hacking! 🚀**

