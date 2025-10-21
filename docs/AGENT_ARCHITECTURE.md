# OfficeLabs Agent Architecture

**Modular Agent System for Document Intelligence**

---

## 🎯 Overview

OfficeLabs uses a **multi-agent architecture** where each agent is specialized for a specific office application and task domain. This document describes the design, implementation, and interaction patterns of the agent system.

---

## 🧩 Agent Taxonomy

```
OfficeLabs Agent System
│
├── Application Agents (Domain-Specific)
│   ├── Writer Agent      → Content & document editing
│   ├── Calc Agent        → Data & spreadsheet operations
│   ├── Impress Agent     → Presentation design
│   └── Macro Agent       → Python automation
│
├── Foundation Services (Shared)
│   ├── LLM Router        → Multi-provider LLM access
│   ├── Context Parser    → Document context interpretation
│   ├── Patch System      → Change generation & application
│   └── Memory Manager    → Conversation & document memory
│
└── Integration Layer
    ├── IPC Server        → Communication with C++ layer
    ├── Document Bridge   → PyUNO interface
    └── Git Manager       → Version control integration
```

---

## 🤖 Agent Specifications

### Base Agent Interface

All agents inherit from `BaseAgent`:

```python
from abc import ABC, abstractmethod
from typing import Dict, List, Optional
from dataclasses import dataclass

@dataclass
class AgentContext:
    """Context provided to agents"""
    app: str                          # "writer", "calc", "impress"
    document_title: str
    document_url: str
    selection: Optional[Dict]         # Current selection/cursor
    document_objects: List[Dict]      # Indexed @-addressable objects
    history: List[Dict]               # Conversation history
    metadata: Dict                    # Additional context

@dataclass
class AgentResponse:
    """Response from agent"""
    message: str                      # Natural language response
    patch: Optional[Dict]             # Structured change to apply
    suggested_prompts: List[str]      # Follow-up suggestions
    metadata: Dict                    # Model, tokens, latency

class BaseAgent(ABC):
    """Base class for all agents"""
    
    def __init__(self, llm_client, config: Dict):
        self.llm = llm_client
        self.config = config
        self.conversation_history = []
    
    @abstractmethod
    def get_system_prompt(self, context: AgentContext) -> str:
        """Generate system prompt for this agent"""
        pass
    
    @abstractmethod
    def process_message(
        self, 
        message: str, 
        context: AgentContext
    ) -> AgentResponse:
        """Process user message and return response"""
        pass
    
    @abstractmethod
    def validate_patch(self, patch: Dict) -> bool:
        """Validate that patch is safe to apply"""
        pass
    
    def get_capabilities(self) -> List[str]:
        """Return list of agent capabilities"""
        return []
```

---

## 📝 Writer Agent

**Purpose:** Content generation, editing, and document structuring

### Capabilities

1. **Content Generation**
   - Write paragraphs, sections, outlines
   - Generate headings and subheadings
   - Create lists (bullet, numbered)
   - Draft emails, letters, reports

2. **Content Editing**
   - Rewrite for tone (formal, casual, technical)
   - Adjust length (expand, condense)
   - Fix grammar and style
   - Improve clarity and flow

3. **Document Structuring**
   - Create table of contents
   - Organize sections logically
   - Add/remove headings
   - Format consistently

4. **Advanced Operations**
   - Insert tables with data
   - Add citations and references
   - Create templates
   - Merge multiple documents

### System Prompt Template

```python
def get_system_prompt(self, context: AgentContext) -> str:
    return f"""You are the Writer Agent for OfficeLabs, an AI office suite.

Your role is to help users create and edit documents naturally.

CURRENT DOCUMENT:
- Title: {context.document_title}
- Structure:
{self._format_document_structure(context)}

CURRENT SELECTION:
{self._format_selection(context)}

CAPABILITIES:
- Generate content (paragraphs, sections, outlines)
- Rewrite text (tone, length, clarity)
- Format documents (headings, tables, lists)
- Structure documents (TOC, organization)

ADDRESSING SYNTAX:
- @Heading("Title") - Reference a heading
- @Para#N - Reference paragraph N
- @Table("Name") - Reference a table

RESPONSE FORMAT:
When modifying content, use this structure:
{{
  "message": "I'll make that paragraph more concise.",
  "patch": {{
    "type": "replace",
    "target": "@Para#3",
    "old_value": "original text...",
    "new_value": "shorter text..."
  }}
}}

Be helpful, concise, and always preview changes.
"""

def process_message(
    self, 
    message: str, 
    context: AgentContext
) -> AgentResponse:
    """Process Writer-specific message"""
    
    # Build full prompt
    system_prompt = self.get_system_prompt(context)
    
    # Call LLM
    llm_response = self.llm.chat(
        system=system_prompt,
        messages=context.history + [
            {"role": "user", "content": message}
        ]
    )
    
    # Parse response
    response_text = llm_response.content
    patch = self._extract_patch(response_text, context)
    
    # Validate patch if present
    if patch:
        if not self.validate_patch(patch):
            patch = None
            response_text += "\n\n(Patch validation failed)"
    
    return AgentResponse(
        message=response_text,
        patch=patch,
        suggested_prompts=self._generate_suggestions(context),
        metadata={
            "model": llm_response.model,
            "tokens": llm_response.usage.total_tokens,
            "latency_ms": llm_response.latency_ms
        }
    )

def validate_patch(self, patch: Dict) -> bool:
    """Validate Writer patch"""
    
    # Check required fields
    if not all(k in patch for k in ["type", "target"]):
        return False
    
    # Check patch type
    if patch["type"] not in ["replace", "insert", "delete"]:
        return False
    
    # Check target address format
    target = patch["target"]
    if not (
        target.startswith("@Heading") or
        target.startswith("@Para") or
        target.startswith("@Table")
    ):
        return False
    
    # Additional validation...
    return True
```

