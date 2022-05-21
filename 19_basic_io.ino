// LED ESP32
const int ledPin = LED0;

//Output PWM
//#define AO0 26 -> udah didef di depan
// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

void ioSetup(){
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
  // Konfigurasi pwm
  ledcSetup(ledChannel, freq, resolution);
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(AO0, ledChannel);
}
