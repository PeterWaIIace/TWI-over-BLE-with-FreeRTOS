
#ifndef __TWI_H__
#define  __TWI_H__ 

#include <stdio.h>
#include "boards.h"
#include "app_util_platform.h"
#include "app_error.h"
#include "nrf_drv_twi.h"

#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

/* MPU-9255 DATA REGISTERS */
//****************************************
#define	PWR_MGMT_1      0x6B	//Power Management. Typical values:0x00(run mode)
#define	SMPLRT_DIV	0x19	//Sample Rate Divider. Typical values:0x07(125Hz) 1KHz internal sample rate
#define	CONFIG		0x1A	//Low Pass Filter.Typical values:0x06(5Hz)
#define	GYRO_CONFIG     0x1B	//Gyro Full Scale Select. Typical values:0x10(1000dps)
#define	ACCEL_CONFIG	0x1C	//Accel Full Scale Select. Typical values:0x01(2g)

// Accel
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40

// Gyros
#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48

/* TWI instance ID. */
#if TWI0_ENABLED
#define TWI_INSTANCE_ID     0
#elif TWI1_ENABLED
#define TWI_INSTANCE_ID     1
#endif

 /* Number of possible TWI addresses. */
#define TWI_ADDRESSES      127

/* Buffer for data */
#define BUFFER_SIZE 6

static uint16_t accel_buffer[3];
static uint16_t gyro_buffer[3];
static uint16_t cal_accel_buffer[3];
static uint16_t cal_gyro_buffer[3];

/* Indicates if operation on TWI has ended. */
static volatile bool m_xfer_done = false;

/* TWI instance. */
static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);

/**
 * @brief TWI initialization.
 */
void twi_init (void);
void MPU9255_init(uint8_t address);
/**
 * @brief Function for find twi sensor.
 */
int twi_find_device(void);

/**
 * @brief Function for find twi sensor.
 */
void MPU9255_calibrate();
void MPU9255_flush_buffers();
void MPU9255_read_sensor(uint8_t address);

uint16_t* MPU9255_get_accel_values(void);
uint16_t* MPU9255_get_gyro_values(void);

uint16_t MPU9255_read_sensor_reg(uint8_t address,uint8_t reg_high,uint8_t reg_low);

#endif /* __TWI_H__ */
