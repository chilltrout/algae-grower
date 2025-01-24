import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome.const import CONF_ID, CONF_NAME, CONF_CHANNEL, CONF_UART_ID

DEPENDENCIES = ['uart']

turbidity_ns = cg.esphome_ns.namespace('turbidity_sensor')
TurbiditySensor = turbidity_ns.class_('TurbiditySensor', sensor.Sensor, cg.Component)
TurbidityPlatform = turbidity_ns.class_('TurbidityPlatform', cg.Component)

CONF_S1_PIN = 's1_pin'
CONF_S2_PIN = 's2_pin'
CONF_S3_PIN = 's3_pin'

turbidity_platform_schema = cv.Schema({
    cv.Required(CONF_ID): cv.declare_id(TurbidityPlatform),
    cv.Required(CONF_S1_PIN): cv.pin,
    cv.Required(CONF_S2_PIN): cv.pin,
    cv.Required(CONF_S3_PIN): cv.pin,
})

CONFIG_SCHEMA = sensor.sensor_schema().extend({
    cv.GenerateID(): cv.declare_id(TurbiditySensor),
    cv.Required(CONF_NAME): cv.string,
    cv.Required(CONF_CHANNEL): cv.int_range(min=0, max=7),
    cv.Required(CONF_UART_ID): cv.use_id(uart.UARTComponent),
}).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], config[CONF_NAME])
    yield cg.register_component(var, config)
    yield sensor.register_sensor(var, config)

    platform = cg.global_ns.get('turbidity_platform')
    var.set_platform(platform)

    uart_component = yield cg.get_variable(config[CONF_UART_ID])
    var.set_uart_id(uart_component)

    var.set_channel(config[CONF_CHANNEL])

def turbidity_platform_to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)

    s1_pin = yield cg.gpio_pin_expression(config[CONF_S1_PIN])
    var.set_s1_pin(s1_pin)

    s2_pin = yield cg.gpio_pin_expression(config[CONF_S2_PIN])
    var.set_s2_pin(s2_pin)

    s3_pin = yield cg.gpio_pin_expression(config[CONF_S3_PIN])
    var.set_s3_pin(s3_pin)

    cg.add_global(turbidity_ns.namespace().set_platform(var))
