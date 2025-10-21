# OfficeLabs Core

**LibreOffice fork with native AI integration (C++ layer)**

[![License](https://img.shields.io/badge/license-MPL%202.0-blue.svg)](LICENSE)
[![Python](https://img.shields.io/badge/python-3.9+-blue.svg)](https://python.org) <!-- For Python repos -->
[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org) <!-- For C++ repos -->

---

## 🎯 Purpose

LibreOffice fork with AI UI integration and IPC hooks

---

## 📦 Key Components

- `officelabs/agent/` - Agent integration layer
- `officelabs/context/` - Document context engine
- `officelabs/ipc/` - IPC communication
- `officelabs/ui/` - Native UI widgets (VCL)
- `vcl/source/ai/` - AI chat panel

---

## 🚀 Quick Start

### Installation

```bash
# Clone repository
git clone https://github.com/Pher217/officelabs-core.git
cd officelabs-core
```

### Setup & Run

```bash
./autogen.sh --with-officelabs --enable-ai-features
make build-nocheck -j8
./instdir/program/soffice --writer
```

---

## 📚 Documentation

- **[Architecture](docs/ARCHITECTURE.md)** - Component architecture
- **[API Reference](docs/API.md)** - API documentation (if applicable)
- **[Development Guide](docs/DEVELOPMENT.md)** - Contributing guidelines

---

## 🏗️ Architecture

See [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) for detailed architecture documentation.

---

## 🤝 Contributing

This repository is part of the OfficeLabs project. See the [main repository](https://github.com/Pher217/officelabs-master) for contribution guidelines.

---

## 📜 License

Licensed under the **Mozilla Public License 2.0 (MPL-2.0)**, consistent with LibreOffice.

See [LICENSE](LICENSE) for details.

---

## 🔗 Related Repositories

- [officelabs-master](https://github.com/Pher217/officelabs-master) - Master orchestration hub
- [officelabs-core](https://github.com/Pher217/officelabs-core) - LibreOffice fork with AI
- [officelabs-backend](https://github.com/Pher217/officelabs-backend) - FastAPI backend
- [officelabs-agent](https://github.com/Pher217/officelabs-agent) - Master agent

---

**Part of the OfficeLabs Project** - AI-native office suite for everyone

*Last Updated: October 21, 2025*
