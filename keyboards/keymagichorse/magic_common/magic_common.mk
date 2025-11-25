MAGIC_COMMON_DIR = keyboards/keymagichorse/magic_common
VPATH += ${MAGIC_COMMON_DIR}
SRC   += ${MAGIC_COMMON_DIR}/magic_common.c
SRC   += ${MAGIC_COMMON_DIR}/magic_task.c
OPT_DEFS += -DMAGIC_COMMON_ENABLED

# 日记类型 不写默认就是不打开 uart是借用蓝牙的串口的简单调试可以
# rtt这个低功耗之后可能就用不了了，用起来就很麻烦，所以添加了uart借用bhq的串口用一下
DEBUG_PRINTF_TYPE ?= not_debug
ifeq ($(strip $(DEBUG_PRINTF_TYPE)), rtt)
	OPT_DEFS += -DMAGIC_DEBUG_PRINTF_TYPE_RTT
	OPT_DEFS += -DMAGIC_DEBUG

	SRC += ${MAGIC_COMMON_DIR}/debug/rtt/SEGGER_RTT.c
	SRC += ${MAGIC_COMMON_DIR}/debug/rtt/SEGGER_RTT_printf.c
	SRC += ${MAGIC_COMMON_DIR}/debug/magic_printf.c
	
	VPATH += ${MAGIC_COMMON_DIR}/debug/rtt/
	VPATH += ${MAGIC_COMMON_DIR}/debug/
endif   
ifeq ($(strip $(DEBUG_PRINTF_TYPE)), uart_bhq)
	OPT_DEFS += -DMAGIC_DEBUG_PRINTF_TYPE_UART_BHQ
	OPT_DEFS += -DMAGIC_DEBUG

	SRC 	+= ${MAGIC_COMMON_DIR}/debug/magic_printf.c
	VPATH 	+= ${MAGIC_COMMON_DIR}/debug/
endif   

# 电池读取功能
WIRELESS_BATTERY_ENABLE ?= NO
ifeq ($(strip $(WIRELESS_BATTERY_ENABLE)), yes)
    OPT_DEFS += -DWIRELESS_BATTERY_ENABLE
    MAGIC_ANALOG_ENABLED = yes
	VPATH += ${MAGIC_COMMON_DIR}/wireless_lib/
	SRC += ${MAGIC_COMMON_DIR}/wireless_lib/battery.c
endif

# ADC  改为自己修改过的 km_analog.c
ifeq ($(strip $(MAGIC_ANALOG_ENABLED)), yes)
    OPT_DEFS += -DHAL_USE_ADC=TRUE
    SRC += ${MAGIC_COMMON_DIR}/magic_analog.c
endif

# 矩阵类型
MATRIX_TYPE ?= default
ifeq ($(strip $(MATRIX_TYPE)), default)
	VPATH += ${MAGIC_COMMON_DIR}/matrix_type/
	SRC += ${MAGIC_COMMON_DIR}/matrix_type/default/matrix_sleep.c
endif   
ifeq ($(strip $(MATRIX_TYPE)), shift595)
	VPATH += ${MAGIC_COMMON_DIR}/matrix_type/
	VPATH += ${MAGIC_COMMON_DIR}/matrix_type/shift595/
endif   
ifeq ($(strip $(MATRIX_TYPE)), ec)
	VPATH += ${MAGIC_COMMON_DIR}/matrix_type/
	VPATH += ${MAGIC_COMMON_DIR}/matrix_type/ec/
endif   

# 无线功能
WIRELESS_ENABLE ?= no
ifeq ($(strip $(WIRELESS_ENABLE)), yes)

	BLUETOOTH_ENABLE = yes
	BLUETOOTH_DRIVER = custom
	OPT_DEFS += -DBLUETOOTH_ENABLE
    OPT_DEFS += -DBLUETOOTH_BHQ
	OPT_DEFS += -DWIRELESS_ENABLE
	NO_USB_STARTUP_CHECK = yes
	UART_DRIVER_REQUIRED = yes


	VPATH += ${MAGIC_COMMON_DIR}/wireless_lib/
	VPATH += ${MAGIC_COMMON_DIR}/wireless_lib/bhq/

	SRC += ${MAGIC_COMMON_DIR}/wireless_lib/bhq/bluetooth_drivers.c
	SRC += ${MAGIC_COMMON_DIR}/wireless_lib/bhq/bhq.c
	SRC += ${MAGIC_COMMON_DIR}/wireless_lib/bhq/bluetooth_bhq.c
	SRC += ${MAGIC_COMMON_DIR}/wireless_lib/bhq/report_buffer.c
	SRC += ${MAGIC_COMMON_DIR}/wireless_lib/transport.c
	SRC += ${MAGIC_COMMON_DIR}/wireless_lib/wireless.c
	SRC += ${MAGIC_COMMON_DIR}/wireless_lib/wireless_common.c

# 低功耗使能
	ifeq ($(strip $(WIRELESS_LPM_ENABLE)), yes)
		OPT_DEFS += -DWIRELESS_LPM_ENABLE
		VPATH += ${MAGIC_COMMON_DIR}/wireless_lib/lpm/
		SRC += ${MAGIC_COMMON_DIR}/wireless_lib/lpm/lpm_stm32f4.c
		SRC += ${MAGIC_COMMON_DIR}/wireless_lib/lpm/lpm.c
	endif

endif   
