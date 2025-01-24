import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID

turbidity_sensor_ns = cg.esphome_ns.namespace('turbidity_sensor')
TurbiditySensor = turbidity_sensor_ns.class_('TurbiditySensor', sensor.Sensor)

CONFIG_SCHEMA = sensor.sensor_schema().extend({
    cv.GenerateID(): cv.declare_id(TurbiditySensor),
    # Add other configuration variables here
})

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield sensor.register_sensor(var, config)
    # Add other code generation logic here
