import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome.const import CONF_ID, CONF_NAME, UNIT_EMPTY, ICON_EMPTY

DEPENDENCIES = ["uart", "atlas_serial_expander"]

turbidity_ns = cg.esphome_ns.namespace("turbidity_sensor")
TurbiditySensor = turbidity_ns.class_("TurbiditySensor", cg.PollingComponent, uart.UARTDevice)

CONF_TURBIDITY_SENSOR = "turbidity_sensor"
CONF_ADC_SENSOR = "adc_sensor"
CONF_TYPE = "type"
CONF_UART_ID = "uart_id"
CONF_EXPANDER_ID = "expander_id"
CONF_CHANNEL = "channel"

TURBIDITY_SENSOR_TYPE = cv.enum({
    "turbidity": 0,
    "adc": 1,
})

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(TurbiditySensor),
        cv.Required(CONF_NAME): cv.string,
        cv.Required(CONF_UART_ID): cv.use_id(uart.UARTComponent),
        cv.Optional(CONF_EXPANDER_ID): cv.use_id(cg.Component),  # Assuming atlas_serial_expander is a Component
        cv.Required(CONF_CHANNEL): cv.int_,
        cv.Required(CONF_TYPE): TURBIDITY_SENSOR_TYPE,
        cv.Optional(CONF_TURBIDITY_SENSOR): sensor.sensor_schema(
            {
                UNIT_EMPTY: UNIT_EMPTY,
                ICON_EMPTY: ICON_EMPTY,
            }
        ),
        cv.Optional(CONF_ADC_SENSOR): sensor.sensor_schema(
            {
                UNIT_EMPTY: UNIT_EMPTY,
                ICON_EMPTY: ICON_EMPTY,
            }
        ),
    }
).extend(cv.polling_component_schema("60s"))


async def to_code(config):
  var = cg.new_Pvariable(config[CONF_ID], config[CONF_NAME])
  await cg.register_component(var, config)

  uart_component = await cg.get_variable(config[CONF_UART_ID])
  cg.add(var.set_uart_parent(uart_component))

  if CONF_EXPANDER_ID in config:
    expander_component = await cg.get_variable(config[CONF_EXPANDER_ID])
    cg.add(var.set_expander_parent(expander_component))

  cg.add(var.set_channel(config[CONF_CHANNEL]))

  if config[CONF_TYPE] == 0:
    cg.add(var.set_type(cg.RawExpression("turbidity_sensor::TurbiditySensorType::TURBIDITY")))
    if CONF_TURBIDITY_SENSOR in config:
      sens = await sensor.new_sensor(config[CONF_TURBIDITY_SENSOR])
      cg.add(var.set_turbidity_sensor(sens))
  else:
    cg.add(var.set_type(cg.RawExpression("turbidity_sensor::TurbiditySensorType::ADC")))
    if CONF_ADC_SENSOR in config:
      sens = await sensor.new_sensor(config[CONF_ADC_SENSOR])
      cg.add(var.set_adc_sensor(sens))

