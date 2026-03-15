#!/usr/bin/env bash
set -e

ROOT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
PATCH_FILE="$ROOT_DIR/patches/readonly-msc-and-dual-targets.patch"
USB_MW_DIR="$ROOT_DIR/vendor/STM32CubeF0/Middlewares/ST/STM32_USB_Device_Library"

if git -C "$USB_MW_DIR" apply --check "$PATCH_FILE" >/dev/null 2>&1; then
    git -C "$USB_MW_DIR" apply "$PATCH_FILE"
elif git -C "$USB_MW_DIR" apply --reverse --check "$PATCH_FILE" >/dev/null 2>&1; then
    echo "Patch already applied: $PATCH_FILE"
else
    echo "Failed to apply patch: $PATCH_FILE" >&2
    exit 1
fi

cmake -B"$ROOT_DIR/build" -DCMAKE_TOOLCHAIN_FILE="$ROOT_DIR/cmake/stm32f072xB.cmake"
cmake --build "$ROOT_DIR/build"
