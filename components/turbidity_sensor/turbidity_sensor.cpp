// turbidity_sensor.cpp
#include "turbidity_sensor.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

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
  if (!this->wait_for_response_()) {
    ESP_LOGW(TAG, "No response for dirty value");
    return;
  }
  delay(100);
  this->request_adc_();
  if (!this->wait_for_response_()) {
    ESP_LOGW(TAG, "No response for ADC value");
    return;
  }
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

bool TurbiditySensor::wait_for_response_() {
  uint32_t start_time = millis();
  while (millis() - start_time < 500) {  // 500ms timeout
    if (this->uart_->available() >= 7) {
      return true;
    }
    delay(10);
  }
  return false;
}

bool TurbiditySensor::parse_dirty_response_(const std::vector<uint8_t> &response, float &value) {
  if (response.size() < 7 || response[0] != 0x01 || response[1] != 0x03 || response[2] != 0x02) {
    ESP_LOGW(TAG, "Invalid response from sensor: incorrect header or length for dirty");
    return false;
  }
  
  value = static_cast<float>(response[3]); // 0-255 range

  uint16_t received_crc = (response[5] << 8) | response[6];
  uint16_t calculated_crc = esphome::crc16(&response[0], response.size() - 2);
  if (received_crc != calculated_crc) {
    ESP_LOGW(TAG, "CRC mismatch (Dirty): Expected %04X, Got %04X", calculated_crc, received_crc);
    return false;
  }

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
  uint16_t calculated_crc = esphome::crc16(&response[0], response.size() - 2);
  if (received_crc != calculated_crc) {
    ESP_LOGW(TAG, "CRC mismatch (ADC): Expected %04X, Got %04X", calculated_crc, received_crc);
    return false;
  }

  return true;
}

}  // namespace turbidity_sensor
}  // namespace esphome
