# OfficeLabs Project Structure

**Complete directory layout for the fork-based architecture**

---

## 🏗️ Repository Structure

OfficeLabs consists of **two main repositories**:

### 1. OfficeLabs Core (LibreOffice Fork)
**Repository:** `https://github.com/Pher217/officelabs-core`

```
officelabs-core/
├── officelabs/                    # OfficeLabs-specific code (NEW)
│   ├── agent/                     # Agent integration layer (C++)
│   │   ├── IAgentModule.hxx       # Base agent interface
│   │   ├── WriterAgent.cxx        # Writer agent module
│   │   ├── CalcAgent.cxx          # Calc agent module
│   │   ├── ImpressAgent.cxx       # Impress agent module
│   │   ├── MacroAgent.cxx         # Macro agent module
│   │   └── AgentRouter.cxx        # Route to correct agent
│   │
│   ├── context/                   # Document context engine (C++)
│   │   ├── ContextEngine.hxx      # Context management
│   │   ├── DocumentObject.hxx     # Object data structures
│   │   ├── AddressResolver.cxx    # @-address resolution
│   │   └── Indexer.cxx            # Real-time indexing
│   │
│   ├── ipc/                       # Inter-process communication (C++)
│   │   ├── AgentConnection.hxx    # IPC to Python backend
│   │   ├── JsonRpc.cxx            # JSON-RPC implementation
│   │   ├── UnixSocket.cxx         # Unix socket (Linux/Mac)
│   │   └── NamedPipe.cxx          # Named pipe (Windows)
│   │
│   ├── ui/                        # UI components (C++)
│   │   ├── ChatPanel.hxx          # Main chat panel
│   │   ├── CommandPalette.hxx     # Ctrl+K command palette
│   │   ├── PatchPreview.hxx       # Diff viewer
│   │   └── StatusIndicator.hxx    # Connection status
│   │
│   ├── git/                       # Git integration (C++)
│   │   ├── GitManager.hxx         # Git operations
│   │   ├── AutoCommit.cxx         # Auto-commit on changes
│   │   └── FlatOdfConverter.cxx   # Binary ↔ Flat ODF
│   │
│   ├── config/                    # Configuration
│   │   ├── Config.hxx             # Config management
│   │   └── officelabs.conf        # Default config
│   │
│   └── Module_officelabs.mk       # Build module definition
│
├── vcl/                           # LibreOffice VCL (MODIFIED)
│   └── source/
│       └── ai/                    # AI-specific widgets
│           ├── ChatPanel.cxx      # Chat panel implementation
│           ├── CommandPalette.cxx # Command palette
│           └── GhostText.cxx      # Inline ghost text
│
├── sw/                            # Writer (MODIFIED)
│   ├── source/
│   │   ├── ai/                    # Writer AI integration
│   │   │   ├── WriterContext.cxx  # Writer context extraction
│   │   │   ├── GhostText.cxx      # Ghost text for Writer
│   │   │   └── PatchApplier.cxx   # Apply patches to doc
│   │   └── uibase/
│   │       └── app/
│   │           └── appenv.cxx     # Register chat panel (MODIFIED)
│   └── sdi/
│       └── swriter.sdi            # Menu items (MODIFIED)
│
├── sc/                            # Calc (MODIFIED)
│   └── source/
│       └── ai/                    # Calc AI integration
│           ├── CalcContext.cxx    # Calc context extraction
│           └── FormulaPatch.cxx   # Apply formula patches
│
├── sd/                            # Impress (MODIFIED)
│   └── source/
│       └── ai/                    # Impress AI integration
│           ├── ImpressContext.cxx # Impress context
│           └── SlidePatch.cxx     # Slide modifications
│
├── Repository_core.mk             # Repository build config
├── RepositoryModule_host.mk       # Module registration (MODIFIED)
├── Makefile.in                    # Main makefile
├── configure.ac                   # Autoconf config (MODIFIED)
├── autogen.sh                     # Build configuration script
│
├── docs/                          # Documentation
│   ├── FORK_ARCHITECTURE.md       # Architecture overview
│   ├── FORK_GUIDE.md              # Build & fork guide
│   ├── AGENT_ARCHITECTURE.md      # Agent system design
│   ├── UI_INTEGRATION.md          # UI implementation
│   └── PROJECT_STRUCTURE.md       # This file
│
└── README.md                      # Main readme
```

