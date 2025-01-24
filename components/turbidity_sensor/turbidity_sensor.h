#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"
#include "../atlas_serial_expander/atlas_serial_expander.h"

namespace esphome {
namespace turbidity_sensor {

class TurbiditySensor : public sensor::Sensor, public Component {
 public:
  void set_uart(uart::UARTComponent *uart) { uart_ = uart; }
  void set_expander(atlas_serial_expander::AtlasSerialExpander *expander) { expander_ = expander; }
  void set_channel(uint8_t channel) { channel_ = channel; }
  
  void setup() override;
  void loop() override;
  void update() override;
  
 protected:
  uart::UARTComponent *uart_{nullptr};
  atlas_serial_expander::AtlasSerialExpander *expander_{nullptr};
  uint8_t channel_{0};
};

}  // namespace turbidity_sensor
}  // namespace esphome

