# OfficeLabs Core - Quick Start Guide

**Get LibreOffice fork building in under 10 minutes (plus build time)**

---

## 🚀 Fast Track Setup

### Step 1: Fork LibreOffice (2 minutes)

1. Open: https://github.com/LibreOffice/core
2. Click "Fork"
3. Settings:
   - Owner: `Pher217`
   - Name: `officelabs-core`
   - **UNCHECK** "Copy the master branch only"
4. Click "Create fork"

---

### Step 2: Run Setup (30 minutes)

**From Windows PowerShell** (this directory):

```powershell
# Copy script to WSL and run
wsl -d Ubuntu bash -c "cp /mnt/c/Users/philh/dev/officelabs-core/setup-officelabs-core.sh ~/ && chmod +x ~/setup-officelabs-core.sh && ~/setup-officelabs-core.sh"
```

**Or from WSL Ubuntu:**

```bash
cp /mnt/c/Users/philh/dev/officelabs-core/setup-officelabs-core.sh ~/
chmod +x ~/setup-officelabs-core.sh
~/setup-officelabs-core.sh
```

The script will:
- Install dependencies
- Clone your fork
- Set up branches
- Prepare for build

---

### Step 3: Build LibreOffice (1-3 hours)

```bash
cd ~/dev/officelabs-master/officelabs-core

# Configure
./autogen.sh --with-vendor="OfficeLabs" --enable-python=system --without-java --enable-ccache

# Build (go get coffee ☕)
make build-nocheck -j$(nproc)

# Test
./instdir/program/soffice --version
```

---

### Step 4: Add OfficeLabs Module (5 minutes)

```bash
# Copy module
cp -r /mnt/c/Users/philh/dev/officelabs-core/officelabs-module-template/officelabs ./

# Register module
vim RepositoryModule_host.mk
# Add: officelabs \ (after odk line)

# Rebuild
make officelabs.all
```

---

### Step 5: Commit & Tag

```bash
git add officelabs/ RepositoryModule_host.mk
git commit -m "feat: add officelabs module structure"
git push origin officelabs-main
git tag -a v0.1.0-alpha.1 -m "Initial OfficeLabs module"
git push origin v0.1.0-alpha.1
```

---

## ✅ Done!

You now have:
- LibreOffice fork building
- OfficeLabs module integrated
- Ready for PROMPT_03 (Backend)

---

## 📚 More Details

- **Full instructions:** `FORK_SETUP_INSTRUCTIONS.md`
- **Development guide:** `officelabs-module-template/CORE_DEVELOPMENT.md`
- **Integration guide:** `officelabs-module-template/INTEGRATION_GUIDE.md`
- **Completion checklist:** `PROMPT_02_COMPLETION_SUMMARY.md`

---

**Estimated Total Time:** 2-4 hours (mostly automated builds)

