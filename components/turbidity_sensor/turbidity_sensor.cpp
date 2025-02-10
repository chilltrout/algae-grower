#include "turbidity_sensor.h"

void TurbiditySensor::read_turbidity() {
  const uint8_t request[] = {0x18, 0x05, 0x00, 0x01, 0x0D};
  this->write_array(request, sizeof(request));
  delay(5);  // Allow the sensor to respond

  if (this->available() >= 5) {
    uint8_t response[5];
    this->read_array(response, 5);

    // Validate static parts of the response
    if (response[0] == 0x18 && response[1] == 0x05 && response[4] == 0x0D) {
      // Extract dynamic data (turbidity value)
      uint8_t turbidity_value = response[3];
      if (this->turbidity_sensor != nullptr) {
        this->turbidity_sensor->publish_state(static_cast<float>(turbidity_value));
      }
    } else {
      ESP_LOGW("TurbiditySensor", "Invalid turbidity response received");
    }
  } else {
    ESP_LOGW("TurbiditySensor", "No response received for turbidity");
  }
}

void TurbiditySensor::read_adc() {
  const uint8_t request[] = {0x18, 0x05, 0x00, 0x02, 0x0D};
  this->write_array(request, sizeof(request));
  delay(5);  // Allow the sensor to respond

  if (this->available() >= 6) {
    uint8_t response[6];
    this->read_array(response, 6);

    // Validate static parts of the response
    if (response[0] == 0x18 && response[1] == 0x06 && response[5] == 0x0D) {
      // Extract dynamic data (12-bit ADC value)
      uint16_t adc_value = (response[3] << 8) | response[4];
      if (this->adc_sensor != nullptr) {
        this->adc_sensor->publish_state(static_cast<float>(adc_value));
      }
    } else {
      ESP_LOGW("TurbiditySensor", "Invalid ADC response received");
    }
  } else {
    ESP_LOGW("TurbiditySensor", "No response received for ADC");
  }
}
