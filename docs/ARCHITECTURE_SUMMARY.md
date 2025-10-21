# OfficeLabs Architecture Summary

**AI-Native Office Suite — Executive Overview for Development Team**

---

## 🎯 What Is OfficeLabs?

OfficeLabs is a **next-generation AI office suite** created by forking LibreOffice and integrating modular AI agents directly into the core, inspired by how **Cursor** transformed VS Code into an AI-first IDE.

Unlike Microsoft 365 Copilot or Google Workspace AI (cloud-only, subscription-based), OfficeLabs is:
- ✅ **Open Source** — No vendor lock-in, full transparency
- ✅ **Privacy-First** — Works offline with local LLMs
- ✅ **Natively Integrated** — AI is built into the core, not bolted on
- ✅ **Modular & Specialized** — Each agent excels at its domain

**Tagline:** *"Cursor for Office — AI-native productivity for everyone"*

---

## 🏗️ High-Level Architecture

```
┌──────────────────────────────────────────────────────────┐
│               OfficeLabs Core (C++ Fork)                  │
│                                                           │
│   Writer    Calc     Impress    All Apps                 │
│     │        │          │           │                     │
│     └────────┴──────────┴───────────┘                     │
│                    │                                      │
│         ┌──────────▼──────────┐                          │
│         │  AI Chat Panel (VCL) │  ← Native widget        │
│         │  Command Palette     │     embedded in         │
│         │  Ghost Text          │     LibreOffice UI      │
│         └──────────┬───────────┘                          │
│                    │                                      │
│         ┌──────────▼──────────┐                          │
│         │  Agent Layer (C++)   │  ← Integration          │
│         │  Context Engine      │     layer               │
│         │  IPC Bridge          │                          │
│         └──────────┬───────────┘                          │
└────────────────────┼──────────────────────────────────────┘
                     │ JSON-RPC (Unix socket / Named pipe)
┌────────────────────▼──────────────────────────────────────┐
│           OfficeLabs Agent (Python Backend)               │
│                                                           │
│  ┌─────────────────────────────────────────────────┐    │
│  │  Agent Router                                    │    │
│  │    ├─ Writer Agent    (content generation)      │    │
│  │    ├─ Calc Agent      (formulas & data)         │    │
│  │    ├─ Impress Agent   (presentations)           │    │
│  │    └─ Macro Agent     (Python automation)       │    │
│  └─────────────────────────────────────────────────┘    │
│                                                           │
│  ┌─────────────────────────────────────────────────┐    │
│  │  LLM Router                                      │    │
│  │    ├─ Claude (Anthropic)                        │    │
│  │    ├─ GPT-4 (OpenAI)                            │    │
│  │    └─ Local LLM (Ollama)                        │    │
│  └─────────────────────────────────────────────────┘    │
│                                                           │
│  ┌─────────────────────────────────────────────────┐    │
│  │  Patch System                                    │    │
│  │    ├─ Generator (create patches from LLM)       │    │
│  │    ├─ Validator (check safety)                  │    │
│  │    └─ Applier (apply via PyUNO)                 │    │
│  └─────────────────────────────────────────────────┘    │
└───────────────────────────────────────────────────────────┘
```

---

## 🧩 Two-Layer System

### Layer 1: OfficeLabs Core (C++ Fork)
**What:** LibreOffice fork with native AI integration  
**Language:** C++ (VCL, UNO)  
**Repository:** `officelabs-core` (to be created)

**Components:**
1. **Native AI Chat Panel** - VCL widget embedded in UI
2. **Command Palette** - Ctrl+K quick actions
3. **Ghost Text** - Inline AI suggestions (Cursor-style)
4. **Agent Integration Layer** - C++ modules for each agent
5. **Context Engine** - Real-time document indexing
6. **IPC Bridge** - Communication with Python backend

### Layer 2: Agent Service (Python Backend)
**What:** FastAPI service providing AI intelligence  
**Language:** Python  
**Repository:** `officelabs-agent` (existing)

**Components:**
1. **Agent Router** - Route to correct agent (Writer/Calc/Impress/Macro)
2. **LLM Router** - Multi-provider support (Claude/GPT/Local)
3. **Patch System** - Generate and apply document changes
4. **Document Operations** - PyUNO interface for document manipulation

---

## 🤖 Agent System

### Four Specialized Agents

**1. Writer Agent**
- Content generation (paragraphs, sections, outlines)
- Content editing (rewrite, tone, length)
- Document structuring (headings, TOC)
- Table/list creation

**2. Calc Agent**
- Formula generation from natural language
- Data operations (sort, filter, clean)
- Chart and pivot table creation
- Statistical analysis

**3. Impress Agent**
- Slide generation (title, content, closing)
- Layout management
- Speaker notes generation
- Visual consistency

