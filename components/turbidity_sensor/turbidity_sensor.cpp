#include "turbidity_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace turbidity_sensor {

static const char *TAG = "turbidity_sensor";

void TurbiditySensor::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Turbidity Sensor on channel %u...", this->channel_);
  // Additional setup code if necessary
}

void TurbiditySensor::update() {
  ESP_LOGD(TAG, "Updating Turbidity Sensor on channel %u...", this->channel_);

  // Example: Set the pins according to the channel
  this->platform_->get_s1_pin()->digital_write((this->channel_ & 0x01) != 0);
  this->platform_->get_s2_pin()->digital_write((this->channel_ & 0x02) != 0);
  this->platform_->get_s3_pin()->digital_write((this->channel_ & 0x04) != 0);

  // Wait for the multiplexer to switch channels
  delay(10);

  // Read data from the sensor via UART
  // Ensure the UART buffer is clear
  while (this->uart_->available()) {
    this->uart_->read();
  }

  // Send a command to the sensor to request data
  const char *command = "R";
  this->uart_->write_str(command);
  this->uart_->write_str("\r");

  // Wait for the sensor to respond
  delay(100);

  // Read the response
  String response;
  while (this->uart_->available()) {
    char c = this->uart_->read();
    if (c == '\r') {
      break;
    }
    response += c;
  }

  // Parse the response
  float turbidity = response.toFloat();
  if (turbidity == 0 && response != "0.0") {
    ESP_LOGW(TAG, "Failed to read turbidity data");
    return;
  }

  // Publish the turbidity value
  this->publish_state(turbidity);
}

}  // namespace turbidity_sensor
}  // namespace esphome
