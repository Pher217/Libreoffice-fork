# OfficeLabs Core - Setup Package

**Complete LibreOffice fork setup automation for OfficeLabs**

---

## 📦 What's Included

This package contains everything needed to set up the OfficeLabs LibreOffice fork:

### 🔧 Setup Scripts
- **`setup-officelabs-core.sh`** - Automated setup for WSL2 Ubuntu
  - Installs LibreOffice build dependencies
  - Configures ccache
  - Clones fork and sets up branches

### 📚 Documentation
- **`QUICK_START.md`** - Fast track guide (start here!)
- **`FORK_SETUP_INSTRUCTIONS.md`** - Detailed step-by-step instructions
- **`PROMPT_02_COMPLETION_SUMMARY.md`** - Complete task summary
- **`README_SETUP_PACKAGE.md`** - This file

### 🧩 OfficeLabs Module Template
- **`officelabs-module-template/`** - Ready-to-integrate module
  - **`officelabs/`** - Module source directory
    - **`Module_officelabs.mk`** - Build definition
    - **`Library_*.mk`** - Library makefiles
    - **`agent/`** - Agent interface headers
    - **`ipc/`** - IPC connection headers
    - **`config/`** - Configuration files
  - **`CORE_DEVELOPMENT.md`** - Development workflow guide
  - **`INTEGRATION_GUIDE.md`** - How to integrate into LibreOffice

---

## 🚀 Getting Started

### Option 1: Quick Start (Recommended)

Follow **`QUICK_START.md`** for the fastest path.

### Option 2: Detailed Guide

Follow **`FORK_SETUP_INSTRUCTIONS.md`** for comprehensive instructions.

---

## 📋 Prerequisites

Before starting:

✅ **Windows 10/11** with WSL2 installed  
✅ **WSL2 Ubuntu** distribution (you have this ✓)  
✅ **16GB+ RAM** (32GB recommended)  
✅ **50GB+ free disk space**  
✅ **GitHub account** (Pher217)  
✅ **2-4 hours** available (mostly automated build time)

---

## 🎯 Setup Process Overview

```
┌─────────────────────────────────────────────────────────────┐
│ 1. Fork LibreOffice on GitHub                       (2 min) │
│    → https://github.com/LibreOffice/core                    │
│    → Click "Fork" → Create as Pher217/officelabs-core       │
└─────────────────────────────────────────────────────────────┘
                           ↓
┌─────────────────────────────────────────────────────────────┐
│ 2. Run setup-officelabs-core.sh                   (30 min)  │
│    → Installs dependencies                                  │
│    → Clones fork                                            │
│    → Sets up Git branches                                   │
└─────────────────────────────────────────────────────────────┘
                           ↓
┌─────────────────────────────────────────────────────────────┐
│ 3. Build LibreOffice (baseline)                 (1-3 hours) │
│    → ./autogen.sh                                           │
│    → make build-nocheck -j$(nproc)                          │
└─────────────────────────────────────────────────────────────┘
                           ↓
┌─────────────────────────────────────────────────────────────┐
│ 4. Integrate OfficeLabs module                     (5 min)  │
│    → Copy officelabs/ directory                             │
│    → Edit RepositoryModule_host.mk                          │
└─────────────────────────────────────────────────────────────┘
                           ↓
┌─────────────────────────────────────────────────────────────┐
│ 5. Test build with module                        (30 min)   │
│    → make officelabs.all                                    │
│    → make build-nocheck                                     │
└─────────────────────────────────────────────────────────────┘
                           ↓
┌─────────────────────────────────────────────────────────────┐
│ 6. Commit and tag                                  (2 min)  │
│    → git commit -m "feat: add officelabs module"            │
│    → git tag -a v0.1.0-alpha.1                              │
└─────────────────────────────────────────────────────────────┘

Total Time: 2-5 hours (mostly automated)
```

---

## 📁 Directory Structure

### Current Location (Windows)
```
C:\Users\philh\dev\officelabs-core\
├── setup-officelabs-core.sh           ← Run this in WSL
├── QUICK_START.md                     ← Start here
├── FORK_SETUP_INSTRUCTIONS.md
├── PROMPT_02_COMPLETION_SUMMARY.md
├── README_SETUP_PACKAGE.md            ← You are here
└── officelabs-module-template/
    ├── officelabs/                    ← Module to integrate
    ├── CORE_DEVELOPMENT.md
    └── INTEGRATION_GUIDE.md
```

### After Setup (WSL2)
```
~/dev/officelabs-master/
└── officelabs-core/                   ← LibreOffice fork
    ├── officelabs/                    ← Integrated module
    ├── instdir/                       ← Built LibreOffice
    ├── workdir/                       ← Build artifacts
    ├── sw/                            ← Writer (LibreOffice)
    ├── sc/                            ← Calc (LibreOffice)
    └── ... (LibreOffice source)
```

---

## 🎯 What You'll Achieve

After completing this setup:

