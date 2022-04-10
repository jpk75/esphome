import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

CONF_DEVICES = "devices"

fauxmo_component_ns = cg.esphome_ns.namespace("fauxmo")
FauxmoComponentConstructor = fauxmo_component_ns.class_("FauxmoComponent", cg.Component)

MULTI_CONF = True
CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(FauxmoComponentConstructor),
        cv.Required(CONF_DEVICES): cv.All(cv.string, cv.Length(min=1)),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    #    for device in config.get(CONF_DEVICES, []):
    #        cg.add(var.register_device(device))

    cg.add(var.register_device(config.get(CONF_DEVICES, [])))
    cg.add_library("FauxmoESP", "3.4")
    cg.add_define("USE_FAUXMO")
