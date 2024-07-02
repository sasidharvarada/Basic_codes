
// Air quality variables
int pm2 = 0;
int pm10 = 0;
unsigned long prev_time = 0; // Added initialization

// co2, vocindex
DFRobot_SGP40 mySgp40;
uint16_t index;
uint16_t estimatedCO2; // Added semicolon at the end
