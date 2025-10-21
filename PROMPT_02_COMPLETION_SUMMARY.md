# PROMPT 02 - OfficeLabs Core Setup - Completion Summary

**Status:** Preparation Complete ✅ | Manual Steps Required ⚠️

---

## ✅ What Has Been Completed

### 1. Setup Scripts Created

- **`setup-officelabs-core.sh`** - Automated setup script for WSL2
  - Installs all LibreOffice build dependencies
  - Configures ccache for faster builds
  - Sets up Git remotes and branch strategy
  - Guides through fork and clone process

### 2. OfficeLabs Module Template Created

Complete module structure ready to integrate into LibreOffice:

```
officelabs-module-template/
├── officelabs/
│   ├── Module_officelabs.mk              ✅ Build module definition
│   ├── Library_officelabs_agent.mk       ✅ Agent library makefile
│   ├── Library_officelabs_ipc.mk         ✅ IPC library makefile
│   ├── Package_officelabs_config.mk      ✅ Config package
│   ├── README.md                         ✅ Module documentation
│   ├── agent/
│   │   ├── IAgentModule.hxx              ✅ Agent interface header
│   │   └── .gitkeep
│   ├── context/
│   │   └── .gitkeep
│   ├── ipc/
│   │   ├── AgentConnection.hxx           ✅ IPC connection header
│   │   └── .gitkeep
│   ├── ui/
│   │   └── .gitkeep
│   ├── git/
│   │   └── .gitkeep
│   └── config/
│       └── officelabs.conf               ✅ Default configuration
├── CORE_DEVELOPMENT.md                   ✅ Development guide
└── INTEGRATION_GUIDE.md                  ✅ Integration instructions
```

### 3. Documentation Created

- **`FORK_SETUP_INSTRUCTIONS.md`** - Step-by-step fork and setup guide
- **`CORE_DEVELOPMENT.md`** - Complete development workflow
- **`INTEGRATION_GUIDE.md`** - How to integrate module into LibreOffice
- **`PROMPT_02_COMPLETION_SUMMARY.md`** - This summary

---

## ⚠️ Manual Steps Required

You must complete these steps to finish PROMPT_02:

### Step 1: Fork LibreOffice on GitHub

**Why manual?** GitHub API requires authentication which is not configured.

**Instructions:**

1. Open browser and go to: https://github.com/LibreOffice/core
2. Click "Fork" button (top right)
3. Configure fork:
   - Owner: `Pher217`
   - Repository name: `officelabs-core`
   - Description: "LibreOffice fork with native AI integration - OfficeLabs Core"
   - **UNCHECK** "Copy the master branch only"
4. Click "Create fork"
5. Wait for fork to complete (~2-5 minutes)

**Verification:** Visit https://github.com/Pher217/officelabs-core

---

### Step 2: Run Setup Script in WSL2

Once the fork is created on GitHub:

**Option A: From Windows PowerShell:**

```powershell
# Copy the setup script to WSL home directory
wsl -d Ubuntu bash -c "cp /mnt/c/Users/philh/dev/officelabs-core/setup-officelabs-core.sh ~/"

# Make executable and run
wsl -d Ubuntu bash -c "cd ~ && chmod +x setup-officelabs-core.sh && ./setup-officelabs-core.sh"
```

**Option B: From WSL Ubuntu Terminal:**

```bash
# Copy script to WSL home
cp /mnt/c/Users/philh/dev/officelabs-core/setup-officelabs-core.sh ~/

# Make executable
chmod +x ~/setup-officelabs-core.sh

# Run it
cd ~
./setup-officelabs-core.sh
```

**What the script does:**
- ✅ Checks prerequisites (RAM, disk space)
- ✅ Installs LibreOffice build dependencies
- ✅ Configures ccache
- ✅ Creates directory structure
- ✅ Clones your fork
- ✅ Configures Git remotes
- ✅ Creates branch strategy

**Time:** 15-30 minutes

---

### Step 3: Build LibreOffice (Baseline)

After the setup script completes:

```bash
cd ~/dev/officelabs-master/officelabs-core

# Configure build
./autogen.sh \
  --with-vendor="OfficeLabs" \
  --enable-python=system \
  --without-java \
  --disable-odk \
  --enable-ccache \
  --enable-debug=no

# Build (this takes 1-3 hours on first build)
make build-nocheck -j$(nproc)

# Verify build
./instdir/program/soffice --version
```

**Expected output:**
```
LibreOffice 7.6.x.x
Build ID: <build-id>
```

**Time:** 1-3 hours (first build), 5-30 minutes (subsequent builds)

---

### Step 4: Integrate OfficeLabs Module

Once LibreOffice builds successfully:

```bash
cd ~/dev/officelabs-master/officelabs-core

# Copy module template from Windows directory
cp -r /mnt/c/Users/philh/dev/officelabs-core/officelabs-module-template/officelabs ./

# Edit RepositoryModule_host.mk to register the module
vim RepositoryModule_host.mk
```

Add `officelabs \` to the module list (alphabetically after `odk`):

```makefile
$(eval $(call gb_Module_add_moduledirs,libreoffice,\
    ...
    odk \
    officelabs \     # <-- ADD THIS
    oox \
    ...
))
```

Save and exit.

---

### Step 5: Test Module Integration

```bash
# Reconfigure to recognize the module
./autogen.sh \
  --with-vendor="OfficeLabs" \
  --enable-python=system \
  --without-java \
  --disable-odk \
  --enable-ccache

# Test module recognition
make officelabs.all

