# Getting started / 搭建指南

[English overview](../README.md) · [中文概览](../README.zh-CN.md)

## Environment / 开发环境

**EN:** Use ESP-IDF **5.1.2**, recorded in `sdkconfig` and historical build metadata, as the reproduction baseline. Install its toolchain and open an ESP-IDF terminal. The existing `.devcontainer/Dockerfile` uses an unpinned `espressif/idf` image, so it does not guarantee this version. Keep the clone in a path without spaces, for example `C:/src/myThemalWindSensor`.

**中文：** 以配置和历史构建元数据中的 ESP-IDF **5.1.2** 为复现基准，安装对应工具链并打开 ESP-IDF 终端。现有容器镜像未锁定版本，不能保证使用 5.1.2。建议克隆到不含空格的路径，例如上述目录。

Official setup / 官方安装：[ESP-IDF 5.1.2](https://docs.espressif.com/projects/esp-idf/en/v5.1.2/esp32s3/get-started/index.html), [Windows workflow](https://docs.espressif.com/projects/esp-idf/en/v5.1.2/esp32s3/get-started/windows-setup.html).

**EN:** Confirm the board matches the ESP32-S3 target, 16 MB flash, octal PSRAM configuration and pin assignments. The hardware model, thermal sensor geometry, constant-current circuit and complete schematic/BOM are not included. Obtain those before powering the sensor assembly. Configured ADC full-scale range does not establish the permissible electrical input voltage.

**中文：** 确认板卡符合 ESP32-S3、16 MB Flash、八线 PSRAM 和引脚配置。仓库未给出完整板卡型号、热敏结构、恒流电路、原理图或物料清单，通电前需取得这些信息。ADC 配置的满量程并不等于实际允许输入电压。

## Sensor wiring / 传感器连接

| Device / 器件 | Bus / 总线 | Address / 地址 | SDA | SCL |
| --- | --- | --- | --- | --- |
| ADS1115 | I²C0, 100 kHz | `0x48` | GPIO12 | GPIO13 |
| QMC5883P | I²C1, 100 kHz | `0x2C` | GPIO7 | GPIO6 |

Confirm power, logic levels, ground and pull-ups using the actual modules. / 根据实际模块确认供电、电平、共地和上拉。

The main application maps readings as follows; these are logical names, not proof of the physical sensor orientation.

主程序按以下方式映射信号；这些是逻辑命名，不能替代实际安装方向。

| Stored reading / 读数 | ADS1115 selection / 通道选择 | Model input / 模型输入 |
| --- | --- | --- |
| `raw1` | AIN2 − AIN3, differential / 差分 | North / 北 |
| `raw2` | AIN3 to GND / 单端 | East / 东 |
| `raw3` | AIN0 − AIN1, differential / 差分 | South / 南 |
| `raw4` | AIN1 to GND / 单端 | West / 西 |

## Network configuration / 网络配置

**EN:** Set `DEFAULT_SSID` and `DEFAULT_PWD` in `main/APP/wifi_config.c` locally for a disposable test network. Set `IP_ADDR` in `main/APP/tcp_client.c` to your computer's reachable LAN address and confirm `LWIP_DEMO_PORT`, currently 8080. Do not copy existing credential values into documentation or commits. The source currently logs passwords; remove/redact that behavior before recording or sharing a demo, and replace any real exposed credentials.

**中文：** 在本地将 Wi-Fi 文件中的名称和密码改为临时测试网络配置，将 TCP 文件的 `IP_ADDR` 改为电脑可达的局域网地址，确认当前端口 8080。不要将现有凭据复制到文档或提交。源码会打印密码，录制或分享演示前需移除/脱敏该行为，并替换已暴露的真实凭据。

**EN:** Start a TCP listener on the computer before booting the device. Permit the chosen port through the local firewall for the test network. Initialization waits for a successful TCP connection before starting ADC/magnetometer setup and tasks.

**中文：** 上电前先在电脑启动 TCP 监听，并为测试网络允许相应端口。程序只有成功建立 TCP 连接后才会继续传感器和任务初始化。

A minimal Python 3 observer / 最小 Python 3 观察程序：

~~~python
import socket

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server:
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server.bind(("0.0.0.0", 8080))
    server.listen(1)
    while True:
        conn, address = server.accept()
        print("Connected:", address)
        with conn:
            while True:
                chunk = conn.recv(4096)
                if not chunk:
                    break
                print(repr(chunk))
~~~

**EN:** Save this snippet locally as `tcp_observe.py` and run `python tcp_observe.py`. It prints raw chunks, not decoded message boundaries. TCP may split or combine application writes; this firmware adds neither a newline nor a length prefix. The example is a diagnostic observer, not a complete protocol parser.

**中文：** 将示例另存为本地脚本运行。它显示原始接收片段，不负责识别消息边界。TCP 可能拆分或合并写入，而固件没有添加换行或长度前缀，因此该程序仅用于观察，非完整协议解析器。

## Build and run / 构建与运行

**EN:** Preserve the checked-in `sdkconfig`: it already targets ESP32-S3 and enables the custom partition table and PSRAM. A blanket `set-target` reset would require restoring/reviewing those settings. Use a new build directory to avoid stale machine-specific paths in the tracked `build/` folder.

**中文：** 保留已有 `sdkconfig`，其中已设置目标芯片、自定义分区和 PSRAM。直接重置目标会需要恢复并核对这些参数。使用新构建目录，避免历史 `build/` 中的机器相关路径。

Run from the repository root in the configured terminal / 在已配置环境的终端、仓库根目录执行：

~~~text
idf.py --version
idf.py -B build-local menuconfig
idf.py -B build-local build
idf.py -B build-local -p COM5 flash monitor
~~~

**EN:** Replace `COM5` with the actual device port. In `menuconfig` verify flash/PSRAM and `MyPartitions.csv`; Wi-Fi/TCP values in this version are source macros, not custom menu entries. Exit the monitor with Ctrl+]. Save compiler output, boot logs and received bytes, with credentials removed. These commands describe the intended workflow; this documentation update did not run an ESP-IDF build or flash hardware.

**中文：** 将 `COM5` 替换为实际端口，在配置界面核对 Flash/PSRAM 和分区表。本版本网络参数位于源码宏中，不是自定义菜单项。Ctrl+] 退出监视器，保存去除凭据后的编译、启动及接收日志。本次仅整理操作流程，未运行 ESP-IDF 构建或实物烧录。

## Troubleshooting / 常见问题

| Symptom / 现象 | Check / 排查 |
| --- | --- |
| No sensor logs after Wi-Fi / 联网后无传感日志 | Reachable TCP server, address, port and firewall / 服务端、地址、端口和防火墙 |
| ADC zeros or implausible output / ADC 零值或异常 | Wiring, address, mixed channel mapping and I²C read errors / 接线、地址、通道组合和读错误 |
| Build/boot memory errors / 构建或启动存储错误 | IDF version, 16 MB layout, PSRAM type and stale caches / 版本、分区、PSRAM 类型及旧缓存 |
| Rotating board does not rotate transmitted heading / 旋转板卡未改变发送风向基准 | Compass value is logged separately and not fused / 方位角尚未融合 |
| Concatenated TCP text / TCP 文本粘连 | Expected byte-stream behavior; no framing / 字节流无消息分隔 |
