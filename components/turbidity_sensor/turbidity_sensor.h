#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace turbidity_sensor {

enum class TURBIDITY_TYPE {
  TURBIDITY,
  ADC
};

class TurbiditySensor : public PollingComponent, public sensor::Sensor {
 public:
  void set_uart(uart::UARTComponent *uart) { this->uart_ = uart; }
  void set_type(TURBIDITY_TYPE type) { this->type_ = type; }

 protected:
  void update() override;
  void request_dirty_();
  void request_adc_();
  bool wait_for_response_();
  bool parse_dirty_response_(const std::vector<uint8_t> &response, float &value);
  bool parse_adc_response_(const std::vector<uint8_t> &response, float &value);

  TURBIDITY_TYPE type_{TURBIDITY_TYPE::TURBIDITY};
  uart::UARTComponent *uart_{nullptr};
  std::vector<uint8_t> rx_buffer_;
};

}  // namespace turbidity_sensor
}  // namespace esphome
