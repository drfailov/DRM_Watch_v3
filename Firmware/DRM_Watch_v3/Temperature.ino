#include "driver/temp_sensor.h"
bool temperatureInit = false;


float temperature(){
  if(!temperatureInit){
    temp_sensor_config_t temp_sensor = TSENS_CONFIG_DEFAULT();
    temp_sensor.dac_offset = TSENS_DAC_L2;  // TSENS_DAC_L2 is default; L4(-40°C ~ 20°C), L2(-10°C ~ 80°C), L1(20°C ~ 100°C), L0(50°C ~ 125°C)
    temp_sensor_set_config(temp_sensor);
    temp_sensor_start();
    float result = 0;
    delay(1);
    temp_sensor_read_celsius(&result);
    temperatureInit = true;
  }

  float result = 0;
  temp_sensor_read_celsius(&result);
  return result;
}