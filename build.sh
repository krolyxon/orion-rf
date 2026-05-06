#!/usr/bin/env bash

set -e

# ========= CONFIG =========
PORT="/dev/ttyACM0"

FQBN="esp32:esp32:esp32s3:\
UploadSpeed=921600,\
USBMode=hwcdc,\
CDCOnBoot=default,\
MSCOnBoot=default,\
DFUOnBoot=default,\
UploadMode=default,\
CPUFreq=240,\
FlashMode=qio,\
FlashSize=16M,\
PartitionScheme=fatflash,\
DebugLevel=none,\
PSRAM=opi,\
LoopCore=1,\
EventsCore=1,\
EraseFlash=none,\
JTAGAdapter=default,\
ZigbeeMode=default"

# ========= FLAGS =========
UPLOAD=false
SKETCH_PATH="./firmware"

while [[ $# -gt 0 ]]; do
    case "$1" in
        -u|--upload)
            UPLOAD=true
            shift
            ;;
        *)
            SKETCH_PATH="$1"
            shift
            ;;
    esac
done

# ========= BUILD =========
echo "Compiling sketch..."

arduino-cli compile \
    --fqbn "$FQBN" \
    "$SKETCH_PATH"

echo "Compile successful."

# ========= UPLOAD =========
if [ "$UPLOAD" = true ]; then
    echo "Uploading to $PORT..."

    arduino-cli upload \
        -p "$PORT" \
        --fqbn "$FQBN" \
        "$SKETCH_PATH"

    echo "Upload complete."
fi
