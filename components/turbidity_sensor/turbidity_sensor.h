// TurbiditySensor External Component Implementation
#include "esphome.h"

class TurbiditySensor : public PollingComponent, public Sensor {
 public:
  TurbiditySensor(GPIOPin *s1, GPIOPin *s2, GPIOPin *s3, UARTComponent *uart, int channel)
      : PollingComponent(10000), s1_pin(s1), s2_pin(s2), s3_pin(s3), uart_bus(uart), channel(channel) {}

  void setup() override {
    ESP_LOGCONFIG(TAG, "Setting up Turbidity Sensor on channel %d", channel);
    s1_pin->setup();
    s2_pin->setup();
    s3_pin->setup();
  }

  void update() override {
    // Select the channel using the GPIO pins
    s1_pin->digital_write((channel & 0x01) != 0);
    s2_pin->digital_write((channel & 0x02) != 0);
    s3_pin->digital_write((channel & 0x04) != 0);

    delay(100);  // Allow the switching to stabilize

    // Send the read command to the sensor
    uint8_t command[] = {0x18, 0x05, 0x00, 0x01, 0x0D};
    uart_bus->write_array(command, sizeof(command));

    delay(100);  // Wait for the sensor to respond

    // Read the response
    std::vector<uint8_t> response;
    while (uart_bus->available()) {
      response.push_back(uart_bus->read());
    }

    if (response.size() >= 5 && response[0] == 0x18 && response[1] == 0x05) {
      float turbidity_value = static_cast<float>(response[3]);
      publish_state(turbidity_value);
    } else {
      ESP_LOGW(TAG, "Invalid response from turbidity sensor on channel %d", channel);
      publish_state(NAN);
    }
  }

 protected:
  GPIOPin *s1_pin;
  GPIOPin *s2_pin;
  GPIOPin *s3_pin;
  UARTComponent *uart_bus;
  int channel;
  static const char *const TAG;
};

const char *const TurbiditySensor::TAG = "TurbiditySensor";
