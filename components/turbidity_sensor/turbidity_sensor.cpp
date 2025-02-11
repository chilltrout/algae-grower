#include "turbidity_sensor.h"
#include "esphome/core/log.h"
#include <string>

namespace esphome {
namespace turbidity_sensor {

static const char *const TAG = "turbidity_sensor";

void TurbiditySensor::setup() {
  ESP_LOGD(TAG, "Setting up TurbiditySensor");
}

void TurbiditySensor::update() {
  // Switch the expander channel and request data
  if (this->expander_ != nullptr) {
    this->expander_->select_channel(this->channel_);
  } else {
    ESP_LOGW(TAG, "Expander is not set!");
    return;
  }
  this->set_timeout(50, [this]() { this->request_data_(); });
}

void TurbiditySensor::request_data_() {
  uint8_t command[5];

  if (this->type_ == TurbiditySensorType::TURBIDITY) {
    // Command to request turbidity (dirty) value
    command[0] = 0x18;
    command[1] = 0x05;
    command[2] = 0x00;
    command[3] = 0x01;
    command[4] = 0x0D;
    ESP_LOGD(TAG, "Requesting turbidity value on channel %d", this->channel_);
  } else if (this->type_ == TurbiditySensorType::ADC) {
    // Command to request ADC value
    command[0] = 0x18;
    command[1] = 0x05;
    command[2] = 0x00;
    command[3] = 0x02;
    command[4] = 0x0D;
    ESP_LOGD(TAG, "Requesting ADC value on channel %d", this->channel_);
  }

  // Send the command via UART
  if (this->uart_parent_ != nullptr) {
    this->uart_parent_->write_array(command, sizeof(command));
    this->uart_parent_->flush();
  } else {
    ESP_LOGW(TAG, "UART parent is not set!");
    return;
  }

  // Schedule reading after a short delay
  this->set_timeout(100, [this]() { this->request_data_(); });
}

void TurbiditySensor::read_data_() {
  uint8_t response[5];
  if (this->uart_parent_ != nullptr && this->uart_parent_->available() >= sizeof(response)) {
    // Read the response from UART
    this->uart_parent_->read_array(response, sizeof(response));

    // Validate the response frame
    if (response[0] == 0x18 && response[1] == 0x05 && response[4] == 0x0D) {
      float value;

      if (this->type_ == TurbiditySensorType::TURBIDITY) {
        value = static_cast<float>(response[3]);  // Dirty turbidity value
        ESP_LOGD(TAG, "Received turbidity value: %.2f on channel %d", value, this->channel_);
      } else if (this->type_ == TurbiditySensorType::ADC) {
        value = static_cast<float>(response[3]);  // ADC value
        ESP_LOGD(TAG, "Received ADC value: %.2f on channel %d", value, this->channel_);
      }

      // Publish the sensor state
      this->publish_state(value);
    } else {
      ESP_LOGW(TAG, "Invalid response frame received on channel %d", this->channel_);
    }
  } else {
    ESP_LOGW(TAG, "Not enough data available to read on channel %d", this->channel_);
  }
}

void TurbiditySensor::dump_config() {
  ESP_LOGCONFIG(TAG, "Turbidity Sensor:");
  ESP_LOGCONFIG(TAG, "  Channel: %d", this->channel_);
  ESP_LOGCONFIG(TAG, "  Type: %s", this->type_ == TurbiditySensorType::TURBIDITY ? "Turbidity" : "ADC");
  LOG_SENSOR("  ", "Turbidity", this);
  LOG_UPDATE_INTERVAL(this);
}

}  // namespace turbidity_sensor
}  // namespace esphome
