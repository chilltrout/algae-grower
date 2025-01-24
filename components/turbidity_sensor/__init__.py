from esphome.components import sensor
from esphome.const import CONF_ID
from . import sensor as turbidity_sensor_sensor

CONFIG_SCHEMA = turbidity_sensor_sensor.CONFIG_SCHEMA
DEPENDENCIES = turbidity_sensor_sensor.DEPENDENCIES
async_setup_entry = sensor.sensor_async_setup_entry
