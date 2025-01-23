#include "esphome.h"

class TurbiditySensor : public PollingComponent {
 public:
  TurbiditySensor(GPIOPin *s1, GPIOPin *s2, GPIOPin *s3, UARTComponent *uart, int channel)
      : PollingComponent(10000), s1_pin(s1), s2_pin(s2), s3_pin(s3), uart_bus(uart), channel(channel) {}

  void setup() override {
    ESP_LOGCONFIG("TurbiditySensor", "Setting up Turbidity Sensor on channel %d", channel);
  }

  void update() override {
    // Set the channel on the serial port expander
    s1_pin->digital_write((channel & 0x01) > 0);
    s2_pin->digital_write((channel & 0x02) > 0);
    s3_pin->digital_write((channel & 0x04) > 0);

    // Small delay to stabilize switching
    delay(100);

    // Send the request command to the sensor
    uint8_t command[] = {0x18, 0x05, 0x00, 0x01, 0x0D};
    uart_bus->write_array(command, sizeof(command));

    // Wait for a response
    delay(100);

    // Read the response
    std::vector<uint8_t> response;
    while (uart_bus->available()) {
      response.push_back(uart_bus->read());
    }

    // Validate and parse the response
    if (response.size() >= 5 && response[0] == 0x18 && response[1] == 0x05) {
      // Extract the turbidity value (byte 3 as per the datasheet)
      float turbidity_value = static_cast<float>(response[3]);
      publish_state(turbidity_value);
    } else {
      ESP_LOGW("TurbiditySensor", "Invalid response on channel %d", channel);
      publish_state(NAN);
    }
  }

 protected:
  GPIOPin *s1_pin;
  GPIOPin *s2_pin;
  GPIOPin *s3_pin;
  UARTComponent *uart_bus;
  int channel;
};
