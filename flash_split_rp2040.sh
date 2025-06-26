#!/bin/bash

# --- Configuration ---
# Set the full path to your compiled UF2 file
UF2_FILE="/home/dave/qmk_firmware/crkbd_rev1_marmaladejam.uf2"

# Set the name of the RP2040 bootloader drive
# This is the name of the volume that appears when you put the controller in bootloader mode.
BOOTLOADER_DRIVE_NAME="RPI-RP2"

# --- Script Logic ---

# Check if the UF2 file exists
if [ ! -f "$UF2_FILE" ]; then
    echo "Error: UF2 file not found at $UF2_FILE"
    echo "Please compile your firmware first and update the UF2_FILE variable in this script."
    exit 1
fi

# --- Flashing First Half (Left) ---
echo "=========================================="
echo "    Ready to flash the LEFT half."
echo "    1. Unplug both keyboard halves."
echo "    2. Hold the BOOTSEL button on the LEFT controller."
echo "    3. Plug the LEFT half into your computer via USB."
echo "    4. Release the BOOTSEL button after a second."
echo "    5. Press Enter to continue once you've done this."
echo "=========================================="
read -p "Press Enter to start waiting for the device..."

# Find the mount point of the RP2040 bootloader drive
MOUNT_POINT=""
while [ -z "$MOUNT_POINT" ]; do
    # On Linux, this is typically under /media or /run/media
    # On macOS, this is typically under /Volumes
    if [ -d "/media/$(whoami)/$BOOTLOADER_DRIVE_NAME" ]; then
        MOUNT_POINT="/media/$(whoami)/$BOOTLOADER_DRIVE_NAME"
    elif [ -d "/run/media/$(whoami)/$BOOTLOADER_DRIVE_NAME" ]; then
        MOUNT_POINT="/run/media/$(whoami)/$BOOTLOADER_DRIVE_NAME"
    elif [ -d "/Volumes/$BOOTLOADER_DRIVE_NAME" ]; then
        MOUNT_POINT="/Volumes/$BOOTLOADER_DRIVE_NAME"
    fi
    if [ -z "$MOUNT_POINT" ]; then
        echo "Waiting for $BOOTLOADER_DRIVE_NAME to be mounted... (press Ctrl+C to cancel)"
        sleep 1
    fi
done

echo "Found RP2040 bootloader drive at: $MOUNT_POINT"
echo "Copying firmware to the left half..."

# Copy the UF2 file to the mounted drive
cp "$UF2_FILE" "$MOUNT_POINT/"

# Wait for the drive to unmount, which indicates flashing is complete and the keyboard has rebooted
while [ -d "$MOUNT_POINT" ]; do
    echo "Flashing in progress... waiting for device to unmount."
    sleep 1
done

echo "Flashing of the LEFT half complete! It should now be working."
echo ""
echo ""

# --- Flashing Second Half (Right) ---
echo "=========================================="
echo "    Ready to flash the RIGHT half."
echo "    1. Unplug the LEFT half from USB."
echo "    2. Hold the BOOTSEL button on the RIGHT controller."
echo "    3. Plug the RIGHT half into your computer via USB."
echo "    4. Release the BOOTSEL button after a second."
echo "    5. Press Enter to continue once you've done this."
echo "=========================================="
read -p "Press Enter to start waiting for the device..."

# Reset the MOUNT_POINT variable for the next loop
MOUNT_POINT=""
while [ -z "$MOUNT_POINT" ]; do
    if [ -d "/media/$(whoami)/$BOOTLOADER_DRIVE_NAME" ]; then
        MOUNT_POINT="/media/$(whoami)/$BOOTLOADER_DRIVE_NAME"
    elif [ -d "/run/media/$(whoami)/$BOOTLOADER_DRIVE_NAME" ]; then
        MOUNT_POINT="/run/media/$(whoami)/$BOOTLOADER_DRIVE_NAME"
    elif [ -d "/Volumes/$BOOTLOADER_DRIVE_NAME" ]; then
        MOUNT_POINT="/Volumes/$BOOTLOADER_DRIVE_NAME"
    fi
    if [ -z "$MOUNT_POINT" ]; then
        echo "Waiting for $BOOTLOADER_DRIVE_NAME to be mounted... (press Ctrl+C to cancel)"
        sleep 1
    fi
done

echo "Found RP2040 bootloader drive at: $MOUNT_POINT"
echo "Copying firmware to the right half..."

cp "$UF2_FILE" "$MOUNT_POINT/"

while [ -d "$MOUNT_POINT" ]; do
    echo "Flashing in progress... waiting for device to unmount."
    sleep 1
done

echo "Flashing of the RIGHT half complete! It should now be working."
echo "=========================================="
echo "    All done! You can now connect both halves with the TRRS cable"
echo "    and plug in the master half (usually the left one) to the computer."
echo "=========================================="