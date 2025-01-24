#include "turbidity_sensor.h"

void TurbiditySensor::setup() {
  // Any setup code needed for the turbidity sensor
}

void TurbiditySensor::update() {
  expander_->select_channel(channel_);
  
  // Read data from the sensor via UART
  // This is a placeholder. You'll need to implement the actual communication protocol
  // with your turbidity sensor here.
  
  char buffer[32];
  if (uart_->read_line(buffer, sizeof(buffer))) {
    float value = atof(buffer);
    publish_state(value);
  }
}

void TurbiditySensor::set_expander(AtlasSerialExpander *expander) {
  expander_ = expander;
}

void TurbiditySensor::set_channel(uint8_t channel) {
  channel_ = channel;
}
