#include "atlas_serial_expander.h"
#include "esphome/core/log.h"

namespace esphome {
namespace atlas_serial_expander {

static const char *TAG = "atlas_serial_expander";

void AtlasSerialExpander::setup() {
  // Setup code here
  ESP_LOGD(TAG, "Setting up Atlas Serial Expander");
  if (s1_pin_ != nullptr) s1_pin_->setup();
  if (s2_pin_ != nullptr) s2_pin_->setup();
  if (s3_pin_ != nullptr) s3_pin_->setup();
}

void AtlasSerialExpander::loop() {
  // Loop code here (if needed)
}

void AtlasSerialExpander::select_channel(uint8_t channel) {
  ESP_LOGD(TAG, "Selecting channel %d", channel);
  if (s1_pin_ != nullptr) s1_pin_->digital_write(channel & 0x01);
  if (s2_pin_ != nullptr) s2_pin_->digital_write((channel >> 1) & 0x01);
  if (s3_pin_ != nullptr) s3_pin_->digital_write((channel >> 2) & 0x01);
}

}  // namespace atlas_serial_expander
}  // namespace esphome
