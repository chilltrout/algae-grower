#include "turbidity_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace turbidity_sensor {

static const char *TAG = "turbidity_sensor";

void TurbiditySensor::setup() {
  this->set_timeout(50, [this]() { this->update(); });
}

void TurbiditySensor::update() {
  uint8_t request[] = {0x18, 0x05, 0x00, 0x01, 0x0D};
  this->write_array(request, sizeof(request));

  uint8_t response[8];
  if (!this->read_array(response, sizeof(response))) {
    ESP_LOGW(TAG, "Error reading turbidity sensor data");
    this->status_set_warning();
    return;
  }

  // Check if the response is valid (frame header and trailer)
  if (response[0] != 0x18 || response[7] != 0x0D) {
    ESP_LOGW(TAG, "Invalid response from turbidity sensor");
    this->status_set_warning();
    return;
  }

  // Extract the turbidity value from the response
  uint16_t raw_value = (response[5] << 8) | response[6];
  float turbidity = raw_value / 10.0f;  // Convert to NTU

  ESP_LOGD(TAG, "Turbidity: %.1f NTU", turbidity);
  this->publish_state(turbidity);
  this->status_clear_warning();
}

void TurbiditySensor::dump_config() {
  ESP_LOGCONFIG(TAG, "Turbidity Sensor:");
  LOG_SENSOR("  ", "Turbidity", this);
}

}  // namespace turbidity_sensor
}  // namespace esphome
