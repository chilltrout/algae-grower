import esphome.config_validation as cv
import esphome.codegen as cg
from esphome import pins
from esphome.components import uart, sensor
from esphome.const import CONF_ID, CONF_NAME

# Define a namespace for the turbidity sensor
turbidity_ns = cg.esphome_ns.namespace('turbidity_sensor')
TurbiditySensor = turbidity_ns.class_('TurbiditySensor', sensor.Sensor, cg.PollingComponent)

# Configuration schema for your component
CONFIG_SCHEMA = (
    sensor.sensor_schema(TurbiditySensor)
    .extend(
        {
            cv.Required(CONF_ID): cv.declare_id(TurbiditySensor),
            cv.Required('s1_pin'): cv.GPIO_PIN,
            cv.Required('s2_pin'): cv.GPIO_PIN,
            cv.Required('s3_pin'): cv.GPIO_PIN,
            cv.Optional(CONF_NAME, default="Turbidity Sensor"): cv.string,
        }
    )
    .extend(uart.UART_DEVICE_SCHEMA)
)

# Method to translate YAML configuration keys into C++ code
def to_code(config):
    # Create a new variable for the turbidity sensor
    var = cg.new_Pvariable(config[CONF_ID])
    # Register the component within ESPHome
    yield cg.register_component(var, config)
    # Register the sensor so it can be used
    yield sensor.register_sensor(var, config)

    # Set the pins from the configuration
    cg.add(var.set_s1_pin(config['s1_pin']))
    cg.add(var.set_s2_pin(config['s2_pin']))
    cg.add(var.set_s3_pin(config['s3_pin']))

    # Associate the UART configuration with the turbidity sensor
    uart_var = yield cg.get_variable(config[uart.CONF_UART_ID])
    cg.add(var.set_uart(uart_var))
