#define ARDUINOJSON_DISABLE_DEPRECATION_WARNINGS //TODO: STOP USING THIS

#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

#include "config.h"  // has Config and defaultConfig
#include "logging/Logger.h"

namespace Haptics {

    Logging::Logger logger("Haptics");

    // Load configuration or init to defaults
    void loadConfig(Config* conf) {
        // If the config file doesn't exist, write default configuration
        if (!LittleFS.exists("/config.json")) {
            Serial.println("Config file not found. Creating default config.");
            DynamicJsonDocument doc(512);
            doc["wifi_ssid"] = defaultConfig.wifi_ssid;
            doc["wifi_password"] = defaultConfig.wifi_password;
            doc["i2c_scl"] = defaultConfig.i2c_scl;
            doc["i2c_sda"] = defaultConfig.i2c_sda;
            doc["i2c_speed"] = defaultConfig.i2c_speed;
            doc["motor_map_i2c_num"] = defaultConfig.motor_map_i2c_num;
            JsonArray motorMapI2cArray = doc.createNestedArray("motor_map_i2c");
            for (int i = 0; i < defaultConfig.motor_map_i2c_num; i++) {
                motorMapI2cArray.add(defaultConfig.motor_map_i2c[i]);
            }

            // Save motor map for LEDC
            doc["motor_map_ledc_num"] = defaultConfig.motor_map_ledc_num;
            JsonArray motorMapLedcArray = doc.createNestedArray("motor_map_ledc");
            for (int i = 0; i < defaultConfig.motor_map_ledc_num; i++) {
                motorMapLedcArray.add(defaultConfig.motor_map_ledc[i]);
            }

            doc["mdns_name"] = defaultConfig.mdns_name;
            
            File configFile = LittleFS.open("/config.json", "w");
            if (configFile) {
                serializeJson(doc, configFile);
                configFile.close();
                // Use default configuration in memory
                memcpy(conf, &defaultConfig, sizeof(Config));
            } else {
                Serial.println("Failed to create config file");
            }
            return;
        }
    
        // If the file exists, read and parse it
        File configFile = LittleFS.open("/config.json", "r");
        if (configFile) {
            logger.debug("Loaded config");
            DynamicJsonDocument doc(JSON_SIZE);
            DeserializationError error = deserializeJson(doc, configFile);
            if (!error) {
                // Check the version
                uint16_t fileVersion = doc["config_version"] | 0;
                if (fileVersion < defaultConfig.config_version || fileVersion == 0) {
                    Serial.println("Config version is outdated. Updating to defaults.");
                    // Overwrite with new defaults. TODO: Merge changed defaults?
                    File configFileWrite = LittleFS.open("/config.json", "w");
                    if (configFileWrite) {
                        doc.clear();
                        doc["wifi_ssid"] = defaultConfig.wifi_ssid;
                        doc["wifi_password"] = defaultConfig.wifi_password;
                        doc["i2c_scl"] = defaultConfig.i2c_scl;
                        doc["i2c_sda"] = defaultConfig.i2c_sda;
                        doc["i2c_speed"] = defaultConfig.i2c_speed;
                        doc["motor_map_i2c_num"] = defaultConfig.motor_map_i2c_num;
                        JsonArray motorMapI2cArray = doc.createNestedArray("motor_map_i2c");
                        for (int i = 0; i < defaultConfig.motor_map_i2c_num; i++) {
                            motorMapI2cArray.add(defaultConfig.motor_map_i2c[i]);
                        }
                        doc["motor_map_ledc_num"] = defaultConfig.motor_map_ledc_num;
                        JsonArray motorMapLedcArray = doc.createNestedArray("motor_map_ledc");
                        for (int i = 0; i < defaultConfig.motor_map_ledc_num; i++) {
                            motorMapLedcArray.add(defaultConfig.motor_map_ledc[i]);
                        }
                        doc["mdns_name"] = defaultConfig.mdns_name;
                        doc["config_version"] = defaultConfig.config_version;
                        
                        serializeJson(doc, configFileWrite);
                        configFileWrite.close();
                        memcpy(conf, &defaultConfig, sizeof(Config));
                        configFile.close();
                        return;
                    } else {
                        Serial.println("Failed to update config file");
                    }
                }

                // Load WiFi settings (using defaults if missing)
                const char* ssid = doc["wifi_ssid"] | defaultConfig.wifi_ssid;
                const char* password = doc["wifi_password"] | defaultConfig.wifi_password;
                strncpy(conf->wifi_ssid, ssid, sizeof(conf->wifi_ssid));
                strncpy(conf->wifi_password, password, sizeof(conf->wifi_password));
                
                // Load I2C settings
                conf->i2c_scl = doc["i2c_scl"] | defaultConfig.i2c_scl;
                conf->i2c_sda = doc["i2c_sda"] | defaultConfig.i2c_sda;
                conf->i2c_speed = doc["i2c_speed"] | defaultConfig.i2c_speed;
                
                // Load motor map for I2C
                conf->motor_map_i2c_num = doc["motor_map_i2c_num"] | defaultConfig.motor_map_i2c_num;
                JsonArray motorMapI2cArray = doc["motor_map_i2c"].as<JsonArray>();
                if (motorMapI2cArray) {
                    int index = 0;
                    for (JsonVariant value : motorMapI2cArray) {
                        if (index < MAX_I2C_MOTORS) {
                            conf->motor_map_i2c[index] = value.as<uint16_t>();
                        }
                        index++;
                    }
                } else {
                    // Fallback to default array if not present
                    memcpy(conf->motor_map_i2c, defaultConfig.motor_map_i2c, sizeof(conf->motor_map_i2c));
                }
                
                // Load motor map for LEDC
                conf->motor_map_ledc_num = doc["motor_map_ledc_num"] | defaultConfig.motor_map_ledc_num;
                JsonArray motorMapLedcArray = doc["motor_map_ledc"].as<JsonArray>();
                if (motorMapLedcArray) {
                    int index = 0;
                    for (JsonVariant value : motorMapLedcArray) {
                        if (index < MAX_LEDC_MOTORS) {
                            conf->motor_map_ledc[index] = value.as<uint16_t>();
                        }
                        index++;
                    }
                } else {
                    // Fallback to default array if not present
                    memcpy(conf->motor_map_ledc, defaultConfig.motor_map_ledc, sizeof(conf->motor_map_ledc));
                }

                // Load mdns_name
                const char* mdnsName = doc["mdns_name"] | defaultConfig.mdns_name;
                strncpy(conf->mdns_name, mdnsName, sizeof(conf->mdns_name));
                doc["config_version"] = defaultConfig.config_version;
            } else {
                Serial.println("Failed to parse config file, using default config.");
                memcpy(conf, &defaultConfig, sizeof(Config));
            }
            configFile.close();
        }
    }
    
