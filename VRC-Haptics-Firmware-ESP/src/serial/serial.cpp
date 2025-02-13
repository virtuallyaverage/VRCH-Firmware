#include "serial.h"
#include "config/config.h"  // Provides Haptics::conf and Haptics::defaultConfig
#include "globals.h"        // Provides Haptics::globals
#include "logging/Logger.h" // For logger calls
#include "board_defines.h"  // Provides board-specific defines
#include "PWM/LEDC/ledc.h"
#include "OSC/callbacks.h"
#include <Arduino.h>

// Helper function to trim leading/trailing whitespace from a String.
static String trim(const String &str) {
  unsigned int start = 0;
  while (start < str.length() && isspace(str.charAt(start)))
    start++;
  unsigned int end = str.length() - 1;
  while (end >= 0 && isspace(str.charAt(end)))
    end--;
  return (end >= start) ? str.substring(start, end + 1) : "";
}

#ifdef SUPPORTS_LEDC
// Allowed LEDC pins from board definition.
const uint8_t allowedLedcPins[] = { LEDC_POSSIBLE_PINS };
const size_t numAllowedLedcPins = sizeof(allowedLedcPins) / sizeof(allowedLedcPins[0]);
#endif

#ifdef SUPPORTS_I2C
// Allowed I2C pins from board definition.
const uint8_t allowedI2cPins[] = { I2C_POSSIBLE_DATA };
const size_t numAllowedI2cPins = sizeof(allowedI2cPins) / sizeof(allowedI2cPins[0]);
#endif

template<typename... Args>
constexpr size_t count_args(const Args...) {
    return sizeof...(Args);
}

namespace Haptics {
  namespace SerialComm {

    Logging::Logger logger("Serial");

    // Forward declarations for helper functions
    static void handleGetCommand(const String &key);
    static void handleSetCommand(const String &key, const String &value);
    static void handleSetMotorCommand(const String &value);
    static void handleSetMotorMapLedcList(const String &listStr);
    static void handleSetMotorMapI2cList(const String &listStr);
    static void handleSetGenericField(const String &key, const String &value);

    // Parses the input string into command, key, and value.
    static void parseInput(const String &input, String &command, String &key, String &value) {
      int firstSpace = input.indexOf(' ');
      if (firstSpace == -1) {
        command = input;
      } else {
        command = input.substring(0, firstSpace);
        int secondSpace = input.indexOf(' ', firstSpace + 1);
        if (secondSpace == -1) {
          key = input.substring(firstSpace + 1);
        } else {
          key = input.substring(firstSpace + 1, secondSpace);
          value = input.substring(secondSpace + 1);
        }
      }
      // Normalize to uppercase.
      command.toUpperCase();
      key.toUpperCase();
    }

