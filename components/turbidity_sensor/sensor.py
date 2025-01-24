import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome.const import CONF_ID, CONF_NAME

DEPENDENCIES = ["uart"]

turbidity_ns = cg.esphome_ns.namespace("turbidity_sensor")
TurbiditySensor = turbidity_ns.class_("TurbiditySensor", sensor.Sensor, cg.Component)

CONF_S1_PIN = "s1_pin"
CONF_S2_PIN = "s2_pin"
CONF_S3_PIN = "s3_pin"
CONF_CHANNEL = "channel"

CONFIG_SCHEMA = (
    sensor.sensor_schema(unit_of_measurement="NTU", accuracy_decimals=2)
    .extend(
        {
            cv.GenerateID(): cv.declare_id(TurbiditySensor),
            cv.Required(CONF_NAME): cv.string,
            cv.Optional(CONF_CHANNEL): cv.int_range(min=0, max=7),
        }
    )
    .extend(uart.UART_DEVICE_SCHEMA)
)

GLOBAL_PIN_CONFIG_SCHEMA = cv.Schema(
    {
        cv.Required(CONF_S1_PIN): cv.int_,
        cv.Required(CONF_S2_PIN): cv.int_,
        cv.Required(CONF_S3_PIN): cv.int_,
    }
)

global_pins = cg.esphome_ns.struct("TurbidityPinConfig")

async def to_code(config):
    uart_component = await cg.get_variable(config[CONF_UART_ID])
    sensor_var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(sensor_var, config)
    await sensor.register_sensor(sensor_var, config)
    cg.add(sensor_var.set_uart(uart_component))

    if CONF_CHANNEL in config:
        cg.add(sensor_var.set_channel(config[CONF_CHANNEL]))

    # Global pin configuration
    pins = global_pins()
    pins.s1 = config[CONF_S1_PIN]
    pins.s2 = config[CONF_S2_PIN]
    pins.s3 = config[CONF_S3_PIN]
    cg.add_global(turbidity_ns.namespace, "pins", pins)
