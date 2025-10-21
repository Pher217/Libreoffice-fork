# OfficeLabs Core Architecture

**Component Architecture and Design**

---

## Overview

LibreOffice fork with native AI integration (C++ layer)

## Purpose

LibreOffice fork with AI UI integration and IPC hooks

## Components

- `officelabs/agent/` - Agent integration layer
- `officelabs/context/` - Document context engine
- `officelabs/ipc/` - IPC communication
- `officelabs/ui/` - Native UI widgets (VCL)
- `vcl/source/ai/` - AI chat panel

## Integration

This component integrates with:

- **officelabs-core** - LibreOffice fork
- **officelabs-backend** - FastAPI backend
- **officelabs-agent** - Master agent orchestrator

## Communication

See main architecture documentation for IPC and API communication patterns.

---

For complete system architecture, see [officelabs-master](https://github.com/Pher217/officelabs-master).

*Last Updated: October 21, 2025*
