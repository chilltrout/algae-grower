#include "turbidity_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace turbidity_sensor {

static const char *TAG = "turbidity_sensor";
static const uint8_t COMMAND[] = {0x18, 0x05, 0x00, 0x01, 0x0D};

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
  uart_->write_array(COMMAND, sizeof(COMMAND));
  
  // Wait for response
  delay(100);  // Adjust this delay if needed

  // Read data from the sensor via UART
  std::vector<uint8_t> buffer;
  while (uart_->available()) {
    uint8_t c;
    if (uart_->read_byte(&c)) {
      buffer.push_back(c);
    }
  }
  
  if (buffer.size() >= 8) {  // Expecting 8 bytes response
    // Parse the response
    if (buffer == 0x18 && buffer[1] == 0x05) {
      uint16_t ad_value = (buffer[3] << 8) | buffer[4];
      float turbidity = ad_value * 0.1f;  // Convert to NTU (adjust this formula if needed)
      publish_state(turbidity);
      ESP_LOGD(TAG, "Turbidity: %.2f NTU", turbidity);
    } else {
      ESP_LOGW(TAG, "Invalid response");
    }
  } else {
    ESP_LOGW(TAG, "Incomplete response");
  }
}

}  // namespace turbidity_sensor
}  // namespace esphome