### Example Interactions

**Content Generation:**
```python
User: "Write an introduction about AI in healthcare"

Agent Response:
message: "Here's an introduction about AI in healthcare:"
patch: {
  "type": "insert",
  "target": "@Para#1",
  "new_value": "Artificial intelligence is transforming healthcare by enabling more accurate diagnoses, personalized treatments, and efficient patient care. From analyzing medical images to predicting disease outcomes, AI systems are becoming indispensable tools for medical professionals worldwide."
}
```

**Content Editing:**
```python
User: "Make @Para#3 more formal"

Context:
selection: {
  "type": "paragraph",
  "address": "@Para#3",
  "content": "AI is really cool and it's gonna change everything..."
}

Agent Response:
message: "I'll make that paragraph more formal:"
patch: {
  "type": "replace",
  "target": "@Para#3",
  "old_value": "AI is really cool and it's gonna change everything...",
  "new_value": "Artificial intelligence represents a significant technological advancement with the potential to transform numerous industries..."
}
```

---

## 📊 Calc Agent

**Purpose:** Data operations, calculations, and business intelligence

### Capabilities

1. **Formula Generation**
   - Generate formulas from natural language
   - Explain existing formulas
   - Fix formula errors
   - Optimize complex calculations

2. **Data Operations**
   - Sort and filter data
   - Remove duplicates
   - Data validation
   - Find and replace

3. **Analysis & Visualization**
   - Create charts (bar, line, pie, scatter)
   - Generate pivot tables
   - Statistical analysis
   - Trend analysis

4. **Advanced Operations**
   - Data transformation
   - Conditional formatting
   - Macro generation
   - Import/export data

### System Prompt Template

```python
def get_system_prompt(self, context: AgentContext) -> str:
    return f"""You are the Calc Agent for OfficeLabs, specialized in spreadsheet operations.

CURRENT SPREADSHEET:
- File: {context.document_title}
- Active Sheet: {self._get_active_sheet(context)}
- Sheets: {self._list_sheets(context)}

CURRENT SELECTION:
{self._format_selection(context)}

CAPABILITIES:
- Generate formulas from natural language
- Create charts and pivot tables
- Analyze data (statistics, trends)
- Transform and clean data

ADDRESSING SYNTAX:
- @Sheet("Name") - Reference a sheet
- @Cell(A1) - Reference a cell
- @Range(A1:D20) or @Range(Name) - Reference a range

FORMULA EXAMPLES:
- "Calculate the sum of @Range(Sales)" → =SUM(A1:D20)
- "Average of column B" → =AVERAGE(B:B)
- "Count non-empty cells in range" → =COUNTA(A1:A100)

Always show formulas and explain what they do.
"""
```

### Example Interactions

**Formula Generation:**
```python
User: "Calculate the average of @Range(Sales) and put it in @Cell(Total)"

Agent Response:
message: "I'll calculate the average sales:"
patch: {
  "type": "formula",
  "target": "@Cell(Total)",
  "formula": "=AVERAGE(A2:A50)",
  "result": "$12,345.67",
  "explanation": "This calculates the average of all values in the Sales range (A2:A50)"
}
```

**Chart Creation:**
```python
User: "Create a bar chart showing monthly revenue from @Range(Revenue)"

Agent Response:
message: "I'll create a bar chart for monthly revenue:"
patch: {
  "type": "chart",
  "target": "@Sheet(Dashboard)",
  "chart_type": "bar",
  "data_range": "A1:B13",
  "title": "Monthly Revenue",
  "x_axis": "Month",
  "y_axis": "Revenue ($)"
}
```

---

## 🎞️ Impress Agent

**Purpose:** Presentation design and slide generation

### Capabilities

1. **Slide Generation**
   - Create title slides
   - Generate content slides
   - Add transition slides
   - Create closing slides

