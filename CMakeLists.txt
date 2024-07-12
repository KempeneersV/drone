# I target a recent cmake, it shouldn't be a problem on a dev machine
cmake_minimum_required(VERSION 3.11)

# Project name
project("flight_controller" C)

# Product filename
set(PRODUCT_NAME flight_controller)

## AVR Chip Configuration
# Specify the microcontroller and compiler options
set(MCU atmega328p)
set(F_CPU 16000000UL)
set(BAUD 115200)
set(PROGRAMMER arduino)
set(PORT /dev/ttyACM0)

# AVR Fuses, must be in concordance with your hardware and F_CPU
# http://eleccelerator.com/fusecalc/fusecalc.php?chip=atmega328p
#set(E_FUSE 0xfd)
#set(H_FUSE 0xda)
#set(L_FUSE 0xfd)
#set(LOCK_BIT 0xff)

# Use AVR GCC toolchain
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_COMPILER avr-gcc)

# mmcu MUST be passed to both the compiler and linker, this handles the linker
set(CMAKE_EXE_LINKER_FLAGS -mmcu=${MCU})

add_compile_options(
    -Os
    -DF_CPU=${F_CPU}
    -mmcu=${MCU} # MCU
    -DBAUD=${BAUD}
)

# Include directories for headers
include_directories(src src/communication src/communication/i2c src/communication/serial)

# Add source files
set(SOURCES
    src/flight_controller.c
    src/mpu6050.c
    src/timer.c
    src/utils.c
    src/communication/i2c/i2c.c
    src/communication/serial/serial_communication.c
)

# Add header files (optional but useful for IDEs)
set(HEADERS
    src/config.h
    src/mpu6050.h
    src/timer.h
    src/utils.h
    src/communication/i2c/i2c.h
    src/communication/serial/serial_communication.h
)

# Create the executable
add_executable(${PRODUCT_NAME} ${SOURCES} ${HEADERS})

set_target_properties(${PRODUCT_NAME} PROPERTIES OUTPUT_NAME ${PRODUCT_NAME}.o)

# Strip binary for upload
add_custom_target(bin ALL avr-gcc -o ${PRODUCT_NAME}.bin ${PRODUCT_NAME}.o)

# Transform binary into hex file, we ignore the eeprom segments in this step
add_custom_target(hex ALL avr-objcopy -O ihex -R .eeprom ${PRODUCT_NAME}.bin ${PRODUCT_NAME}.hex DEPENDS bin)

# Transform binary into hex file for EEPROM part (empty if you don't use eeprom static variables)
# add_custom_target(eeprom avr-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 -O ihex ${PRODUCT_NAME}.elf ${PRODUCT_NAME}.eep DEPENDS strip)

# Upload the firmware with avrdude
add_custom_target(upload avrdude -F -V -c ${PROGRAMMER} -p ${MCU} -P ${PORT} -b ${BAUD} -U flash:w:${PRODUCT_NAME}.hex DEPENDS hex)

# Upload the eeprom with avrdude
# add_custom_target(upload_eeprom avrdude -c ${PROGRAMMER} -p ${MCU} -P ${PORT} -b ${BAUD} -U eeprom:w:${PRODUCT_NAME}.eep DEPENDS eeprom)

# Burn fuses
# add_custom_target(fuses avrdude -c ${PROGRAMMER} -p ${MCU} -P ${PORT} -b ${BAUD} -U lfuse:w:${L_FUSE}:m -U hfuse:w:${H_FUSE}:m -U efuse:w:${E_FUSE}:m -U lock:w:${LOCK_BIT}:m)

# Clean extra files
#set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${PRODUCT_NAME}.hex;${PRODUCT_NAME}.eep;${PRODUCT_NAME}.lst"
