#!/bin/bash
# OfficeLabs Core - Pre-flight Check
# Run this to verify your system is ready

set -e

echo "================================================="
echo "  OfficeLabs Core - Pre-flight Check"
echo "================================================="
echo ""

# Colors
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

CHECKS_PASSED=0
CHECKS_FAILED=0

# Function to check and report
check() {
    local name="$1"
    local command="$2"
    local required="$3"
    
    echo -n "Checking $name... "
    
    if eval "$command" &>/dev/null; then
        echo -e "${GREEN}✓${NC}"
        CHECKS_PASSED=$((CHECKS_PASSED + 1))
        return 0
    else
        if [ "$required" = "required" ]; then
            echo -e "${RED}✗ (REQUIRED)${NC}"
            CHECKS_FAILED=$((CHECKS_FAILED + 1))
        else
            echo -e "${YELLOW}✗ (optional)${NC}"
        fi
        return 1
    fi
}

echo "System Requirements:"
echo "-------------------"

# Check if in WSL
if grep -q Microsoft /proc/version; then
    echo -e "WSL Environment: ${GREEN}✓${NC}"
else
    echo -e "WSL Environment: ${RED}✗ (Not in WSL)${NC}"
    exit 1
fi

# Check RAM
TOTAL_RAM=$(free -g | awk '/^Mem:/{print $2}')
echo -n "RAM: ${TOTAL_RAM}GB "
if [ "$TOTAL_RAM" -ge 16 ]; then
    echo -e "${GREEN}✓${NC}"
    CHECKS_PASSED=$((CHECKS_PASSED + 1))
elif [ "$TOTAL_RAM" -ge 12 ]; then
    echo -e "${YELLOW}⚠ (16GB+ recommended)${NC}"
    CHECKS_PASSED=$((CHECKS_PASSED + 1))
else
    echo -e "${RED}✗ (Too low - need 16GB+)${NC}"
    CHECKS_FAILED=$((CHECKS_FAILED + 1))
fi

# Check disk space
AVAILABLE_SPACE=$(df -BG ~ | tail -1 | awk '{print $4}' | sed 's/G//')
echo -n "Free disk space: ${AVAILABLE_SPACE}GB "
if [ "$AVAILABLE_SPACE" -ge 50 ]; then
    echo -e "${GREEN}✓${NC}"
    CHECKS_PASSED=$((CHECKS_PASSED + 1))
else
    echo -e "${RED}✗ (Need 50GB+)${NC}"
    CHECKS_FAILED=$((CHECKS_FAILED + 1))
fi

echo ""
echo "Build Dependencies:"
echo "-------------------"

check "git" "which git" "required"
check "make" "which make" "required"
check "gcc" "which gcc" "required"
check "g++" "which g++" "required"
check "python3" "which python3" "required"
check "java" "which java" "optional"
check "ccache" "which ccache" "optional"
check "autoconf" "which autoconf" "required"
check "pkg-config" "which pkg-config" "required"

echo ""
echo "Network Connectivity:"
echo "--------------------"

echo -n "GitHub reachable... "
if ping -c 1 github.com &>/dev/null; then
    echo -e "${GREEN}✓${NC}"
    CHECKS_PASSED=$((CHECKS_PASSED + 1))
else
    echo -e "${RED}✗${NC}"
    CHECKS_FAILED=$((CHECKS_FAILED + 1))
fi

echo ""
echo "Directory Structure:"
echo "-------------------"

echo -n "Home directory... "
if [ -d "$HOME" ]; then
    echo -e "${GREEN}✓${NC} ($HOME)"
    CHECKS_PASSED=$((CHECKS_PASSED + 1))
else
    echo -e "${RED}✗${NC}"
    CHECKS_FAILED=$((CHECKS_FAILED + 1))
fi

echo -n "~/dev directory... "
if [ -d "$HOME/dev" ]; then
    echo -e "${GREEN}✓${NC} (exists)"
    CHECKS_PASSED=$((CHECKS_PASSED + 1))
else
    echo -e "${YELLOW}⚠${NC} (will be created)"
fi

echo ""
echo "================================================="
echo "Summary:"
echo "================================================="
echo -e "Checks passed: ${GREEN}${CHECKS_PASSED}${NC}"
echo -e "Checks failed: ${RED}${CHECKS_FAILED}${NC}"
echo ""

if [ $CHECKS_FAILED -eq 0 ]; then
    echo -e "${GREEN}✅ System is ready for OfficeLabs Core setup!${NC}"
    echo ""
    echo "Next step: Run setup-officelabs-core.sh"
    exit 0
else
    echo -e "${RED}⚠️  Please address the failed checks before proceeding.${NC}"
    echo ""
    echo "To install missing dependencies:"
    echo "  sudo apt-get update"
    echo "  sudo apt-get install build-essential git autoconf pkg-config"
    exit 1
fi

