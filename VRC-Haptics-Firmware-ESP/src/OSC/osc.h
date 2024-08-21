void startOSCWifi();
void oscTick();
void printMotorDuty();

//OSC overloads
void sendOSC(const String& oscAddress, const bool& value);
void sendOSC(const String& oscAddress, const uint16_t& value);
void sendOSC(const String& oscAddress, const uint8_t& value);
void sendOSC(const String& oscAddress, const int16_t& value);
void sendOSC(const String& oscAddress, const int8_t& value);
void sendOSC(const String& oscAddress, const float_t& value);
void sendOSC(const String& oscAddress, char *value, uint16_t size);

