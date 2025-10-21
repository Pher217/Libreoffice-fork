# OfficeLabs Quickstart Guide

Get OfficeLabs up and running in 10 minutes.

## Prerequisites

- **Python 3.9+**
- **LibreOffice 7.6+**
- **Git**
- **API Key** (Claude or OpenAI)

## Installation

### Step 1: Clone Repositories

```bash
# Clone main repository
git clone https://github.com/officelabs/officelabs.git

# Clone agent repository
git clone https://github.com/officelabs/officelabs-agent.git
```

### Step 2: Setup OfficeLabs Extension

```bash
cd officelabs

# Create virtual environment
python -m venv venv

# Activate virtual environment
# Linux/macOS:
source venv/bin/activate
# Windows:
venv\Scripts\activate

# Install dependencies
pip install -r requirements.txt

# Build extension
# Linux/macOS:
cd officelabs/build
./build_extension.sh
./install.sh

# Windows:
cd officelabs\build
.\build_extension.ps1
.\install.ps1
```

### Step 3: Setup Agent Service

```bash
cd ../../officelabs-agent

# Create virtual environment
python -m venv venv

# Activate
source venv/bin/activate  # or venv\Scripts\activate on Windows

# Install dependencies
pip install -r requirements.txt

# Configure
cp config/config.example.yaml config/config.yaml

# Edit config.yaml and add your API key
# For Claude:
nano config/config.yaml  # or notepad config/config.yaml on Windows
# Set: api_key: sk-ant-your-key-here

# For OpenAI:
# Set: provider: openai
# Set: api_key: sk-your-key-here
```

### Step 4: Start Services

**Terminal 1 - Agent Service:**
```bash
cd officelabs-agent
source venv/bin/activate  # or venv\Scripts\activate
python -m src.main
```

You should see:
```
INFO:     Started server process
INFO:     Waiting for application startup.
INFO:     Starting OfficeLabs Agent...
INFO:     Using LLM provider: claude
INFO:     Server listening on localhost:8765
INFO:     Application startup complete.
```

**Terminal 2 - LibreOffice:**
```bash
# Linux/macOS
soffice --writer

# Windows
"C:\Program Files\LibreOffice\program\soffice.exe" --writer
```

## First Use

### 1. Open Kiro Assistant

In LibreOffice:
1. Go to **View** → **Sidebar**
2. Look for **Kiro Assistant** panel
3. If not visible, restart LibreOffice

### 2. Test Connection

In the Kiro sidebar:
- Check the status bar at the top
- Should show: "Connected to agent"
- If offline, verify agent service is running

### 3. Index Current Document

1. Create a new document or open an existing one
2. Add some content (headings, paragraphs)
3. Click **Reindex Document** button
4. Status should show: "Indexed N objects"

### 4. Chat with Kiro

Try these examples:

**Simple query:**
```
You: What's in this document?
Kiro: This document contains...
```

**Document editing:**
```
You: Make @Para#1 more concise
Kiro: [Shows preview]
- Old: The quick brown fox jumps over the lazy dog repeatedly.
+ New: The quick brown fox jumps over the lazy dog.
[Accept] [Reject]
```

**Spreadsheet operation (in Calc):**
```
You: Calculate the sum of @Range(Sales) and put it in @Cell(Total)
Kiro: [Shows formula]
=SUM(A1:D20)
[Accept] [Reject]
```

## Common Issues

### "Agent offline - check service"

**Fix:**
```bash
cd officelabs-agent
python -m src.main
```

### "No document to index"

**Fix:**
1. Create or open a document
2. Add some content
3. Click **Reindex Document**

### "Invalid API key"

**Fix:**
1. Click **Configuration** in sidebar
2. Enter valid API key
3. Save
4. Restart agent service

### Extension not loading

**Fix:**
```bash
# Check if installed
unopkg list | grep officelabs

# Reinstall
cd officelabs/build
./install.sh  # or .\install.ps1
```

## Next Steps

- **Read the [User Guide](USER_GUIDE.md)** - Learn all features
- **Try @-addressing** - Reference document objects
- **Explore Git versioning** - Track your changes
- **Customize configuration** - Adjust settings

## Examples

### Writer Example

```markdown
Document: Project Proposal

Headings:
- Introduction (@Heading("Introduction"))
- Background (@Heading("Background"))
- Methodology (@Heading("Methodology"))

Chat examples:
- "Summarize @Heading('Introduction')"
- "Rewrite @Para#3 in active voice"
- "Add a new paragraph after @Para#5 about the benefits"
```

### Calc Example

```markdown
Spreadsheet: Sales Data

Sheets:
- Q1 Sales (@Sheet("Q1 Sales"))
- Q2 Sales (@Sheet("Q2 Sales"))

Chat examples:
- "Calculate the average of @Range(Q1Sales)"
- "Create a chart from @Range(Sales!A1:D20)"
- "Explain the formula in @Cell(Total)"
```

### Impress Example

```markdown
Presentation: Quarterly Review

Slides:
- @Slide("Title")
- @Slide("Overview")
- @Slide("Results")

Chat examples:
- "Change the title of @Slide#1 to 'Q4 2024 Results'"
- "Add a bullet point to @Slide('Overview') about growth"
- "Create a new slide after @Slide#3 with a summary"
```

## Getting Help

- **User Guide:** [docs/USER_GUIDE.md](USER_GUIDE.md)
- **Issues:** https://github.com/officelabs/officelabs/issues
- **Discussions:** https://github.com/officelabs/officelabs/discussions
- **Discord:** https://discord.gg/officelabs

## Welcome to OfficeLabs!

You're now ready to use AI-assisted document editing. Enjoy! 🎉

