import esphome.config_validation as cv
import esphome.codegen as cg
from esphome import pins
from esphome.components import uart, sensor
from esphome.const import CONF_ID, CONF_NAME

turbidity_ns = cg.esphome_ns.namespace('turbidity_sensor')
TurbiditySensor = turbidity_ns.class_('TurbiditySensor', sensor.Sensor, cg.PollingComponent)

CONF_S1_PIN = "s1_pin"
CONF_S2_PIN = "s2_pin"
CONF_S3_PIN = "s3_pin"

CONFIG_SCHEMA = (
    sensor.sensor_schema(TurbiditySensor)
    .extend(
        {
            cv.Required(CONF_ID): cv.declare_id(TurbiditySensor),
            cv.Required(CONF_S1_PIN): pins.gpio_output_pin_schema,
            cv.Required(CONF_S2_PIN): pins.gpio_output_pin_schema,
            cv.Required(CONF_S3_PIN): pins.gpio_output_pin_schema,
            cv.Optional(CONF_NAME, default="Turbidity Sensor"): cv.string,
        }
    )
    .extend(uart.UART_DEVICE_SCHEMA)
)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield sensor.register_sensor(var, config)

    # Configure sensor pins
    s1_pin = yield cg.gpio_pin_expression(config[CONF_S1_PIN])
    cg.add(var.set_s1_pin(s1_pin))
    
    s2_pin = yield cg.gpio_pin_expression(config[CONF_S2_PIN])
    cg.add(var.set_s2_pin(s2_pin))
    
    s3_pin = yield cg.gpio_pin_expression(config[CONF_S3_PIN])
    cg.add(var.set_s3_pin(s3_pin))

    uart_var = yield cg.get_variable(config[uart.CONF_UART_ID])
    cg.add(var.set_uart(uart_var))
