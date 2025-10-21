# OfficeLabs Core - Fork Setup Instructions

**Part of PROMPT_02: LibreOffice Fork Setup**

---

## ⚠️ Important: Complete These Steps First

Before running the automated setup, you need to fork LibreOffice on GitHub.

---

## Step 1: Fork LibreOffice Repository

### Option A: Via GitHub Web Interface (Recommended)

1. **Navigate to LibreOffice Core:**
   ```
   https://github.com/LibreOffice/core
   ```

2. **Click the "Fork" button** (top right corner of the page)

3. **Configure your fork:**
   - **Owner:** Select `Pher217` (or your GitHub username)
   - **Repository name:** `officelabs-core`
   - **Description:** `LibreOffice fork with native AI integration - OfficeLabs Core`
   - **❌ UNCHECK "Copy the master branch only"**
     - This is important! We need all branches for proper upstream tracking
   - **✅ Make it public** (or private if you prefer)

4. **Click "Create fork"**

5. **Wait for the fork to complete**
   - This may take a few minutes as LibreOffice is a large repository
   - GitHub will show a progress indicator

6. **Verify the fork:**
   - You should now see the repository at:
     ```
     https://github.com/Pher217/officelabs-core
     ```

### Option B: Via GitHub CLI (If Available)

If you have `gh` CLI installed and authenticated:

```bash
# In WSL2 Ubuntu terminal
gh repo fork LibreOffice/core \
  --clone=false \
  --remote=true \
  --repo=Pher217/officelabs-core
```

---

## Step 2: Run Automated Setup

Once the fork is created on GitHub, run the setup script:

### From Windows PowerShell:

```powershell
# Make the script executable and run it in WSL
wsl -d Ubuntu bash -c "cd ~/dev && chmod +x /mnt/c/Users/philh/dev/officelabs-core/setup-officelabs-core.sh && bash /mnt/c/Users/philh/dev/officelabs-core/setup-officelabs-core.sh"
```

### From WSL2 Ubuntu Terminal:

```bash
# Navigate to the script location
cd /mnt/c/Users/philh/dev/officelabs-core

# Make it executable
chmod +x setup-officelabs-core.sh

# Run it
./setup-officelabs-core.sh
```

---

## What the Automated Setup Will Do

The `setup-officelabs-core.sh` script will:

1. ✅ Check prerequisites (RAM, disk space, WSL)
2. ✅ Install LibreOffice build dependencies (~300MB download)
3. ✅ Configure ccache for faster rebuilds
4. ✅ Create directory structure at `~/dev/officelabs-master/`
5. ✅ Clone your fork
6. ✅ Configure Git remotes (origin + upstream)
7. ✅ Create branch strategy:
   - `officelabs-main` - Our development branch
   - `libreoffice-sync` - Tracks upstream LibreOffice
8. ✅ Prepare for build

**Time Estimate:** 15-30 minutes (mostly downloading dependencies)

---

## After Setup: Building LibreOffice

Once the setup script completes, you'll need to build LibreOffice:

```bash
cd ~/dev/officelabs-master/officelabs-core

# Configure build (5-10 minutes)
./autogen.sh \
  --with-vendor="OfficeLabs" \
  --enable-python=system \
  --without-java \
  --disable-odk \
  --enable-ccache \
  --enable-debug=no

# Build (1-3 hours on first build, 5-30 minutes on subsequent builds)
make build-nocheck -j$(nproc)

# Test the build
./instdir/program/soffice --version
```

---

## Expected Disk Usage

| Component | Size |
|-----------|------|
| LibreOffice source | ~2GB |
| Build artifacts | ~20GB |
| ccache | ~20GB |
| **Total** | **~45GB** |

Make sure you have at least **50GB free** on your WSL filesystem.

---

## Troubleshooting

### Fork not showing up on GitHub

- Refresh the page
- Check: `https://github.com/Pher217/officelabs-core`
- It may take 1-2 minutes for large repositories

### Setup script fails with "permission denied"

```bash
chmod +x setup-officelabs-core.sh
```

### "Not enough disk space" error

Check available space:
```bash
df -h ~
```

Free up space or choose a different location with more space.

### WSL2 performance issues

For better performance, work in WSL's native filesystem (`~/dev/`) rather than mounted Windows drives (`/mnt/c/`).

---

## Next Steps

After completing this setup:

1. ✅ Build LibreOffice (baseline)
2. ✅ Create OfficeLabs module structure
3. ✅ Integrate with build system
4. ✅ Test module build

See `PROMPT_02_BACKEND_SETUP.md` for the next phase.

---

## Support

If you encounter issues:

1. Check the build log in `workdir/`
2. Consult LibreOffice build docs: https://wiki.documentfoundation.org/Development/BuildingOnLinux
3. Review `docs/FORK_GUIDE.md` for detailed build information

---

**Last Updated:** October 21, 2025  
**Prerequisites:** WSL2 Ubuntu, 16GB RAM, 50GB disk space  
**Estimated Time:** 2-4 hours (mostly build time)

