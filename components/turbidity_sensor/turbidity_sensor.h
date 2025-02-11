#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/atlas_serial_expander/atlas_serial_expander.h"

namespace esphome {
namespace turbidity_sensor {

enum class TurbiditySensorType {
  TURBIDITY,
  ADC,
};

class TurbiditySensor : public PollingComponent, public sensor::Sensor {
 public:
  void set_uart_parent(uart::UARTComponent *uart_parent) { this->uart_parent_ = uart_parent; }
  void set_expander_parent(esphome::atlas_serial_expander::AtlasSerialExpander *expander_parent) { this->expander_parent_ = expander_parent; }
  void set_channel(uint8_t channel) { this->channel_ = channel; }
  void set_type(TurbiditySensorType type) { this->type_ = type; }

 protected:
  void update() override;
  void request_data_();
  bool wait_for_response_();
  bool parse_response_(const std::vector<uint8_t> &response, float &value);

 private:
  uart::UARTComponent *uart_parent_{nullptr};
  esphome::atlas_serial_expander::AtlasSerialExpander *expander_parent_{nullptr};
  uint8_t channel_{0};
  TurbiditySensorType type_{TurbiditySensorType::TURBIDITY};
};

}  // namespace turbidity_sensor
}  // namespace esphome
