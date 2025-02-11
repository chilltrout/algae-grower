// turbidity_sensor.h
#pragma once

#include "esphome.h"
#include "esphome/components/atlas_serial_expander/atlas_serial_expander.h"

namespace esphome {
namespace turbidity_sensor {

class TurbiditySensor : public PollingComponent, public sensor::Sensor {
 public:
  void setup() override;
  void update() override;
  void set_uart(UARTComponent *uart) { this->uart_ = uart; }
  void set_expander(atlas_serial_expander::AtlasSerialExpander *expander) { this->expander_ = expander; }
  void set_channel(uint8_t channel) { this->channel_ = channel; }

 private:
  UARTComponent *uart_;
  atlas_serial_expander::AtlasSerialExpander *expander_;
  uint8_t channel_;
  void request_data_();
  bool parse_response_(const std::vector<uint8_t> &response, float &turbidity, float &adc_value);
};

}  // namespace turbidity_sensor
}  // namespace esphome
