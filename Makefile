CROSS = arm-none-eabi-
CC = $(CROSS)gcc
CXX = $(CROSS)g++
OBJCOPY = $(CROSS)objcopy
NRFUTIL = adafruit-nrfutil

COMFLAGS = -DARDUINO=100 -DFLOAT_ABI_HARD \
	-DNRF52 -DNRF52832_XXAA -DS132 \
	-Wno-register

MCUFLAGS = -mcpu=cortex-m4 -mthumb -mabi=aapcs \
	-mfloat-abi=hard -mfpu=fpv4-sp-d16 \
	-ffunction-sections -fdata-sections

CFLAGS = $(MCUFLAGS) --std=gnu99
CXXFLAGS = $(MCUFLAGS) --std=c++17 -fno-builtin -fno-exceptions \
	-fno-strict-aliasing

SFLAGS = $(MCUFLAGS) -x assembler-with-cpp \
	-DCONFIG_GPIO_AS_PINRESET \
	-DBLE_STACK_SUPPORT_REQD \
	-DBSP_DEFINES_ONLY \
	-DSWI_DISABLE0 \
	-DSOFTDEVICE_PRESENT

SSRC = $(ARDUINO)/cores/nRF5/linker/gcc_startup_nrf52.S

CSRC = $(wildcard $(ARDUINO)/cores/nRF5/freertos/Source/*.c) \
	$(ARDUINO)/cores/nRF5/freertos/portable/CMSIS/nrf52/port_cmsis.c \
	$(ARDUINO)/cores/nRF5/freertos/portable/CMSIS/nrf52/port_cmsis_systick.c \
	$(ARDUINO)/cores/nRF5/freertos/portable/GCC/nrf52/port.c \
	$(ARDUINO)/cores/nRF5/freertos/Source/portable/MemMang/heap_3.c \
	$(ARDUINO)/cores/nRF5/flash/flash_nrf5x.c \
	$(ARDUINO)/cores/nRF5/flash/flash_cache.c \
	$(ARDUINO)/cores/nRF5/nordic/nrfx/mdk/system_nrf52.c \
	$(ARDUINO)/cores/nRF5/wiring.c \
	$(ARDUINO)/cores/nRF5/wiring_digital.c \
	$(ARDUINO)/libraries/FileSystem/src/littlefs/lfs.c \
	$(ARDUINO)/libraries/FileSystem/src/littlefs/lfs_util.c \
	$(ARDUINO)/libraries/Bluefruit52Lib/src/utility/bootloader_util.c

CXXSRC = $(wildcard source/*.cpp) \
	$(wildcard source/sharp/*.cpp) \
	$(ARDUINO)/libraries/Bluefruit52Lib/src/BLEAdvertising.cpp \
	$(ARDUINO)/libraries/Bluefruit52Lib/src/BLECentral.cpp \
	$(ARDUINO)/libraries/Bluefruit52Lib/src/BLECharacteristic.cpp \
	$(ARDUINO)/libraries/Bluefruit52Lib/src/BLEClientCharacteristic.cpp \
	$(ARDUINO)/libraries/Bluefruit52Lib/src/BLEClientService.cpp \
	$(ARDUINO)/libraries/Bluefruit52Lib/src/BLEDiscovery.cpp \
	$(ARDUINO)/libraries/Bluefruit52Lib/src/BLEGap.cpp \
	$(ARDUINO)/libraries/Bluefruit52Lib/src/BLEGatt.cpp \
	$(ARDUINO)/libraries/Bluefruit52Lib/src/BLEService.cpp \
	$(ARDUINO)/libraries/Bluefruit52Lib/src/BLEScanner.cpp \
	$(ARDUINO)/libraries/Bluefruit52Lib/src/BLEUuid.cpp \
	$(ARDUINO)/libraries/Bluefruit52Lib/src/bluefruit.cpp \
	$(ARDUINO)/libraries/Bluefruit52Lib/src/services/BLEUart.cpp \
	$(ARDUINO)/libraries/Bluefruit52Lib/src/services/BLEDfu.cpp \
	$(ARDUINO)/libraries/Bluefruit52Lib/src/utility/bonding.cpp \
	$(ARDUINO)/libraries/Bluefruit52Lib/src/utility/AdaMsg.cpp \
	$(ARDUINO)/libraries/SoftwareSerial/SoftwareSerial.cpp \
	$(ARDUINO)/libraries/FileSystem/src/InternalFS.cpp \
	$(ARDUINO)/libraries/FileSystem/src/Bluefruit_FileIO.cpp \
	$(ARDUINO)/variants/feather_nrf52832/variant.cpp \
	$(ARDUINO)/cores/nRF5/utility/adafruit_fifo.cpp \
	$(ARDUINO)/cores/nRF5/delay.cpp \
	$(ARDUINO)/cores/nRF5/Print.cpp \
	$(ARDUINO)/cores/nRF5/RingBuffer.cpp \
	$(ARDUINO)/cores/nRF5/Uart.cpp \
	$(ARDUINO)/cores/nRF5/rtos.cpp \
	$(ARDUINO)/cores/nRF5/utility/AdaCallback.cpp \
	$(ARDUINO)/cores/nRF5/main.cpp

OBJ = $(patsubst %.cpp, %.o, $(CXXSRC)) \
      $(patsubst %.c, %.o, $(CSRC)) \
      $(patsubst %.S, %.o, $(SSRC))
OUTELF = main.elf
OUTHEX = $(patsubst %.elf, %.hex, $(OUTELF))
OUTDFU = $(patsubst %.elf, %_dfu.zip, $(OUTELF))

ARDUINO = arduino
INCLUDES = -I$(ARDUINO)/libraries/Bluefruit52Lib/src \
	-I$(ARDUINO)/libraries/SoftwareSerial \
	-I$(ARDUINO)/libraries/FileSystem/src \
	-I$(ARDUINO)/cores/nRF5 \
	-I$(ARDUINO)/cores/nRF5/nordic \
	-I$(ARDUINO)/cores/nRF5/nordic/nrfx \
	-I$(ARDUINO)/cores/nRF5/nordic/nrfx/hal \
	-I$(ARDUINO)/cores/nRF5/nordic/nrfx/mdk \
	-I$(ARDUINO)/cores/nRF5/cmsis/include \
	-I$(ARDUINO)/cores/nRF5/nordic/softdevice/s132_nrf52_6.1.1_API/include \
	-I$(ARDUINO)/cores/nRF5/freertos/config \
	-I$(ARDUINO)/cores/nRF5/freertos/Source/include \
	-I$(ARDUINO)/cores/nRF5/freertos/portable/CMSIS/nrf52 \
	-I$(ARDUINO)/cores/nRF5/freertos/portable/GCC/nrf52 \
	-I$(ARDUINO)/variants/feather_nrf52832 \
	-Isource/sharp

all: $(OUTELF) $(OUTHEX) $(OUTDFU)

$(OUTELF): $(OBJ)
	@echo "  LINK  " $(OUTELF)
	@$(CXX) $(MCUFLAGS) -Wl,--gc-sections --specs=nano.specs -lc -lnosys \
		-L$(ARDUINO)/cores/nRF5/linker \
		-T$(ARDUINO)/cores/nRF5/linker/nrf52832_s132_v6.ld \
		$(OBJ) -o $(OUTELF)

$(OUTHEX): $(OUTELF)
	@echo "        " $(OUTHEX)
	@$(OBJCOPY) $(OUTELF) -Oihex $(OUTHEX)

$(OUTDFU): $(OUTHEX)
	@echo "        " $(OUTDFU)
	@$(NRFUTIL) dfu genpkg --dev-type 0x0052 --application $(OUTHEX) \
		$(OUTDFU)

clean:
	@echo "  CLEAN"
	@rm -f $(OBJ) $(OUTELF) $(OUTHEX) $(OUTDFU)

upload: $(OUTDFU)
	@echo "Uploading: " $(OUTDFU)
	@$(NRFUTIL) dfu serial --package $(OUTDFU) -p /dev/ttyUSB0 -b 115200

%.o: %.cpp
	@echo "  CXX   " $<
	@$(CXX) $(COMFLAGS) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.c
	@echo "  CC    " $<
	@$(CC) $(COMFLAGS) $(CFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.S
	@echo "  AS    " $<
	@$(CC) $(COMFLAGS) $(SFLAGS) -c $< -o $@

