# OfficeLabs Module Integration Guide

**How to integrate the OfficeLabs module into your LibreOffice fork**

---

## 📋 Overview

This guide shows you how to add the OfficeLabs AI module to your LibreOffice fork.

**Prerequisites:**
- LibreOffice fork cloned and built successfully
- Working directory: `~/dev/officelabs-master/officelabs-core`

---

## 🚀 Integration Steps

### Step 1: Copy OfficeLabs Module

```bash
# From the template directory
cd ~/dev/officelabs-master/officelabs-core

# Copy the officelabs module
cp -r /path/to/officelabs-module-template/officelabs ./

# Verify structure
ls -la officelabs/
```

Expected structure:

```
officelabs/
├── Module_officelabs.mk
├── Library_officelabs_agent.mk
├── Library_officelabs_ipc.mk
├── Package_officelabs_config.mk
├── README.md
├── agent/
│   ├── IAgentModule.hxx
│   └── .gitkeep
├── context/
│   └── .gitkeep
├── ipc/
│   ├── AgentConnection.hxx
│   └── .gitkeep
├── ui/
│   └── .gitkeep
├── git/
│   └── .gitkeep
└── config/
    └── officelabs.conf
```

---

### Step 2: Register Module in Build System

Edit `RepositoryModule_host.mk`:

```bash
vim RepositoryModule_host.mk
```

Find the module list (around line 50-150) and add `officelabs \` alphabetically:

```makefile
$(eval $(call gb_Module_add_moduledirs,libreoffice,\
    accessibility \
    android \
    animations \
    ...
    odk \
    officelabs \          # <-- ADD THIS LINE
    oox \
    openssl \
    ...
))
```

**Important:** Make sure to add the backslash `\` at the end!

---

### Step 3: (Optional) Add Configure Flag

Edit `configure.ac` to add `--with-officelabs` flag:

```bash
vim configure.ac
```

Find the section with feature flags (search for `--with-`) and add:

```autoconf
AC_ARG_WITH(officelabs,
    AS_HELP_STRING([--with-officelabs],
        [Enable OfficeLabs AI integration features.])
)

if test "$with_officelabs" = "yes"; then
    AC_DEFINE([HAVE_OFFICELABS])
    AC_SUBST([WITH_OFFICELABS],[TRUE])
fi
```

**Note:** This is optional. The module will build without this flag.

---

### Step 4: Clean Previous Build

```bash
# Clean to ensure fresh build
make clean

# Remove configuration cache
rm -f config_host.mk autogen.lastrun
```

---

### Step 5: Reconfigure Build

```bash
./autogen.sh \
  --with-vendor="OfficeLabs" \
  --enable-python=system \
  --without-java \
  --disable-odk \
  --enable-ccache \
  --enable-debug=no
```

**Expected output:** Should show no errors and complete with:

```
configure: creating ./config.status
config.status: creating config_host.mk
...
configure: Configured LibreOffice.
```

---

### Step 6: Test Module Recognition

```bash
# Check if module is recognized
make officelabs.all

# Should output:
# (No targets to build, but no errors either)
```

If you see `No rule to make target 'officelabs.all'`, the module is not registered correctly.

---

### Step 7: Build Everything

```bash
# Full build (1-3 hours on first build)
make build-nocheck -j$(nproc)

# Or just build the module (once LibreOffice is built)
make officelabs.build
```

---

### Step 8: Verify Integration

```bash
# Check if OfficeLabs libraries are built (they won't exist yet, that's OK)
ls instdir/program/libofficelabs_* 2>/dev/null || echo "Libraries not built yet (expected - no source files yet)"

# Verify LibreOffice still works
./instdir/program/soffice --version

# Should output:
# LibreOffice 7.6.x.x
# Build ID: ...
```

---

## 🧩 Next Steps: Adding Source Files

Now that the module is integrated, you can add actual C++ implementations:

### 1. Implement IAgentModule

Create `officelabs/agent/IAgentModule.cxx`:

```cpp
#include "IAgentModule.hxx"

namespace officelabs {
namespace agent {

std::shared_ptr<IAgentModule> createAgent(AgentType type) {
    // TODO: Implement factory
    return nullptr;
}

} // namespace agent
} // namespace officelabs
```

### 2. Enable Library in Module

Edit `officelabs/Module_officelabs.mk` and uncomment the libraries:

```makefile
$(eval $(call gb_Module_add_targets,officelabs,\
    Library_officelabs_agent \      # Uncomment this
    Library_officelabs_ipc \        # Uncomment this
))
```

### 3. Rebuild

```bash
make officelabs.build
```

---

## 🔍 Troubleshooting

### Error: "Module officelabs not found"

**Cause:** Module not registered in `RepositoryModule_host.mk`

**Solution:**

```bash
# Check registration
grep officelabs RepositoryModule_host.mk

# Should show: officelabs \
```

If not found, add it to the module list.

---

### Error: "Cannot find officelabs/Module_officelabs.mk"

**Cause:** Module directory not in correct location

**Solution:**

```bash
# Verify path
ls -l officelabs/Module_officelabs.mk

# Should exist at: ~/dev/officelabs-master/officelabs-core/officelabs/Module_officelabs.mk
```

---

### Error: Library build fails with undefined symbols

**Cause:** Missing source files or incorrect library dependencies

**Solution:**

```bash
# Start with empty libraries
# Comment out source files in Library_officelabs_*.mk
# Add them incrementally as you implement
```

---

### Build is very slow

**Solution:**

```bash
# Verify ccache is enabled
ccache -s

# Should show:
# cache hit rate: XX.XX %

# If not:
./autogen.sh --enable-ccache
```

---

## 📦 Module File Reference

### Module Definition

**File:** `officelabs/Module_officelabs.mk`

- Defines the module
- Lists libraries to build
- Platform-specific targets

### Library Definition

**File:** `officelabs/Library_officelabs_*.mk`

- Lists source files (.cxx)
- Specifies dependencies
- Sets include paths

### Package Definition

**File:** `officelabs/Package_officelabs_*.mk`

- Defines files to install
- Config files, resources, etc.

---

## 🎯 Validation Checklist

After integration, verify:

- [ ] `grep officelabs RepositoryModule_host.mk` shows the module
- [ ] `make officelabs.all` completes without errors
- [ ] `make build-nocheck` completes successfully
- [ ] `./instdir/program/soffice --version` works
- [ ] ccache is enabled and working

---

## 📚 Resources

- **LibreOffice Build System:** https://wiki.documentfoundation.org/Development/How_to_build
- **Module Structure:** See existing modules (e.g., `sw/`, `sc/`, `sd/`)
- **Makefile Syntax:** See `solenv/gbuild/` for build system docs

---

## 🔄 Updating the Module

To update the module after changes:

```bash
# Pull changes
git pull origin officelabs-main

# Rebuild module only
make officelabs.build

# Or full rebuild if needed
make clean
make build-nocheck
```

---

## 🚀 Ready for Development!

Your OfficeLabs module is now integrated. You can now:

1. Add C++ implementations in `officelabs/`
2. Build incrementally with `make officelabs.build`
3. Test with `./instdir/program/soffice`

See `CORE_DEVELOPMENT.md` for development workflow.

---

**Last Updated:** October 21, 2025  
**Status:** Ready for implementation

