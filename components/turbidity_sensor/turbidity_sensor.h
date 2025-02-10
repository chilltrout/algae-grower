#pragma once

#include "esphome.h"

namespace esphome {
namespace turbidity_sensor {

class TurbiditySensor : public Component, public UARTDevice {
 public:
  TurbiditySensor(UARTComponent *parent) : UARTDevice(parent) {}

  void set_turbidity_sensor(Sensor *sensor) { turbidity_sensor_ = sensor; }
  void set_adc_sensor(Sensor *sensor) { adc_sensor_ = sensor; }

  void read_sensors();

 protected:
  Sensor *turbidity_sensor_{nullptr};
  Sensor *adc_sensor_{nullptr};
};

}  // namespace turbidity_sensor
}  // namespace esphome
