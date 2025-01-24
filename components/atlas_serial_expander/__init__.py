import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import gpio
from esphome.const import CONF_ID

DEPENDENCIES = ['gpio']

atlas_serial_expander_ns = cg.esphome_ns.namespace('atlas_serial_expander')
AtlasSerialExpander = atlas_serial_expander_ns.class_('AtlasSerialExpander', cg.Component)

CONF_S1_PIN = 's1_pin'
CONF_S2_PIN = 's2_pin'
CONF_S3_PIN = 's3_pin'

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(AtlasSerialExpander),
    cv.Required('s1_pin'): cv.All(cv.int_, cv.Range(min=0)),
    cv.Required('s2_pin'): cv.All(cv.int_, cv.Range(min=0)),
    cv.Required('s3_pin'): cv.All(cv.int_, cv.Range(min=0)),
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    s1 = await cg.gpio_pin_expression(config['s1_pin'])
    s2 = await cg.gpio_pin_expression(config['s2_pin'])
    s3 = await cg.gpio_pin_expression(config['s3_pin'])

    cg.add(var.set_pins(s1, s2, s3))
