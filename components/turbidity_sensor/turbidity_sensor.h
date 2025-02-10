#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"
#include "../atlas_serial_expander/atlas_serial_expander.h"
#pragma once

#include "esphome.h"

class TurbiditySensor : public Component, public UARTDevice {
 public:
  Sensor *turbidity_sensor = nullptr;  // Turbidity (dirty value) sensor
  Sensor *adc_sensor = nullptr;        // ADC value sensor

  explicit TurbiditySensor(UARTComponent *parent) : UARTDevice(parent) {}

  void setup() override {}

  void loop() override {
    this->read_turbidity();
    this->read_adc();
  }

  void read_turbidity();
  void read_adc();
};

