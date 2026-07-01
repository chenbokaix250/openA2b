# OpenA2B - Open Automotive Audio Bus Driver Framework

[![CI](https://github.com/open-a2b/open-a2b/actions/workflows/ci.yml/badge.svg)](https://github.com/open-a2b/open-a2b/actions)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

OpenA2B is a platform-independent, RTOS-independent, MCU-independent A2B (Automotive Audio Bus) master stack designed for automotive audio applications.

## Features

- **Platform Independent**: Runs on STM32, S32K, TC397, Linux, and more
- **RTOS Independent**: No dependency on specific RTOS APIs
- **Clean Architecture**: Device-centric, ops-based design (Linux Kernel style)
- **Mock Hardware**: Full CI testing without target hardware
- **Comprehensive Tests**: 103+ unit tests with Unity framework

## Quick Start

### Prerequisites

- CMake 3.15+
- C11 compatible compiler
- (Optional) Doxygen for documentation

### Build

```bash
git clone https://github.com/open-a2b/open-a2b.git
cd open-a2b
mkdir build && cd build
cmake ..
make
```

### Run Tests

```bash
ctest -V
```

## Project Structure

```
OpenA2B/
├── include/          # Public headers (a2b_*.h)
├── core/             # Framework core (a2b_*.c)
├── hal/              # Hardware abstraction layer
├── mock/             # Mock AD2428 for testing
├── register/         # AD2428 register definitions
├── test/             # Unity unit tests
└── docs/             # Documentation
```

## Modules

| Module | Description |
|--------|-------------|
| `a2b_discovery` | Node discovery and enumeration |
| `a2b_network` | Network topology management |
| `a2b_audio` | Audio routing (upstream/downstream) |
| `a2b_slot` | TDM slot configuration |
| `a2b_clock` | PLL clock management |
| `a2b_irq` | Interrupt handling |
| `a2b_state` | State machine |
| `a2b_diagnostic` | Diagnostic event logging |
| `a2b_recovery` | Fault recovery strategies |
| `a2b_remote` | Remote register access |

## Documentation

- [API Reference](docs/api_reference.md)
- [Architecture](docs/architecture.md)
- [Design Specification](docs/superpowers/specs/2026-06-30-OpenA2B-design.md)

## License

MIT License - see [LICENSE](LICENSE)

## Contributing

Contributions welcome! Please see CONTRIBUTING.md for guidelines.
