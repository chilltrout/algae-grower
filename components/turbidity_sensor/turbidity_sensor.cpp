#include "turbidity_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace turbidity_sensor {

static const char *TAG = "turbidity_sensor";
static const std::vector<uint8_t> EXPECTED_HEADER = {0x18, 0x05};

void TurbiditySensor::setup() {
  // Setup code here
}

void TurbiditySensor::update() {
  if (expander_ == nullptr || uart_ == nullptr) {
    ESP_LOGW(TAG, "Expander or UART not set");
    return;
  }

  expander_->select_channel(channel_);
  
  // Send command to request data
  static const uint8_t COMMAND[] = {0x18, 0x05, 0x00, 0x01, 0x0D};
  uart_->write_array(COMMAND, sizeof(COMMAND));
  
  // Wait for response
  delay(100);  // Adjust this delay if needed

  // Read data from the sensor via UART
  std::vector<uint8_t> buffer;
  uint32_t start_time = millis();
  while (millis() - start_time < 500) {  // 500ms timeout
    if (uart_->available()) {
      uint8_t c;
      if (uart_->read_byte(&c)) {
        buffer.push_back(c);
        if (buffer.size() >= 8) break;  // We have a complete frame
      }
    }
    yield();
  }
  
  if (buffer.size() >= 8) {  // Expecting 8 bytes response
    // Check if the response starts with 0x18 and ends with 0x0D
    if (buffer == 0x18 && buffer[7] == 0x0D) {
      uint16_t raw_value = (buffer[5] << 8) | buffer[6];
      float turbidity = raw_value * 0.1f;  // Convert to NTU
      publish_state(turbidity);
      ESP_LOGD(TAG, "Turbidity: %.2f NTU", turbidity);
    } else {
      ESP_LOGW(TAG, "Invalid response header or trailer: %02X %02X", buffer, buffer[7]);
    }
  } else {
    ESP_LOGW(TAG, "Incomplete response: received %d bytes", buffer.size());
  }
}

}  // namespace turbidity_sensor
}  // namespace esphome
