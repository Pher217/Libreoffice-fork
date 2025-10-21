# OfficeLabs Core Setup - Execution Checklist

**Print this page and check off each step as you complete it**

---

## ✅ Pre-Execution Checklist

Before starting, verify:

- [ ] Windows 10/11 with WSL2 installed
- [ ] Ubuntu 24.04 running in WSL2
- [ ] 16GB+ RAM available  
- [ ] 50GB+ free disk space
- [ ] GitHub account (Pher217) ready
- [ ] 2-4 hours available
- [ ] Read `QUICK_START.md`

---

## 📋 Execution Steps

### Phase 1: Fork LibreOffice (5 minutes)

- [ ] Open https://github.com/LibreOffice/core in browser
- [ ] Click "Fork" button (top right)
- [ ] Set owner to: `Pher217`
- [ ] Set repository name to: `officelabs-core`
- [ ] Set description to: "LibreOffice fork with native AI integration - OfficeLabs Core"
- [ ] **UNCHECK** "Copy the master branch only" ⚠️ Important!
- [ ] Click "Create fork"
- [ ] Wait for fork to complete (~2-5 minutes)
- [ ] Verify fork exists at: https://github.com/Pher217/officelabs-core
- [ ] Note the time: _____________

---

### Phase 2: Run Setup Script (30 minutes)

- [ ] Open PowerShell in Windows
- [ ] Navigate to: `cd C:\Users\philh\dev\officelabs-core`
- [ ] Run command:
  ```powershell
  wsl -d Ubuntu bash -c "cp /mnt/c/Users/philh/dev/officelabs-core/setup-officelabs-core.sh ~/ && cd ~ && chmod +x setup-officelabs-core.sh && ./setup-officelabs-core.sh"
  ```
- [ ] Script starts running
- [ ] Press ENTER when prompted (after fork is verified)
- [ ] Wait for script to complete
- [ ] Verify output shows: "Setup Complete!"
- [ ] Note the time: _____________

**Expected Duration:** 15-30 minutes

---

### Phase 3: Configure Build (10 minutes)

- [ ] Open WSL Ubuntu terminal (or continue in PowerShell with `wsl`)
- [ ] Navigate to core directory:
  ```bash
  cd ~/dev/officelabs-master/officelabs-core
  ```
- [ ] Run autogen:
  ```bash
  ./autogen.sh --with-vendor="OfficeLabs" --enable-python=system --without-java --disable-odk --enable-ccache --enable-debug=no
  ```
- [ ] Wait for configuration to complete
- [ ] Verify output shows: "Configured LibreOffice"
- [ ] Note the time: _____________

**Expected Duration:** 5-10 minutes

---

### Phase 4: Build LibreOffice Baseline (1-3 hours)

⏰ **This is the longest step - can run unattended**

- [ ] Start the build:
  ```bash
  make build-nocheck -j$(nproc)
  ```
- [ ] Note start time: _____________
- [ ] ☕ Take a break! This will run for 1-3 hours
- [ ] (Optional) Monitor progress in another terminal:
  ```bash
  watch -n 10 'ccache -s'
  ```
- [ ] Wait for build to complete
- [ ] Build completes successfully (no fatal errors)
- [ ] Note end time: _____________
- [ ] Build duration: _____________ (for reference)

**Expected Duration:** 1-3 hours

---

### Phase 5: Verify Baseline Build (2 minutes)

- [ ] Test LibreOffice runs:
  ```bash
  ./instdir/program/soffice --version
  ```
- [ ] Output shows LibreOffice version
- [ ] Output shows OfficeLabs vendor
- [ ] No error messages
- [ ] Note the time: _____________

**Expected Output:**
```
LibreOffice 7.6.x.x
Build ID: ...
OfficeLabs
```

---

### Phase 6: Integrate OfficeLabs Module (10 minutes)

- [ ] Copy module template:
  ```bash
  cp -r /mnt/c/Users/philh/dev/officelabs-core/officelabs-module-template/officelabs ./
  ```
- [ ] Verify module copied:
  ```bash
  ls -la officelabs/
  ```
- [ ] Edit build configuration:
  ```bash
  vim RepositoryModule_host.mk
  ```
