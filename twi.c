#include "twi.h"



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
void calibrate(address){
  
  flush_buffers();

  int calibration_range = 100;
  uint32_t cal_AX =0;
  uint32_t cal_AY =0;
  uint32_t cal_AZ =0;
  uint32_t cal_GX =0;
  uint32_t cal_GY =0;
  uint32_t cal_GZ =0;
  for(int i=0;i<calibration_range;i++){
    cal_AX += twi_read_sensor_reg(address,ACCEL_XOUT_H,ACCEL_XOUT_L);
    cal_AY += twi_read_sensor_reg(address,ACCEL_YOUT_H,ACCEL_YOUT_L);
    cal_AZ += twi_read_sensor_reg(address,ACCEL_ZOUT_H,ACCEL_ZOUT_L);
    cal_GX += twi_read_sensor_reg(address,GYRO_XOUT_H,GYRO_XOUT_L);
    cal_GY += twi_read_sensor_reg(address,GYRO_YOUT_H,GYRO_YOUT_L);
    cal_GZ += twi_read_sensor_reg(address,GYRO_ZOUT_H,GYRO_ZOUT_L);
    nrf_delay_ms(10);
  }
  
  cal_accel_buffer[0] = cal_AX/calibration_range;
  cal_accel_buffer[1] = cal_AY/calibration_range;
  cal_accel_buffer[2] = cal_AZ/calibration_range;
  cal_gyro_buffer[0] = cal_GX/calibration_range;
  cal_gyro_buffer[1] = cal_GY/calibration_range;
  cal_gyro_buffer[2] = cal_GZ/calibration_range;
  NRF_LOG_INFO("callibration Accel_data: AX: %d | AY: %d | AZ: %d | GX: %d | GY: %d | GZ: %d ",cal_accel_buffer[0],cal_accel_buffer[1],cal_accel_buffer[2],cal_gyro_buffer[0],cal_gyro_buffer[1],cal_gyro_buffer[2]);

  flush_buffers();
}

void flush_buffers(){
  accel_buffer[0]=0;
  accel_buffer[1]=0;
  accel_buffer[2]=0;
  gyro_buffer[0]=0;
  gyro_buffer[1]=0;
  gyro_buffer[2]=0;
}

void twi_read_sensor(uint8_t address){

  accel_buffer[0] = twi_read_sensor_reg(address,ACCEL_XOUT_H,ACCEL_XOUT_L)-cal_accel_buffer[0];
  accel_buffer[1] = twi_read_sensor_reg(address,ACCEL_YOUT_H,ACCEL_YOUT_L)-cal_accel_buffer[1];
  accel_buffer[2] = twi_read_sensor_reg(address,ACCEL_XOUT_H,ACCEL_XOUT_L)-cal_accel_buffer[2];
  gyro_buffer[0] = twi_read_sensor_reg(address,GYRO_XOUT_H,GYRO_XOUT_L)-cal_gyro_buffer[0];
  gyro_buffer[1] = twi_read_sensor_reg(address,GYRO_YOUT_H,GYRO_YOUT_L)-cal_gyro_buffer[1];
  gyro_buffer[2] = twi_read_sensor_reg(address,GYRO_ZOUT_H,GYRO_ZOUT_L)-cal_gyro_buffer[2];
  NRF_LOG_INFO("AX: %d | AY: %d | AZ: %d | GX: %d | GY: %d | GZ: %d ",accel_buffer[0],accel_buffer[1],accel_buffer[2],gyro_buffer[0],gyro_buffer[1],gyro_buffer[2]);

}
/**
 * @brief Function for reading data from temperature sensor.
 */
uint16_t twi_read_sensor_reg(uint8_t address,uint8_t reg_high,uint8_t reg_low)
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

void twi_init (void){
    ret_code_t err_code;

    const nrf_drv_twi_config_t twi_config = {
       .scl                = ARDUINO_SCL_PIN,
       .sda                = ARDUINO_SDA_PIN,
       .frequency          = NRF_DRV_TWI_FREQ_100K,
       .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
       .clear_bus_init     = false
    };

    err_code = nrf_drv_twi_init(&m_twi, &twi_config, NULL, NULL);
    APP_ERROR_CHECK(err_code);

    nrf_drv_twi_enable(&m_twi);
}


/**
 * @brief Function for find twi sensor.
 */
int twi_find_device(void)
{
    ret_code_t err_code;
    uint8_t address = 0;
    uint8_t sample_data;
    bool detected_device = false;

    NRF_LOG_INFO("TWI scanner started.");
    NRF_LOG_FLUSH();

    for (address = 1; address <= TWI_ADDRESSES; address++)
    {
        err_code = nrf_drv_twi_rx(&m_twi, address, &sample_data, sizeof(sample_data));
        if (err_code == NRF_SUCCESS)
        {
            detected_device = true;
            NRF_LOG_INFO("TWI device detected at address 0x%x.", address);
            NRF_LOG_FLUSH();
            break;
        }
        NRF_LOG_FLUSH();
    }

    if (!detected_device)
    {
        NRF_LOG_INFO("No device was found.");
        NRF_LOG_FLUSH();
    }

    return address;
}

/** @} */
