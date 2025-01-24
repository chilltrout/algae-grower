#ifndef TURBIDITY_SENSOR_H
#define TURBIDITY_SENSOR_H

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/log.h"

namespace esphome {
namespace turbidity_sensor {

class TurbiditySensor : public sensor::Sensor, public PollingComponent {
 public:
  TurbiditySensor(GPIOPin *s1, GPIOPin *s2, GPIOPin *s3, uart::UARTComponent *uart, int channel)
      : PollingComponent(10000), s1_pin_(s1), s2_pin_(s2), s3_pin_(s3), uart_bus_(uart), channel_(channel) {}

  void setup() override {
    ESP_LOGCONFIG(TAG, "Setting up Turbidity Sensor on channel %d", channel_);
    s1_pin_->setup();
    s2_pin_->setup();
    s3_pin_->setup();
  }

  void update() override {
    // Set the channel on the serial port expander
    s1_pin_->digital_write((channel_ & 0x01) > 0);
    s2_pin_->digital_write((channel_ & 0x02) > 0);
    s3_pin_->digital_write((channel_ & 0x04) > 0);

    // Small delay to stabilize switching
    delay(100);

    // Send the request command to the sensor
    uint8_t command[] = {0x18, 0x05, 0x00, 0x01, 0x0D};
    uart_bus_->write_array(command, sizeof(command));

    // Wait for a response
    delay(100);

    // Read the response
    std::vector<uint8_t> response;
    while (uart_bus_->available()) {
      response.push_back(uart_bus_->read());
    }

    // Validate and parse the response
    if (response.size() >= 5 && response[0] == 0x18 && response[1] == 0x05) {
      // Extract the turbidity value (byte 3 as per the datasheet)
      float turbidity_value = static_cast<float>(response[3]);
      ESP_LOGD(TAG, "Turbidity reading on channel %d: %.2f", channel_, turbidity_value);
      publish_state(turbidity_value);
    } else {
      ESP_LOGW(TAG, "Invalid response on channel %d", channel_);
      publish_state(NAN);
    }
  }

  void dump_config() override {
    ESP_LOGCONFIG(TAG, "Turbidity Sensor:");
    ESP_LOGCONFIG(TAG, "  Channel: %d", channel_);
    LOG_PIN("  S1 Pin: ", s1_pin_);
    LOG_PIN("  S2 Pin: ", s2_pin_);
    LOG_PIN("  S3 Pin: ", s3_pin_);
    LOG_UART("  UART Bus: ", uart_bus_);
  }

 protected:
  GPIOPin *s1_pin_;
  GPIOPin *s2_pin_;
  GPIOPin *s3_pin_;
  uart::UARTComponent *uart_bus_;
  int channel_;
  static const char *const TAG;
};

}  // namespace turbidity_sensor
}  // namespace esphome

#endif
