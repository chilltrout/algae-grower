#include "atlas_serial_expander.h"

namespace esphome {
namespace atlas_serial_expander {

void AtlasSerialExpander::setup() {
  s1_pin_->setup();
  s2_pin_->setup();
  s3_pin_->setup();
}

void AtlasSerialExpander::loop() {
  // Any continuous operations if needed
}

void AtlasSerialExpander::select_channel(uint8_t channel) {
  s1_pin_->digital_write(channel & 0x01);
  s2_pin_->digital_write((channel >> 1) & 0x01);
  s3_pin_->digital_write((channel >> 2) & 0x01);
}

}  // namespace atlas_serial_expander
}  // namespace esphome
