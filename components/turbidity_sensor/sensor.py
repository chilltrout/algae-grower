import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import sensor, uart

# Define namespace
CONF_TURBIDITY_SENSOR = "turbidity_sensor"
turbidity_ns = cg.esphome_ns.namespace(CONF_TURBIDITY_SENSOR)
TurbiditySensor = turbidity_ns.class_("TurbiditySensor", sensor.Sensor, cg.Component)

# Configuration options
CONFIG_SCHEMA = sensor.sensor_schema().extend({
    cv.GenerateID(): cv.declare_id(TurbiditySensor),
    cv.Required("s1_pin"): pins.gpio_output_pin_schema,
    cv.Required("s2_pin"): pins.gpio_output_pin_schema,
    cv.Required("s3_pin"): pins.gpio_output_pin_schema,
    cv.Required("uart_id"): cv.use_id(uart.UARTComponent),
    cv.Required("channel"): cv.int_range(min=0, max=7),
})

# Register the component
async def to_code(config):
    var = cg.new_Pvariable(config[CONF_TURBIDITY_SENSOR])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)

    # Assign pins
    s1_pin = await cg.gpio_pin_expression(config["s1_pin"])
    cg.add(var.set_s1_pin(s1_pin))
    s2_pin = await cg.gpio_pin_expression(config["s2_pin"])
    cg.add(var.set_s2_pin(s2_pin))
    s3_pin = await cg.gpio_pin_expression(config["s3_pin"])
    cg.add(var.set_s3_pin(s3_pin))

    # Assign UART
    uart_comp = await cg.get_variable(config["uart_id"])
    cg.add(var.set_uart(uart_comp))

    # Assign channel
    cg.add(var.set_channel(config["channel"]))
