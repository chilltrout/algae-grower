#pragma once

#include "esphome.h"

class AtlasSerialExpander : public Component {
 public:
  AtlasSerialExpander() = default;

  void set_pins(GPIOPin *s1_pin, GPIOPin *s2_pin, GPIOPin *s3_pin) {
    s1_pin_ = s1_pin;
    s2_pin_ = s2_pin;
    s3_pin_ = s3_pin;
  }

  void setup() override {
    s1_pin_->setup();
    s2_pin_->setup();
    s3_pin_->setup();
  }

  void select_channel(uint8_t channel) {
    s1_pin_->digital_write(channel & 0x01);
    s2_pin_->digital_write((channel >> 1) & 0x01);
    s3_pin_->digital_write((channel >> 2) & 0x01);
  }

 protected:
  GPIOPin *s1_pin_;
  GPIOPin *s2_pin_;
  GPIOPin *s3_pin_;
};