### 2. OfficeLabs Agent (Python Backend)
**Repository:** `https://github.com/Pher217/officelabs-agent`

```
officelabs-agent/
├── src/
│   ├── main.py                    # FastAPI entry point
│   ├── config.py                  # Configuration management
│   │
│   ├── api/                       # API endpoints
│   │   ├── __init__.py
│   │   ├── chat.py                # POST /chat
│   │   ├── index.py               # POST /index/update, GET /index/objects
│   │   ├── patches.py             # POST /patch/preview, /patch/apply
│   │   └── health.py              # GET /health
│   │
│   ├── agents/                    # Agent implementations
│   │   ├── __init__.py
│   │   ├── base_agent.py          # BaseAgent class
│   │   ├── writer_agent.py        # WriterAgent
│   │   ├── calc_agent.py          # CalcAgent
│   │   ├── impress_agent.py       # ImpressAgent
│   │   ├── macro_agent.py         # MacroAgent
│   │   └── router.py              # AgentRouter
│   │
│   ├── llm/                       # LLM providers
│   │   ├── __init__.py
│   │   ├── base_client.py         # BaseLLMClient
│   │   ├── claude_client.py       # ClaudeClient
│   │   ├── openai_client.py       # OpenAIClient
│   │   ├── local_client.py        # LocalClient (Ollama)
│   │   └── router.py              # LLMRouter
│   │
│   ├── document/                  # Document operations
│   │   ├── __init__.py
│   │   ├── context_parser.py      # Parse context from C++ layer
│   │   ├── address_parser.py      # Parse @-addresses
│   │   └── pyuno_client.py        # PyUNO connection
│   │
│   ├── patches/                   # Patch system
│   │   ├── __init__.py
│   │   ├── generator.py           # Generate patches from LLM
│   │   ├── applier.py             # Apply patches via PyUNO
│   │   ├── validator.py           # Validate patch safety
│   │   └── storage.py             # Store patches temporarily
│   │
│   ├── git/                       # Git integration
│   │   ├── __init__.py
│   │   ├── manager.py             # Git operations
│   │   ├── commit.py              # Auto-commit logic
│   │   └── flat_odf.py            # Flat ODF helpers
│   │
│   ├── ipc/                       # IPC server
│   │   ├── __init__.py
│   │   ├── server.py              # Unix socket/named pipe server
│   │   └── jsonrpc.py             # JSON-RPC handler
│   │
│   ├── memory/                    # Agent memory (future)
│   │   ├── __init__.py
│   │   ├── conversation.py        # Conversation history
│   │   └── preferences.py         # User preferences
│   │
│   └── mcp/                       # MCP server (future)
│       ├── __init__.py
│       └── server.py              # Model Context Protocol
│
├── config/                        # Configuration files
│   ├── config.example.yaml        # Example config
│   └── prompts/                   # System prompt templates
│       ├── writer.txt
│       ├── calc.txt
│       ├── impress.txt
│       └── macro.txt
│
├── tests/                         # Tests
│   ├── test_agents/
│   │   ├── test_writer_agent.py
│   │   ├── test_calc_agent.py
│   │   └── test_impress_agent.py
│   ├── test_llm/
│   ├── test_patches/
│   └── test_api/
│
├── scripts/                       # Utility scripts
│   ├── start_dev.sh               # Start in dev mode
│   └── test_connection.py         # Test IPC connection
│
├── requirements.txt               # Python dependencies
├── pyproject.toml                 # Python project config
├── pytest.ini                     # Pytest config
├── Dockerfile                     # Docker image
└── README.md                      # Agent readme
```

---

## 📦 Build Artifacts

After building, the structure looks like:

```
officelabs-core/
├── instdir/                       # Installation directory
│   ├── program/
│   │   ├── soffice                # OfficeLabs executable
│   │   ├── soffice.bin            # Main binary
│   │   ├── libofficelabs_agent.so # Agent library
│   │   ├── libofficelabs_ipc.so   # IPC library
│   │   └── ...                    # Other libraries
│   └── share/
│       └── config/
│           └── officelabs.conf    # Config file
│
└── workdir/                       # Build working directory
    └── ...                        # Intermediate build files
```

