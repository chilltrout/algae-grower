import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID
from . import sensor as turbidity_sensor_sensor

DEPENDENCIES = ['uart']
AUTO_LOAD = ['sensor']

CONFIG_SCHEMA = turbidity_sensor_sensor.CONFIG_SCHEMA
DEPENDENCIES = turbidity_sensor_sensor.DEPENDENCIES

async def to_code(config):
    await sensor.register_sensor(config)
