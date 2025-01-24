import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID

DEPENDENCIES = ['uart']
AUTO_LOAD = ['sensor']

turbidity_sensor_ns = cg.esphome_ns.namespace('turbidity_sensor')
TurbiditySensor = turbidity_sensor_ns.class_('TurbiditySensor', cg.Component, sensor.Sensor)

CONF_EXPANDER_ID = 'expander_id'
CONF_CHANNEL = 'channel'

CONFIG_SCHEMA = sensor.sensor_schema().extend({
    cv.GenerateID(): cv.declare_id(TurbiditySensor),
    cv.Required(CONF_CHANNEL): cv.int_range(min=0, max=7),
    cv.Required(CONF_EXPANDER_ID): cv.use_id(cg.Component),
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = await cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)

    expander = await cg.get_variable(config[CONF_EXPANDER_ID])
    cg.add(var.set_expander(expander))
    cg.add(var.set_channel(config[CONF_CHANNEL]))
