import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome.const import (
    CONF_ID,
    CONF_NAME,
    CONF_TYPE,
    UNIT_EMPTY,
    ICON_EMPTY,
    CONF_UNIT_OF_MEASUREMENT,
    CONF_ICON,
    CONF_ACCURACY_DECIMALS,
)

DEPENDENCIES = ["uart", "atlas_serial_expander"]

# Define TurbiditySensor component
turbidity_ns = cg.esphome_ns.namespace("turbidity_sensor")
TurbiditySensor = turbidity_ns.class_("TurbiditySensor", cg.PollingComponent, uart.UARTDevice)

# Enums
TurbiditySensorType = turbidity_ns.enum("TurbiditySensorType")
TURBIDITY_SENSOR_TYPE_OPTIONS = {
    "turbidity": TurbiditySensorType.TURBIDITY,
    "adc": TurbiditySensorType.ADC,
}

# Configuration options
CONF_UART_ID = "uart_id"
CONF_EXPANDER_ID = "expander_id"
CONF_CHANNEL = "channel"
CONF_TURBIDITY_SENSOR = "turbidity_sensor"
CONF_ADC_SENSOR = "adc_sensor"

# Sensor Schemas
TURBIDITY_SENSOR_SCHEMA = sensor.sensor_schema(
    UNIT_EMPTY,
    ICON_EMPTY,
    1,  # Precision for dirty value
)

ADC_SENSOR_SCHEMA = sensor.sensor_schema(
    UNIT_EMPTY,
    ICON_EMPTY,
    0,  # Precision for raw ADC value
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(TurbiditySensor),
        cv.Required(CONF_NAME): cv.string,
        cv.Required(CONF_UART_ID): cv.use_id(uart.UARTDevice),
        cv.Required(CONF_EXPANDER_ID): cv.use_id(cg.global_ns.class_("atlas_serial_expander::AtlasSerialExpander")),
        cv.Required(CONF_CHANNEL): cv.int_range(min=0, max=7),
        cv.Required(CONF_TYPE): cv.enum(TURBIDITY_SENSOR_TYPE_OPTIONS, upper=False),
        cv.Optional(CONF_TURBIDITY_SENSOR): TURBIDITY_SENSOR_SCHEMA,
        cv.Optional(CONF_ADC_SENSOR): ADC_SENSOR_SCHEMA,
    }
).extend(cv.polling_component_schema("60s")).extend(uart.UART_DEVICE_SCHEMA)


# Build function to register sensors
async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], config[CONF_NAME])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    # Get references to parent objects
    uart_bus = await cg.get_variable(config[CONF_UART_ID])
    cg.add(var.set_uart_parent(uart_bus))

    expander = await cg.get_variable(config[CONF_EXPANDER_ID])
    cg.add(var.set_expander_parent(expander))

    cg.add(var.set_type(config[CONF_TYPE]))

    # Register dirty sensor (optional)
    if CONF_TURBIDITY_SENSOR in config:
        turbidity_sensor = await sensor.new_sensor(config[CONF_TURBIDITY_SENSOR])
        cg.add(var.set_turbidity_sensor(turbidity_sensor))

    # Register ADC sensor (optional)
    if CONF_ADC_SENSOR in config:
        adc_sensor = await sensor.new_sensor(config[CONF_ADC_SENSOR])
        cg.add(var.set_adc_sensor(adc_sensor))

    cg.add(var.set_channel(config[CONF_CHANNEL]))
