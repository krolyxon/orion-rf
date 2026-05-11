# 🛰️ Orion RF (WIP)

**Orion RF** is a portable RF & wireless experimentation toolkit built for the ESP32 platform with NRF24 support.

> ⚠️ This project is currently under active development. Some features are experimental, incomplete, or may change frequently.

---

# 🚀 Current Features

## 📡 Wireless Tools

### BLE Scan
- Discover nearby Bluetooth Low Energy devices
- View MAC address, RSSI, and device information
- Real-time scanning interface

### WiFi Scan
- Scan nearby WiFi access points
- View:
  - SSID
  - Signal strength (RSSI)
  - Channel
  - Encryption type

### WiFi Analyzer
- Analyze surrounding WiFi activity
- Useful for understanding channel congestion and wireless environments

### RF Capture *(WIP)*
- Experimental RF packet capture functionality
- Replay support planned

---

# 📻 NRF24 Tools

### BLE Jammer
- NRF24-based BLE interference testing
- Functional

### Bluetooth Jammer
- NRF24-based Bluetooth interference testing
- Functional

### WiFi Jammer
- NRF24-based WiFi interference testing
- Able to jam 2.4Ghz wifi connections (Not 5G)

---

# ⌨️ BadUSB

- HID keyboard emulation
- Execute automated payloads/scripts
- Useful for testing and automation workflows

---

# 🖱️ BLE Mouse

- ESP32-based Bluetooth mouse emulation
- Experimental HID functionality

---

# 🔧 System Utilities

### System Info
- Display device/system information
- Useful for debugging and diagnostics

### Device Check
- Verify connected peripherals/modules
- Hardware diagnostics utility

### Restart
- Quick device reboot option from UI

---

# 🧩 Project Status

| Feature              | Status            |
| -------------------- | ----------------- |
| BLE Scan             | ✅ Working        |
| WiFi Scan            | ✅ Working        |
| WiFi Analyzer        | ✅ Working        |
| BadUSB               | ✅ Working        |
| BLE Mouse            | ✅ Working        |
| BLE Jammer           | ✅ Working        |
| Bluetooth Jammer     | ✅ Working        |
| WiFi Jammer          | ⚠️ Experimental   |
| RF Capture           | 🚧 WIP            |
| RF Replay            | 🚧 WIP            |
| SD Card Support      | 🚧 Planned        |
| Emulator             | 🚧 Planned        |
| NFC                  | 🚧 Planned        |

---

# 🛠️ Hardware

- 1 x ESP32-S3
- 2 x NRF24L01 Modules
- 2 x CC1101 Modules
- 1 x OLED Display
- Battery-powered portable setup

---

# 📌 Notes

- Designed primarily for learning, experimentation, and embedded RF research
- Optimized for portable usage
- UI-driven navigation system

---

# ⚖️ Disclaimer

This project is intended for:
- Educational purposes
- RF experimentation
- Hardware research
- Authorized security testing

The authors are not responsible for misuse or illegal operation of this project.
