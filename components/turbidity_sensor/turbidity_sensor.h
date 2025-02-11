
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

class TurbiditySensor : public PollingComponent, public uart::UARTDevice {
 public:
  void set_uart_parent(uart::UARTDevice *uart_parent) { this->uart_parent_ = uart_parent; }
  void set_expander_parent(esphome::atlas_serial_expander::AtlasSerialExpander* expander_parent) { this->expander_parent_ = expander_parent; }
  void set_type(TurbiditySensorType type) { this->type_ = type; }
  void set_channel(uint8_t channel) { this->channel_ = channel; }
  void set_turbidity_sensor(sensor::Sensor *turbidity_sensor) { this->turbidity_sensor_ = turbidity_sensor; }
  void set_adc_sensor(sensor::Sensor *adc_sensor) { this->adc_sensor_ = adc_sensor; }

 protected:
  void update() override;
  bool wait_for_response_();
  bool parse_dirty_response_(const std::vector<uint8_t> &response, float &value);
  bool parse_adc_response_(const std::vector<uint8_t> &response, float &value);
  void write_command_(const std::vector<uint8_t>& command);
  std::vector<uint8_t> read_response_();
  void request_data_();

  TurbiditySensorType type_ = TurbiditySensorType::TURBIDITY;
  sensor::Sensor *turbidity_sensor_{nullptr};
  sensor::Sensor *adc_sensor_{nullptr};
  uart::UARTDevice *uart_parent_{nullptr};
  esphome::atlas_serial_expander::AtlasSerialExpander* expander_parent_{nullptr};
  uint8_t channel_{0};
};

}  // namespace turbidity_sensor
}  // namespace esphome