---

## 🔧 Build System Integration

### Module Registration

**File:** `RepositoryModule_host.mk`

Add OfficeLabs module:

```makefile
$(eval $(call gb_Module_add_moduledirs,libreoffice,\
    accessibility \
    android \
    animations \
    ...
    officelabs \              # <-- NEW
    oox \
    ...
))
```

### OfficeLabs Module Definition

**File:** `officelabs/Module_officelabs.mk`

```makefile
# OfficeLabs module build definition

$(eval $(call gb_Module_Module,officelabs))

$(eval $(call gb_Module_add_targets,officelabs,\
    Library_officelabs_agent \
    Library_officelabs_context \
    Library_officelabs_ipc \
    Library_officelabs_ui \
    Library_officelabs_git \
))

# Platform-specific targets
ifneq ($(OS),WNT)
$(eval $(call gb_Module_add_targets,officelabs,\
    Package_officelabs_config \
    Package_officelabs_scripts \
))
endif
```

### Library Definitions

**File:** `officelabs/Library_officelabs_agent.mk`

```makefile
$(eval $(call gb_Library_Library,officelabs_agent))

$(eval $(call gb_Library_add_exception_objects,officelabs_agent,\
    officelabs/agent/IAgentModule \
    officelabs/agent/WriterAgent \
    officelabs/agent/CalcAgent \
    officelabs/agent/ImpressAgent \
    officelabs/agent/MacroAgent \
    officelabs/agent/AgentRouter \
))

$(eval $(call gb_Library_use_libraries,officelabs_agent,\
    sal \
    cppu \
    cppuhelper \
    officelabs_ipc \
    officelabs_context \
))

$(eval $(call gb_Library_use_externals,officelabs_agent,\
    boost_headers \
))
```

---

## 📝 Configuration Files

### OfficeLabs Config

**Location:** `~/.config/officelabs/config.yaml`

```yaml
# OfficeLabs Configuration

llm:
  provider: claude              # claude, openai, or local
  api_key: sk-ant-...           # Your API key
  model: claude-3-5-sonnet-20241022
  timeout: 30                   # seconds
  max_tokens: 4096

agent:
  host: localhost
  port: 8765
  socket_path: /tmp/officelabs-agent.sock  # Unix
  # pipe_name: \\.\pipe\officelabs-agent   # Windows
  auto_start: true              # Auto-start agent service
  debug: false

features:
  chat_panel: true
  command_palette: true
  ghost_text: true
  patch_preview: true
  auto_index: true

git:
  enabled: true
  auto_commit: false            # Commit on every save
  commit_on_ai_change: true     # Commit AI changes
  default_format: flat_odf      # flat_odf or binary
  export_readable: true         # Export TXT/CSV/MD

ui:
  theme: glacier_blue
  panel_width: 400
  panel_position: right         # right, left, bottom
  show_timestamps: true
  compact_mode: false

privacy:
  send_document_content: true   # Send to LLM
  local_only: false             # Use only local LLM
  telemetry: false              # Usage analytics
```

### Agent Service Config

**Location:** `officelabs-agent/config/config.yaml`

```yaml
# OfficeLabs Agent Service Configuration

server:
  host: localhost
  port: 8765
  socket_path: /tmp/officelabs-agent.sock
  log_level: INFO
  debug: false

llm:
  provider: claude
  api_key: ${ANTHROPIC_API_KEY}  # From environment
  model: claude-3-5-sonnet-20241022
  temperature: 0.7
  max_tokens: 4096
  
  # Fallback providers
  fallback:
    - provider: openai
      model: gpt-4
    - provider: local
      model: llama2

pyuno:
  host: localhost
  port: 2002
  connection_type: socket       # socket, pipe, or direct

storage:
  type: memory                  # memory, redis, or sqlite
  # redis_url: redis://localhost:6379
  # sqlite_path: /tmp/officelabs.db

logging:
  level: INFO
  file: /var/log/officelabs/agent.log
  max_size: 10MB
  backup_count: 5
```

---

