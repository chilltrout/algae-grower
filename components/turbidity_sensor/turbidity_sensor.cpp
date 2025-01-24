#include "esphome.h"

namespace esphome {
namespace turbidity_sensor {

class TurbiditySensor : public sensor::Sensor, public PollingComponent {
 public:
  TurbiditySensor(GPIOPin *s1_pin, GPIOPin *s2_pin, GPIOPin *s3_pin, UARTComponent *uart, int channel)
      : PollingComponent(10000), s1_pin_(s1_pin), s2_pin_(s2_pin), s3_pin_(s3_pin), uart_bus_(uart), channel_(channel) {}

  void setup() override {
    ESP_LOGCONFIG("TurbiditySensor", "Setting up Turbidity Sensor on channel %d", channel_);
  }

  void update() override {
    // Set the channel on the serial port expander
    this->s1_pin_->digital_write((channel_ & 0x01) > 0);
    this->s2_pin_->digital_write((channel_ & 0x02) > 0);
    this->s3_pin_->digital_write((channel_ & 0x04) > 0);

    // Small delay to stabilize switching
    delay(100);

    // Send the request command to the sensor
    uint8_t command[] = {0x18, 0x05, 0x00, 0x01, 0x0D};
    this->uart_bus_->write_array(command, sizeof(command));

    // Wait for a response
    delay(100);

    // Read the response
    std::vector<uint8_t> response;
    while (this->uart_bus_->available()) {
      response.push_back(this->uart_bus_->read());
    }

    // Validate and parse the response
    if (response.size() >= 5 && response[0] == 0x18 && response[1] == 0x05) {
      float turbidity_value = static_cast<float>(response[3]);
      ESP_LOGD("TurbiditySensor", "Channel %d: Turbidity value = %.2f", channel_, turbidity_value);
      publish_state(turbidity_value);
    } else {
      ESP_LOGW("TurbiditySensor", "Invalid response on channel %d", channel_);
      publish_state(NAN);
    }
  }

  void dump_config() override {
    ESP_LOGCONFIG("TurbiditySensor", "Turbidity Sensor:");
    ESP_LOGCONFIG("TurbiditySensor", "  Channel: %d", channel_);
    LOG_PIN("  S1 Pin: ", s1_pin_);
    LOG_PIN("  S2 Pin: ", s2_pin_);
    LOG_PIN("  S3 Pin: ", s3_pin_);
    LOG_UART("  UART Bus: ", uart_bus_);
  }

 protected:
  GPIOPin *s1_pin_;
  GPIOPin *s2_pin_;
  GPIOPin *s3_pin_;
  UARTComponent *uart_bus_;
  int channel_;
};

}  // namespace turbidity_sensor
}  // namespace esphome
