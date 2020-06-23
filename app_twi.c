#include "app_twi.h"

void twi_init (void){ // add global flag for init 
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
        address = 0;
        NRF_LOG_INFO("No device was found.");
        NRF_LOG_FLUSH();
        
    }

    return address;
}
