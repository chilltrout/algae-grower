import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import gpio
from esphome.const import CONF_ID

DEPENDENCIES = ['gpio']

atlas_serial_expander_ns = cg.esphome_ns.namespace('atlas_serial_expander')
AtlasSerialExpander = atlas_serial_expander_ns.class_('AtlasSerialExpander', cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(AtlasSerialExpander),
    cv.Required('s1_pin'): gpio.gpio_output_pin_schema,
    cv.Required('s2_pin'): gpio.gpio_output_pin_schema,
    cv.Required('s3_pin'): gpio.gpio_output_pin_schema,
}).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)

    s1 = yield gpio.setup_gpio_output_pin(config['s1_pin'])
    s2 = yield gpio.setup_gpio_output_pin(config['s2_pin'])
    s3 = yield gpio.setup_gpio_output_pin(config['s3_pin'])

    cg.add(var.set_pins(s1, s2, s3))
