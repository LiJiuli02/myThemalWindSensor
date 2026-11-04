# 热式风传感器

[English](README.md) · [搭建指南](docs/GETTING_STARTED.md) · [系统架构](docs/ARCHITECTURE.md) · [验证与限制](docs/VALIDATION.md) · [项目展示](docs/PORTFOLIO.md)

本课程项目是基于 ESP32-S3 的热式风传感原型。固件通过 ADS1115 采集四组传感信号，使用配置的热模型估计风矢量，并通过 Wi-Fi/TCP 发送文本结果。QMC5883P 磁力计在独立任务中运行。

仓库保留原名 `myThemalWindSensor`，ESP-IDF 工程名为 `HotWireV1.0`。

**状态：** 未完成原型，与原提交说明一致。上传内容没有验证风速物理单位所需的标定数据集，磁力计方位角也 **尚未融合到发送的风向中**。原通用 ESP-IDF 示例 README 已替换为针对项目本身的文档。

## 源码中的功能

- 混合差分/单端通道的 ADS1115 采集。
- 每组信号读取五次，去除最大和最小值后求平均。
- 通过 ADC 队列和风结果队列连接 FreeRTOS 实时操作系统任务。
- 连续角度与八方位计算函数；主程序使用连续角度版本。
- QMC5883P 方位角计算及配置的磁偏角修正。
- Wi-Fi 工作站连接与 TCP 客户端文本发送。

## 项目概览

| 项目 | 源码配置 |
| --- | --- |
| 目标芯片 | ESP32-S3 |
| 记录的框架 | ESP-IDF 5.1.2 |
| 存储配置 | 16 MB Flash，启用八线 PSRAM |
| ADC | ADS1115，I²C0，地址 `0x48`，SDA GPIO12，SCL GPIO13 |
| 磁力计 | QMC5883P，I²C1，地址 `0x2C`，SDA GPIO7，SCL GPIO6 |
| I²C 频率 | 两条总线均为 100 kHz |
| 网络 | 主动连接 IPv4 TCP 服务端，配置端口为 8080 |
| 消息内容 | `Wind Direction: %.0f, Speed: %.2f`，不含换行分隔符 |

## 文档导航

- [搭建指南](docs/GETTING_STARTED.md)：环境、传感通道、网络配置与构建流程。
- [系统架构](docs/ARCHITECTURE.md)：任务、队列、模型公式和 TCP 格式。
- [验证与限制](docs/VALIDATION.md)：标定、恢复行为和已知问题。
- [项目展示](docs/PORTFOLIO.md)：项目介绍与演示流程。
- [贡献指南](CONTRIBUTING.md)：证据、双语维护与来源说明。

## 仓库结构

| 路径 | 内容 |
| --- | --- |
| `main/main.c` | 初始化、采集、计算、方位角与发送任务 |
| `main/APP/` | Wi-Fi 和 TCP 辅助代码 |
| `components/BSP/ADC/` | ADS1115 驱动 |
| `components/BSP/QMC5883P/` | 磁力计驱动 |
| `components/BSP/WIND_CALCU/` | 风模型与常数 |
| `components/BSP/KEY/`、`components/BSP/SWITCH/` | 辅助模块，不是主程序当前启用的功能 |
| `sdkconfig`、`MyPartitions.csv` | 目标、存储与分区配置 |
| `build/` | 历史生成文件，应使用独立目录构建 |
| `docs/` | 中英双语工程与展示文档 |

请先阅读[搭建指南](docs/GETTING_STARTED.md)。启动时必须连接到服务端后，程序才会继续初始化传感器。

Wi-Fi 辅助代码含硬编码凭据并打印密码。请使用临时测试网络配置，避免发布含凭据的日志，并在公开演示前处理此问题；本文档不重复任何凭据值。

核查版本没有仓库级许可证文件。Wi-Fi 文件头注明正点原子（ALIENTEK）及其版权声明，本次更新不改变其许可。

文档于 **2026-11-04** 对照提交 `0e55473` 编写。