2. **Content Management**
   - Edit slide text
   - Add bullet points
   - Insert images/shapes
   - Format text consistently

3. **Design & Layout**
   - Apply themes
   - Consistent branding
   - Layout suggestions
   - Color scheme recommendations

4. **Speaker Support**
   - Generate speaker notes
   - Suggest talking points
   - Estimate presentation time
   - Create handouts

### System Prompt Template

```python
def get_system_prompt(self, context: AgentContext) -> str:
    return f"""You are the Impress Agent for OfficeLabs, specialized in presentations.

CURRENT PRESENTATION:
- Title: {context.document_title}
- Slide Count: {self._get_slide_count(context)}
- Current Slide: {self._get_current_slide(context)}

ADDRESSING SYNTAX:
- @Slide#N - Reference slide by number
- @Slide("Title") - Reference slide by title

CAPABILITIES:
- Generate slides (title, content, closing)
- Edit slide content and layout
- Add images, charts, shapes
- Generate speaker notes

When creating slides, maintain visual consistency and clear hierarchy.
"""
```

### Example Interactions

**Slide Generation:**
```python
User: "Create a title slide for 'Q4 2024 Results'"

Agent Response:
message: "I'll create a title slide:"
patch: {
  "type": "insert_slide",
  "position": 0,
  "layout": "title",
  "content": {
    "title": "Q4 2024 Results",
    "subtitle": "Financial Performance Review",
    "footer": "OfficeLabs Inc. | December 2024"
  }
}
```

---

## 🐍 Macro Agent

**Purpose:** Python automation and scripting

### Capabilities

1. **Macro Generation**
   - Generate Python scripts from NL
   - Explain existing macros
   - Debug macro errors
   - Optimize performance

2. **Document Automation**
   - Batch operations
   - Document processing
   - Data import/export
   - Template filling

3. **Integration**
   - API calls
   - Database connections
   - File operations
   - External tool integration

### System Prompt Template

```python
def get_system_prompt(self, context: AgentContext) -> str:
    return f"""You are the Macro Agent for OfficeLabs, specialized in Python automation.

CONTEXT:
- Application: {context.app}
- Document: {context.document_title}

CAPABILITIES:
- Generate Python macros using PyUNO
- Automate document operations
- Integrate with external systems

PYUNO EXAMPLES:
```python
# Access document
doc = XSCRIPTCONTEXT.getDocument()

# Writer: Get all paragraphs
text = doc.getText()
paragraphs = text.createEnumeration()

# Calc: Get cell value
sheet = doc.getSheets().getByIndex(0)
cell = sheet.getCellByPosition(0, 0)
value = cell.getValue()

# Impress: Get slides
slides = doc.getDrawPages()
slide = slides.getByIndex(0)
```

Generate safe, well-documented code. Always include error handling.
"""
```

### Example Interactions

**Macro Generation:**
```python
User: "Create a macro to auto-format all tables in this document"

Agent Response:
message: "Here's a macro to format all tables:"
patch: {
  "type": "macro",
  "language": "python",
  "code": """
def format_all_tables():
    \"\"\"Format all tables in the current document\"\"\"
    try:
        doc = XSCRIPTCONTEXT.getDocument()
        tables = doc.getTextTables()
        
        for i in range(tables.getCount()):
            table = tables.getByIndex(i)
            
            # Set background color
            table.setPropertyValue("BackTransparent", False)
            table.setPropertyValue("BackColor", 0xF0F0F0)
            
            # Set borders
            table.setPropertyValue("TopBorder", create_border())
            table.setPropertyValue("BottomBorder", create_border())
            
        return f"Formatted {tables.getCount()} tables"
    except Exception as e:
        return f"Error: {str(e)}"

def create_border():
    \"\"\"Helper to create border style\"\"\"
    from com.sun.star.table import BorderLine2
    border = BorderLine2()
    border.Color = 0x000000
    border.LineWidth = 20
    return border

# Execute
format_all_tables()
""",
  "description": "Formats all tables with gray background and black borders"
}
```

---

## 🔄 Agent Communication Flow

### Request Flow

```
User Input
    ↓
[C++ Layer] Chat Panel
    ↓
[IPC] Unix Socket / Named Pipe
    ↓
[Python] Agent Router
    ↓
    ├→ Writer Agent    (if document is Writer)
    ├→ Calc Agent      (if document is Calc)
    ├→ Impress Agent   (if document is Impress)
    └→ Macro Agent     (if macro request)
    ↓
[LLM] Router → Claude/OpenAI/Local
    ↓
[Agent] Process Response
    ↓
[Patch System] Generate Structured Change
    ↓
[IPC] Return to C++ Layer
    ↓
[UI] Display Preview
```

### Agent Router Implementation

