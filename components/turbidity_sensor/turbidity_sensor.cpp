#include "turbidity_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace turbidity_sensor {

static const char *const TAG = "turbidity.sensor";

void TurbiditySensor::update() {
  if (this->expander_parent_ != nullptr) {
    this->expander_parent_->select_channel(this->channel_);
  }

  this->request_data_();
  
  if (!this->wait_for_response_()) {
    ESP_LOGW(TAG, "No response from turbidity sensor");
    return;
  }

  float value;
  if (this->parse_response_(this->rx_buffer_, value)) {
    ESP_LOGD(TAG, "Parsed value: %.2f", value);
    this->publish_state(value);
  } else {
    ESP_LOGW(TAG, "Failed to parse response");
  }
}

void TurbiditySensor::request_data_() {
  static const uint8_t dirty_command[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A};
  static const uint8_t adc_command[] = {0x01, 0x03, 0x00, 0x01, 0x00, 0x01, 0xD5, 0xCA};

  const uint8_t *command = (this->type_ == TurbiditySensorType::TURBIDITY) ? dirty_command : adc_command;
  
  for (size_t i = 0; i < sizeof(dirty_command); ++i) {
    this->uart_parent_->write(command[i]);
  }
}

bool TurbiditySensor::wait_for_response_() {
  delay(100); // Allow time for response
  return this->uart_parent_->available() > 0;
}

bool TurbiditySensor::parse_response_(const std::vector<uint8_t> &response, float &value) {
  if (response.size() < 7) {
    ESP_LOGW(TAG, "Invalid response length");
    return false;
  }

  uint16_t high_byte = response[3];
  uint16_t low_byte = response[4];
  
  value = static_cast<float>((high_byte << 8) | low_byte);
  
  return true;
}

} // namespace turbidity_sensor
} // namespace esphome
