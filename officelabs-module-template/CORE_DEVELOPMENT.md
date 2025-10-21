# OfficeLabs Core Development Guide

Complete guide for developing the OfficeLabs C++ integration layer.

---

## 🚀 Quick Start

### Build Commands

```bash
# Full rebuild
make clean && ./autogen.sh --with-officelabs && make build-nocheck -j$(nproc)

# Rebuild only OfficeLabs module
make officelabs.build

# Rebuild specific library
make Library_officelabs_agent
make Library_officelabs_ipc
```

### Testing

```bash
# Run LibreOffice Writer
./instdir/program/soffice --writer

# Run headless (for testing)
./instdir/program/soffice --headless --version

# Check if OfficeLabs libraries are built
ls instdir/program/libofficelabs_*.so  # Linux
ls instdir/program/libofficelabs_*.dylib  # macOS
dir instdir\program\libofficelabs_*.dll  # Windows
```

---

## 📁 Project Structure

```
officelabs-core/
├── officelabs/                    # OfficeLabs module (NEW)
│   ├── Module_officelabs.mk       # Module definition
│   ├── Library_officelabs_*.mk    # Library build definitions
│   ├── agent/                     # Agent layer
│   ├── context/                   # Context engine
│   ├── ipc/                       # IPC communication
│   ├── ui/                        # UI components
│   ├── git/                       # Git integration
│   └── config/                    # Configuration
│
├── RepositoryModule_host.mk       # Module registration (MODIFIED)
├── configure.ac                   # Build config (MAY MODIFY)
└── ... (LibreOffice source)
```

---

## 🔧 Development Workflow

### 1. Make Changes

Edit files in `officelabs/` directory:

```bash
# Example: Add new agent method
vim officelabs/agent/WriterAgent.cxx
```

### 2. Rebuild Module

```bash
# Rebuild just the OfficeLabs module
make officelabs.build

# Or rebuild specific library
make Library_officelabs_agent
```

### 3. Test Changes

```bash
# Run LibreOffice
./instdir/program/soffice --writer

# Test with agent service running
# (In another terminal)
cd ~/dev/officelabs-master/officelabs-agent
python -m src.main
```

### 4. Commit Changes

```bash
git add officelabs/
git commit -m "feat(agent): add new capability to WriterAgent"
git push origin officelabs-main
```

---

## 📦 Adding New Components

### Adding a New Library

**Example: Adding `Library_officelabs_context`**

1. **Create library makefile:**

   File: `officelabs/Library_officelabs_context.mk`

   ```makefile
   $(eval $(call gb_Library_Library,officelabs_context))
   
   $(eval $(call gb_Library_add_exception_objects,officelabs_context,\
       officelabs/context/ContextEngine \
       officelabs/context/DocumentObject \
       officelabs/context/AddressResolver \
       officelabs/context/Indexer \
   ))
   
   $(eval $(call gb_Library_use_libraries,officelabs_context,\
       sal \
       cppu \
       cppuhelper \
   ))
   
   $(eval $(call gb_Library_use_externals,officelabs_context,\
       boost_headers \
   ))
   ```

2. **Register in module:**

   Edit `officelabs/Module_officelabs.mk`:

   ```makefile
   $(eval $(call gb_Module_add_targets,officelabs,\
       Library_officelabs_agent \
       Library_officelabs_context \   # <-- ADD THIS
       Library_officelabs_ipc \
   ))
   ```

3. **Create source files:**

   ```bash
   touch officelabs/context/ContextEngine.cxx
   touch officelabs/context/ContextEngine.hxx
   ```

4. **Rebuild:**

   ```bash
   make Library_officelabs_context
   ```

### Adding a New Source File

1. **Create the file:**

   ```bash
   touch officelabs/agent/NewFeature.cxx
   touch officelabs/agent/NewFeature.hxx
   ```

2. **Add to library makefile:**

   Edit `officelabs/Library_officelabs_agent.mk`:

   ```makefile
   $(eval $(call gb_Library_add_exception_objects,officelabs_agent,\
       officelabs/agent/IAgentModule \
       officelabs/agent/WriterAgent \
       officelabs/agent/NewFeature \   # <-- ADD THIS
   ))
   ```

3. **Rebuild:**

   ```bash
   make Library_officelabs_agent
   ```

---

## 🔄 Upstream Sync

### Pull Latest from LibreOffice

```bash
# Switch to sync branch
git checkout libreoffice-sync

# Fetch and merge upstream
git fetch upstream
git merge upstream/master

# Resolve conflicts (if any)
# Most conflicts will be in build files

# Test the sync
./autogen.sh
make build-nocheck

# Push sync branch
git push origin libreoffice-sync
```

### Cherry-pick Fixes to officelabs-main

```bash
# Identify commits to cherry-pick (e.g., security fixes)
git log upstream/master --oneline | head -20

# Switch to main branch
git checkout officelabs-main

# Cherry-pick specific commits
git cherry-pick <commit-hash>

# Or cherry-pick multiple commits
git cherry-pick <commit1> <commit2> <commit3>

# Test
make build-nocheck

# Push
git push origin officelabs-main
```

### Quarterly Sync Schedule

- **January:** Major upstream sync
- **April:** Security updates + feature sync
- **July:** Security updates
- **October:** Security updates + feature sync

