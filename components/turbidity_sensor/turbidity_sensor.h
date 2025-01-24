#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/gpio.h"

namespace esphome {
namespace turbidity_sensor {

class TurbidityPlatform : public Component {
 public:
  void set_s1_pin(GPIOPin *pin) { s1_pin_ = pin; }
  void set_s2_pin(GPIOPin *pin) { s2_pin_ = pin; }
  void set_s3_pin(GPIOPin *pin) { s3_pin_ = pin; }

  GPIOPin *get_s1_pin() const { return s1_pin_; }
  GPIOPin *get_s2_pin() const { return s2_pin_; }
  GPIOPin *get_s3_pin() const { return s3_pin_; }

 private:
  GPIOPin *s1_pin_;
  GPIOPin *s2_pin_;
  GPIOPin *s3_pin_;
};

class TurbiditySensor : public sensor::Sensor, public Component {
 public:
  void set_channel(uint8_t channel) { channel_ = channel; }
  void set_uart_id(UARTComponent *uart) { uart_ = uart; }
  void set_platform(TurbidityPlatform *platform) { platform_ = platform; }

  void setup() override;
  void update() override;

 protected:
  uint8_t channel_;
  UARTComponent *uart_;
  TurbidityPlatform *platform_;
};