# Should complete without errors
```

---

### Step 6: Rebuild with Module

```bash
# Clean previous build
make clean

# Full rebuild with module
make build-nocheck -j$(nproc)

# Verify LibreOffice still works
./instdir/program/soffice --version
```

**Time:** 30-60 minutes (with ccache)

---

### Step 7: Commit Initial Setup

```bash
cd ~/dev/officelabs-master/officelabs-core

# Check status
git status

# Stage changes
git add officelabs/
git add RepositoryModule_host.mk

# Commit
git commit -m "feat: add officelabs module structure

- Created officelabs/ module directory
- Added Module_officelabs.mk build definition
- Registered module in build system
- Added agent and IPC interfaces
- Added development documentation

This establishes the foundation for AI integration."

# Push to fork
git push origin officelabs-main

# Tag release
git tag -a v0.1.0-alpha.1 -m "Initial OfficeLabs module structure"
git push origin v0.1.0-alpha.1
```

---

## 📋 Completion Checklist

Mark each item as you complete it:

### Prerequisites
- [ ] WSL2 Ubuntu installed and running
- [ ] 16GB+ RAM available
- [ ] 50GB+ free disk space

### GitHub Fork
- [ ] LibreOffice forked to Pher217/officelabs-core
- [ ] Fork visible at https://github.com/Pher217/officelabs-core

### Environment Setup
- [ ] Setup script executed successfully
- [ ] Build dependencies installed
- [ ] ccache configured
- [ ] Fork cloned to `~/dev/officelabs-master/officelabs-core`
- [ ] Git remotes configured (origin + upstream)
- [ ] Branches created (officelabs-main, libreoffice-sync)

### Baseline Build
- [ ] `./autogen.sh` completed successfully
- [ ] `make build-nocheck` completed without errors
- [ ] `./instdir/program/soffice --version` works

### Module Integration
- [ ] officelabs/ directory copied
- [ ] RepositoryModule_host.mk updated
- [ ] `make officelabs.all` works
- [ ] Full rebuild with module succeeds

### Documentation
- [ ] CORE_DEVELOPMENT.md reviewed
- [ ] INTEGRATION_GUIDE.md reviewed
- [ ] Build commands understood

### Git
- [ ] Initial setup committed
- [ ] Changes pushed to origin/officelabs-main
- [ ] Tagged as v0.1.0-alpha.1

---

## 📊 System Resources Used

After completion, expect:

| Component | Size |
|-----------|------|
| LibreOffice source | ~2GB |
| Build artifacts | ~20GB |
| ccache | ~20GB |
| **Total** | **~45GB** |

---

## 🎯 Success Criteria

**You have successfully completed PROMPT_02 when:**

1. ✅ LibreOffice builds from your fork
2. ✅ OfficeLabs module is integrated into build system
3. ✅ `make officelabs.all` command works
4. ✅ Changes committed to `officelabs-main` branch
5. ✅ `./instdir/program/soffice --version` outputs OfficeLabs vendor

---

## 🚀 Next Steps

After completing PROMPT_02, proceed to:

**PROMPT_03: Backend Setup**
- Set up FastAPI backend
- Create agent infrastructure
- Implement IPC server
- Test C++ ↔ Python communication

---

## 🆘 Support & Troubleshooting

### Getting Help

If you encounter issues:

1. **Check logs:** Look in `workdir/` for build logs
2. **Review documentation:** See `INTEGRATION_GUIDE.md`
3. **LibreOffice build docs:** https://wiki.documentfoundation.org/Development/BuildingOnLinux
4. **Check disk space:** `df -h ~`
5. **Verify ccache:** `ccache -s`

### Common Issues

**Issue:** Out of disk space

**Solution:**
```bash
# Clean build artifacts
make clean

# Clear ccache
ccache --clear
```

**Issue:** Build fails with missing dependencies

**Solution:**
```bash
# Re-run dependency installation
sudo apt-get update
sudo apt-get install -y <missing-package>
```

**Issue:** Module not recognized

**Solution:**
```bash
# Verify registration
grep officelabs RepositoryModule_host.mk

# Should output: officelabs \
```

---

## 📁 File Locations

### Windows (Current Directory)
```
C:\Users\philh\dev\officelabs-core\
├── setup-officelabs-core.sh
├── FORK_SETUP_INSTRUCTIONS.md
├── PROMPT_02_COMPLETION_SUMMARY.md
└── officelabs-module-template/
```

### WSL2 (After Setup)
```
~/dev/officelabs-master/
└── officelabs-core/          # LibreOffice fork
    ├── officelabs/           # OfficeLabs module
    ├── instdir/              # Built LibreOffice
    └── workdir/              # Build artifacts
```

---

## ⏱️ Time Estimates

| Task | Time |
|------|------|
| Fork on GitHub | 5 minutes |
| Run setup script | 15-30 minutes |
| First LibreOffice build | 1-3 hours |
| Integrate module | 10 minutes |
| Test rebuild | 30-60 minutes |
| **Total** | **2-5 hours** |

**Most time is spent on the initial build.** Subsequent builds are much faster with ccache.

---

## 🎉 Conclusion

You now have:

- ✅ Complete setup infrastructure
- ✅ Automated installation script
- ✅ OfficeLabs module template
- ✅ Comprehensive documentation
- ✅ Clear next steps

**Follow the manual steps above to complete the setup!**

---

**Last Updated:** October 21, 2025  
**Status:** Ready for execution  
**Next:** Fork on GitHub → Run setup script → Build LibreOffice

