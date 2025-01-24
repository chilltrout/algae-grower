#pragma once

#include "esphome/core/component.h"
#include "esphome/components/gpio/gpio.h"

namespace esphome {
namespace atlas_serial_expander {

class AtlasSerialExpander : public Component {
 public:
  void setup() override;
  void loop() override;
};

}  // namespace atlas_serial_expander
}  // namespace esphome
