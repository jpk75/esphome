#pragma once
#include "esphome.h"
#include "fauxmoESP.h"
#include <list>
#include <string>

namespace esphome{
namespace fauxmo {
class FauxmoComponent : public Component {
 public:  
  FauxmoComponent();
  void setup() override;
  void loop() override;
  void dump_config() override;
  float get_setup_priority() const override;
  void register_device(const std::string& device_name);

protected:
  fauxmoESP fauxmo;
  std::list<std::string> devices_list;
};

} // namespace fauxmo
} // namespace esphome
