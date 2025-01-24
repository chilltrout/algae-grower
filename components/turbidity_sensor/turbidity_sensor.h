#pragma once

#include "esphome.h"
#include "../atlas_serial_expander/atlas_serial_expander.h"

class TurbiditySensor : public Component, public Sensor {
 public:
  TurbiditySensor(UARTComponent *parent, AtlasSerialExpander *expander, uint8_t channel)
      : uart_(parent), expander_(expander), channel_(channel) {}

  void setup() override {
    // Initialize the sensor
  }

  void update() override {
    expander_->select_channel(channel_);
    // Read data from the sensor
  }

 protected:
  UARTComponent *uart_;
  AtlasSerialExpander *expander_;
  uint8_t channel_;
};
