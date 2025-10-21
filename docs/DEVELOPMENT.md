# OfficeLabs Development Guide

Guide for developers contributing to OfficeLabs.

## Table of Contents

- [Development Setup](#development-setup)
- [Repository Structure](#repository-structure)
- [Building from Source](#building-from-source)
- [Testing](#testing)
- [Contributing](#contributing)
- [Code Style](#code-style)
- [Debugging](#debugging)

## Development Setup

### Prerequisites

- **Python 3.9+**
- **LibreOffice 7.6+** (with development headers)
- **Git**
- **Node.js 18+** (for future UI work)
- **API Key** (Claude or OpenAI)

### Initial Setup

1. **Clone both repositories:**
   ```bash
   git clone https://github.com/yourusername/officelabs.git
   git clone https://github.com/yourusername/officelabs-agent.git
   ```

2. **Set up OfficeLabs:**
   ```bash
   cd officelabs
   python -m venv venv
   source venv/bin/activate  # Windows: venv\Scripts\activate
   pip install -r requirements.txt
   pip install -e .  # Editable install
   ```

3. **Set up Agent:**
   ```bash
   cd ../officelabs-agent
   python -m venv venv
   source venv/bin/activate
   pip install -r requirements.txt
   pip install -e .
   ```

4. **Configure Agent:**
   ```bash
   cp config/config.example.yaml config/config.yaml
   # Edit config.yaml with your API keys
   ```

## Repository Structure

### OfficeLabs (Main Repository)

```
officelabs/
├── officelabs/              # Python package
│   ├── __init__.py
│   ├── config.py            # Configuration management
│   ├── sidebar/             # Sidebar UI
│   │   └── kiro_sidebar.py
│   ├── doc_indexer/         # Document indexing
│   │   ├── base.py
│   │   ├── writer_indexer.py
│   │   ├── calc_indexer.py
│   │   └── impress_indexer.py
│   ├── diff_engine/         # Git integration
│   │   ├── convert.py
│   │   ├── git_handler.py
│   │   └── differ.py
│   ├── pyuno_bridge/        # LibreOffice connection
│   │   ├── connection.py
│   │   └── utils.py
│   └── build/               # Build scripts
│       ├── build_extension.sh
│       ├── build_extension.ps1
│       ├── manifest.xml
│       └── META-INF/
├── docs/                    # Documentation
├── tests/                   # Tests
├── officelabs-patches/      # Feature patches
├── sync-upstream.sh         # Upstream sync script
├── requirements.txt
├── pytest.ini
└── README.md
```

### OfficeLabs Agent (Separate Repository)

```
officelabs-agent/
├── src/
│   ├── main.py             # FastAPI app
│   ├── config.py           # Configuration
│   ├── api/                # API routes
│   │   ├── chat.py
│   │   ├── index.py
│   │   └── patches.py
│   ├── llm/                # LLM clients
│   │   ├── base.py
│   │   ├── claude_client.py
│   │   ├── openai_client.py
│   │   └── llm_router.py
│   ├── document/           # Document operations
│   │   └── context.py
│   └── patches/            # Patch system
│       ├── generator.py
│       └── applier.py
├── config/
│   └── config.example.yaml
├── tests/
├── requirements.txt
└── README.md
```

## Building from Source

### Build Extension

**Linux/macOS:**
```bash
cd officelabs/build
./build_extension.sh
```

**Windows:**
```powershell
cd officelabs\build
.\build_extension.ps1
```

**Output:** `officelabs/build/output/OfficeLabs.oxt`

### Install Extension

**Linux/macOS:**
```bash
./install.sh
```

**Windows:**
```powershell
.\install.ps1
```

**Manual Installation:**
```bash
unopkg add --shared output/OfficeLabs.oxt
```

### Uninstall Extension

```bash
unopkg remove com.officelabs.extension
```

## Testing

### Run All Tests

**OfficeLabs:**
```bash
cd officelabs
pytest
```

**Agent:**
```bash
cd officelabs-agent
pytest
```

### Run Specific Tests

```bash
# Unit tests only
pytest tests/test_indexer.py

# Integration tests
pytest tests/integration/

# With coverage
pytest --cov=officelabs --cov-report=html
```

### Test with Different LibreOffice Versions

```bash
# Set LibreOffice path
export LIBREOFFICE_PATH=/path/to/libreoffice

# Run tests
pytest
```

### Manual Testing

1. **Start Agent:**
   ```bash
   cd officelabs-agent
   python -m src.main --dev
   ```

2. **Start LibreOffice:**
   ```bash
   soffice --writer
   ```

3. **Open Kiro Assistant** (View → Sidebar)

4. **Test features:**
   - Document indexing
   - Chat functionality
   - @-addressing
   - Patch preview/apply

## Contributing

### Workflow

1. **Fork the repository**
2. **Create a feature branch:**
   ```bash
   git checkout -b feature/amazing-feature
   ```

3. **Make changes:**
   - Write code
   - Add tests
   - Update documentation

4. **Run tests:**
   ```bash
   pytest
   flake8 officelabs/
   black officelabs/
   ```

5. **Commit:**
   ```bash
   git commit -m "Add amazing feature"
   ```

6. **Push:**
   ```bash
   git push origin feature/amazing-feature
   ```

7. **Open Pull Request**

### Commit Messages

Follow [Conventional Commits](https://www.conventionalcommits.org/):

```
feat: Add @-address autocomplete
fix: Fix indexer crash on empty documents
docs: Update user guide with examples
test: Add tests for calc indexer
refactor: Simplify patch generation
```

### Pull Request Guidelines

- **Title:** Clear and descriptive
- **Description:** What, why, and how
- **Tests:** Include tests for new features
- **Documentation:** Update docs if needed
- **Changelog:** Add entry to CHANGELOG.md

## Code Style

### Python

**Follow PEP 8** with these tools:

```bash
# Format code
black officelabs/

# Check style
flake8 officelabs/

# Sort imports
isort officelabs/

# Type checking
mypy officelabs/
```

**Configuration:**
- Line length: 100
- Quote style: Double quotes
- Docstrings: Google style

### Example

```python
"""Module docstring."""

from typing import List, Optional


class DocumentIndexer:
    """
    Document indexer that builds @-addressable object maps.
    
    Args:
        document: UNO document object
        config: Optional configuration
    
    Example:
        >>> indexer = DocumentIndexer(document)
        >>> index = indexer.index()
        >>> print(len(index.objects))
        42
    """
    
    def __init__(self, document, config: Optional[Config] = None):
        """Initialize indexer."""
        self.document = document
        self.config = config or get_config()
    
    def index(self) -> DocumentIndex:
        """
        Build document index.
        
        Returns:
            DocumentIndex with all objects
        
        Raises:
            IndexError: If document cannot be indexed
        """
        # Implementation
        pass
```

## Debugging

### Debug OfficeLabs Extension

**Enable LibreOffice Python debugging:**

1. Set environment variable:
   ```bash
   export PYUNO_LOGLEVEL=DEBUG
   ```

2. Start LibreOffice from terminal:
   ```bash
   soffice --writer
   ```

3. View logs in terminal

**Or use Python debugger:**

```python
# In kiro_sidebar.py
import pdb; pdb.set_trace()
```

### Debug Agent Service

**Use FastAPI development mode:**

```bash
python -m src.main --dev
```

**Or use debugger:**

```python
# In any file
import debugpy
debugpy.listen(5678)
debugpy.wait_for_client()
```

**Then attach VS Code debugger** (launch.json):

```json
{
  "type": "python",
  "request": "attach",
  "name": "Attach to Agent",
  "connect": {
    "host": "localhost",
    "port": 5678
  }
}
```

### Debug LLM Calls

**Enable request logging:**

```python
# In config.yaml
llm:
  debug_requests: true
```

**Or set environment variable:**

```bash
export OFFICELABS_LLM_DEBUG=1
```

### Common Issues

**Issue:** Extension not loading

**Solution:**
1. Check unopkg installation: `unopkg list`
2. Verify manifest.xml is correct
3. Check LibreOffice logs: `~/.libreoffice/*/user/log/`

**Issue:** Agent connection failed

**Solution:**
1. Verify agent is running: `curl http://localhost:8765/health`
2. Check firewall settings
3. View agent logs

**Issue:** PyUNO errors

**Solution:**
1. Verify LibreOffice Python: `soffice --version`
2. Check PYTHONPATH includes UNO: `echo $PYTHONPATH`
3. Test PyUNO: `python -c "import uno"`

## Syncing with LibreOffice Upstream

### Update from Upstream

```bash
./sync-upstream.sh
```

**This script:**
1. Fetches from LibreOffice/core
2. Merges changes into officelabs-main
3. Reapplies OfficeLabs patches
4. Runs build verification

### Creating Patches

**For new features:**

1. Create feature branch:
   ```bash
   git checkout -b feature/sidebar-ui
   ```

2. Make changes and commit

3. Generate patch:
   ```bash
   git format-patch master --stdout > officelabs-patches/sidebar-ui.patch
   ```

4. Test patch application:
   ```bash
   git apply --check officelabs-patches/sidebar-ui.patch
   ```

### Rebasing Patches

**After upstream sync:**

```bash
cd officelabs-patches/
for patch in *.patch; do
    git apply --check "$patch"
    # If it fails, manually rebase
done
```

## Release Process

### Version Bumping

1. Update version in:
   - `officelabs/__init__.py`
   - `officelabs/build/description.xml`
   - `src/__init__.py` (agent)

2. Update CHANGELOG.md

3. Commit:
   ```bash
   git commit -m "chore: Bump version to 0.2.0"
   ```

### Creating Release

1. Tag release:
   ```bash
   git tag -a v0.2.0 -m "Release 0.2.0"
   git push origin v0.2.0
   ```

2. Build extension:
   ```bash
   ./build/build_extension.sh
   ```

3. Create GitHub release with `.oxt` file

## CI/CD

### GitHub Actions

**.github/workflows/test.yml:**

```yaml
name: Test

on: [push, pull_request]

jobs:
  test:
    runs-on: ${{ matrix.os }}
    strategy:
      matrix:
        os: [ubuntu-latest, macos-latest, windows-latest]
        python-version: [3.9, "3.10", "3.11"]
    
    steps:
      - uses: actions/checkout@v3
      - uses: actions/setup-python@v4
        with:
          python-version: ${{ matrix.python-version }}
      - run: pip install -r requirements.txt
      - run: pytest
```

## Resources

- [LibreOffice API](https://api.libreoffice.org/)
- [PyUNO Guide](https://wiki.documentfoundation.org/Macros/Python_Guide)
- [FastAPI Documentation](https://fastapi.tiangolo.com/)
- [Anthropic API](https://docs.anthropic.com/)
- [OpenAI API](https://platform.openai.com/docs/)

## Getting Help

- **Discussions**: https://github.com/officelabs/officelabs/discussions
- **Issues**: https://github.com/officelabs/officelabs/issues
- **Discord**: https://discord.gg/officelabs

