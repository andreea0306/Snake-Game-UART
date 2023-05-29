#include <M5StickCPlus.h>

MPU6886 IMU; // create an instance of the MPU6886 class 
int THRESHOLD = 5;
void setup() {
  M5.begin(); 
  Wire.begin();
  M5.IMU.Init();
  Serial.begin(115200);
}

void loop() {
  // getting data from sensor
  float ax = 0, ay = 0, az = 0;
    M5.IMU.getAccelData(&ax,&ay,&az);
    // compare data from sensor to a user defined thrashold 
    if(ax*10 > THRESHOLD) { // if direction is already opposite the command has no effect
      // left
      Serial.write("2");
    } else if(ax*10 < -THRESHOLD) {
     // right
      Serial.write("0");
    }
    else if(ay*10 > THRESHOLD) {
      // down
      Serial.write("1");
    } else if(ay*10 < -THRESHOLD){
       // up
      Serial.write("3");
    }
    delay(500);
}
