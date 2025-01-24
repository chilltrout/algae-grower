import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

DEPENDENCIES = ['gpio']

atlas_serial_expander_ns = cg.esphome_ns.namespace('atlas_serial_expander')
AtlasSerialExpander = atlas_serial_expander_ns.class_('AtlasSerialExpander', cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(AtlasSerialExpander),
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
