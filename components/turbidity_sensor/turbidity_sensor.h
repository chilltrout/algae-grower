#pragma once
#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/polling.h"
#include "esphome/components/atlas_serial_expander/atlas_serial_expander.h"

namespace esphome {
namespace turbidity_sensor {

enum class TurbiditySensorType {
  TURBIDITY,
  ADC,
};

struct TurbiditySensorConfig {
  int channel;
  TurbiditySensorType type;
};

class TurbiditySensor : public PollingComponent, public sensor::Sensor {
 public:
  void set_channel(int channel) { channel_ = channel; }
  int get_channel() const { return channel_; }

  void set_type(TurbiditySensorType type) { type_ = type; }
  TurbiditySensorType get_type() const { return type_; }

  void set_uart_parent(uart::UARTComponent *parent) { uart_parent_ = parent; }
  void set_expander(atlas_serial_expander::AtlasSerialExpander *expander) { expander_ = expander; }

  void setup() override;
  void update() override;
  void dump_config() override;
  float get_setup_priority() const override { return esphome::setup_priority::DATA; }

 protected:
  void request_data_();
  void read_data_();

  int channel_{0};
  TurbiditySensorType type_{TurbiditySensorType::TURBIDITY};
  uart::UARTComponent *uart_parent_{nullptr};
  atlas_serial_expander::AtlasSerialExpander *expander_{nullptr};
};

}  // namespace turbidity_sensor
}  // namespace esphome