    // Handles GET commands.
    static void handleGetCommand(const String &key) {
      if (key == "ALL") {
        logger.info("Current Configuration:");
        logger.info("wifi_ssid: %s", conf.wifi_ssid);
        logger.info("wifi_password: %s", conf.wifi_password);
        logger.info("mdns_name: %s", conf.mdns_name);
        logger.info("i2c_scl: %d", conf.i2c_scl);
        logger.info("i2c_sda: %d", conf.i2c_sda);
        logger.info("i2c_speed: %lu", conf.i2c_speed);
        logger.info("motor_map_i2c_num: %d", conf.motor_map_i2c_num);
        {
          String motorMapI2c = "";
          for (int i = 0; i < conf.motor_map_i2c_num; i++) {
            motorMapI2c += String(conf.motor_map_i2c[i]) + " ";
          }
          logger.info("motor_map_i2c: %s", motorMapI2c.c_str());
        }
        logger.info("motor_map_ledc_num: %d", conf.motor_map_ledc_num);
        {
          String motorMapLedc = "";
          for (int i = 0; i < conf.motor_map_ledc_num; i++) {
            motorMapLedc += String(conf.motor_map_ledc[i]) + " ";
          }
          logger.info("motor_map_ledc: %s", motorMapLedc.c_str());
        }
        logger.info("config_version: %d", conf.config_version);

        /// BOARD DEFINES
        logger.info("BOARD DEFINES: (Can't be set)");
        #ifdef SUPPORTS_LEDC
        logger.infoArray("ledc pins: ", allowedLedcPins, numAllowedLedcPins);
        #else 
        logger.info("No LEDC Support");
        #endif// SUPPORTS_LEDC

        #ifdef SUPPORTS_I2C
        unsigned int speeds[] = {I2C_SPEEDS};
        logger.infoArray("I2C SCL/SDA pins: ", allowedI2cPins, numAllowedI2cPins);
        logger.infoArray("I2C speeds: ", speeds, count_args(I2C_SPEEDS));
        #else
        logger.info("No I2C Support");
        #endif // SUPPORTS_I2C
      }
      else if (key == "WIFI_SSID")
        logger.info("wifi_ssid: %s", conf.wifi_ssid);
      else if (key == "WIFI_PASSWORD")
        logger.info("wifi_password: %s", conf.wifi_password);
      else if (key == "MDNS_NAME")
        logger.info("mdns_name: %s", conf.mdns_name);
      else if (key == "I2C_SCL")
        logger.info("i2c_scl: %d", conf.i2c_scl);
      else if (key == "I2C_SDA")
        logger.info("i2c_sda: %d", conf.i2c_sda);
      else if (key == "I2C_SPEED")
        logger.info("i2c_speed: %lu", conf.i2c_speed);
      else if (key == "I2C_NUM")
        logger.info("i2c_num: %d", conf.motor_map_i2c_num);
      else if (key == "LEDC_NUM") 
        logger.info("ledc_num: %d", conf.motor_map_ledc_num);
      else if (key == "CONFIG_VERSION")
        logger.info("config_version: %d", conf.config_version);
      else
        logger.warn("Unknown key: %s", key.c_str());
    }

    // Handles SET commands.
    static void handleSetCommand(const String &key, const String &value) {
      if (key == "DEFAULT") {
        memcpy(&conf, &defaultConfig, sizeof(Config));
        Haptics::saveConfig();
        logger.info("Configuration reset to default.");
      }
      else if (key == "MOTOR") {
        handleSetMotorCommand(value);
      }
      else if (key == "MOTOR_MAP_LEDC") {
        handleSetMotorMapLedcList(value);
        Haptics::saveConfig();
        Haptics::globals.reinitLEDC = true;
      }
      else if (key == "MOTOR_MAP_I2C") {
        handleSetMotorMapI2cList(value);
        Haptics::saveConfig();
      }
      else {
        handleSetGenericField(key, value);
        Haptics::saveConfig();
      }
    }

    // Handles the "SET MOTOR <index> <value>" command.
    static void handleSetMotorCommand(const String &value) {
      int spaceIndex = value.indexOf(' ');
      if (spaceIndex == -1) {
        logger.error("Invalid MOTOR command format. Use: SET MOTOR <index> <value>");
        return;
      }
      String indexStr = value.substring(0, spaceIndex);
      String motorValueStr = value.substring(spaceIndex + 1);
      int idx = indexStr.toInt();
      uint16_t motorValue = (uint16_t)motorValueStr.toInt();
      int totalMotors = MAX_LEDC_MOTORS + MAX_I2C_MOTORS;
      if (idx >= 0 && idx < totalMotors) {
        globals.allMotorVals[idx] = motorValue;
        Wireless::updateMotorVals(); //pushes updates to their respective devices
        logger.info("Set motor value at index %d to %d", idx, motorValue);
      } else {
        logger.error("Motor index out of range.");
      }
    }

    // Handles the "SET MOTOR_MAP_LEDC_LIST <list>" command.
    static void handleSetMotorMapLedcList(const String &listStr) {
#ifdef SUPPORTS_LEDC
      int count = 0;
      String remaining = listStr;
      while (remaining.length() > 0 && count < MAX_LEDC_MOTORS) {
        int commaIndex = remaining.indexOf(',');
        String token;
        if (commaIndex == -1) {
          token = remaining;
          remaining = "";
        } else {
          token = remaining.substring(0, commaIndex);
          remaining = remaining.substring(commaIndex + 1);
        }
        token = trim(token);
        int pin = token.toInt();
        bool valid = false;
        for (size_t i = 0; i < numAllowedLedcPins; i++) {
          if (allowedLedcPins[i] == pin) {
            valid = true;
            break;
          }
        }
        if (!valid) {
          logger.error("Invalid LEDC pin: %d", pin);
          return;
        }
        conf.motor_map_ledc[count++] = (uint16_t)pin;
      }
      conf.motor_map_ledc_num = count;
      logger.info("Updated MOTOR_MAP_LEDC list with %d entries", count);
      Haptics::globals.reinitLEDC = true;
      Haptics::saveConfig();
#else
      logger.error("LEDC not supported on this board.");
#endif
    }

