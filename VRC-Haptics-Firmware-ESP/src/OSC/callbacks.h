#ifndef CALLBACKS_H 
#define CALLBACKS_H

#include <ArduinoOSCWiFi.h>
#include "globals.hpp"
#include "osc.h"
#include "software_defines.h"

namespace Haptics  {
namespace Wireless { 
    inline void printRaw();
    inline void updateMotorVals();
    void motorMessage_callback(const OscMessage& message);
    void printOSCMessage(const OscMessage& message);


} // namespace Wireless
} // namespace Haptics

#endif // CALLBACKS_H