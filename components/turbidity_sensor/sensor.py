import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.components import uart, sensor
from esphome.const import CONF_ID, CONF_NAME

turbidity_ns = cg.esphome_ns.namespace('turbidity_sensor')
TurbiditySensor = turbidity_ns.class_('TurbiditySensor', sensor.Sensor, cg.PollingComponent)

CONFIG_SCHEMA = (
    sensor.sensor_schema(TurbiditySensor)
    .extend(
        {
            cv.Required(CONF_ID): cv.declare_id(TurbiditySensor),
            cv.Required('s1_pin'): cv.gpio_pin,  # Correct method for GPIO pin
            cv.Required('s2_pin'): cv.gpio_pin,
            cv.Required('s3_pin'): cv.gpio_pin,
            cv.Optional(CONF_NAME, default="Turbidity Sensor"): cv.string,
        }
    )
    .extend(uart.UART_DEVICE_SCHEMA)
)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield sensor.register_sensor(var, config)

    # Assume these methods exist in your C++ code to set the pins
    cg.add(var.set_s1_pin(config['s1_pin']))
    cg.add(var.set_s2_pin(config['s2_pin']))
    cg.add(var.set_s3_pin(config['s3_pin']))

    uart_var = yield cg.get_variable(config[uart.CONF_UART_ID])
    cg.add(var.set_uart(uart_var))
