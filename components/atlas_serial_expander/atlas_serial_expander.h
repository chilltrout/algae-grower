#pragma once

#include "esphome/core/component.h"
#include "esphome/core/gpio.h"

namespace esphome {
namespace atlas_serial_expander {

class AtlasSerialExpander : public Component {
 public:
  void set_pins(GPIOPin *s1, GPIOPin *s2, GPIOPin *s3) {
    s1_pin_ = s1;
    s2_pin_ = s2;
    s3_pin_ = s3;
  }

  void setup() override;
  void loop() override;

  void select_channel(uint8_t channel);

 protected:
  GPIOPin *s1_pin_;
  GPIOPin *s2_pin_;
  GPIOPin *s3_pin_;
};

}  // namespace atlas_serial_expander
}  // namespace esphome