    // Handles the "SET MOTOR_MAP_I2C_LIST <list>" command.
    static void handleSetMotorMapI2cList(const String &listStr) {
#ifdef SUPPORTS_I2C
      int count = 0;
      String remaining = listStr;
      while (remaining.length() > 0 && count < MAX_I2C_MOTORS) {
        int commaIndex = remaining.indexOf(',');
        String token;
        if (commaIndex == -1) {
          token = remaining;
          remaining = "";
        } else {
          token = remaining.substring(0, commaIndex);
          remaining = remaining.substring(commaIndex + 1);
        }
        token = trim(token);
        int pin = token.toInt(); // TODO: add I2c Sanatization
        conf.motor_map_i2c[count++] = (uint16_t)pin;
      }
      conf.motor_map_i2c_num = count;
      logger.info("Updated MOTOR_MAP_I2C list with %d entries", count);
      Haptics::saveConfig();
#else
      logger.error("I2C not supported on this board.");
#endif
    }

    // Handles generic SET field commands.
    static void handleSetGenericField(const String &key, const String &value) {
      if (key == "WIFI_SSID") {
        value.toCharArray(conf.wifi_ssid, sizeof(conf.wifi_ssid));
        logger.info("Set wifi_ssid to %s", conf.wifi_ssid);
      }
      else if (key == "WIFI_PASSWORD") {
        value.toCharArray(conf.wifi_password, sizeof(conf.wifi_password));
        logger.info("Set wifi_password to %s", conf.wifi_password);
      }
      else if (key == "MDNS_NAME") {
        value.toCharArray(conf.mdns_name, sizeof(conf.mdns_name));
        logger.info("Set mdns_name to %s", conf.mdns_name);
      }
      else if (key == "I2C_SCL") {
        conf.i2c_scl = (uint8_t)value.toInt();
        logger.info("Set i2c_scl to %d", conf.i2c_scl);
      }
      else if (key == "I2C_SDA") {
        conf.i2c_sda = (uint8_t)value.toInt();
        logger.info("Set i2c_sda to %d", conf.i2c_sda);
      }
      else if (key == "I2C_SPEED") {
        uint32_t newSpeed = (uint32_t)value.toInt();
        if (newSpeed != 4000000U && newSpeed != 100000U) {
          logger.error("Invalid I2C_SPEED. Allowed values: 4000000 or 100000.");
          return;
        }
        conf.i2c_speed = newSpeed;
        logger.info("Set i2c_speed to %lu", conf.i2c_speed);
      }
      else if (key == "I2C_NUM") {
        conf.motor_map_i2c_num = (uint16_t)value.toInt();
        logger.info("Set i2c_num to %d", conf.motor_map_i2c_num);
      }
      else if (key == "LEDC_NUM") {
        conf.motor_map_ledc_num = (uint16_t)value.toInt();
        logger.info("Set ledc_num to %d", conf.motor_map_ledc_num);
        Haptics::globals.reinitLEDC = true;
      }
      else if (key == "CONFIG_VERSION") {
        logger.error("Cannot set config version manually.");
        return;
      }
      else {
        logger.warn("Unknown key for SET: %s", key.c_str());
        return;
      }
    }

    // Main tick() function called from loop().
    void tick() {
      if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n');
        input = trim(input);
        if (input.length() == 0)
          return;
        
        String command, key, value;
        parseInput(input, command, key, value);
        
        if (command == "GET") {
          handleGetCommand(key);
        }
        else if (command == "SET") {
          handleSetCommand(key, value);
        }
        else {
          logger.warn("Unknown command: %s", command.c_str());
        }
      }
    }

  } // namespace SerialComm
} // namespace Haptics
