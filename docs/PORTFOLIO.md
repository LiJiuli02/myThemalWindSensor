# Portfolio brief / 项目展示说明

[English overview](../README.md) · [中文概览](../README.zh-CN.md)

## Project summary / 项目简介

**EN:** An ESP32-S3 thermal wind-sensing prototype organized as a FreeRTOS pipeline: ADS1115 acquisition, outlier-resistant averaging, model-based vector estimation and Wi-Fi/TCP reporting. A separate QMC5883P path explores magnetic heading. The project demonstrates embedded sensor integration, concurrent task design and networked telemetry.

**中文：** 本 ESP32-S3 热式风传感原型使用 FreeRTOS 组织 ADS1115 采集、去极值平均、模型风矢量估计与 Wi-Fi/TCP 报告，并通过独立 QMC5883P 通路探索磁方位角，展示传感器集成、并发任务设计和网络遥测。

## Technical discussion / 技术讨论

- **Sensor interfaces / 传感接口：** Explain two I²C buses, differential/single-ended mapping and signed voltage conversion. / 解释双总线、差分与单端映射及有符号电压换算。
- **Concurrency / 并发：** Explain queue boundaries, task priorities, blocking behavior and startup dependencies. / 解释队列边界、优先级、阻塞和启动依赖。
- **Signal processing / 信号处理：** Explain trimmed means, quiet baselines and model calibration. / 解释去极值平均、静态基线和模型标定。
- **Communication / 通信：** Explain TCP byte streams, framing, partial writes and reconnection. / 解释字节流、分帧、部分写入和重连。
- **Scope / 当前范围：** Distinguish model estimates from calibrated wind speed and logged compass values from fused direction. / 区分模型估计与标定风速、独立方位角与融合风向。

## Demonstration / 演示

**EN:** Show the diagram and physical channel mapping. Start a local TCP observer, boot the board, compare ADC and wind logs with received text, then demonstrate the compass separately. Label speed as uncalibrated until reference data exists. Use a test network and redact credentials before recording.

**中文：** 展示架构和实际通道位置，先启动 TCP 观察程序，再上电，对比采集/风计算日志与收到的文本，单独展示磁力计。标定前将速度标注为未标定，并使用测试网络、在录制前脱敏。

## Evidence to add / 建议补充证据

**EN:** Add the thermal-element/constant-current schematic, BOM, sensor geometry and mounting photo, calibration dataset, measured update rate, clean build instructions verified on hardware, and a short video. Record course/date, team roles and individual contributions before making personal résumé claims. No ownership percentage, measured accuracy or calibrated speed range is inferred here.

**中文：** 建议补充热敏元件与恒流电路、物料清单、几何布局与照片、标定数据、实测更新率、硬件验证过的构建说明及视频。个人简历表述前应记录课程日期、分工和个人贡献；本文不推断贡献比例、实测精度或标定量程。
