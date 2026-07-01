# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).

## [1.0.0] - 2026-06-30

### Added

- Phase 1: Foundation
  - CMake build system
  - Platform HAL (Linux I2C/Timer)
  - Mock I2C with error injection
  - AD2428 register definitions
  - Logger system with compile-time levels
  - Mock AD2428 hardware
  - Unity test framework
  - GitHub Actions CI

- Phase 2: Core Features
  - Device and network managers
  - Discovery manager with node enumeration
  - IRQ event callbacks
  - Remote register access
  - State machine with transition validation

- Phase 3: Advanced Features
  - Audio routing manager (upstream/downstream)
  - TDM slot configuration
  - PLL clock management
  - Diagnostic event logging (circular buffer)
  - Recovery manager (node reset, link renegotiate, full discovery)