## 🗂️ Data Directories

### Runtime Data

```
~/.local/share/officelabs/
├── cache/
│   ├── document_indexes/      # Cached document indexes
│   └── llm_responses/         # Cached LLM responses (opt-in)
├── history/
│   └── conversations/         # Chat history
└── templates/
    └── prompts/               # Custom prompt templates
```

### Log Files

```
/var/log/officelabs/           # Linux
~/Library/Logs/OfficeLabs/     # macOS
%APPDATA%\OfficeLabs\Logs\     # Windows

├── core.log                   # C++ layer logs
├── agent.log                  # Agent service logs
└── ipc.log                    # IPC communication logs
```

---

## 🔗 Dependency Management

### C++ Dependencies (LibreOffice Core)

**Managed by:** LibreOffice build system

- Boost (headers only)
- ICU
- OpenSSL
- libxml2
- Cairo
- ...and standard LibreOffice deps

### Python Dependencies (Agent Service)

**File:** `officelabs-agent/requirements.txt`

```
# Web framework
fastapi==0.104.1
uvicorn[standard]==0.24.0
pydantic==2.5.0

# LLM clients
anthropic==0.7.0
openai==1.3.0
ollama==0.1.0

# PyUNO
uno==0.1.0  # If available via pip

# Document processing
python-docx==1.1.0
openpyxl==3.1.2
odfpy==1.4.1

# Git integration
GitPython==3.1.40

# Utils
pyyaml==6.0.1
python-dotenv==1.0.0
aiofiles==23.2.1

# Testing
pytest==7.4.3
pytest-asyncio==0.21.1
httpx==0.25.1

# Development
black==23.11.0
flake8==6.1.0
mypy==1.7.0
```

---

## 🧪 Testing Structure

### C++ Tests (Core)

```
officelabs-core/
└── officelabs/
    └── qa/
        ├── unit/
        │   ├── test_agent_connection.cxx
        │   ├── test_context_engine.cxx
        │   └── test_address_resolver.cxx
        └── integration/
            ├── test_writer_integration.cxx
            ├── test_calc_integration.cxx
            └── test_impress_integration.cxx
```

### Python Tests (Agent)

```
officelabs-agent/
└── tests/
    ├── test_agents/
    ├── test_llm/
    ├── test_patches/
    ├── test_api/
    └── integration/
        ├── test_full_flow.py
        └── test_ipc.py
```

---

## 📚 Documentation Structure

```
docs/
├── architecture/
│   ├── FORK_ARCHITECTURE.md       # Overall architecture
│   ├── AGENT_ARCHITECTURE.md      # Agent system
│   └── UI_INTEGRATION.md          # UI design
│
├── guides/
│   ├── FORK_GUIDE.md              # How to build
│   ├── DEVELOPMENT.md             # Contributing
│   ├── USER_GUIDE.md              # Using OfficeLabs
│   └── QUICKSTART.md              # Quick start
│
├── reference/
│   ├── API.md                     # Agent API reference
│   ├── CONFIG.md                  # Configuration options
│   └── ADDRESSING.md              # @-address syntax
│
└── PROJECT_STRUCTURE.md           # This file
```

---

## 🚀 Deployment Structure

### Linux (AppImage)

```
OfficeLabs.AppImage
├── usr/
│   ├── bin/
│   │   └── soffice
│   ├── lib/
│   │   └── libreoffice/
│   │       └── program/
│   │           ├── soffice.bin
│   │           └── libofficelabs_*.so
│   └── share/
│       └── applications/
│           └── officelabs.desktop
├── AppRun
└── officelabs.desktop
```

### macOS (DMG)

```
OfficeLabs.app/
├── Contents/
│   ├── Info.plist
│   ├── MacOS/
│   │   └── soffice
│   ├── Resources/
│   │   └── OfficeLabs.icns
│   └── Frameworks/
│       └── ...
```

### Windows (MSI)

```
C:\Program Files\OfficeLabs\
├── program\
│   ├── soffice.exe
│   ├── soffice.bin
│   └── officelabs_*.dll
├── share\
│   └── config\
└── python\
    └── officelabs-agent\
```

---

**OfficeLabs** - Clean, modular, powerful

