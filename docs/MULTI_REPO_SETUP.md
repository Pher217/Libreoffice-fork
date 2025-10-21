# OfficeLabs Multi-Repository Setup Guide

**Created:** October 21, 2025  
**Status:** Ready for GitHub deployment

---

## 🎯 Overview

This guide provides the complete structure for setting up OfficeLabs as a multi-repository architecture with 8 specialized repositories.

## 📋 Repository List

| Repository | Purpose | Language | Status |
|------------|---------|----------|--------|
| **officelabs-master** | Orchestration hub | Python | ✅ Ready |
| **officelabs-core** | LibreOffice fork + AI UI | C++ | ✅ Ready |
| **officelabs-backend** | FastAPI backend | Python | ✅ Ready |
| **officelabs-agent** | Master agent orchestrator | Python | ✅ Ready |
| **officelabs-writer-agent** | Writer AI assistant | Python | ✅ Ready |
| **officelabs-calc-agent** | Calc AI assistant | Python | ✅ Ready |
| **officelabs-impress-agent** | Impress AI assistant | Python | ✅ Ready |
| **officelabs-macro-agent** | Macro automation engine | Python | ✅ Ready |

---

## 🔐 Prerequisites

### GitHub Authentication for MCP

Before creating repositories, configure GitHub authentication:

1. **Generate GitHub Personal Access Token:**
   - Go to: https://github.com/settings/tokens
   - Click "Generate new token (classic)"
   - Scopes needed: `repo`, `workflow`, `write:packages`
   - Copy the token

2. **Configure MCP Server:**
   - The MCP GitHub integration needs authentication
   - Add token to your MCP configuration
   - Restart Cursor/IDE

### Required Tools

- Git
- GitHub CLI (optional): `gh` command
- Python 3.9+
- C++ compiler (for core)

---

## 🚀 Quick Setup (Automated)

All repository structures have been generated in the `repo_templates/` directory.

### Option 1: Using GitHub CLI (Recommended)

```bash
# Navigate to repo templates
cd repo_templates

# For each repository:
cd officelabs-master
gh repo create Pher217/officelabs-master --public --source=. --remote=origin --push
cd ..

cd officelabs-core
gh repo create Pher217/officelabs-core --public --source=. --remote=origin --push
cd ..

cd officelabs-backend
gh repo create Pher217/officelabs-backend --public --source=. --remote=origin --push
cd ..

cd officelabs-agent
gh repo create Pher217/officelabs-agent --public --source=. --remote=origin --push
cd ..

cd officelabs-writer-agent
gh repo create Pher217/officelabs-writer-agent --public --source=. --remote=origin --push
cd ..

cd officelabs-calc-agent
gh repo create Pher217/officelabs-calc-agent --public --source=. --remote=origin --push
cd ..

cd officelabs-impress-agent
gh repo create Pher217/officelabs-impress-agent --public --source=. --remote=origin --push
cd ..

cd officelabs-macro-agent
gh repo create Pher217/officelabs-macro-agent --public --source=. --remote=origin --push
cd ..
```

### Option 2: Using Provided Script

```bash
cd repo_templates
python ../setup_all_repos.py
```

---

## 📁 Repository Structures

See detailed structure in each `repo_templates/officelabs-*/` directory.

---

## 🔄 Next Steps After Repository Creation

### 1. Initialize officelabs-core

The core repository needs to be initialized as a LibreOffice fork:

```bash
cd officelabs-core

# Add LibreOffice upstream
git remote add upstream https://github.com/LibreOffice/core.git

# Fetch upstream
git fetch upstream

# Create sync branch
git checkout -b libreoffice-sync upstream/master

# Create main development branch
git checkout -b officelabs-main

# Begin integration
# (See docs/FORK_GUIDE.md for detailed build instructions)
```

### 2. Set Up Development Environment

```bash
# Clone master repo
git clone https://github.com/Pher217/officelabs-master.git
cd officelabs-master

# Initialize all submodules
git submodule update --init --recursive

# Run setup script
python setup_master.py
```

### 3. Configure Agent Backend

```bash
cd officelabs-backend

# Create virtual environment
python -m venv venv
source venv/bin/activate  # Windows: venv\Scripts\activate

# Install dependencies
pip install -r requirements.txt

# Configure
cp config/config.example.yaml config/config.yaml
# Edit config.yaml with your API keys

# Start backend
python src/main.py
```

---

## 📊 Repository Details

### officelabs-master

**Purpose:** Central orchestration hub linking all repositories

