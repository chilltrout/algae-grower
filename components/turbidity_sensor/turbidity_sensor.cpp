// turbidity_sensor.cpp
#include "turbidity_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace turbidity_sensor {

static const char *TAG = "turbidity_sensor";

void TurbiditySensor::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Turbidity Sensor on channel %u", this->channel_);
}

void TurbiditySensor::update() {
  if (this->expander_ != nullptr) {
    this->expander_->select_channel(this->channel_);
  }
  this->request_dirty_();
  delay(100);
  this->request_adc_();
}

void TurbiditySensor::request_dirty_() {
  if (this->uart_ == nullptr) {
    ESP_LOGE(TAG, "UART not set for Turbidity Sensor");
    return;
  }
  
  uint8_t command[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x85, 0xDB};
  this->uart_->write_array(command, sizeof(command));
}

void TurbiditySensor::request_adc_() {
  if (this->uart_ == nullptr) {
    ESP_LOGE(TAG, "UART not set for Turbidity Sensor");
    return;
  }
  
  uint8_t command[] = {0x01, 0x03, 0x00, 0x01, 0x00, 0x01, 0xD5, 0xDB};
  this->uart_->write_array(command, sizeof(command));
}

bool TurbiditySensor::parse_dirty_response_(const std::vector<uint8_t> &response, float &value) {
  if (response.size() < 7 || response[0] != 0x01 || response[1] != 0x03 || response[2] != 0x02) {
    ESP_LOGW(TAG, "Invalid response from sensor: incorrect header or length for dirty");
    return false;
  }

  value = static_cast<float>(response[3]); // 0-255 range
  uint16_t received_crc = (response[5] << 8) | response[6];
  ESP_LOGD(TAG, "CRC Received (Dirty): %04X", received_crc);
  return true;
}

bool TurbiditySensor::parse_adc_response_(const std::vector<uint8_t> &response, float &value) {
  if (response.size() < 7 || response[0] != 0x01 || response[1] != 0x03 || response[2] != 0x02) {
    ESP_LOGW(TAG, "Invalid response from sensor: incorrect header or length for ADC");
    return false;
  }

  uint16_t raw_value = (response[3] << 8) | response[4]; // 0-4096 range
  value = static_cast<float>(raw_value);

  uint16_t received_crc = (response[5] << 8) | response[6];
  ESP_LOGD(TAG, "CRC Received (ADC): %04X", received_crc);
  return true;
}

}  // namespace turbidity_sensor
}  // namespace esphome
