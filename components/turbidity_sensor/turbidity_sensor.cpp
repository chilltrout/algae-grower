]#include "turbidity_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace turbidity_sensor {

static const char *const TAG = "turbidity.sensor";

void TurbiditySensor::update() {
  if (type_ == TURBIDITY_TYPE::TURBIDITY) {
    this->request_dirty_();
    if (!this->wait_for_response_()) {
      ESP_LOGW(TAG, "No response from dirty sensor");
      return;
    }
    float value;
    if (this->parse_dirty_response_(this->rx_buffer_, value)) {
      ESP_LOGD(TAG, "Parsed dirty value: %.2f", value);
      this->publish_state(value);
    } else {
      ESP_LOGW(TAG, "Failed to parse dirty response");
    }
  } else {
    this->request_adc_();
    if (!this->wait_for_response_()) {
      ESP_LOGW(TAG, "No response from ADC sensor");
      return;
    }
    float value;
    if (this->parse_adc_response_(this->rx_buffer_, value)) {
      ESP_LOGD(TAG, "Parsed ADC value: %.2f", value);
      this->publish_state(value);
    } else {
      ESP_LOGW(TAG, "Failed to parse ADC response");
    }
  }
}

void TurbiditySensor::request_dirty_() {
  static const uint8_t dirty_command[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A};
  this->uart_->write(dirty_command, sizeof(dirty_command));
  this->rx_buffer_.clear();
}

void TurbiditySensor::request_adc_() {
  static const uint8_t adc_command[] = {0x01, 0x03, 0x00, 0x01, 0x00, 0x01, 0xD5, 0xCA};
  this->uart_->write(adc_command, sizeof(adc_command));
  this->rx_buffer_.clear();
}

bool TurbiditySensor::wait_for_response_() {
  // Implement your waiting and timeout logic here
  // Return true if a valid response is received, false otherwise
  delay(100);  // Short delay to allow response
  return this->uart_->available() > 0;  // Check if data is available
}

bool TurbiditySensor::parse_dirty_response_(const std::vector<uint8_t> &response, float &value) {
  if (response.size() < 7) {
    ESP_LOGW(TAG, "Invalid response from sensor: incorrect length");
    return false;
  }
  uint16_t high_byte = response[3];
  uint16_t low_byte = response[4];
  value = (high_byte << 8) | low_byte;
  return true;
}

bool TurbiditySensor::parse_adc_response_(const std::vector<uint8_t> &response, float &value) {
  if (response.size() < 7) {
    ESP_LOGW(TAG, "Invalid response from sensor: incorrect length");
    return false;
  }
  uint16_t high_byte = response[3];
  uint16_t low_byte = response[4];
  value = (high_byte << 8) | low_byte;
  return true;
}

}  // namespace turbidity_sensor
}  // namespace esphome
