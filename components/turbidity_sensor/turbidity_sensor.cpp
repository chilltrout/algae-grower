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
  while (uart_->available()) {
    uint8_t c;
    if (uart_->read_byte(&c)) {
      buffer.push_back(c);
      if (buffer.size() >= 5) break;  // We have a complete frame
    }
  }
  
  if (buffer.size() >= 5) {  // Expecting 5 bytes response
    uint8_t turbidity_raw = buffer[3];  // Use the 4th byte as the turbidity value
    float turbidity = turbidity_raw * 0.1f;  // Convert to NTU (adjust this conversion if needed)
    publish_state(turbidity);
    ESP_LOGD(TAG, "Raw: %d, Turbidity: %.1f NTU", turbidity_raw, turbidity);
  } else {
    ESP_LOGW(TAG, "Incomplete response: received %d bytes", buffer.size());
  }
}

}  // namespace turbidity_sensor
}  // namespace esphome
