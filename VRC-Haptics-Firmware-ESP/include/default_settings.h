namespace Haptics {
    namespace BoardDefines {
        // Which pins are allowed to be used as LEDC (straight pwm from board)
        #define LEDC_POSSIBLE_PINS 4, 3, 2, 1, 5, 6, 7, 8, 9, 10
        // Preferred SCL and SDA pins (is a list with 0th index as default)
        #define I2C_POSSIBLE_SCL 9
        #define I2C_POSSIBLE_SDA 8
        // Keep prefferred speed at front
        #define I2C_SPEEDS 4000000U, 100000U
    } // namespace BoardDefines
} // namespace Haptics