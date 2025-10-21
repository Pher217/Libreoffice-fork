# OfficeLabs Fork Architecture

**Version:** 2.0 (Fork-based Approach)  
**Status:** Design Phase  
**Last Updated:** October 2025

---

## 🎯 Vision

OfficeLabs transforms LibreOffice into an AI-first office suite, inspired by how **Cursor** revolutionized VS Code. Instead of bolting AI onto existing tools, we're **deeply integrating AI agents** into the core experience.

**Key Principle:** AI is not a feature—it's the foundation.

---

## 🏗️ Fork vs. Extension Approach

### Why Fork LibreOffice?

| Aspect | Extension Approach ❌ | Fork Approach ✅ |
|--------|---------------------|-----------------|
| **UI Integration** | Sidebar panel only | Native embedded chat everywhere |
| **Performance** | PyUNO overhead | Direct C++ integration |
| **UX Consistency** | Feels "bolted on" | Seamless, native experience |
| **Control** | Limited by LibreOffice API | Full control over UI/UX |
| **Branding** | Still "LibreOffice" | **OfficeLabs** identity |
| **Upstream Sync** | Must track API changes | Controlled merge from upstream |

### Forking Strategy

```
LibreOffice/core (upstream)
    ↓
    → officelabs/core (our fork)
         ↓
         ├── officelabs-main (branch: our integrated features)
         └── libreoffice-sync (branch: tracks upstream)
```

**Sync Process:**
1. Pull upstream changes into `libreoffice-sync`
2. Cherry-pick security/bug fixes into `officelabs-main`
3. Maintain our AI features separate from upstream
4. Quarterly upstream merges (controlled)

---

## 🧩 Architecture Overview

```
┌────────────────────────────────────────────────────────────────┐
│                     OfficeLabs Suite                            │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐         │
│  │    Writer    │  │     Calc     │  │   Impress    │         │
│  │   (ODT/DOC)  │  │   (ODS/XLS)  │  │   (ODP/PPT)  │         │
│  └──────┬───────┘  └──────┬───────┘  └──────┬───────┘         │
│         │                  │                  │                  │
│  ┌──────▼──────────────────▼──────────────────▼───────┐        │
│  │         Native AI Chat Panel (C++ Widget)           │        │
│  │  ┌────────────────────────────────────────────┐    │        │
│  │  │  • Command Palette (Ctrl+K)                │    │        │
│  │  │  • Inline Ghost Text                       │    │        │
│  │  │  • Context-aware suggestions                │    │        │
│  │  │  • Multi-turn conversation                 │    │        │
│  │  └────────────────────────────────────────────┘    │        │
│  └──────────────────────┬──────────────────────────────┘        │
│                         │                                        │
│  ┌──────────────────────▼──────────────────────────────┐        │
│  │         OfficeLabs Core Integration Layer            │        │
│  │  ┌────────────┐  ┌────────────┐  ┌────────────┐   │        │
│  │  │  Writer    │  │   Calc     │  │  Impress   │   │        │
│  │  │  Agent     │  │   Agent    │  │  Agent     │   │        │
│  │  │  Module    │  │   Module   │  │  Module    │   │        │
│  │  └────────────┘  └────────────┘  └────────────┘   │        │
│  │  ┌───────────────────────────────────────────────┐ │        │
│  │  │         Macro Agent (Python Bridge)           │ │        │
│  │  └───────────────────────────────────────────────┘ │        │
│  │  ┌───────────────────────────────────────────────┐ │        │
│  │  │         Document Context Engine               │ │        │
│  │  │  • Real-time indexing                         │ │        │
│  │  │  • @-addressing                               │ │        │
│  │  │  • Selection tracking                         │ │        │
│  │  └───────────────────────────────────────────────┘ │        │
│  └──────────────────────┬──────────────────────────────┘        │
└─────────────────────────┼───────────────────────────────────────┘
                          │ IPC (Unix Socket / Named Pipe)
┌─────────────────────────▼───────────────────────────────────────┐
│              OfficeLabs Agent Service                            │
│              (Python/FastAPI Backend)                            │
│  ┌──────────────────────────────────────────────────────────┐  │
│  │                    Agent Router                           │  │
│  │  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐  │  │
│  │  │   Writer     │  │    Calc      │  │   Impress    │  │  │
│  │  │   Agent      │  │    Agent     │  │   Agent      │  │  │
│  │  └──────────────┘  └──────────────┘  └──────────────┘  │  │
│  │  ┌───────────────────────────────────────────────────┐  │  │
│  │  │            Macro Agent (Python Execution)         │  │  │
│  │  └───────────────────────────────────────────────────┘  │  │
│  └──────────────────────────────────────────────────────────┘  │
│  ┌──────────────────────────────────────────────────────────┐  │
│  │                  LLM Router                              │  │
│  │  ┌────────────┐  ┌────────────┐  ┌────────────┐       │  │
│  │  │   Claude   │  │   OpenAI   │  │   Local    │       │  │
│  │  │   Client   │  │   Client   │  │   (Ollama) │       │  │
│  │  └────────────┘  └────────────┘  └────────────┘       │  │
│  └──────────────────────────────────────────────────────────┘  │
│  ┌──────────────────────────────────────────────────────────┐  │
│  │              Document Operations Engine                  │  │
│  │  • UNO Bridge (PyUNO)                                   │  │
│  │  • Patch Generator & Applier                            │  │
│  │  • Git Integration (versioning)                         │  │
│  │  • MCP Server (future)                                  │  │
│  └──────────────────────────────────────────────────────────┘  │
└──────────────────────────────────────────────────────────────────┘
```

