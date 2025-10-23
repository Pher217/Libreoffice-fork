# OfficeLabs

**AI-Native Office Suite** - LibreOffice reimagined with Cursor-inspired AI integration

[![License](https://img.shields.io/badge/license-MPL%202.0-blue.svg)](LICENSE)
[![LibreOffice Fork](https://img.shields.io/badge/fork-LibreOffice%207.6+-green.svg)](https://www.libreoffice.org/)
[![AI Powered](https://img.shields.io/badge/AI-Claude%20%7C%20GPT%20%7C%20Local-blue.svg)]()

## 🎯 What is OfficeLabs?

OfficeLabs is a **LibreOffice fork** that deeply integrates AI capabilities into the core office experience, inspired by how **Cursor** transformed VS Code. Unlike extensions or plugins, AI is **native** to OfficeLabs.

**Think:** Cursor for documents, spreadsheets, and presentations.

### Key Features

- 🤖 **Native AI Chat Panel** - Embedded directly in Writer, Calc, and Impress (not a sidebar hack)
- 📍 **@-Addressable Objects** - Reference any heading, paragraph, table, cell, or slide naturally
- 👻 **Inline Ghost Text** - Cursor-style AI suggestions (Tab to accept)
- ⌨️ **Command Palette** - `Ctrl+K` for instant AI actions
- 🔍 **Context-Aware AI** - Understands your document structure and selection
- 🎨 **Structured Patches** - See exactly what changes before applying
- 📚 **Git-Based Versioning** - Every document is a Git repo with full history
- 🌍 **Multi-Model Support** - Claude, GPT-4, or local LLMs (Ollama)
- 🐍 **Python Macro Agent** - Generate and execute Python automation
- 💾 **Full Office Compatibility** - Native DOCX/XLSX/PPTX support

## 🏗️ Architecture

OfficeLabs is built as a **two-layer system**:

### Layer 1: OfficeLabs Core (C++ Fork)
**Repository:** This repo (fork of LibreOffice/core)

- **Native AI Chat Widget** - VCL-based UI embedded in all apps
- **Agent Integration Layer** - C++ modules for each agent type
- **Context Engine** - Real-time document indexing and @-addressing
- **IPC Bridge** - Unix socket/named pipe to Python backend
- **Git Integration** - Automatic versioning for all documents

### Layer 2: Agent Service (Python Backend)
**Repository:** [officelabs-agent](https://github.com/Pher217/officelabs-agent) this is the master agent and all sub agent for example macro agent

- **Writer Agent** - Content generation, editing, structuring
- **Calc Agent** - Formula generation, data modeling, charts
- **Impress Agent** - Presentation creation, slide generation
- **Macro Agent** - Python automation and scripting
- **LLM Router** - Multi-provider support (Claude, OpenAI, Ollama)
- **Patch System** - Generate and apply structured changes

## 🚀 Quick Start

### Two Deployment Options

**Option 1: Use Pre-built Binaries** (Coming Soon)
```bash
# Download from releases
wget https://github.com/Pher217/officelabs/releases/latest/OfficeLabs-x86_64.AppImage
chmod +x OfficeLabs-x86_64.AppImage
./OfficeLabs-x86_64.AppImage
```

**Option 2: Build from Source** (For Developers)

### Prerequisites

**System:**
- Python 3.9+
- C++ compiler (GCC 11+ / Clang 14+ / MSVC 2022)
- Git
- 16GB+ RAM (for building)
- 50GB+ disk space

**API Keys:**
- Claude API key (Anthropic) OR
- OpenAI API key OR
- Local LLM (Ollama, LM Studio)

### Installation (Source Build)

**1. Clone and set up the fork:**
```bash
# Clone OfficeLabs core (LibreOffice fork)
git clone https://github.com/Pher217/officelabs-core.git
cd officelabs-core

# Add upstream LibreOffice
git remote add upstream https://github.com/LibreOffice/core.git
```

**2. Build OfficeLabs:**
```bash
# Configure build
./autogen.sh \
  --with-vendor="OfficeLabs" \
  --with-theme="officelabs" \
  --enable-python=system \
  --with-officelabs \
  --enable-ai-features

# Build (takes 1-3 hours first time)
make build-nocheck -j8
```

**3. Set up Agent Service:**
```bash
# Clone agent backend
cd ..
git clone https://github.com/Pher217/officelabs-agent.git
cd officelabs-agent

# Install Python dependencies
python -m venv venv
source venv/bin/activate  # Windows: venv\Scripts\activate
pip install -r requirements.txt

# Configure API keys
cp config/config.example.yaml config/config.yaml
nano config/config.yaml  # Add your API key

# Start agent service
python -m src.main
```

**4. Launch OfficeLabs:**
```bash
# In another terminal
cd ../officelabs-core
./instdir/program/soffice --writer
```

The **Kiro Assistant** chat panel is now embedded natively in the UI!

### First Use

1. Press `Ctrl+K` to open the AI command palette
2. Or click the 🤖 icon to open the chat panel
3. Try: "What's in this document?"
4. Or: "Make @Para#1 more concise"

## 📖 Documentation

### Core Documentation
- **[Fork Architecture](docs/FORK_ARCHITECTURE.md)** - Complete architectural overview of the fork
- **[Fork Guide](docs/FORK_GUIDE.md)** - How to build OfficeLabs from LibreOffice
- **[User Guide](docs/USER_GUIDE.md)** - Using OfficeLabs features
- **[Agent Architecture](docs/AGENT_ARCHITECTURE.md)** - Modular agent system design
- **[UI Integration](docs/UI_INTEGRATION.md)** - Native chat panel implementation

### Reference
- [API Reference](docs/API.md) - Agent service API documentation
- [Development Guide](docs/DEVELOPMENT.md) - Contributing guidelines
- [Original Architecture](docs/ARCHITECTURE.md) - Extension-based approach (deprecated)

## ✨ Features in Detail

### 🤖 Native AI Chat

Unlike extensions, the AI chat is **built into the core UI**:

```
Press Ctrl+K anywhere:
┌────────────────────────────────────┐
│ 🤖 AI Command Palette              │
├────────────────────────────────────┤
│ > rewrite this paragraph_          │
├────────────────────────────────────┤
│ 📝 Rewrite selection              │
│ ✨ Generate content               │
│ 🔍 Summarize document             │
│ 📊 Create table                   │
└────────────────────────────────────┘
```

**Or** use the embedded chat panel for multi-turn conversations.

### 👻 Inline Ghost Text (Cursor-Style)

AI suggestions appear as ghost text directly in your document:

```
Type: "Write an intro about AI"
See:  Write an intro about AI and its impact on productivity...
Press Tab to accept
```

### 📍 @-Addressable Objects

Reference any element naturally in chat:

| App | Syntax | Example |
|-----|--------|---------|
| **Writer** | `@Heading("Title")` | `@Heading("Introduction")` |
|  | `@Para#N` | `@Para#12` |
|  | `@Table("Name")` | `@Table("Budget2024")` |
| **Calc** | `@Sheet("Name")` | `@Sheet("Sales")` |
|  | `@Cell(Ref)` | `@Cell(B66)` |
|  | `@Range(Name)` | `@Range(A1:D20)` |
| **Impress** | `@Slide#N` | `@Slide#5` |
|  | `@Slide("Title")` | `@Slide("Overview")` |

**Example Prompts:**
```
"Make @Para#3 more concise"
"Calculate the average of @Range(Sales)"
"Change @Slide#2 title to 'Q4 Results'"
```

### 🎨 Structured Patches

Every AI change is previewed as a **git-style diff** before applying:

```diff
@Para#3 (Line 45-47)
- The implementation was done in a manner that was quite complex
- and involved multiple interdependent components working together
- to achieve the desired functionality.
+ The implementation was complex and involved multiple 
+ interdependent components.

[✓ Accept]  [✗ Reject]  [↻ Regenerate]
```

### 📚 Git-Based Versioning

Every document is a Git repository:

- **Auto-commit** on saves and AI changes
- **Full history** with `git log`
- **Diff any versions** with `git diff`
- **Flat ODF format** (`.fodt`/`.fods`/`.fodp`) for readable diffs
- **Branch and merge** documents like code

```bash
cd my_document.fodt.git
git log --oneline
# a3b2c1d AI: rewrite @Para#3
# d4e5f6a Manual edit: added table
# g7h8i9j AI: generated introduction
```

### 🐍 Python Macro Agent

Generate and execute Python macros from natural language:

```
You: "Create a macro to auto-format all tables in this document"
Agent: [Shows generated Python code]
```python
def format_all_tables(doc):
    for table in doc.getTextTables():
        table.setPropertyValue("BackTransparent", False)
        table.setPropertyValue("BackColor", 0xF0F0F0)
        # ... more formatting
```
```
[✓ Run]  [✗ Cancel]  [📝 Edit]
```

## 🛠️ Building from Source

**Detailed guide:** [FORK_GUIDE.md](docs/FORK_GUIDE.md)

### Quick Build

```bash
# Clone OfficeLabs fork
git clone https://github.com/Pher217/officelabs-core.git
cd officelabs-core

# Configure with AI features
./autogen.sh \
  --with-vendor="OfficeLabs" \
  --with-officelabs \
  --enable-ai-features

# Build (1-3 hours first time, then fast with ccache)
make build-nocheck -j8

# Launch
./instdir/program/soffice --writer
```

### Syncing with LibreOffice Upstream

```bash
# Update from upstream (quarterly)
git checkout libreoffice-sync
git fetch upstream
git merge upstream/master

# Cherry-pick fixes to main
git checkout officelabs-main
git cherry-pick <security-fix-hash>
```

## 🧪 Testing

```bash
# Run unit tests
python -m pytest tests/

# Run integration tests
python -m pytest tests/integration/

# Test specific document type
python -m pytest tests/integration/test_writer.py
```

## 📝 Configuration

Configuration file: `~/.config/officelabs/config.yaml`

```yaml
llm:
  provider: claude  # or openai
  api_key: sk-...
  model: claude-3-5-sonnet-20241022

agent:
  host: localhost
  port: 8765
  timeout: 30

features:
  auto_commit: false
  preview_required: true
  auto_index: true

git:
  default_format: flat_odf  # or binary
  auto_commit_on_save: false
  export_readable_diffs: true
```

## 🤝 Contributing

We welcome contributions! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

### Development Workflow

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/amazing-feature`
3. Make your changes
4. Run tests: `python -m pytest`
5. Commit: `git commit -m 'Add amazing feature'`
6. Push: `git push origin feature/amazing-feature`
7. Open a Pull Request

## 📜 License

OfficeLabs is licensed under the Mozilla Public License 2.0 (MPL-2.0), the same license as LibreOffice.

See [LICENSE](LICENSE) for details.

## 🙏 Acknowledgments

- **LibreOffice** - The foundation of OfficeLabs
- **Cursor** - Inspiration for the AI-assisted editing model
- **Anthropic & OpenAI** - LLM providers

## 🔗 Links

### Repositories
- **[OfficeLabs Core](https://github.com/Pher217/officelabs-core)** - LibreOffice fork with AI integration
- **[OfficeLabs Agent](https://github.com/Pher217/officelabs-agent)** - Python backend service
- **[LibreOffice Upstream](https://github.com/LibreOffice/core)** - Original LibreOffice

### Resources
- [Documentation Hub](docs/)
- [Fork Architecture](docs/FORK_ARCHITECTURE.md)
- [Issue Tracker](https://github.com/Pher217/officelabs-core/issues)
- [Discussions](https://github.com/Pher217/officelabs-core/discussions)

### Inspiration
- **[Cursor](https://cursor.sh)** - The AI-first code editor that inspired OfficeLabs
- **[LibreOffice](https://www.libreoffice.org/)** - The foundation we build upon

---

## 🎯 Project Status

**Current Phase:** Architecture & Initial Fork (Q1 2026)

**Roadmap:**
- ✅ Architecture design complete
- ✅ Fork strategy defined
- 🔄 LibreOffice fork in progress
- ⏳ Native AI panel (C++ widget)
- ⏳ Writer Agent (MVP)
- ⏳ Agent service backend

**Target:** MVP release Q2 2026

---

## 💡 Why OfficeLabs?

**Problem:** Current office suites treat AI as an afterthought—extensions, plugins, or cloud-only features.

**Solution:** OfficeLabs makes AI **native**. Just like Cursor transformed VS Code, we're transforming LibreOffice.

**Philosophy:**
- 🌍 **Open Source** - No vendor lock-in, full transparency
- 🔒 **Privacy First** - Local LLMs supported, your data stays yours
- 🎨 **Design Matters** - Beautiful, minimal, fast
- 🧩 **Modular** - Each agent is specialized and excellent
- 🚀 **Developer Friendly** - Python macros, API access, extensible

---

Built with ❤️ by the OfficeLabs community

**"Cursor for Office" - AI-native productivity for everyone**

