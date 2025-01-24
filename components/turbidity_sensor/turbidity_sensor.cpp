#include "turbidity_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace turbidity_sensor {

static const char *TAG = "turbidity_sensor";

void TurbiditySensor::setup() {
  // Setup code here
}

void TurbiditySensor::update() {
  if (expander_ == nullptr || uart_ == nullptr) {
    ESP_LOGW(TAG, "Expander or UART not set");
    return;
  }

  expander_->select_channel(channel_);
  
  // Read data from the sensor via UART
  std::string buffer;
  uint8_t c;
  while (uart_->available() > 0) {
    if (uart_->read_byte(&c)) {
      if (c == '\n') {
        break;
      }
      if (c != '\r') {
        buffer += static_cast<char>(c);
      }
    }
  }
  
  if (!buffer.empty()) {
    float value = atof(buffer.c_str());
    publish_state(value);
  }
}

}  // namespace turbidity_sensor
}  // namespace esphome