    // Function to save the current configuration to LittleFS
    void saveConfig(Config* conf) {
        DynamicJsonDocument doc(JSON_SIZE);
        doc["wifi_ssid"] = conf->wifi_ssid;
        doc["wifi_password"] = conf->wifi_password;

        doc["i2c_scl"] = conf->i2c_scl;
        doc["i2c_sda"] = conf->i2c_sda;
        doc["i2c_speed"] = conf->i2c_speed;

        doc["motor_map_i2c_num"] = conf->motor_map_i2c_num;
        JsonArray motorMapI2cArray = doc.createNestedArray("motor_map_i2c");
        for (int i = 0; i < conf->motor_map_i2c_num; i++) {
            motorMapI2cArray.add(conf->motor_map_i2c[i]);
        }
        // Save motor map for LEDC
        doc["motor_map_ledc_num"] = conf->motor_map_ledc_num;
        JsonArray motorMapLedcArray = doc.createNestedArray("motor_map_ledc");
        for (int i = 0; i < conf->motor_map_ledc_num; i++) {
            motorMapLedcArray.add(conf->motor_map_ledc[i]);
        }
        
        doc["mdns_name"] = conf->mdns_name;

        File configFile = LittleFS.open("/config.json", "w");
        if (configFile) {
            serializeJson(doc, configFile);
            configFile.close();
            Serial.println("Configuration saved.");
        } else {
            Serial.println("Failed to open config file for writing.");
        }
    }
} // namespace Haptics
