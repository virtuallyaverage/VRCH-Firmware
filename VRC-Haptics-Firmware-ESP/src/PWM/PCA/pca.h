#define PCA_FREQUENCY 1500
#define PCA_1 0x40 
#define PCA_2 0x41

void startPCA();

void setMotorDuty(uint8_t motorIndex, uint16_t dutyCycle);
void setAllDuty();
void setToDuty(uint16_t duty);