**Key Files:**
- `.gitmodules` - Submodule configuration
- `setup_master.py` - Automated setup script
- `Makefile` - Build orchestration
- `docs/ARCHITECTURE.md` - System architecture

**Clone:**
```bash
git clone --recurse-submodules https://github.com/Pher217/officelabs-master.git
```

### officelabs-core

**Purpose:** LibreOffice fork with native AI integration (C++ layer)

**Key Components:**
- `officelabs/agent/` - Agent integration layer
- `officelabs/context/` - Document context engine
- `officelabs/ipc/` - IPC communication
- `officelabs/ui/` - Native UI widgets (VCL)
- `vcl/source/ai/` - AI chat panel

**Build:**
```bash
./autogen.sh --with-officelabs --enable-ai-features
make build-nocheck -j8
```

### officelabs-backend

**Purpose:** FastAPI backend for API routing, patching, and LLM communication

**Key Components:**
- `src/api/` - REST API endpoints
- `src/llm/` - LLM provider clients
- `src/patches/` - Patch system
- `src/ipc/` - IPC server

**Start:**
```bash
uvicorn src.main:app --reload
```

### officelabs-agent

**Purpose:** Master agent orchestrator linking all sub-agents

**Responsibilities:**
- Route requests to specialized agents
- Manage agent lifecycle
- Coordinate multi-agent workflows
- Handle cross-application tasks

### officelabs-writer-agent

**Purpose:** AI assistant for Writer

**Capabilities:**
- Content generation (paragraphs, sections, outlines)
- Content editing (rewrite, tone, length)
- Document structuring (headings, TOC)
- Table/list creation

### officelabs-calc-agent

**Purpose:** AI assistant for Calc

**Capabilities:**
- Formula generation from natural language
- Data operations (sort, filter, clean)
- Chart and pivot table creation
- Statistical analysis

### officelabs-impress-agent

**Purpose:** AI assistant for Impress

**Capabilities:**
- Slide generation (title, content, closing)
- Layout management
- Speaker notes generation
- Visual consistency

### officelabs-macro-agent

**Purpose:** Python macro engine

**Capabilities:**
- Python code generation from natural language
- Document automation
- Batch operations
- External integrations

---

## 🔗 Inter-Repository Dependencies

```
officelabs-master
├── officelabs-core (submodule)
├── officelabs-backend (submodule)
│   └── depends on: officelabs-agent
├── officelabs-agent (submodule)
│   ├── officelabs-writer-agent (submodule)
│   ├── officelabs-calc-agent (submodule)
│   ├── officelabs-impress-agent (submodule)
│   └── officelabs-macro-agent (submodule)
```

---

## 🎯 Development Workflow

### 1. Feature Development

```bash
# Work in specific repository
cd officelabs-writer-agent
git checkout -b feature/new-capability
# Make changes
git commit -m "Add new capability"
git push origin feature/new-capability
```

### 2. Integration Testing

```bash
# Return to master
cd officelabs-master
# Update submodule reference
git submodule update --remote officelabs-writer-agent
git commit -m "Update writer agent"
git push
```

### 3. Release

```bash
# Tag all repositories
cd officelabs-master
python scripts/release.py v0.1.0
```

---

## 📦 Versioning Strategy

- **Core:** Follows LibreOffice versioning + OfficeLabs version
  - Example: `LibreOffice-7.6-OfficeLabs-0.1.0`
- **Backend/Agents:** Semantic versioning
  - Example: `v0.1.0`, `v0.2.0`, `v1.0.0`
- **Master:** Tracks overall system version
  - Example: `v0.1.0` (MVP), `v1.0.0` (Full release)

---

## 🐛 Troubleshooting

### Submodule Issues

```bash
# Reset all submodules
git submodule deinit -f .
git submodule update --init --recursive
```

### Authentication Issues

```bash
# Configure Git credentials
git config --global credential.helper store
gh auth login
```

### Build Issues

```bash
# Clean build
cd officelabs-core
make clean
./autogen.sh --with-officelabs
make build-nocheck -j8
```

---

## 📞 Support

- **Documentation:** https://github.com/Pher217/officelabs-master/docs
- **Issues:** https://github.com/Pher217/officelabs-master/issues
- **Discussions:** https://github.com/Pher217/officelabs-master/discussions

---

## 📜 License

All repositories are licensed under **Mozilla Public License 2.0 (MPL-2.0)**, consistent with LibreOffice.

---

**Generated:** October 21, 2025  
**OfficeLabs Team**

