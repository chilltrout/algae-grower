#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace turbidity_sensor {

class TurbiditySensor : public sensor::Sensor, public PollingComponent {
 public:
  TurbiditySensor(GPIOPin *s1_pin, GPIOPin *s2_pin, GPIOPin *s3_pin, UARTComponent *uart, int channel);
  void setup() override;
  void update() override;
  void dump_config() override;

 protected:
  GPIOPin *s1_pin_;
  GPIOPin *s2_pin_;
  GPIOPin *s3_pin_;
  UARTComponent *uart_bus_;
  int channel_;
};

}  // namespace turbidity_sensor
}  // namespace esphome
