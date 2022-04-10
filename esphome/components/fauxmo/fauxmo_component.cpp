#include "fauxmo_component.h"

namespace esphome{
namespace fauxmo {

static const char *const TAG = "fauxmo";

FauxmoComponent::FauxmoComponent(){

}

void FauxmoComponent::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Fauxmo...");

  // By default, fauxmoESP creates it's own webserver on the defined port
  // The TCP port must be 80 for gen3 devices (default is 1901)
  // This has to be done before the call to enable()
  this->fauxmo.createServer(true); // not needed, this is the default value
  this->fauxmo.setPort(80); // This is required for gen3 devices

  // You have to call enable(true) once you have a WiFi connection
  // You can enable or disable the library at any moment
  // Disabling it will prevent the devices from being discovered and switched
  this->fauxmo.enable(true);

  // You can use different ways to invoke alexa to modify the devices state:
  // "Alexa, turn yellow lamp on"
  // "Alexa, turn on yellow lamp
  // "Alexa, set yellow lamp to fifty" (50 means 50% of brightness, note, this example does not use this functionality)

  // Add virtual devices
  //fauxmo.addDevice(ID_YELLOW);


  this->fauxmo.onSetState([&](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
      ESP_LOGV("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
      for(auto& device : this->devices_list){
      if (device == device_name) {
          ESP_LOGV("[onSetState] Device '%s' matched", device.c_str());
          return;
      }
      }
  }); 
}
void FauxmoComponent::loop() {
  // fauxmoESP uses an async TCP server but a sync UDP server
  // Therefore, we have to manually poll for UDP packets
  this->fauxmo.handle();
  // You can also log messages
  //ESP_LOGD("custom", "The GPIO pin 5 is HIGH!");
}

void FauxmoComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Fauxmo:");
  ESP_LOGCONFIG(TAG, "Registered Devices:");
  for(auto& device : this->devices_list){
    ESP_LOGCONFIG(TAG, "  %s", device.c_str());
  }
}

float FauxmoComponent::get_setup_priority() const { return esphome::setup_priority::AFTER_WIFI; }

void FauxmoComponent::register_device(const std::string& device_name){
  ESP_LOGCONFIG(TAG, "Register device '%s' ...", device_name.c_str());
  for(auto& device : this->devices_list)
      if (device == device_name) return;
  this->devices_list.push_back(device_name);

  // Add virtual devices
  this->fauxmo.addDevice(device_name.c_str());
}

} // namespace fauxmo
} // namespace esphome
