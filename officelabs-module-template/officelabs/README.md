# OfficeLabs Module

**AI Integration Layer for LibreOffice**

---

## 📁 Structure

```
officelabs/
├── agent/          # Agent integration layer (C++)
│   ├── IAgentModule.hxx      # Base agent interface
│   ├── WriterAgent.cxx       # Writer-specific agent
│   ├── CalcAgent.cxx         # Calc-specific agent
│   ├── ImpressAgent.cxx      # Impress-specific agent
│   └── MacroAgent.cxx        # Python macro agent
│
├── context/        # Document context engine (C++)
│   ├── ContextEngine.hxx     # Context management
│   ├── DocumentObject.hxx    # Object data structures
│   ├── AddressResolver.cxx   # @-address resolution
│   └── Indexer.cxx           # Real-time indexing
│
├── ipc/            # Inter-process communication (C++)
│   ├── AgentConnection.hxx   # IPC to Python backend
│   ├── JsonRpc.cxx           # JSON-RPC implementation
│   ├── UnixSocket.cxx        # Unix socket (Linux/Mac)
│   └── NamedPipe.cxx         # Named pipe (Windows)
│
├── ui/             # UI components (C++)
│   ├── ChatPanel.hxx         # Main chat panel
│   ├── CommandPalette.hxx    # Ctrl+K command palette
│   ├── PatchPreview.hxx      # Diff viewer
│   └── StatusIndicator.hxx   # Connection status
│
├── git/            # Git integration (C++)
│   ├── GitManager.hxx        # Git operations
│   ├── AutoCommit.cxx        # Auto-commit on changes
│   └── FlatOdfConverter.cxx  # Binary ↔ Flat ODF
│
└── config/         # Configuration
    ├── Config.hxx            # Config management
    └── officelabs.conf       # Default config
```

---

## 🔧 Building

The OfficeLabs module is integrated into the LibreOffice build system.

### Build Everything

```bash
make build-nocheck
```

### Build Only OfficeLabs Module

```bash
make officelabs.build
```

### Build Specific Library

```bash
make Library_officelabs_agent
make Library_officelabs_ipc
```

---

## 🧪 Testing

```bash
# Run unit tests
make officelabs.check

# Run integration tests
make officelabs.integrationtest
```

---

## 🔌 Integration Points

This module integrates with:

- **vcl/** - VCL UI widgets for chat panel
- **sw/** - Writer integration
- **sc/** - Calc integration
- **sd/** - Impress integration
- **sfx2/** - Document framework

---

## 📚 Documentation

- **Architecture:** See `docs/FORK_ARCHITECTURE.md` in repository root
- **API Reference:** See `docs/API.md`
- **Development:** See `docs/DEVELOPMENT.md`

---

## 🤝 Contributing

See the main OfficeLabs repository for contribution guidelines:
https://github.com/Pher217/officelabs-master

---

**OfficeLabs** - AI-native office suite

