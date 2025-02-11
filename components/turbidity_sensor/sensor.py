import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome.const import CONF_ID, CONF_NAME

DEPENDENCIES = ["uart", "atlas_serial_expander"]

turbidity_ns = cg.esphome_ns.namespace("turbidity_sensor")
TurbiditySensor = turbidity_ns.class_("TurbiditySensor", cg.PollingComponent, sensor.Sensor)

CONF_TYPE = "type"
CONF_CHANNEL = "channel"
CONF_EXPANDER_ID = "expander_id"
CONF_UART_ID = "uart_id"

TURBIDITY_SENSOR_TYPE = cv.enum({
    "turbidity": "TURBIDITY",
    "adc": "ADC",
})

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(TurbiditySensor),
        cv.Required(CONF_NAME): cv.string,
        cv.Required(CONF_UART_ID): cv.use_id(uart.UARTComponent),
        cv.Required(CONF_TYPE): TURBIDITY_SENSOR_TYPE,
        cv.Required(CONF_CHANNEL): cv.int_,
        cv.Optional(CONF_EXPANDER_ID): cv.use_id(cg.Component),
    }
).extend(cv.polling_component_schema("60s"))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], TurbiditySensor)
    await cg.register_component(var, config)

    uart_component = await cg.get_variable(config[CONF_UART_ID])
    cg.add(var.set_uart_parent(uart_component))

    if CONF_EXPANDER_ID in config:
        expander_parent = await cg.get_variable(config[CONF_EXPANDER_ID])
        cg.add(var.set_expander_parent(expander_parent))

    cg.add(var.set_channel(config[CONF_CHANNEL]))
    
    if config[CONF_TYPE] == "turbidity":
        cg.add(var.set_type(cg.RawExpression("turbidity_sensor::TurbiditySensorType::TURBIDITY")))
    elif config[CONF_TYPE] == "adc":
        cg.add(var.set_type(cg.RawExpression("turbidity_sensor::TurbiditySensorType::ADC")))
