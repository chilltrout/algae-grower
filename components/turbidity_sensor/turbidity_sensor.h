#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace turbidity_sensor {

enum class TurbiditySensorType {
  TURBIDITY,
  ADC,
};

class TurbiditySensor : public PollingComponent, public uart::UARTDevice {
 public:
  void set_type(TurbiditySensorType type) { this->type_ = type; }
  void set_turbidity_sensor(sensor::Sensor *turbidity_sensor) { this->turbidity_sensor_ = turbidity_sensor; }
  void set_adc_sensor(sensor::Sensor *adc_sensor) { this->adc_sensor_ = adc_sensor; }

 protected:
  void update() override;
  void process_response_();
  float extract_value(const std::vector<uint8_t> &response);

  TurbiditySensorType type_ = TurbiditySensorType::TURBIDITY;
  sensor::Sensor *turbidity_sensor_{nullptr};
  sensor::Sensor *adc_sensor_{nullptr};
  std::vector<uint8_t> rx_buffer_;
};

}  // namespace turbidity_sensor
}  // namespace esphome
