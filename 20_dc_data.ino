/*
 * Data untuk DC motor dan Opto Coupler
 */

// konstanta untuk PWM motor
#define PWM_CHANNEL 0
#define PWM_RES   8

#define PWM_MIN 0
#define PWM_MAX 255

// pin yang digunakan
int pin_motorA = DO0;
int pin_motorB = DO1;
int pin_opto = DI0;

// variabel kontrol/pengukuran motor
int motor_freq = 15;  // sesuaikan yang tepat untuk motor
int motor_pwm = 0;
int motor_rpm = 0;

// variabel pengukuran opto_coupler
portMUX_TYPE opto_mux = portMUX_INITIALIZER_UNLOCKED;
volatile unsigned opto_counter = 0;
unsigned long opto_last_time = 0;

// Menggunakan Ring Buffer untuk moving average
// pengukuran kecepatan motor

#define N_WINDOW 1000
struct OptoData {
  uint16_t interval;
  uint16_t count;
};

RingBuffer<N_WINDOW, OptoData> obuff;
unsigned long opto_sum;
unsigned long opto_interval;

// periode task 
unsigned ts_display = 100; // display setiap 100 ms
unsigned ts_measure = 10;  // mengukur setiap 10 ms
