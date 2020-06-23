#include "MPU9255.h"

/**
 * @brief Function for handling data from temperature sensor.
 *
 * @param[in] temp          Temperature in Celsius degrees read from sensor.
 */

__STATIC_INLINE void data_handler(uint8_t* data_buffer)
{   
    NRF_LOG_INFO("Accel_data: %d | %d | %d | %d | %d | %d.", data_buffer[0],data_buffer[1],data_buffer[2],data_buffer[3],data_buffer[4],data_buffer[5]);
}

///**
// * @brief TWI events handler.
// */
void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{
    switch (p_event->type)
    {
        case NRF_DRV_TWI_EVT_DONE:
            if (p_event->xfer_desc.type == NRF_DRV_TWI_XFER_RX)
            {
                data_handler(accel_buffer);
            }
            m_xfer_done = true;
            break;
        default:
            break;
    }
}

void IMU_set_10DOF_Waveshare(){
//    err_code = nrf_drv_twi_tx(&m_twi, LM75B_ADDR, reg, sizeof(reg), false);
//    APP_ERROR_CHECK(err_code);
//    while (m_xfer_done == false);
//
//    err_code = nrf_drv_twi_tx(&m_twi, LM75B_ADDR, reg, 1, false);
//    APP_ERROR_CHECK(err_code);
//    while (m_xfer_done == false);
}

void MPU9255_calibrate(address){
  
  MPU9255_flush_buffers();

  int calibration_range = 100;
  uint32_t cal_AX =0;
  uint32_t cal_AY =0;
  uint32_t cal_AZ =0;
  uint32_t cal_GX =0;
  uint32_t cal_GY =0;
  uint32_t cal_GZ =0;
  for(int i=0;i<calibration_range;i++){
    cal_AX += MPU9255_read_sensor_reg(address,ACCEL_XOUT_H,ACCEL_XOUT_L);
    cal_AY += MPU9255_read_sensor_reg(address,ACCEL_YOUT_H,ACCEL_YOUT_L);
    cal_AZ += MPU9255_read_sensor_reg(address,ACCEL_ZOUT_H,ACCEL_ZOUT_L);
    cal_GX += MPU9255_read_sensor_reg(address,GYRO_XOUT_H,GYRO_XOUT_L);
    cal_GY += MPU9255_read_sensor_reg(address,GYRO_YOUT_H,GYRO_YOUT_L);
    cal_GZ += MPU9255_read_sensor_reg(address,GYRO_ZOUT_H,GYRO_ZOUT_L);
    nrf_delay_ms(10);
  }
  
  cal_accel_buffer[0] = cal_AX/calibration_range;
  cal_accel_buffer[1] = cal_AY/calibration_range;
  cal_accel_buffer[2] = cal_AZ/calibration_range;
  cal_gyro_buffer[0] = cal_GX/calibration_range;
  cal_gyro_buffer[1] = cal_GY/calibration_range;
  cal_gyro_buffer[2] = cal_GZ/calibration_range;
  NRF_LOG_INFO("callibration Accel_data: AX: %d | AY: %d | AZ: %d | GX: %d | GY: %d | GZ: %d ",cal_accel_buffer[0],cal_accel_buffer[1],cal_accel_buffer[2],cal_gyro_buffer[0],cal_gyro_buffer[1],cal_gyro_buffer[2]);

  MPU9255_flush_buffers();
}

void MPU9255_flush_buffers(){
  accel_buffer[0]=0;
  accel_buffer[1]=0;
  accel_buffer[2]=0;
  gyro_buffer[0]=0;
  gyro_buffer[1]=0;
  gyro_buffer[2]=0;
}

void MPU9255_read_sensor(uint8_t address){

  accel_buffer[0] = MPU9255_read_sensor_reg(address,ACCEL_XOUT_H,ACCEL_XOUT_L)-cal_accel_buffer[0];
  accel_buffer[1] = MPU9255_read_sensor_reg(address,ACCEL_YOUT_H,ACCEL_YOUT_L)-cal_accel_buffer[1];
  accel_buffer[2] = MPU9255_read_sensor_reg(address,ACCEL_XOUT_H,ACCEL_XOUT_L)-cal_accel_buffer[2];
  gyro_buffer[0] = MPU9255_read_sensor_reg(address,GYRO_XOUT_H,GYRO_XOUT_L)-cal_gyro_buffer[0];
  gyro_buffer[1] = MPU9255_read_sensor_reg(address,GYRO_YOUT_H,GYRO_YOUT_L)-cal_gyro_buffer[1];
  gyro_buffer[2] = MPU9255_read_sensor_reg(address,GYRO_ZOUT_H,GYRO_ZOUT_L)-cal_gyro_buffer[2];
//  NRF_LOG_INFO("AX: %d | AY: %d | AZ: %d | GX: %d | GY: %d | GZ: %d ",accel_buffer[0],accel_buffer[1],accel_buffer[2],gyro_buffer[0],gyro_buffer[1],gyro_buffer[2]);

}

uint16_t* MPU9255_get_accel_values(void){
  return accel_buffer;
}

uint16_t* MPU9255_get_gyro_values(void){
  return gyro_buffer;
}
/**
 * @brief Function for reading data from temperature sensor.
 */
uint16_t MPU9255_read_sensor_reg(uint8_t address,uint8_t reg_high,uint8_t reg_low)
{
    uint8_t reg_read_data[2];
    ret_code_t err_code = nrf_drv_twi_tx(&m_twi, address, &reg_high, sizeof(uint8_t), false);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_twi_rx(&m_twi, address, &reg_read_data[0], sizeof(accel_buffer[0]));

    err_code = nrf_drv_twi_tx(&m_twi, address, &reg_low, sizeof(uint8_t), false);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_twi_rx(&m_twi, address, &reg_read_data[1], sizeof(accel_buffer[0]));

    uint16_t data_to_return = ((uint16_t)reg_read_data[0]<<8)+reg_read_data[1];
    APP_ERROR_CHECK(err_code);
    return data_to_return;
}

void MPU9255_init(uint8_t address){
    ret_code_t err_code = nrf_drv_twi_tx(&m_twi, address, PWR_MGMT_1, sizeof(uint8_t), false);
    APP_ERROR_CHECK(err_code);
    err_code = nrf_drv_twi_tx(&m_twi, address, SMPLRT_DIV, sizeof(uint8_t), false);
    APP_ERROR_CHECK(err_code);
    
    err_code = nrf_drv_twi_tx(&m_twi, address, CONFIG, sizeof(uint8_t), false);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_twi_tx(&m_twi, address, GYRO_CONFIG, sizeof(uint8_t), false);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_twi_tx(&m_twi, address, ACCEL_CONFIG, sizeof(uint8_t), false);
    APP_ERROR_CHECK(err_code);

}