**4. Macro Agent**
- Python code generation from natural language
- Document automation
- Batch operations
- External integrations

---

## 📍 @-Addressing System

**Core Innovation:** Reference any document object naturally

### Syntax Examples

**Writer:**
```
@Heading("Introduction")  → Reference a heading
@Para#12                  → Reference paragraph 12
@Table("Budget2024")      → Reference a table
```

**Calc:**
```
@Sheet("Sales")          → Reference a sheet
@Cell(A1)                → Reference a cell
@Range(A1:D20)           → Reference a range
@Range(TotalSales)       → Reference named range
```

**Impress:**
```
@Slide#5                 → Reference slide 5
@Slide("Overview")       → Reference slide by title
```

### Usage in Chat

```
User: "Make @Para#3 more concise"
AI:   Shows preview of shortened paragraph
User: Accepts or rejects
```

---

## 🎨 UI Features

### 1. Native AI Chat Panel

**Not a sidebar—a native VCL widget.**

```
┌─────────────────────────────────────┐
│ 🤖 Kiro Assistant        [_][□][×] │
├─────────────────────────────────────┤
│ 🟢 Connected to Claude Sonnet 4    │
│ 📄 Context: @Para#3 selected       │
├─────────────────────────────────────┤
│                                     │
│  You: Make this more concise       │
│                                     │
│  Kiro: Here's a shorter version:   │
│  [Diff preview]                    │
│  - Old: The implementation was...  │
│  + New: The implementation was...  │
│                                     │
│  [✓ Accept]  [✗ Reject]           │
│                                     │
├─────────────────────────────────────┤
│  Type a message...      [Send]     │
└─────────────────────────────────────┘
```

### 2. Command Palette (Ctrl+K)

**Instant AI actions without typing full prompts.**

```
┌────────────────────────────────────┐
│ 🤖 AI Command Palette              │
├────────────────────────────────────┤
│ > rewrite_                         │
├────────────────────────────────────┤
│ 📝 Rewrite selection              │
│ ✨ Generate content               │
│ 🔍 Summarize document             │
│ 📊 Create table                   │
│ 📈 Generate chart                 │
└────────────────────────────────────┘
```

### 3. Inline Ghost Text

**Cursor-style AI suggestions.**

```
User types: "Write an intro about AI"
Ghost text appears: ...and its impact on productivity
User presses Tab → accepts suggestion
User presses Esc → rejects
```

---

## 🔄 How It Works (Example Flow)

### Editing a Document

**1. User Action:**
```
User: "Make @Para#3 more formal"
```

**2. C++ Layer:**
- Chat panel captures input
- Context engine identifies @Para#3
- Sends request via IPC to Python backend

**3. Python Backend:**
- Agent router selects WriterAgent
- WriterAgent builds system prompt with context
- Calls LLM (Claude/GPT)
- Parses response, extracts patch
- Returns structured response

**4. Back to C++ Layer:**
- Receives response with patch
- Displays diff preview in chat panel
- User clicks "Accept"

**5. Apply Change:**
- C++ layer applies patch to document
- Git commits change (if enabled)
- Updates context engine

---

## 🔌 Communication Protocol

### IPC (Inter-Process Communication)

**Transport:**
- Unix socket (Linux/Mac): `/tmp/officelabs-agent.sock`
- Named pipe (Windows): `\\.\pipe\officelabs-agent`

**Protocol:** JSON-RPC 2.0

**Request Example:**
```json
{
  "jsonrpc": "2.0",
  "id": "uuid-1234",
  "method": "chat",
  "params": {
    "message": "Make @Para#3 more concise",
    "context": {
      "app": "writer",
      "document_title": "My Document.odt",
      "selection": {
        "type": "paragraph",
        "address": "@Para#3",
        "content": "The implementation was done..."
      }
    }
  }
}
```

**Response Example:**
```json
{
  "jsonrpc": "2.0",
  "id": "uuid-1234",
  "result": {
    "message": "Here's a more concise version:",
    "patch": {
      "type": "replace",
      "target": "@Para#3",
      "old_value": "The implementation was done...",
      "new_value": "The implementation was complex.",
      "diff": "- Old\n+ New"
    }
  }
}
```

---

## 🛠️ Technology Stack

