# Orion RF

Orion RF is a portable RF and wireless toolkit built on the ESP32 platform with NRF24 and CC1101 support.

## Features

### Wireless
- BLE scanning
- WiFi scanning
- WiFi analyzer
- RF capture (WIP)
- RF replay (planned)

### NRF24
- BLE interference testing
- Bluetooth interference testing
- WiFi interference testing

### HID
- BadUSB keyboard emulation
- BLE mouse emulation

### Utilities
- System information
- Hardware diagnostics
- Device restart

## Hardware
- ESP32-S3 N16R8
- 2x NRF24L01 Module
- 2x CC1101 Module
- PN532 NFC Module
- MicroSD card Module
- TP4056 Charging Module
- OLED display
- Battery-powered portable setup

## Status

| Feature | Status |
|---|---|
| BLE Scan | Working |
| WiFi Scan | Working |
| WiFi Analyzer | Working |
| BadUSB | Working |
| BLE Mouse | Working |
| BLE Jammer | Working |
| Bluetooth Jammer | Working |
| WiFi Jammer | Working |
| RF Capture | WIP |
| RF Replay | WIP |
| SD Card Support | Planned |
| Emulator | Planned |
| NFC | WIP |


# Installation
## Installation

### End User Installation

The easiest way to install ORION-RF is through the web installer.

1. Connect your ESP32-S3 to your computer using a USB cable.
2. Open the installer page:
   * https://krolyxon.github.io/orion-rf/
3. Click **Connect** and select your device.
4. Click **Install** and wait for the flashing process to complete.
5. Reboot the device if required.

> Google Chrome, Microsoft Edge, and other Chromium-based browsers are recommended. It may or may not work on Firefox based browsers.

---

### Development Build

#### Clone the Repository

```bash
git clone https://github.com/krolyxon/orion-rf.git
cd orion-rf
```

#### Build Firmware

```bash
pio run
```

Compiled binaries will be available in:

```text
.pio/build/<environment>/
```

#### Upload to Device

```bash
pio run -t upload
```

#### Monitor Serial Output

```bash
pio device monitor
```

