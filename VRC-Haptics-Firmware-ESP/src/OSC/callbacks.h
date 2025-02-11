#ifndef CALLBACKS_H 
#define CALLBACKS_H

#include <ArduinoOSCWiFi.h>
#include "globals.h"
#include "osc.h"
#include "software_defines.h"

namespace Haptics  {
namespace Wireless { 
    void printRaw();
    inline void updateMotorVals(Globals *globals, Config *conf);
    void motorMessage_callback(const OscMessage& message);
    void printOSCMessage(const OscMessage& message);


} // namespace Wireless
} // namespace Haptics

#endif // CALLBACKS_H