### OfficeLabs Core (C++)
- **UI Framework:** VCL (LibreOffice's native toolkit)
- **Language:** C++17
- **Build System:** GNU Make + Autotools
- **IPC:** Unix sockets / Named pipes

### Agent Service (Python)
- **Web Framework:** FastAPI
- **LLM Clients:** Anthropic SDK, OpenAI SDK
- **Document Operations:** PyUNO
- **Git Integration:** GitPython
- **Testing:** pytest

---

## 📦 Build Process

### Building OfficeLabs Core

```bash
# Clone fork
git clone https://github.com/Pher217/officelabs-core.git
cd officelabs-core

# Configure with AI features
./autogen.sh \
  --with-vendor="OfficeLabs" \
  --with-officelabs \
  --enable-ai-features

# Build (1-3 hours first time)
make build-nocheck -j8

# Launch
./instdir/program/soffice --writer
```

### Setting Up Agent Service

```bash
# Clone agent repo
git clone https://github.com/Pher217/officelabs-agent.git
cd officelabs-agent

# Install dependencies
pip install -r requirements.txt

# Configure
cp config/config.example.yaml config/config.yaml
nano config/config.yaml  # Add API key

# Start service
python -m src.main
```

---

## 📚 Key Documentation

### Architecture & Design
1. **[FORK_ARCHITECTURE.md](docs/FORK_ARCHITECTURE.md)** - Complete technical architecture
2. **[AGENT_ARCHITECTURE.md](docs/AGENT_ARCHITECTURE.md)** - Agent system design
3. **[UI_INTEGRATION.md](docs/UI_INTEGRATION.md)** - Native UI implementation
4. **[PROJECT_STRUCTURE.md](docs/PROJECT_STRUCTURE.md)** - Repository layout

### Implementation & Development
5. **[FORK_GUIDE.md](docs/FORK_GUIDE.md)** - How to build the fork
6. **[DEVELOPMENT.md](docs/DEVELOPMENT.md)** - Contributing guidelines
7. **[API.md](docs/API.md)** - Agent service API reference

### Planning & Roadmap
8. **[ROADMAP.md](ROADMAP.md)** - Development timeline
9. **[MVP_STATUS.md](MVP_STATUS.md)** - Current status

---

## 🎯 Development Phases

| Phase | Timeline | Deliverable |
|-------|----------|-------------|
| **Phase 0** | Oct 2025 | ✅ Architecture & Design |
| **Phase 1** | Q1 2026 | Fork & IPC & Basic UI |
| **Phase 2** | Q2 2026 | **Writer Agent MVP** |
| **Phase 3** | Q3 2026 | Calc Agent |
| **Phase 4** | Q4 2026 | Impress & Macro Agents |
| **Phase 5** | 2027 | Advanced Features |

**MVP Target:** Writer Agent by **June 2026**  
**1.0 Target:** Full suite by **December 2026**

---

## 🚀 Next Steps

### Immediate (November 2025)
1. Create `officelabs-core` repository (fork LibreOffice)
2. Set up build system
3. Begin IPC implementation

### Q1 2026
1. Complete IPC layer (C++ ↔ Python)
2. Implement basic chat panel (VCL widget)
3. Create agent service backend
4. First working prototype

### Q2 2026
1. Complete Writer Agent
2. Full @-addressing support
3. Patch system working
4. **MVP Release**

---

## 💡 Why This Matters

### The Problem
Current office suites treat AI as an afterthought:
- Microsoft 365 Copilot: Cloud-only, subscription, privacy concerns
- Google Workspace: Same issues
- LibreOffice: No AI at all

### The OfficeLabs Solution
- ✅ **Open Source** - No vendor lock-in
- ✅ **Privacy First** - Local LLM support
- ✅ **Native AI** - Deeply integrated, not bolted on
- ✅ **Modular** - Specialized agents
- ✅ **Beautiful** - Modern UI like Cursor

**Just like Cursor transformed VS Code, OfficeLabs transforms LibreOffice.**

---

## 🤝 How to Get Involved

**Needed Skills:**
- C++ (LibreOffice/VCL development)
- Python (FastAPI, LLM integration)
- UI/UX Design
- Technical Writing
- DevOps (CI/CD)

**Get Started:**
1. Read [FORK_GUIDE.md](docs/FORK_GUIDE.md)
2. Review [FORK_ARCHITECTURE.md](docs/FORK_ARCHITECTURE.md)
3. Join GitHub Discussions (coming soon)
4. Pick an issue to work on

---

## 📊 Success Metrics

### MVP (June 2026)
- 100+ weekly active users
- Response latency < 2s
- 80%+ patch success rate

### 1.0 (December 2026)
- 10,000+ weekly active users
- All agents functional
- 95%+ patch success rate
- 4.5/5 user satisfaction

---

## 🌟 The Vision

**OfficeLabs will be the first truly AI-native office suite.**

- Documents that write themselves
- Spreadsheets that analyze themselves
- Presentations that design themselves
- All powered by AI, all open source, all privacy-respecting

**"Cursor for Office" - AI-native productivity for everyone.**

---

*Last Updated: October 21, 2025*  
*Next Update: Fork creation (November 2025)*

