import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome.const import CONF_ID, CONF_NAME, CONF_TYPE

DEPENDENCIES = ["uart"]

# Define TurbiditySensor component
turbidity_ns = cg.esphome_ns.namespace("turbidity_sensor")
TurbiditySensor = turbidity_ns.class_("TurbiditySensor", cg.PollingComponent, uart.UARTDevice)

# Configuration options
CONF_UART_ID = "uart_id"
CONF_EXPANDER_ID = "expander_id"
CONF_CHANNEL = "channel"

# Supported types
TYPE_OPTIONS = {
    "turbidity": turbidity_ns.enum("TURBIDITY_TYPE_TURBIDITY"),
    "adc": turbidity_ns.enum("TURBIDITY_TYPE_ADC"),
}

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(TurbiditySensor),
        cv.Required(CONF_NAME): cv.string,
        cv.Required(CONF_UART_ID): cv.use_id(uart.UARTDevice),
        cv.Required(CONF_EXPANDER_ID): cv.use_id(cg.global_ns.class_("AtlasSerialExpander")),
        cv.Required(CONF_CHANNEL): cv.int_range(min=0, max=7),
        cv.Required(CONF_TYPE): cv.enum(TYPE_OPTIONS),  # Removed upper=True
    }
).extend(cv.polling_component_schema(DEFAULT_UPDATE_INTERVAL)).extend(uart.UART_DEVICE_SCHEMA)

# Build function to register sensors
async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    # Set parameters
    cg.add(var.set_name(config[CONF_NAME]))
    cg.add(var.set_uart_parent(await cg.get_variable(config[CONF_UART_ID])))
    cg.add(var.set_expander_parent(await cg.get_variable(config[CONF_EXPANDER_ID])))
    cg.add(var.set_channel(config[CONF_CHANNEL]))
    cg.add(var.set_type(config[CONF_TYPE]))
