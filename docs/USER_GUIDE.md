# OfficeLabs User Guide

Complete guide to using OfficeLabs, the AI-assisted Office suite.

## Table of Contents

- [Getting Started](#getting-started)
- [Using the Kiro Assistant](#using-the-kiro-assistant)
- [@-Addressing Objects](#addressing-objects)
- [Common Tasks](#common-tasks)
- [Keyboard Shortcuts](#keyboard-shortcuts)
- [Tips and Best Practices](#tips-and-best-practices)

## Getting Started

### First Launch

1. **Install OfficeLabs** following the installation instructions in the main README
2. **Start the Agent Service**:
   ```bash
   cd officelabs-agent
   python -m src.main
   ```
3. **Open LibreOffice** (Writer, Calc, or Impress)
4. **Open Kiro Assistant** from View → Sidebar → Kiro Assistant

### Initial Configuration

1. Click the **Configuration** button in the sidebar
2. Set your API key (Claude or OpenAI)
3. Choose your preferred LLM provider
4. Click **Save**

## Using the Kiro Assistant

### Opening the Sidebar

- **Menu**: View → Sidebar → Kiro Assistant
- **Or**: Press the Kiro icon in the sidebar panel

### Chat Interface

The Kiro Assistant sidebar contains:
- **Status bar**: Connection status and current operation
- **Chat history**: Your conversation with Kiro
- **Input field**: Type your messages here
- **Send button**: Send your message
- **Reindex button**: Manually reindex the current document
- **Configuration button**: Open configuration dialog

### Basic Conversation

Simply type what you want to do:

```
You: What's in this document?
Kiro: This document contains 5 headings, 23 paragraphs, and 2 tables...

You: Summarize the introduction section
Kiro: The introduction discusses...
```

## @-Addressing Objects

OfficeLabs lets you reference specific parts of your document using @-addresses.

### Writer (Documents)

| Object Type | Syntax | Example |
|-------------|--------|---------|
| Heading | `@Heading("Title")` | `@Heading("Introduction")` |
| Paragraph | `@Para#N` | `@Para#12` |
| Table | `@Table("Name")` | `@Table("Budget2024")` |

### Calc (Spreadsheets)

| Object Type | Syntax | Example |
|-------------|--------|---------|
| Sheet | `@Sheet("Name")` | `@Sheet("Sales")` |
| Cell | `@Cell(Address)` | `@Cell(A1)` |
| Range | `@Range(Name)` | `@Range(TotalSales)` |
| Formula Cell | `@Cell(Sheet!Address)` | `@Cell(Sales!B66)` |

### Impress (Presentations)

| Object Type | Syntax | Example |
|-------------|--------|---------|
| Slide by Number | `@Slide#N` | `@Slide#5` |
| Slide by Title | `@Slide("Title")` | `@Slide("Overview")` |
| Shape | `@Slide#N.Shape("Name")` | `@Slide#2.Shape("Chart1")` |

### Autocomplete

As you type `@`, Kiro will show suggestions:
- Type `@` to see all objects
- Type `@Head` to filter headings
- Type `@Para` to filter paragraphs
- Type `@Table` to filter tables

## Common Tasks

### Rewriting Content

```
You: Rewrite @Para#3 to be more concise
Kiro: [Shows preview of changes]
      - Old: The implementation was done in a manner that was quite complex...
      + New: The implementation was complex.
      
      [Accept] [Reject] buttons
```

### Adding Content

```
You: Add a new paragraph after @Para#5 about the benefits of this approach
Kiro: [Shows the new paragraph]
      + New benefits include improved performance and maintainability.
      
      [Accept] [Reject] buttons
```

### Working with Tables

```
You: Add a new row to @Table("Budget") with Item: "Marketing" and Cost: "$5,000"
Kiro: [Shows the change]
      Added new row to Budget table
      
      [Accept] [Reject] buttons
```

### Spreadsheet Operations

```
You: Calculate the sum of @Range(SalesData) and put it in @Cell(Total)
Kiro: [Shows formula]
      =SUM(A1:D20)
      Result: $45,230.50
      
      [Accept] [Reject] buttons
```

### Presentation Editing

```
You: Change the title of @Slide#1 to "Q4 2024 Results"
Kiro: [Shows preview]
      - Old: Q3 2024 Results
      + New: Q4 2024 Results
      
      [Accept] [Reject] buttons
```

## Patch Preview and Application

### Understanding Patches

When you ask Kiro to modify content, it generates a **patch** - a structured change description.

**Patch Preview Shows:**
- Target object (@-address)
- Old value (before)
- New value (after)
- Unified diff (line-by-line changes)

### Accepting Changes

1. Review the patch preview
2. Click **Accept** to apply
3. Changes are applied immediately
4. Git commit is created (if enabled)

### Rejecting Changes

1. Click **Reject** to discard
2. Continue conversation with Kiro
3. Provide feedback: "That's too long" or "Try again with a different tone"

## Git Versioning

OfficeLabs automatically tracks changes with Git.

### Automatic Commits

- Every accepted patch creates a commit
- Commit message: "OfficeLabs: replace @Para#3"
- View history: `git log` in document directory

### Manual Commits

- File → Save creates a commit (if auto-commit enabled)
- Or use regular Git commands

### Viewing History

```bash
cd /path/to/document
git log --oneline
git diff HEAD~1  # Compare with previous version
```

### Flat ODF Format

Documents are saved as Flat ODF by default:
- `.fodt` for Writer (instead of `.odt`)
- `.fods` for Calc (instead of `.ods`)
- `.fodp` for Impress (instead of `.odp`)

**Benefits:**
- Git-friendly (text-based)
- Easy to diff
- Merge-friendly
- Same functionality as binary format

**Converting Back:**
- File → Save As → Choose `.odt`/`.ods`/`.odp` format

## Keyboard Shortcuts

| Action | Shortcut |
|--------|----------|
| Open Kiro Assistant | `Ctrl+K` (configurable) |
| Send Message | `Ctrl+Enter` |
| Reindex Document | `F5` |
| Accept Patch | `Ctrl+Shift+A` |
| Reject Patch | `Ctrl+Shift+R` |

## Tips and Best Practices

### Be Specific

❌ "Fix this paragraph"
✅ "Rewrite @Para#3 to be more formal"

### Use @-Addressing

❌ "Change the introduction"
✅ "Change @Heading('Introduction')"

### Iterative Refinement

```
You: Make @Para#5 more concise
Kiro: [Shows shortened version]
You: That's too short. Keep the key details but remove fluff.
Kiro: [Shows better version]
```

### Document Context

Kiro automatically understands:
- Document structure (headings, sections)
- Current content
- Object relationships

But help it by being explicit:
```
You: Looking at @Table("Sales"), add a row for December with $10,000
```

### Save Regularly

- Kiro changes are applied immediately
- But save the document to persist to disk
- Git commits happen on save (if enabled)

### Reindex When Needed

If you make manual changes outside Kiro:
- Click **Reindex Document** button
- Or Kiro will auto-reindex on next interaction

## Troubleshooting

### "Agent offline - check service"

**Solution:**
```bash
cd officelabs-agent
python -m src.main
```

### "@-address not found"

**Cause:** Object doesn't exist or document not indexed

**Solution:**
1. Click **Reindex Document**
2. Verify object exists
3. Try autocomplete to see available objects

### "API error: Invalid API key"

**Solution:**
1. Click **Configuration**
2. Enter valid API key
3. Save and retry

### Changes not applied

**Cause:** Patch application error

**Solution:**
1. Check LibreOffice is responding
2. Try smaller changes
3. Reindex document
4. Check OfficeLabs Agent logs

## Getting Help

- **Documentation**: https://officelabs.readthedocs.io/
- **Issues**: https://github.com/officelabs/officelabs/issues
- **Discussions**: https://github.com/officelabs/officelabs/discussions

## What's Next?

- [Development Guide](DEVELOPMENT.md) - Contributing to OfficeLabs
- [Architecture](ARCHITECTURE.md) - How OfficeLabs works
- [API Reference](API.md) - Agent service API

