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
    cv.Required(CONF_S1_PIN): cv.pins.gpio_output_pin_schema,
    cv.Required(CONF_S2_PIN): cv.pins.gpio_output_pin_schema,
    cv.Required(CONF_S3_PIN): cv.pins.gpio_output_pin_schema,
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    s1 = await gpio.setup_gpio_output_pin(config[CONF_S1_PIN])
    s2 = await gpio.setup_gpio_output_pin(config[CONF_S2_PIN])
    s3 = await gpio.setup_gpio_output_pin(config[CONF_S3_PIN])

    cg.add(var.set_pins(s1, s2, s3))
