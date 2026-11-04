# Architecture and interfaces / 系统架构与接口

[English overview](../README.md) · [中文概览](../README.zh-CN.md)

~~~mermaid
flowchart LR
    A["ADS1115 / 4 logical signals 四路逻辑信号"] --> B["adc_task / trimmed mean 去极值平均"]
    B --> C["adc_queue / depth 10"]
    C --> D["wind_task / model 模型"]
    D --> E["wind_queue / depth 10"]
    E --> F["send_task / Wi-Fi TCP"]
    G["QMC5883P"] --> H["mag_task / heading log 方位角日志"]
~~~

## Startup and tasks / 初始化与任务

**EN:** `app_main` initializes NVS, creates two queues, connects Wi-Fi and TCP, initializes both sensors, then creates four FreeRTOS tasks. If TCP cannot connect, its retry loop blocks startup before sensor initialization. Key handling is commented out; the SWITCH directory is not included in the BSP source-directory list.

**中文：** 入口先初始化 NVS、建立两个队列、连接 Wi-Fi/TCP、初始化传感器，再创建四个任务。TCP 无法连接时，重试循环会阻止程序继续初始化传感器。按键任务被注释，BSP 构建列表未包含 SWITCH 目录。

| Task / 任务 | Priority / 优先级 | Behavior / 行为 |
| --- | --- | --- |
| `adc_task` | 9 | Four signal readings, five conversions each, then 1000 ms delay / 四组信号各五次转换，随后延时 |
| `wind_task` | 7 | Block on ADC queue, calculate and enqueue result / 等待 ADC 队列并计算 |
| `send_task` | 7 | Block on result queue, format text and write socket / 等待结果并发送 |
| `mag_task` | 5 | Read heading, then 1000 ms delay / 读取方位角后延时 |

**EN:** Each task is configured with stack depth 4096 in the ESP-IDF task-creation API. Queues hold ten items and use blocking operations. Acquisition includes conversion waits and inter-channel delays, so a 1000 ms task delay does not imply an exact 1 Hz output rate.

**中文：** 每个任务在 ESP-IDF 创建接口中的栈深度参数为 4096，队列容量为十项并使用阻塞操作。采集包含转换等待和通道间延时，因此 1000 ms 延时不代表严格 1 Hz 输出。

## ADC processing / ADC 处理

**EN:** The main path uses `ADS1115_GAIN_4_096V` and 128 SPS conversion configuration. Raw signed codes are scaled by 0.125 mV/count. Five readings are collected, one maximum and one minimum removed, and the remaining three averaged. Failed reads are inserted as zero, which can bias the result. Four logical readings mix differential and single-ended channels; see the setup mapping.

**中文：** 主通路使用 ±4.096 V 满量程配置与 128 SPS 转换设置，按 0.125 mV/码换算有符号码值。五次读数去除最大和最小后，对剩余三次平均。失败读数被记为零，可能引入偏差；四组信号混合使用差分与单端通道。

## Wind calculation / 风矢量计算

Source: [wind_calcu.c](../components/BSP/WIND_CALCU/wind_calcu.c) and [wind_calcu.h](../components/BSP/WIND_CALCU/wind_calcu.h).

For each direction `d` / 对每个方向 `d`：

~~~text
S_d = ((I × (V_quiet,d − V_d) / 1000 − A) / B)^n
NS = S_N − S_S
EW = S_E − S_W
speed = sqrt(NS² + EW²)
angle = atan2(−NS, −EW) × 180 / pi
if angle < 0: angle += 360
if speed < 1: direction = −1
else: direction = angle
~~~

| Constant / 常数 | Source value / 源码值 |
| --- | --- |
| `I` / `CURRENT` | 18.6, commented as mA / 注释为毫安 |
| `A`, `B`, `n` | 0.2, 0.05, 2 |
| Quiet voltages N/S/E/W / 四向静态电压 | 2146 / 2143 / 2143 / 2144 mV |

**EN:** These are stored model parameters. Their fit, physical units and valid range are not established by a calibration dataset. `speed` is therefore documented as a model output, not a verified m/s measurement. The angle follows the signed Cartesian expression above; it is not automatically the meteorological convention of clockwise degrees from north. `−1` indicates the configured low-speed/unknown-direction case.

**中文：** 以上是模型参数，缺少数据集来验证拟合、物理单位及适用范围。因此将 `speed` 描述为模型输出，不认定为已验证的 m/s。角度遵循上述笛卡尔坐标表达式，不自动等同于从北顺时针的气象风向。`−1` 表示配置的低速/方向未知状态。

**EN:** The alternative `calculate_wind_direction` computes eight sectors with a different sign convention. It is retained but not called by the main wind task.

**中文：** 另一函数计算八方位，符号约定不同，当前主任务未调用。

## Magnetometer / 磁力计

**EN:** `qmc5883p_get_heading_deg` computes `atan2(y,x)` in degrees plus `magnetic_declination = -2` and logs it. The task does not place the result in the wind calculation or transmitted record. The stored declination is a configuration value, not a validated correction for every location.

**中文：** 方位角函数用 `atan2(y,x)` 转为角度，再加配置的 −2° 磁偏角并打印日志。结果未参与风计算或发送记录，该偏角不能视为任意地点均适用的校正值。

## TCP format / TCP 格式

~~~text
Wind Direction: 123, Speed: 4.56
~~~

**EN:** This is an illustrative formatting example. Direction is rounded to zero decimal places, speed to two; there is no unit, timestamp, delimiter or length prefix. TCP preserves byte order but not message boundaries. `write` can return a partial count; the current sender only checks for negative results. A robust receiver/protocol requires explicit framing and send-completion handling.

**中文：** 上述仅为格式示例。方向保留零位小数，速度保留两位，没有单位、时间戳、分隔符或长度前缀。TCP 保证字节顺序但不保证消息边界；写入可能只完成部分字节，当前代码只判断负返回值。可靠协议需要增加分帧和完整发送处理。

## Source map / 源码索引

| Source / 源码 | Responsibility / 职责 |
| --- | --- |
| [main/main.c](../main/main.c) | Startup, tasks, queues and payload / 启动、任务、队列和消息 |
| [ADS1115 driver](../components/BSP/ADC/ads1115.c) | I²C access, scaling and averaging / 总线、换算与平均 |
| [QMC5883P driver](../components/BSP/QMC5883P/magsensor.c) | Magnetic readings and heading / 磁场与方位角 |
| [TCP helper](../main/APP/tcp_client.c) | Socket connection/retry / 套接字连接与重试 |
| [Wi-Fi helper](../main/APP/wifi_config.c) | Station events and connection / 工作站事件与联网 |
| [MyPartitions.csv](../MyPartitions.csv) | NVS, PHY, factory app, FAT and SPIFFS layout / 存储分区 |
