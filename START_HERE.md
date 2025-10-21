# 🚀 OfficeLabs Core Setup - START HERE

**Date:** October 21, 2025  
**Status:** ✅ Ready to Execute Step 2  
**Your Fork:** https://github.com/Pher217/Libreoffice-fork

---

## ✅ Step 1: Fork Complete!

Your LibreOffice fork is ready at:
### **https://github.com/Pher217/Libreoffice-fork**

Repository name: `Libreoffice-fork`

**✅ VERIFIED:** Setup matches Cursor-style AI integration architecture  
See `ARCHITECTURE_VERIFICATION.md` for detailed confirmation

---

## 🎯 What We're Doing

We're setting up a LibreOffice fork with the OfficeLabs AI integration module. This involves:

1. ✅ **Fork LibreOffice** - Done! (Libreoffice-fork)
2. ⏳ **Install dependencies** - Next step (automated)
3. ⏳ **Build LibreOffice** - After setup (1-3 hours)
4. ⏳ **Add OfficeLabs module** - Final integration

---

## 🚀 Next Step: Run Setup Script

### Option 1: From Windows PowerShell (Recommended)

Open PowerShell in this directory and run:

```powershell
wsl -d Ubuntu bash -c "cp /mnt/c/Users/philh/dev/officelabs-core/setup-officelabs-core.sh ~/ && cd ~ && chmod +x setup-officelabs-core.sh && ./setup-officelabs-core.sh"
```

### Option 2: From WSL Ubuntu Terminal

```bash
# Copy the setup script
cp /mnt/c/Users/philh/dev/officelabs-core/setup-officelabs-core.sh ~/

# Make it executable
chmod +x ~/setup-officelabs-core.sh

# Run it
~/setup-officelabs-core.sh
```

---

## ⏱️ What the Setup Script Will Do (30 minutes)

The script will automatically:

1. ✅ Check system prerequisites (RAM, disk space)
2. ✅ Install LibreOffice build dependencies (~50 packages)
3. ✅ Configure ccache (build cache - 20GB)
4. ✅ Clone your fork: `Libreoffice-fork`
5. ✅ Set up Git remotes (origin + upstream)
6. ✅ Create branches:
   - `officelabs-main` (your development branch)
   - `libreoffice-sync` (tracks upstream LibreOffice)

**Time:** 30 minutes (mostly downloading dependencies)

---

## 📋 After Setup Completes

Once the setup script finishes, you'll run:

```bash
# Navigate to the fork
cd ~/dev/officelabs-master/Libreoffice-fork

# Configure the build
./autogen.sh --with-vendor="OfficeLabs" --enable-python=system --without-java --enable-ccache

# Build LibreOffice (1-3 hours - go get coffee ☕)
make build-nocheck -j$(nproc)

# Verify it works
./instdir/program/soffice --version
```

---

## 📚 Additional Resources

- **Quick Guide:** `QUICK_START.md`
- **Detailed Checklist:** `EXECUTION_CHECKLIST.md`
- **Full Instructions:** `FORK_SETUP_INSTRUCTIONS.md`
- **Status & Updates:** `STATUS_UPDATED.md`

---

## 🎯 Success Path

```
✅ Step 1: Fork (DONE)
    ↓
⏳ Step 2: Run setup script (30 min) ← YOU ARE HERE
    ↓
⏳ Step 3: Build LibreOffice (1-3 hours)
    ↓
⏳ Step 4: Add OfficeLabs module (10 min)
    ↓
⏳ Step 5: Commit & tag (5 min)
    ↓
🎉 COMPLETE!
```

---

## 💡 Quick Tips

- **First build takes 1-3 hours** - subsequent builds are 5-30 minutes with ccache
- **You need 50GB+ disk space** - check with `df -h ~` in WSL
- **The process is mostly automated** - you can walk away during builds
- **WSL2 is required** - you have Ubuntu 24.04 LTS ✓

---

## 🆘 Need Help?

If you encounter issues:

1. Check `STATUS_UPDATED.md` for troubleshooting
2. Review `EXECUTION_CHECKLIST.md` for step-by-step guidance
3. See `PROMPT_02_COMPLETION_SUMMARY.md` for detailed explanations

---

## ✨ Ready to Go!

**Your next command:**

```powershell
wsl -d Ubuntu bash -c "cp /mnt/c/Users/philh/dev/officelabs-core/setup-officelabs-core.sh ~/ && cd ~ && chmod +x setup-officelabs-core.sh && ./setup-officelabs-core.sh"
```

This will start the automated setup process for your LibreOffice fork.

---

**Repository:** https://github.com/Pher217/Libreoffice-fork  
**Current Status:** Ready for Step 2  
**Estimated Total Time:** 2-4 hours (mostly automated)

🚀 Let's build OfficeLabs!

