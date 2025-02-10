#include "turbidity_sensor.h"

namespace esphome {
namespace turbidity_sensor {

void TurbiditySensor::read_sensors() {
  // Request turbidity (dirty) value
  const uint8_t turbidity_request[] = {0x18, 0x05, 0x00, 0x01, 0x0D};
  this->write_array(turbidity_request, sizeof(turbidity_request));
  delay(5);  // Wait for the sensor to respond

  if (this->available() >= 5) {
    uint8_t response[5];
    this->read_array(response, 5);
    if (response[0] == 0x18 && response[1] == 0x05 && response[4] == 0x0D) {
      uint8_t turbidity_value = response[3];
      if (turbidity_sensor_ != nullptr) {
        turbidity_sensor_->publish_state(static_cast<float>(turbidity_value));
      }
    } else {
      ESP_LOGW("TurbiditySensor", "Invalid turbidity response");
    }
  } else {
    ESP_LOGW("TurbiditySensor", "No turbidity response");
  }

  // Request raw ADC value
  const uint8_t adc_request[] = {0x18, 0x05, 0x00, 0x02, 0x0D};
  this->write_array(adc_request, sizeof(adc_request));
  delay(5);  // Wait for the sensor to respond

  if (this->available() >= 6) {
    uint8_t response[6];
    this->read_array(response, 6);
    if (response[0] == 0x18 && response[1] == 0x06 && response[5] == 0x0D) {
      uint16_t adc_value = (response[3] << 8) | response[4];
      if (adc_sensor_ != nullptr) {
        adc_sensor_->publish_state(static_cast<float>(adc_value));
      }
    } else {
      ESP_LOGW("TurbiditySensor", "Invalid ADC response");
    }
  } else {
    ESP_LOGW("TurbiditySensor", "No ADC response");
  }
}

}  // namespace turbidity_sensor
}  // namespace esphome

