#ifndef SOFTWARE_DEFINES_H
#define SOFTWARE_DEFINES_H

/// Motor defines
#define MAX_I2C_MOTORS  64
#define MAX_LEDC_MOTORS 64

#define PCA_FREQUENCY 1500 // pwm frequency of pca motors
#define PCA_1 0x40 
#define PCA_2 0x41

#define LEDC_FREQUENCY 60 //defaults from senseshift
#define LEDC_RESOLUTION 12 //defaults from senseshift

/// Wireless defines
#define OSC_MOTOR_CHAR_NUM 4
#define RECIEVE_PORT 1027

#define HEARTBEAT_ADDRESS "/hrtbt"
#define PING_ADDRESS "/ping"
#define MOTOR_ADDRESS "/h"

#define MDNS_SERVICE_NAME "haptics"
#define MDNS_SERVICE_PROTOCOL "udp"


#endif // Software defines