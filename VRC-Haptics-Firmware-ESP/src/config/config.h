// config.h
#ifndef CONFIG_H
#define CONFIG_H
#include "Arduino.h"
#include "software_defines.h"

namespace Haptics {

    /// user-configurable, persistent values
    struct Config {
        char wifi_ssid[32];
        char wifi_password[64];
        char mdns_name[12];
        uint8_t i2c_scl;
        uint8_t i2c_sda;
        uint32_t i2c_speed;
        uint16_t motor_map_i2c_num;
        uint16_t motor_map_i2c[MAX_I2C_MOTORS]; // needs to be an array
        uint16_t motor_map_ledc_num;
        uint16_t motor_map_ledc[MAX_LEDC_MOTORS]; // needs to be an array
        uint16_t config_version;
    };

    // sensible defaults
    const Config defaultConfig = {
    "SlimeServer", //ssid
    "95815480", //password
    "VRCHaptics", // name that will be displayed on the gui
    SCL, // scl default of board
    SDA, // sda default of board
    400000U, // i2c clock
    0, // i2c num
    {0}, 
    0,
    {0},
    CONFIG_VERSION,
    };

    void loadConfig();
    void saveConfig();

    inline Config conf;

} // namespace Haptics

#endif // CONFIG_H