✅ LibreOffice fork building from source  
✅ OfficeLabs AI module integrated into build  
✅ Development environment ready  
✅ Git branches configured (officelabs-main, libreoffice-sync)  
✅ Build system optimized with ccache  
✅ Ready for PROMPT_03 (Backend development)

---

## 🔍 File Descriptions

### Setup Scripts

**`setup-officelabs-core.sh`**
- Comprehensive setup automation
- Checks prerequisites
- Installs all dependencies
- Configures Git
- Sets up build environment

### Documentation

**`QUICK_START.md`**
- Fastest path to success
- 5 simple steps
- Minimal explanation

**`FORK_SETUP_INSTRUCTIONS.md`**
- Detailed instructions
- Explanations for each step
- Troubleshooting tips

**`PROMPT_02_COMPLETION_SUMMARY.md`**
- Complete task breakdown
- What's automated vs manual
- Success criteria
- Troubleshooting

**`README_SETUP_PACKAGE.md`** (this file)
- Package overview
- Quick navigation
- File descriptions

### Module Template

**`officelabs-module-template/officelabs/`**

Core module files:
- `Module_officelabs.mk` - Build system integration
- `Library_officelabs_agent.mk` - Agent library
- `Library_officelabs_ipc.mk` - IPC library
- `Package_officelabs_config.mk` - Config packaging
- `agent/IAgentModule.hxx` - Agent interface
- `ipc/AgentConnection.hxx` - IPC interface
- `config/officelabs.conf` - Default config

**`officelabs-module-template/CORE_DEVELOPMENT.md`**
- Development workflow
- Build commands
- Adding components
- Debugging tips

**`officelabs-module-template/INTEGRATION_GUIDE.md`**
- How to integrate module
- Build system registration
- Testing integration
- Troubleshooting

---

## ⚡ Quick Commands

### Run Setup (from Windows PowerShell)
```powershell
wsl -d Ubuntu bash -c "cp /mnt/c/Users/philh/dev/officelabs-core/setup-officelabs-core.sh ~/ && chmod +x ~/setup-officelabs-core.sh && ~/setup-officelabs-core.sh"
```

### Check WSL Status
```powershell
wsl --list --verbose
```

### Access WSL Ubuntu
```powershell
wsl -d Ubuntu
```

### From WSL: Build LibreOffice
```bash
cd ~/dev/officelabs-master/officelabs-core
./autogen.sh --with-vendor="OfficeLabs" --enable-ccache
make build-nocheck -j$(nproc)
```

---

## 🆘 Need Help?

### Common Issues

**"WSL not found"**
- Install WSL2: `wsl --install`
- Restart computer

**"Out of disk space"**
- Need 50GB+ free
- Check: `wsl df -h ~`
- Clear space or use different location

**"Build failed"**
- Check `PROMPT_02_COMPLETION_SUMMARY.md` troubleshooting section
- Review build logs in `workdir/`

**"Module not recognized"**
- Verify `grep officelabs RepositoryModule_host.mk`
- See `INTEGRATION_GUIDE.md`

### Resources

- **LibreOffice Build Guide:** https://wiki.documentfoundation.org/Development/BuildingOnLinux
- **WSL Documentation:** https://docs.microsoft.com/en-us/windows/wsl/
- **OfficeLabs Docs:** See `docs/` directory

---

## 🎓 Learning Path

If you're new to LibreOffice development:

1. **Start simple:** Run the automated setup
2. **Build baseline:** Get LibreOffice building first
3. **Add module:** Integrate OfficeLabs step-by-step
4. **Read code:** Explore existing modules (sw/, sc/, sd/)
5. **Small changes:** Make small edits and rebuild
6. **Iterate:** Development workflow becomes natural

---

## 📊 Expected Timeline

| Phase | Time | Can Run Unattended? |
|-------|------|---------------------|
| Fork on GitHub | 2 min | No - manual step |
| Setup script | 30 min | Yes ✓ |
| First LibreOffice build | 1-3 hours | Yes ✓ |
| Integrate module | 5 min | No - requires edits |
| Test build | 30 min | Yes ✓ |
| Commit & push | 2 min | No - requires review |
| **Total** | **2-5 hours** | Mostly automated |

---

## 🚀 Next Steps

1. **Read** `QUICK_START.md`
2. **Fork** LibreOffice on GitHub
3. **Run** `setup-officelabs-core.sh`
4. **Build** LibreOffice
5. **Integrate** OfficeLabs module
6. **Celebrate** 🎉

Then proceed to **PROMPT_03: Backend Setup**

---

## 📜 License

This setup package is part of the OfficeLabs project.

OfficeLabs Core (LibreOffice fork):
- **License:** Mozilla Public License 2.0 (MPL-2.0)
- Consistent with LibreOffice licensing

---

## 🤝 Support

Part of the OfficeLabs project:
- **Main Repository:** https://github.com/Pher217/officelabs-master
- **Core Fork:** https://github.com/Pher217/officelabs-core (you'll create this)

---

**Last Updated:** October 21, 2025  
**Version:** 1.0.0  
**Status:** Ready for use ✅

---

**Happy Building!** 🚀

*Transform LibreOffice into an AI-first office suite*

