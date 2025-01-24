import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome.const import CONF_ID, CONF_CHANNEL
from .. import atlas_serial_expander

DEPENDENCIES = ['uart', 'atlas_serial_expander']
AUTO_LOAD = ['sensor']

CONF_EXPANDER_ID = 'expander_id'

turbidity_sensor_ns = cg.esphome_ns.namespace('turbidity_sensor')
TurbiditySensor = turbidity_sensor_ns.class_('TurbiditySensor', cg.Component, sensor.Sensor)

CONFIG_SCHEMA = sensor.sensor_schema().extend({
    cv.GenerateID(): cv.declare_id(TurbiditySensor),
    cv.Required(CONF_CHANNEL): cv.int_range(min=0, max=7),
    cv.Required(CONF_EXPANDER_ID): cv.use_id(atlas_serial_expander.AtlasSerialExpander),
    cv.Required('uart_id'): cv.use_id(uart.UARTComponent),
})

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)

    uart_component = await cg.get_variable(config['uart_id'])
    cg.add(var.set_uart(uart_component))

    expander = await cg.get_variable(config[CONF_EXPANDER_ID])
    cg.add(var.set_expander(expander))
    cg.add(var.set_channel(config[CONF_CHANNEL]))
