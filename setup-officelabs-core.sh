#!/bin/bash
# OfficeLabs Core Setup Script
# Run this in WSL2 Ubuntu

set -e  # Exit on error

echo "================================================="
echo "  OfficeLabs Core - LibreOffice Fork Setup"
echo "================================================="
echo ""

# Colors for output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Step 1: Check prerequisites
echo -e "${BLUE}Step 1: Checking prerequisites...${NC}"

# Check if we're in WSL
if ! grep -q Microsoft /proc/version; then
    echo -e "${RED}Error: This script must be run in WSL2${NC}"
    exit 1
fi

# Check RAM (need 16GB+ recommended)
TOTAL_RAM=$(free -g | awk '/^Mem:/{print $2}')
if [ "$TOTAL_RAM" -lt 12 ]; then
    echo -e "${YELLOW}Warning: Only ${TOTAL_RAM}GB RAM detected. 16GB+ recommended.${NC}"
    read -p "Continue anyway? (y/n) " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        exit 1
    fi
fi

# Check disk space (need 50GB+)
AVAILABLE_SPACE=$(df -BG . | tail -1 | awk '{print $4}' | sed 's/G//')
if [ "$AVAILABLE_SPACE" -lt 50 ]; then
    echo -e "${RED}Error: Need at least 50GB free space. Only ${AVAILABLE_SPACE}GB available.${NC}"
    exit 1
fi

echo -e "${GREEN}✓ Prerequisites check passed${NC}"
echo ""

# Step 2: Install system dependencies
echo -e "${BLUE}Step 2: Installing LibreOffice build dependencies...${NC}"
echo "This may take 10-20 minutes..."

sudo apt-get update

sudo apt-get install -y \
  git build-essential zip ccache \
  python3 python3-dev python3-pip \
  openjdk-11-jdk ant \
  gperf bison flex \
  libkrb5-dev libssl-dev libcups2-dev \
  libfontconfig1-dev libfreetype6-dev \
  libxrandr-dev libx11-dev libxrender-dev \
  libgl1-mesa-dev libglu1-mesa-dev \
  libjpeg-dev libpng-dev \
  autoconf automake libtool pkg-config \
  libgtk-3-dev libdbus-glib-1-dev \
  libxml2-dev libxslt1-dev \
  nasm \
  wget curl

echo -e "${GREEN}✓ System dependencies installed${NC}"
echo ""

# Step 3: Configure ccache
echo -e "${BLUE}Step 3: Configuring ccache (build cache)...${NC}"

ccache --set-config=max_size=20G
ccache --set-config=compression=true
ccache -s

echo -e "${GREEN}✓ ccache configured (20GB cache)${NC}"
echo ""

# Step 4: Create directory structure
echo -e "${BLUE}Step 4: Creating directory structure...${NC}"

mkdir -p ~/dev/officelabs-master
cd ~/dev/officelabs-master

echo -e "${GREEN}✓ Directory structure created at ~/dev/officelabs-master${NC}"
echo ""

# Step 5: Fork verification
echo -e "${GREEN}=================================================${NC}"
echo -e "${GREEN}  Fork Already Exists!${NC}"
echo -e "${GREEN}=================================================${NC}"
echo ""
echo "Your fork is at: https://github.com/Pher217/Libreoffice-fork"
echo ""
echo -e "${GREEN}Press ENTER to continue with setup...${NC}"
read

# Step 6: Clone the fork
echo -e "${BLUE}Step 6: Cloning your fork...${NC}"

if [ -d "Libreoffice-fork" ]; then
    echo -e "${YELLOW}Directory Libreoffice-fork already exists. Remove it? (y/n)${NC}"
    read -p "" -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        rm -rf Libreoffice-fork
    else
        echo "Skipping clone..."
        cd Libreoffice-fork
    fi
fi

if [ ! -d "Libreoffice-fork" ]; then
    git clone https://github.com/Pher217/Libreoffice-fork.git
    cd Libreoffice-fork
fi

echo -e "${GREEN}✓ Fork cloned${NC}"
echo ""

# Step 7: Configure remotes
echo -e "${BLUE}Step 7: Configuring Git remotes...${NC}"

git remote -v

# Add upstream if it doesn't exist
if ! git remote get-url upstream &>/dev/null; then
    git remote add upstream https://github.com/LibreOffice/core.git
    echo "Added upstream remote"
fi

git fetch upstream

echo -e "${GREEN}✓ Remotes configured${NC}"
echo ""

# Step 8: Create branch strategy
echo -e "${BLUE}Step 8: Creating branch strategy...${NC}"

# Create libreoffice-sync branch
if ! git show-ref --verify --quiet refs/heads/libreoffice-sync; then
    git checkout -b libreoffice-sync upstream/master
    git push -u origin libreoffice-sync
    echo "Created libreoffice-sync branch"
else
    echo "libreoffice-sync branch already exists"
fi

# Create officelabs-main branch
if ! git show-ref --verify --quiet refs/heads/officelabs-main; then
    git checkout -b officelabs-main
    git push -u origin officelabs-main
    echo "Created officelabs-main branch"
else
    git checkout officelabs-main
    echo "Checked out officelabs-main branch"
fi

git branch -a

echo -e "${GREEN}✓ Branch strategy created${NC}"
echo ""

# Step 9: Ready to build
echo -e "${GREEN}=================================================${NC}"
echo -e "${GREEN}  Setup Complete!${NC}"
echo -e "${GREEN}=================================================${NC}"
echo ""
echo "Next steps:"
echo ""
echo "1. Configure LibreOffice build:"
echo "   cd ~/dev/officelabs-master/Libreoffice-fork"
echo "   ./autogen.sh --with-vendor=\"OfficeLabs\" --enable-python=system --without-java --disable-odk --enable-ccache"
echo ""
echo "2. Build LibreOffice (this takes 1-3 hours):"
echo "   make build-nocheck -j\$(nproc)"
echo ""
echo "3. Test the build:"
echo "   ./instdir/program/soffice --version"
echo ""
echo "Current location: $(pwd)"
echo ""
echo -e "${YELLOW}Note: The initial build will take 1-3 hours. Subsequent builds are much faster with ccache.${NC}"