---

## 🔌 Core Components

### 1. Native AI Chat Panel (C++ Widget)

**Location:** `officelabs/core/vcl/source/ai/chat_panel.cxx`

**Features:**
- **Embedded UI:** Native VCL widget, not a sidebar extension
- **Command Palette:** `Ctrl+K` / `⌘+K` for quick AI actions
- **Inline Ghost Text:** Cursor-style suggestions (Tab to accept)
- **Context Display:** Shows current selection, @-addresses
- **Multi-turn Chat:** Full conversation history
- **Patch Preview:** Built-in diff viewer for changes

**Technology Stack:**
- **UI Framework:** VCL (LibreOffice's native toolkit)
- **Rendering:** Cairo for custom chat bubbles
- **Styling:** CSS-like theme system (OfficeLabs blue/white)
- **IPC:** Unix sockets (Linux/Mac), Named pipes (Windows)

### 2. Agent Integration Layer (C++)

**Location:** `officelabs/core/officelabs/agent/`

**Modules:**
```cpp
// Core agent interface
officelabs/agent/
├── IAgentModule.hxx          // Base interface for all agents
├── WriterAgent.cxx           // Writer-specific logic
├── CalcAgent.cxx             // Calc-specific logic
├── ImpressAgent.cxx          // Impress-specific logic
├── MacroAgent.cxx            // Python macro bridge
├── AgentConnection.cxx       // IPC to backend service
└── ContextEngine.cxx         // Document context tracking
```

**Responsibilities:**
- Track document state (selections, edits, cursor position)
- Build context for AI (current paragraph, table, cell, slide)
- Send requests to backend agent service
- Apply patches returned by agents
- Handle real-time indexing

### 3. Document Context Engine (C++)

**Location:** `officelabs/core/officelabs/context/`

**Features:**
- **Real-time Indexing:** Track all document objects as user types
- **@-Address Resolution:** Map `@Para#3` → actual paragraph object
- **Selection Tracking:** Know what user has selected
- **Change Detection:** Incremental updates on edits
- **Context Serialization:** Send relevant context to AI

**Data Structure:**
```cpp
struct DocumentObject {
    ObjectType type;           // HEADING, PARAGRAPH, TABLE, CELL, etc.
    std::string address;       // @Heading("Introduction")
    std::string displayName;   // "Heading: Introduction"
    std::string preview;       // First 100 chars
    std::map<std::string, std::string> metadata;
};

class ContextEngine {
    std::vector<DocumentObject> objects;
    DocumentObject* currentSelection;
    
    void indexDocument();
    void updateObject(const std::string& address);
    DocumentObject* resolve(const std::string& address);
    std::string getContextForAI();
};
```

### 4. OfficeLabs Agent Service (Python)

**Repository:** `https://github.com/Pher217/officelabs-agent.git`

**Structure:**
```
officelabs-agent/
├── src/
│   ├── main.py                    # FastAPI app
│   ├── router.py                  # Route to correct agent
│   ├── agents/
│   │   ├── base_agent.py          # Abstract agent class
│   │   ├── writer_agent.py        # Writer logic
│   │   ├── calc_agent.py          # Calc logic
│   │   ├── impress_agent.py       # Impress logic
│   │   └── macro_agent.py         # Python macro generation
│   ├── llm/
│   │   ├── router.py              # LLM provider abstraction
│   │   ├── claude_client.py
│   │   ├── openai_client.py
│   │   └── local_client.py        # Ollama/LM Studio
│   ├── document/
│   │   ├── context_parser.py      # Parse context from C++ layer
│   │   ├── patch_generator.py     # Generate structured patches
│   │   └── patch_applier.py       # Apply via PyUNO
│   ├── git/
│   │   ├── versioning.py          # Git integration
│   │   └── commit_manager.py      # Auto-commit logic
│   └── mcp/
│       └── server.py              # MCP server (future)
├── config/
│   └── config.yaml
└── requirements.txt
```

---

## 🤖 Agent Modules

### Writer Agent

**Purpose:** Content generation, editing, structuring

**Capabilities:**
- Generate paragraphs, sections, outlines
- Rewrite for tone, clarity, length
- Grammar and style checking
- Format tables, lists, headings
- Insert citations, references

**Example Prompts:**
```
"Write an introduction about AI in healthcare"
"Make @Para#3 more formal"
"Create a table comparing @Topic1 and @Topic2"
"Add 3 bullet points after @Heading('Benefits')"
```

**System Prompt:**
```
You are a Writer Agent for OfficeLabs. You help users create and edit documents.
Current document: {{title}}
Structure: {{headings}}
Selected: {{selection}}
Task: {{user_message}}
```

### Calc Agent

**Purpose:** Data modeling, calculations, business intelligence

**Capabilities:**
- Generate formulas from natural language
- Create charts and pivot tables
- Data cleaning and transformation
- Financial modeling
- Statistical analysis

**Example Prompts:**
```
"Calculate the average of column A"
"Create a bar chart from @Range(Sales)"
"Fill @Range(Forecast) with a 5% growth projection"
"Find duplicate entries in @Sheet(Customers)"
```

**System Prompt:**
```
You are a Calc Agent for OfficeLabs. You help with spreadsheets and data.
Current sheet: {{sheet_name}}
Selected range: {{selection}}
Available ranges: {{named_ranges}}
Task: {{user_message}}
```

### Impress Agent

**Purpose:** Presentation design, slide creation

**Capabilities:**
- Generate slide layouts
- Create visually consistent decks
- Suggest content for slides
- Auto-format for brand consistency
- Generate speaker notes

**Example Prompts:**
```
"Create a title slide for 'Q4 Results'"
"Add 3 slides about our product features"
"Make @Slide#2 match the brand theme"
"Generate speaker notes for @Slide('Overview')"
```

**System Prompt:**
```
You are an Impress Agent for OfficeLabs. You help create presentations.
Current presentation: {{title}}
Slide count: {{slide_count}}
Current slide: {{current_slide}}
Task: {{user_message}}
```

### Macro Agent

**Purpose:** Python automation for the entire suite

**Capabilities:**
- Generate Python macros from natural language
- Execute document automation
- Batch operations across documents
- Custom extensions and plugins
- Integration with external APIs

**Example Prompts:**
```
"Create a macro to auto-format all tables"
"Write Python to export all sheets as CSV"
"Generate a script to merge data from multiple documents"
"Create a macro to update all charts with latest data"
```

**Execution Model:**
```python
# Sandboxed Python execution
class MacroAgent:
    def generate_macro(self, task: str) -> str:
        """Generate Python macro from natural language"""
        prompt = f"Write a Python macro using PyUNO to: {task}"
        code = self.llm.generate(prompt)
        return code
    
    def execute_macro(self, code: str, context: dict):
        """Execute in sandboxed environment"""
        sandbox = MacroSandbox(context)
        return sandbox.run(code)
```

---

## 🎨 UI/UX Design

### Native Chat Panel Integration

**Design Principles:**
1. **Minimal & Clean:** Swiss design aesthetic
2. **Context-aware:** Always show what AI "sees"
3. **Non-intrusive:** Collapsible, keyboard-driven
4. **Fast:** Instant response, streaming output
5. **Predictable:** Clear patch previews before applying

**Layout:**

```
┌─────────────────────────────────────────────────────────┐
│  File  Edit  View  Insert  Format  Tools  Window  Help │
├─────────────────────────────────────────────────────────┤
│  [B] [I] [U]  Font ▼  Size ▼   🤖 AI Chat (Ctrl+K)    │
├───────────────┬─────────────────────────────────────────┤
│               │  Document Content Area                  │
│  Sidebar      │                                         │
│  (Optional)   │  Lorem ipsum dolor sit amet...          │
│               │                                         │
│               │  [Inline ghost text appears here...]    │
│               │                                         │
├───────────────┴─────────────────────────────────────────┤
│  🤖 Kiro Assistant                              [_][□][×]│
│  ┌───────────────────────────────────────────────────┐ │
│  │ 🟢 Connected to Claude Sonnet 4                  │ │
│  │ 📄 Context: @Para#3 selected                     │ │
│  ├───────────────────────────────────────────────────┤ │
│  │                                                   │ │
│  │  You: Make this more concise                     │ │
│  │                                                   │ │
│  │  Kiro: Here's a shorter version:                 │ │
│  │  [Shows diff preview]                            │ │
│  │  - Old: The implementation was done in a...      │ │
│  │  + New: The implementation was complex.          │ │
│  │                                                   │ │
│  │  [✓ Accept]  [✗ Reject]  [↻ Regenerate]         │ │
│  │                                                   │ │
│  ├───────────────────────────────────────────────────┤ │
│  │  Type a message... (Ctrl+Enter to send)          │ │
│  │  [Send]                                          │ │
│  └───────────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────────────┘
```

### Command Palette (Ctrl+K)

```
┌─────────────────────────────────────────┐
│  🤖 AI Command Palette                  │
├─────────────────────────────────────────┤
│  > make more concise_                   │
├─────────────────────────────────────────┤
│  📝 Rewrite selection                   │
│  ✨ Generate content                    │
│  🔍 Summarize document                  │
│  📊 Create table                        │
│  📈 Generate chart                      │
│  🔧 Fix grammar                         │
│  🎨 Format selection                    │
│  🐍 Generate macro                      │
└─────────────────────────────────────────┘
```

### Theme & Branding

**Colors:**
- **Primary:** Glacier Blue (#4A90E2)
- **Secondary:** Deep Navy (#2C3E50)
- **Accent:** Electric Teal (#00D9FF)
- **Background:** Pure White (#FFFFFF)
- **Text:** Charcoal (#333333)
- **Borders:** Light Gray (#E0E0E0)

**Typography:**
- **UI:** Inter (system font fallback)
- **Code/Monospace:** JetBrains Mono
- **Chat:** System default (Segoe UI / SF Pro / Ubuntu)

**Logo:**
```
 ____  __  __ _          _       _         
/ __ \/ _|/ _(_)        | |     | |        
| |  | | |_| |_ ___ ___| | __ _| |__  ___ 
| |  | |  _|  _|/ __/ _ \ |/ _` | '_ \/ __|
| |__| | | | | | (_|  __/ | (_| | |_) \__ \
\____/|_| |_|  \___\___|_|\__,_|_.__/|___/
```

---

## 🔌 Communication Protocol

### IPC Between C++ and Python

**Unix Socket (Linux/Mac):**
```
/tmp/officelabs-agent.sock
```

**Named Pipe (Windows):**
```
\\.\pipe\officelabs-agent
```

**Message Format (JSON-RPC 2.0):**

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
        "content": "The implementation was done...",
        "range": [150, 250]
      },
      "document_objects": [...],
      "history": [...]
    }
  }
}
```

**Response:**
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
      "diff": "- The implementation was done...\n+ The implementation was complex."
    },
    "metadata": {
      "model": "claude-3-5-sonnet-20241022",
      "tokens": 250,
      "latency_ms": 850
    }
  }
}
```

---

## 🛠️ Build System Integration

### Modified LibreOffice Build

**New Modules:**
```
officelabs/core/
├── officelabs/          # Our AI integration code
│   ├── agent/           # Agent modules (C++)
│   ├── context/         # Context engine (C++)
│   ├── ui/              # Chat panel UI (C++)
│   └── ipc/             # IPC with Python backend
├── vcl/source/ai/       # VCL chat widget
└── sw/source/ai/        # Writer-specific AI
```

**Configure Flags:**
```bash
./autogen.sh \
  --with-officelabs \
  --enable-ai-features \
  --with-agent-socket=/tmp/officelabs-agent.sock
```

**Compile:**
```bash
make build-nocheck
```

---

## 🔄 Git-Based Versioning

### Document as Repository

Every document is a Git repo:
```
my_document.fodt
├── .git/
│   ├── commits by AI agents
│   └── version history
├── .officelabs/
│   ├── context-cache.json
│   └── agent-history.log
└── my_document.fodt (Flat ODF)
```

### Auto-Commit on AI Changes

```python
def apply_patch(patch: Patch):
    # Apply to document
    document.apply(patch)
    
    # Git commit
    git.add(document.path)
    git.commit(
        message=f"AI: {patch.type} {patch.target}",
        author="Kiro Assistant <ai@officelabs.dev>"
    )
```

### Diff Viewer Integration

Built into chat panel:
```cpp
class PatchPreview : public vcl::Window {
    void renderDiff(const Patch& patch) {
        // Syntax-highlighted diff view
        // Like GitHub's diff viewer
    }
};
```

---

## 🚀 Development Roadmap

### Phase 1: Fork & Foundation (Q1 2026)
- [ ] Fork LibreOffice core
- [ ] Set up build system for OfficeLabs
- [ ] Create basic AI chat panel (C++ widget)
- [ ] Implement IPC between C++ and Python
- [ ] Deploy agent service structure

### Phase 2: Writer Agent (Q2 2026)
- [ ] Context engine for Writer
- [ ] @-addressing for headings, paragraphs, tables
- [ ] Writer agent with content generation
- [ ] Patch system for text edits
- [ ] Git integration

### Phase 3: Calc Agent (Q3 2026)
- [ ] Context engine for Calc
- [ ] @-addressing for cells, ranges, sheets
- [ ] Formula generation from NL
- [ ] Chart/pivot table creation
- [ ] Data transformation agent

### Phase 4: Impress & Macro Agents (Q4 2026)
- [ ] Impress context engine
- [ ] Presentation generation
- [ ] Python macro agent
- [ ] Macro sandbox execution
- [ ] Full suite integration

### Phase 5: Advanced Features (2027)
- [ ] Inline ghost text (Cursor-style)
- [ ] Multi-file operations
- [ ] MCP server for external tools
- [ ] Collaborative features
- [ ] Local LLM optimization

---

## 📚 References

- **LibreOffice Core:** https://github.com/LibreOffice/core
- **OfficeLabs Agent:** https://github.com/Pher217/officelabs-agent.git
- **Cursor:** https://cursor.sh (inspiration)
- **VCL Toolkit:** https://docs.libreoffice.org/vcl/html/
- **PyUNO:** https://wiki.documentfoundation.org/Macros/Python_Guide
- **MCP Protocol:** https://modelcontextprotocol.io/

---

## 🤝 Contributing

See [FORK_GUIDE.md](./FORK_GUIDE.md) for instructions on building the fork.

---

**OfficeLabs** — AI-native office suite for the open-source era.