---

## 🐛 Debugging

### Enable Debug Logging

```bash
# Configure with debug enabled
./autogen.sh --with-officelabs --enable-debug=yes

# Set log level
export OFFICELABS_DEBUG=1
export OFFICELABS_LOG_LEVEL=TRACE

# Run with debugging
./instdir/program/soffice --writer
```

### Debug with GDB

```bash
# Build with debug symbols
./autogen.sh --with-officelabs --enable-debug=yes
make build-nocheck

# Run with GDB
gdb --args ./instdir/program/soffice --writer

# GDB commands:
# break officelabs::agent::WriterAgent::sendMessage
# run
# next
# print variableName
```

### Check Library Loading

```bash
# Linux: Check loaded libraries
ldd ./instdir/program/soffice | grep officelabs

# macOS: Check loaded libraries
otool -L ./instdir/program/soffice | grep officelabs

# Verify library is built
ls -lh instdir/program/libofficelabs_*.so
```

---

## 🧪 Testing

### Unit Tests

Create tests in `officelabs/qa/unit/`:

```cpp
// officelabs/qa/unit/test_agent.cxx
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <officelabs/agent/IAgentModule.hxx>

class TestAgent : public CppUnit::TestFixture {
public:
    void testCreateAgent() {
        auto agent = officelabs::agent::createAgent(
            officelabs::agent::AgentType::WRITER
        );
        CPPUNIT_ASSERT(agent != nullptr);
    }
    
    CPPUNIT_TEST_SUITE(TestAgent);
    CPPUNIT_TEST(testCreateAgent);
    CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestAgent);
```

Run tests:

```bash
make officelabs.check
```

### Integration Tests

Test with agent service:

```bash
# Start agent service
cd ~/dev/officelabs-master/officelabs-agent
python -m src.main &

# Run LibreOffice
cd ~/dev/officelabs-master/officelabs-core
./instdir/program/soffice --writer

# Test AI features manually
```

---

## 🔍 Troubleshooting

### Build fails with "Module officelabs not found"

**Solution:**

```bash
# Clean build
make clean

# Reconfigure
./autogen.sh --with-officelabs

# Rebuild
make build-nocheck
```

### Library not loading at runtime

**Solution:**

```bash
# Check if library is built
ls instdir/program/libofficelabs_*.so

# Check library dependencies
ldd instdir/program/libofficelabs_agent.so

# Rebuild specific library
make Library_officelabs_agent
```

### Agent connection fails

**Solution:**

```bash
# Check if agent service is running
ps aux | grep officelabs-agent

# Check socket exists
ls -l /tmp/officelabs-agent.sock

# Test socket connection
nc -U /tmp/officelabs-agent.sock
# Type: {"jsonrpc":"2.0","method":"health","id":"1"}
```

### ccache not working

**Solution:**

```bash
# Clear ccache
ccache --clear

# Check ccache stats
ccache -s

# Reconfigure
./autogen.sh --enable-ccache
```

### Out of memory during build

**Solution:**

```bash
# Reduce parallel jobs
make build-nocheck -j4  # instead of -j$(nproc)

# Or disable ccache temporarily
./autogen.sh --disable-ccache

# Check memory usage
free -h
```

---

## 📊 Build Performance

### Incremental Build Times

| What Changed | Rebuild Time |
|-------------|--------------|
| Single .cxx file | 10-30 seconds |
| Library (officelabs_agent) | 1-2 minutes |
| Full module (officelabs) | 3-5 minutes |
| Full LibreOffice | 1-3 hours |

### Optimization Tips

1. **Use ccache:** Speeds up rebuilds by 10-50x
2. **Parallel builds:** Use `-j$(nproc)` or `-j8`
3. **Incremental builds:** Only rebuild changed modules
4. **Disable unused features:** `--without-java`, `--disable-odk`
5. **Use WSL2 native filesystem:** Don't build on `/mnt/c/`

---

## 🔗 Useful Commands

```bash
# Show all build targets
make help

# Show OfficeLabs-specific targets
make help | grep officelabs

# Clean only OfficeLabs module
make officelabs.clean

# Show library dependencies
make Library_officelabs_agent.showdeliverables

# Check syntax without building
make officelabs.check

# Generate compile_commands.json (for LSP)
make vim-ide-integration
```

---

## 📚 Resources

- **LibreOffice Build Guide:** https://wiki.documentfoundation.org/Development/BuildingOnLinux
- **VCL Documentation:** https://docs.libreoffice.org/vcl/html/
- **UNO API:** https://api.libreoffice.org/
- **LibreOffice Dev Docs:** https://docs.libreoffice.org/

---

## 🤝 Contributing

See main repository for contribution guidelines:
- https://github.com/Pher217/officelabs-master

---

## 🎓 Learning Resources

### LibreOffice Development

- Start with `sw/` (Writer) - simplest to understand
- Read `HACKING` file in LibreOffice root
- Join LibreOffice dev mailing list

### C++ Best Practices

- Follow LibreOffice coding style
- Use `sal::` types (e.g., `sal_Int32`, `sal_Bool`)
- Prefer `OUString` over `std::string`
- Use VCL smart pointers (`VclPtr<>`)

---

**Happy Hacking!** 🚀

*Last Updated: October 21, 2025*