- [ ] Find the module list (around line 50-150)
- [ ] Add `officelabs \` after the `odk \` line
- [ ] Save and exit (`:wq` in vim)
- [ ] Verify registration:
  ```bash
  grep officelabs RepositoryModule_host.mk
  ```
- [ ] Output shows: `officelabs \`
- [ ] Note the time: _____________

**Expected Duration:** 5-10 minutes

---

### Phase 7: Reconfigure with Module (5 minutes)

- [ ] Reconfigure build:
  ```bash
  ./autogen.sh --with-vendor="OfficeLabs" --enable-python=system --without-java --disable-odk --enable-ccache
  ```
- [ ] Configuration completes successfully
- [ ] Test module recognition:
  ```bash
  make officelabs.all
  ```
- [ ] Command completes without errors
- [ ] Note the time: _____________

**Expected Duration:** 5 minutes

---

### Phase 8: Rebuild with Module (30-60 minutes)

- [ ] Clean previous build:
  ```bash
  make clean
  ```
- [ ] Rebuild everything:
  ```bash
  make build-nocheck -j$(nproc)
  ```
- [ ] Note start time: _____________
- [ ] Wait for build to complete (faster than first build due to ccache)
- [ ] Build completes successfully
- [ ] Note end time: _____________
- [ ] Rebuild duration: _____________

**Expected Duration:** 30-60 minutes (thanks to ccache)

---

### Phase 9: Verify Module Integration (2 minutes)

- [ ] Test LibreOffice still works:
  ```bash
  ./instdir/program/soffice --version
  ```
- [ ] Verify OfficeLabs libraries (they won't exist yet, but check):
  ```bash
  ls instdir/program/libofficelabs_* 2>/dev/null || echo "No libraries yet (expected)"
  ```
- [ ] Module is recognized but no libraries built yet ✓
- [ ] Note the time: _____________

---

### Phase 10: Review Documentation (10 minutes)

- [ ] Read `officelabs-module-template/CORE_DEVELOPMENT.md`
- [ ] Review build commands
- [ ] Understand development workflow
- [ ] Bookmark for future reference
- [ ] Note the time: _____________

---

### Phase 11: Commit Initial Setup (5 minutes)

- [ ] Check status:
  ```bash
  git status
  ```
- [ ] Stage changes:
  ```bash
  git add officelabs/
  git add RepositoryModule_host.mk
  ```
- [ ] Review changes:
  ```bash
  git diff --cached
  ```
- [ ] Commit:
  ```bash
  git commit -m "feat: add officelabs module structure

- Created officelabs/ module directory
- Added Module_officelabs.mk build definition
- Registered module in build system
- Added agent and IPC interfaces
- Added development documentation

This establishes the foundation for AI integration."
  ```
- [ ] Push to fork:
  ```bash
  git push origin officelabs-main
  ```
- [ ] Tag release:
  ```bash
  git tag -a v0.1.0-alpha.1 -m "Initial OfficeLabs module structure"
  git push origin v0.1.0-alpha.1
  ```
- [ ] Verify on GitHub: https://github.com/Pher217/officelabs-core
- [ ] Note the time: _____________

---

## 🎉 Completion Verification

### Final Checks

- [ ] LibreOffice builds successfully
- [ ] `make officelabs.all` works
- [ ] Changes committed to `officelabs-main`
- [ ] Tagged as `v0.1.0-alpha.1`
- [ ] Fork visible on GitHub
- [ ] Development documentation reviewed

### Success Criteria

- [ ] `./instdir/program/soffice --version` works
- [ ] Shows "OfficeLabs" as vendor
- [ ] `grep officelabs RepositoryModule_host.mk` shows module
- [ ] Git history shows commit
- [ ] Tag exists on remote

---

## 📊 Time Tracking

| Phase | Estimated | Actual | Notes |
|-------|-----------|--------|-------|
| Fork on GitHub | 5 min | _____ | ________________________ |
| Setup script | 30 min | _____ | ________________________ |
| Configure | 10 min | _____ | ________________________ |
| First build | 1-3 hrs | _____ | ________________________ |
| Verify | 2 min | _____ | ________________________ |
| Integrate module | 10 min | _____ | ________________________ |
| Reconfigure | 5 min | _____ | ________________________ |
| Rebuild | 30-60 min | _____ | ________________________ |
| Verify module | 2 min | _____ | ________________________ |
| Review docs | 10 min | _____ | ________________________ |
| Commit & tag | 5 min | _____ | ________________________ |
| **Total** | **2-5 hrs** | **_____** | ________________________ |

---

## 📝 Notes & Issues

Use this space to record any issues or notes during execution:

```
___________________________________________________________________________

___________________________________________________________________________

___________________________________________________________________________

___________________________________________________________________________

___________________________________________________________________________

___________________________________________________________________________
```

---

## 🆘 Troubleshooting Quick Reference

| Issue | Solution | Reference |
|-------|----------|-----------|
| Out of disk space | Clear ccache: `ccache --clear` | `PROMPT_02_COMPLETION_SUMMARY.md` |
| Build fails | Check logs in `workdir/` | `INTEGRATION_GUIDE.md` |
| Module not found | Verify `RepositoryModule_host.mk` | `INTEGRATION_GUIDE.md` Step 2 |
| Dependencies missing | Re-run setup script | `FORK_SETUP_INSTRUCTIONS.md` |
| ccache not working | Run `ccache -s` to check | `CORE_DEVELOPMENT.md` |

---

## 🎯 What's Next

After completing all checkboxes:

- [ ] Proceed to **PROMPT_03: Backend Setup**
- [ ] Set up FastAPI agent service
- [ ] Implement IPC communication
- [ ] Test C++ ↔ Python integration

---

## ✅ Checklist Complete!

Date completed: _______________  
Total time: _______________  
Ready for PROMPT_03: [ ]

---

**Congratulations! OfficeLabs Core foundation is ready! 🎉**

*Keep this checklist for reference and proceed to backend development.*

