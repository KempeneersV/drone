#!/bin/bash

# Path to the generated .hex file
hex_file="/home/vm/Documents/drone/build/flight_controller.hex"

# Target directory where you want to copy the .hex file
target_dir="/media/sf_share"

# Check if the .hex file exists
if [ -f "$hex_file" ]; then
    # Check if the .hex file already exists in the target directory
    if [ -f "$target_dir/$hex_file" ]; then
        # Remove the existing .hex file in the target directory
        rm -f "$target_dir/$hex_file"
        echo "Old .hex file removed from $target_dir"
    fi
    
    # Copy the new .hex file to the target directory
    cp "$hex_file" "$target_dir"
    echo "New .hex file copied successfully to $target_dir"
else
    echo "Error: The file $hex_file does not exist."
    exit 1
fi
