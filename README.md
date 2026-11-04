# Thermal Wind Sensor

[简体中文](README.zh-CN.md) · [Setup](docs/GETTING_STARTED.md) · [Architecture](docs/ARCHITECTURE.md) · [Validation](docs/VALIDATION.md) · [Portfolio](docs/PORTFOLIO.md)

An ESP32-S3 course-project prototype for thermal wind sensing (热式风传感). The firmware acquires four sensor signals through an ADS1115, estimates a wind vector from a configured thermal model, and sends text results over Wi-Fi/TCP. A QMC5883P magnetometer (磁力计) runs in a separate task.

The repository keeps its original name, `myThemalWindSensor`; the ESP-IDF project is named `HotWireV1.0`.

**Status:** incomplete prototype, consistent with the original commit note. The calculated speed has no verified physical unit or calibration dataset in this upload, and the magnetometer heading is **not fused into the transmitted wind direction**. The existing generic ESP-IDF sample README has been replaced with project-specific documentation.

## Implemented in source

- ADS1115 acquisition using mixed differential/single-ended channels.
- Five readings per signal, with maximum/minimum removed before averaging.
- FreeRTOS tasks (实时操作系统任务) connected by ADC and wind-result queues.
- Continuous-angle and eight-sector wind calculation functions; the main application uses the continuous-angle version.
- QMC5883P heading calculation with a configured declination offset.
- Wi-Fi station connection and a TCP client sending formatted wind results.

## At a glance

| Item | Source configuration |
| --- | --- |
| Target | ESP32-S3 |
| Recorded framework | ESP-IDF 5.1.2 |
| Memory configuration | 16 MB flash; octal PSRAM enabled |
| ADC | ADS1115, I²C0, address `0x48`, SDA GPIO12, SCL GPIO13 |
| Magnetometer | QMC5883P, I²C1, address `0x2C`, SDA GPIO7, SCL GPIO6 |
| I²C bus frequency | 100 kHz on each bus |
| Network | Outbound IPv4 TCP connection, configured server port 8080 |
| Payload | `Wind Direction: %.0f, Speed: %.2f`, without a newline delimiter |

## Documentation

- [Getting started](docs/GETTING_STARTED.md): environment, sensor mapping, network setup and build workflow.
- [Architecture](docs/ARCHITECTURE.md): tasks, queues, model equations and TCP format.
- [Validation](docs/VALIDATION.md): calibration, recovery behavior and known limitations.
- [Portfolio brief](docs/PORTFOLIO.md): project narrative and demonstration outline.
- [Contributing](CONTRIBUTING.md): evidence, bilingual updates and attribution.

## Repository layout

| Path | Purpose |
| --- | --- |
| `main/main.c` | Startup, acquisition, calculation, heading and transmission tasks |
| `main/APP/` | Wi-Fi and TCP helpers |
| `components/BSP/ADC/` | ADS1115 driver |
| `components/BSP/QMC5883P/` | Magnetometer driver |
| `components/BSP/WIND_CALCU/` | Wind model and constants |
| `components/BSP/KEY/`, `components/BSP/SWITCH/` | Auxiliary modules; not active features of the main application |
| `sdkconfig`, `MyPartitions.csv` | Target, memory and partition settings |
| `build/` | Historical generated files; use a separate build directory |
| `docs/` | Bilingual engineering and portfolio documentation |

Start with [the setup guide](docs/GETTING_STARTED.md). The server must be reachable before startup can proceed to sensor initialization.

The Wi-Fi helper contains hard-coded credentials and password logging. Use disposable test-network settings, do not publish logs containing credentials, and address this before a public demo. No credential values are reproduced in these docs.

No repository-level license file was found. Wi-Fi source headers credit ALIENTEK and retain its copyright notice; this update does not relicense that code.

Reviewed against commit `0e55473` on **2026-11-04**.