```python
class AgentRouter:
    """Routes requests to appropriate agent"""
    
    def __init__(self, config: Dict):
        self.llm = LLMRouter(config)
        
        # Initialize agents
        self.agents = {
            "writer": WriterAgent(self.llm, config),
            "calc": CalcAgent(self.llm, config),
            "impress": ImpressAgent(self.llm, config),
            "macro": MacroAgent(self.llm, config)
        }
    
    def route(
        self, 
        message: str, 
        context: AgentContext
    ) -> AgentResponse:
        """Route message to correct agent"""
        
        # Detect if macro request
        if self._is_macro_request(message):
            agent = self.agents["macro"]
        else:
            # Route based on document type
            agent = self.agents.get(context.app)
            
            if not agent:
                raise ValueError(f"Unknown app: {context.app}")
        
        # Process with agent
        return agent.process_message(message, context)
    
    def _is_macro_request(self, message: str) -> bool:
        """Detect if message is requesting macro generation"""
        keywords = [
            "macro", "script", "automate", "python",
            "generate code", "write a script"
        ]
        return any(kw in message.lower() for kw in keywords)
```

---

## 🧠 LLM Router

**Multi-provider LLM access**

```python
class LLMRouter:
    """Abstract LLM provider access"""
    
    def __init__(self, config: Dict):
        self.provider = config.get("llm", {}).get("provider", "claude")
        
        # Initialize clients
        self.clients = {
            "claude": ClaudeClient(config),
            "openai": OpenAIClient(config),
            "local": LocalClient(config)
        }
    
    def chat(
        self,
        system: str,
        messages: List[Dict],
        **kwargs
    ) -> LLMResponse:
        """Send chat request to LLM"""
        
        client = self.clients[self.provider]
        return client.chat(
            system=system,
            messages=messages,
            **kwargs
        )
```

---

## 🔒 Security Considerations

### Macro Execution Sandboxing

```python
class MacroSandbox:
    """Sandboxed Python execution for macros"""
    
    def __init__(self, context: AgentContext):
        self.context = context
        self.allowed_modules = [
            "uno", "unohelper",
            "math", "datetime", "re"
        ]
    
    def run(self, code: str) -> Any:
        """Execute code in sandbox"""
        
        # Validate code
        if not self._is_safe(code):
            raise SecurityError("Unsafe code detected")
        
        # Create restricted globals
        restricted_globals = {
            "__builtins__": self._get_safe_builtins(),
            "XSCRIPTCONTEXT": self._get_script_context()
        }
        
        # Execute
        exec(code, restricted_globals)
    
    def _is_safe(self, code: str) -> bool:
        """Check if code is safe to execute"""
        
        # Block dangerous operations
        forbidden = [
            "import os", "import sys", "import subprocess",
            "eval(", "exec(", "__import__",
            "open(", "file("
        ]
        
        return not any(f in code for f in forbidden)
```

---

## 📊 Agent Performance Metrics

Agents track and report:
- **Response latency** - Time to generate response
- **LLM tokens** - Tokens used per request
- **Patch success rate** - % of patches successfully applied
- **User satisfaction** - Implicit feedback (accept/reject)

```python
class AgentMetrics:
    """Track agent performance"""
    
    def record_request(
        self,
        agent_type: str,
        message: str,
        response: AgentResponse,
        outcome: str  # "accepted", "rejected", "regenerated"
    ):
        """Record request metrics"""
        
        self.db.insert({
            "timestamp": datetime.now(),
            "agent": agent_type,
            "tokens": response.metadata["tokens"],
            "latency_ms": response.metadata["latency_ms"],
            "outcome": outcome
        })
```

---

## 🚀 Future Enhancements

### Multi-Agent Collaboration

Agents can collaborate on complex tasks:

```python
User: "Create a presentation from this spreadsheet data"

Flow:
1. Calc Agent extracts key metrics
2. Impress Agent generates slides
3. Writer Agent crafts narratives
```

### Agent Memory

Long-term memory for personalization:

```python
class AgentMemory:
    """Remember user preferences and patterns"""
    
    def remember(self, user_id: str, preference: Dict):
        """Store user preference"""
        pass
    
    def recall(self, user_id: str) -> Dict:
        """Retrieve user preferences"""
        pass
```

### Proactive Suggestions

Agents can offer unsolicited help:

```python
Agent: "I noticed you're writing a report. Would you like me to:
- Generate a table of contents?
- Add section summaries?
- Format headings consistently?"
```

---

## 📚 References

- [Base Agent Interface](../officelabs-agent/src/agents/base_agent.py)
- [Writer Agent Implementation](../officelabs-agent/src/agents/writer_agent.py)
- [LLM Router](../officelabs-agent/src/llm/router.py)
- [Patch System](../officelabs-agent/src/patches/)

---

**OfficeLabs Agents** - Specialized AI for every office task

