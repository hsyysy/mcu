#ifndef __COMMON_H__
#define __COMMON_H__

#include <pin.h>

#define ARRAY_SIZE(array)	(sizeof(array) / sizeof(array[0]))
#define ROUND_UP(x, n)		(((x) + ((n) - 1)) / n)

#define MCU_SLAVE_ADDR		0x17

#define I2C1_OA1		0x17
#define I2C1_OA2		0x68
#define I2C1_OA2_MASK		0x03

#define EEPROM_BASE	0x08080c00
#define EEPROM_SIZE	0x00000c00

#define EEPROM_CELL_SIZE	32
#define EEPROM_CELL_OFFSET(n)	(EEPROM_CELL_SIZE * n)
#define EEPROM_CELL_ADDR(n)	(EEPROM_BASE + EEPROM_CELL_OFFSET(n))

#define EEPROM_MAC0_OFFSET		EEPROM_CELL_OFFSET(0)
#define EEPROM_MAC1_OFFSET		EEPROM_CELL_OFFSET(1)
#define EEPROM_SN_OFFSET		EEPROM_CELL_OFFSET(2)
#define EEPROM_BOARD_TYPE_OFFSET	EEPROM_CELL_OFFSET(3)
#define EEPROM_POWER_OFF_REASON_OFFSET	EEPROM_CELL_OFFSET(5)

#define EEPROM_MAC0_ADDR		EEPROM_CELL_ADDR(0)
#define EEPROM_MAC1_ADDR		EEPROM_CELL_ADDR(1)
#define EEPROM_SN_ADDR			EEPROM_CELL_ADDR(2)
#define EEPROM_BOARD_TYPE_ADDR		EEPROM_CELL_ADDR(3)
#define EEPROM_POWER_OFF_REASON_ADDR	EEPROM_CELL_ADDR(5)

#define BIT(n)		(1 << n)

#define LED_FREQ_ALWAYS_ON	1000
#define LED_FREQ_ALWAYS_OFF	0

enum {
	WORK_MODE_SOC = 1,
	WORK_MODE_PCIE = 2,
};

enum {
	DDR_TYPE_LPDDR4X = 0,
	DDR_TYPE_LPDDR4 = 1,
};

void set_board_type(uint8_t type);
uint8_t get_board_type(void);
uint8_t get_firmware_version(void);
int get_work_mode(void);
void board_init(void);
uint8_t get_declared_board_type(void);
uint8_t get_ddr_type(void);
char *get_board_type_name();
void led_init(void);
void led_set_frequency(unsigned long freq);

int get_board_temp(void);
int get_soc_temp(void);
void set_board_temp(int temp);
void set_soc_temp(int temp);

void root_power_on(void);
void root_power_off(void);
void root_power_reboot(void);

#endif