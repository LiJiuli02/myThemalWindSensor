# Validation and limitations / 验证与限制

[English overview](../README.md) · [中文概览](../README.zh-CN.md)

## Review scope / 核查范围

**EN:** Reviewed on 2026-11-04 against `0e55473dd8746897407184cd5348c411effe9203`: README, main application, drivers, model, network helpers, CMake, configuration and selected historical build metadata. The original README was an ESP-IDF sample template with a broad target list and references to files not present; its useful build-system explanation is retained in project-specific form, without repeating the unsupported multi-target claim.

**中文：** 对照上述提交核查原 README、主程序、驱动、模型、网络代码、CMake、配置及部分历史构建元数据。原 README 为通用示例模板，包含宽泛目标列表及不存在文件的描述；本次保留有用的构建系统说明，改为项目相关内容，不沿用未经验证的多平台支持声明。

**EN:** No clean ESP-IDF build, flash, network integration test or wind-tunnel calibration was performed during this documentation update. Historical binaries do not establish a reproducible current build.

**中文：** 本次未执行全新 ESP-IDF 构建、烧录、网络集成测试或风洞标定；历史二进制不能证明当前源码可复现。

## Known limitations / 已知限制

| Finding / 发现 | Consequence / 影响 |
| --- | --- |
| No calibration data or verified speed units / 缺少标定与速度单位验证 | Do not label output m/s or claim accuracy / 不应标为 m/s 或宣称精度 |
| Compass remains separate / 磁力计独立运行 | Wind direction is not north-referenced by sensor fusion / 风向尚未融合北向信息 |
| TCP connect loop blocks startup / TCP 连接阻塞启动 | Unreachable server prevents acquisition tasks starting / 服务端不可达时无法启动采集任务 |
| Failed connect sets `g_sock=-1` before closing / 连接失败时先覆盖描述符 | Original socket can leak on retries / 重试可能泄漏套接字 |
| Sender ignores partial successful writes / 未处理部分写入 | Complete payload delivery is not ensured / 无法保证完整消息发送 |
| No framing or timestamps / 无分帧和时间戳 | Receiver cannot rely on one receive call per record / 不能按每次接收划分记录 |
| ADC errors become zero / ADC 错误填零 | Averaging and model output can be biased / 影响滤波与模型 |
| Magnetometer I²C errors/readiness not fully propagated / 磁力计错误与就绪状态处理不足 | Heading validity needs explicit checking / 需显式判断有效性 |
| Password literals and password logging / 密码硬编码且输出日志 | Use test settings; redact demos and replace exposed real credentials / 使用测试设置、脱敏并替换已暴露真实凭据 |
| BSP compiler option `-Wno-error=format=-Wno-format` is unusual / 编译选项异常 | Check actual compiler diagnostics in a clean build / 全新构建时检查诊断 |
| Unpinned container and tracked build cache / 容器未锁版本且保存旧缓存 | Pin environment and use a fresh build directory / 固定环境并使用新目录 |

**EN:** The Wi-Fi initializer deletes its event group after waiting while registered handlers remain active; reconnect behavior should be reviewed and exercised. These are source-level observations, not a claim that every failure was reproduced.

**中文：** Wi-Fi 初始化等待结束后删除事件组，但事件处理器仍注册，需核查并实测重连行为。以上为源码观察，不代表所有故障都已复现。

## Proposed test plan / 建议测试计划

| Stage / 阶段 | Check / 检查 | Evidence / 证据 |
| --- | --- | --- |
| Build / 构建 | ESP-IDF 5.1.2, target, memory/partitions, warnings / 版本、目标、存储及告警 | Commit, tool versions, fresh build log / 提交、版本和全新日志 |
| Acquisition / 采集 | Known voltages, both differential signs, channel mapping, disconnected sensor / 已知电压、差分正负、通道及断线 | Reference voltages, raw codes and mV / 参考值、码值和毫伏 |
| Filtering / 滤波 | Five-sample trimming, outlier and read-error cases / 去极值、异常与读失败 | Input samples and resulting mean / 输入样本与结果 |
| Model / 模型 | Quiet baseline, directional stimuli, symmetry, low-speed threshold / 静态、方向、对称性及阈值 | Parameters, outputs and coordinate definition / 参数、结果与坐标定义 |
| Calibration / 标定 | Several known speeds/directions and temperatures / 多风速、方向及环境温度 | Raw dataset, fitted model, held-out errors / 原数据、拟合和独立验证误差 |
| Compass / 磁力计 | Rotation and magnetic calibration, error/readiness cases / 旋转、磁校准与错误状态 | Reference angles and heading logs / 参考角及日志 |
| Network / 网络 | Server absent, disconnect/reconnect, partial writes and long run / 服务缺失、断重连、部分写入及长时运行 | Credential-free logs, byte counts, resource usage / 脱敏日志、字节计数和资源占用 |

**EN:** Record board/sensor assembly, supply, reference instruments, IDF version, commit, environment, expected/observed values and error definitions. The 128 SPS ADC setting is a conversion rate, not the four-signal output rate.

**中文：** 记录板卡与传感组件、供电、参考仪器、框架版本、提交、环境、预期/实测值及误差定义。128 SPS 是 ADC 转换设置，不是完整四路结果的输出频率。

## Next development steps / 后续开发方向

**EN:** Prioritize reproducible build and error handling, then protocol framing and credential configuration, then calibration and heading fusion. Keep each improvement separate from the current capability claims.

**中文：** 建议依次完善可复现构建与错误处理、协议分帧与凭据配置、标定与方位角融合；将后续改进与当前功能明确区分。
