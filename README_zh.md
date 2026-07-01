# OpenA2B - 开源汽车音频总线驱动框架

[![CI](https://github.com/chenbokaix250/openA2b/actions/workflows/ci.yml/badge.svg)](https://github.com/chenbokaix250/openA2b/actions)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

OpenA2B 是一个平台无关、RTOS无关、MCU无关的 A2B（汽车音频总线）主栈，专为汽车音频应用设计。

## 特性

- **平台无关**: 支持 STM32、S32K、TC397、Linux 等多种平台
- **RTOS无关**: 不依赖特定RTOS API
- **简洁架构**: 设备中心化、ops-based 设计（Linux 内核风格）
- **Mock硬件**: 无需真实硬件即可完成CI测试
- **全面测试**: 103+ 单元测试，基于 Unity 测试框架

## 快速开始

### 前置条件

- CMake 3.15+
- C11 兼容编译器
- （可选）Doxygen 用于生成文档

### 编译

```bash
git clone https://github.com/chenbokaix250/openA2b.git
cd openA2b
mkdir build && cd build
cmake ..
make
```

### 运行测试

```bash
ctest -V
```

## 项目结构

```
OpenA2B/
├── include/          # 公共头文件 (a2b_*.h)
├── core/             # 框架核心 (a2b_*.c)
├── hal/              # 硬件抽象层
├── mock/             # Mock AD2428 用于测试
├── register/         # AD2428 寄存器定义
├── test/             # Unity 单元测试
└── docs/             # 文档
```

## 模块

| 模块 | 描述 |
|------|------|
| `a2b_discovery` | 节点发现与枚举 |
| `a2b_network` | 网络拓扑管理 |
| `a2b_audio` | 音频路由（上行/下行）|
| `a2b_slot` | TDM 时隙配置 |
| `a2b_clock` | PLL 时钟管理 |
| `a2b_irq` | 中断处理 |
| `a2b_state` | 状态机 |
| `a2b_diagnostic` | 诊断事件日志 |
| `a2b_recovery` | 故障恢复策略 |
| `a2b_remote` | 远程寄存器访问 |

## 文档

- [API 参考文档](docs/api_reference.md)
- [架构文档](docs/architecture.md)
- [设计规范](docs/superpowers/specs/2026-06-30-OpenA2B-design.md)

## 许可证

MIT 许可证 - 参见 [LICENSE](LICENSE)

## 贡献

欢迎贡献！请参见 CONTRIBUTING.md 了解指南。
