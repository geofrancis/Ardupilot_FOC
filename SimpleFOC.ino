
#include "mavlink/common/mavlink.h"
#include "mavlink/common/mavlink_msg_servo_output_raw.h"
#include <SimpleFOC.h>
#include <esp_task_wdt.h>
TaskHandle_t motorTaskHandle;

#define ESC1
//#define ESC2
//#define ESC3

int DZ = 20;  // dead
float MAXRPM = 10;
float target_velocity = 0;

float Vlimit = 3;


#ifdef ESC1
int ESC = 140;  //board number

HallSensor sensor1 = HallSensor(18, 19, 13, 15);  
HallSensor sensor = HallSensor(23, 5, 15, 15);    

BLDCMotor motor = BLDCMotor(15);
BLDCDriver3PWM driver = BLDCDriver3PWM(32, 25, 33, 15);

BLDCMotor motor1 = BLDCMotor(15);
BLDCDriver3PWM driver1 = BLDCDriver3PWM(26, 27, 14, 15);
#endif

#ifdef ESC2
int ESC = 141;  //board number
HallSensor sensor = HallSensor(18, 15, 19, 15);
HallSensor sensor1 = HallSensor(5, 23, 13, 15);
BLDCMotor motor = BLDCMotor(15);
BLDCDriver3PWM driver = BLDCDriver3PWM(32, 33, 25, 15);
BLDCMotor motor1 = BLDCMotor(15);
BLDCDriver3PWM driver1 = BLDCDriver3PWM(26, 27, 14, 15);
#endif

#ifdef ESC3
int ESC = 142;  //board number
HallSensor sensor1 = HallSensor(15, 18, 19, 15);
HallSensor sensor = HallSensor(23, 5, 13, 15);
BLDCMotor motor = BLDCMotor(15);
BLDCDriver3PWM driver1 = BLDCDriver3PWM(32, 33, 25, 15);
BLDCMotor motor1 = BLDCMotor(15);
BLDCDriver3PWM driver = BLDCDriver3PWM(27, 26, 14, 15);
#endif



void doA() {
  sensor.handleA();
}
void doB() {
  sensor.handleB();
}
void doC() {
  sensor.handleC();
}

void doA1() {
  sensor1.handleA();
}
void doB1() {
  sensor1.handleB();
}
void doC1() {
  sensor1.handleC();
}




int leftoutputraw = 1500;
int rightoutputraw = 1500;

unsigned long previousMillis = 0;
const long telem = 2000;

float leftoutput = 0;
float rightoutput = 0;

int DI1O = 0;
int FCHB = 0;
int FCOK = 0;

int BASEMODE = 0;
int armed;
int active = 0;

float targetL = 0;
float velocityL = 0;
float voltageqL = 0;
float currentqL = 0;

float targetR = 0;
float velocityR = 0;
float voltageqR = 0;
float currentqR = 0;


uint8_t system_id = 1;
uint8_t component_id = 158;
uint8_t severity = 1;
uint16_t id = 0;
uint8_t chunk_seq = 0;



void Left_task(void* pvParameters) {
  FOC_SETUPL();
  for (;;) {
    // noInterrupts();
    motor.loopFOC();
    // interrupts();
    // vTaskDelay(1);
    motor.move(3);
  }
}


void Right_task(void* pvParameters) {
  FOC_SETUPR();
  for (;;) {
    // noInterrupts();
    motor1.loopFOC();
    // interrupts();
    //vTaskDelay(1);
    motor1.move(3);
  }
}




void setup() {
  Serial.begin(115200);  //Main serial port for console output
  Serial1.begin(230400, SERIAL_8N1, 16, 17);
  //Serial2.begin(38400, SERIAL_8N1, 2, 4);  //GPS+AIS

  esp_task_wdt_config_t twdt_config = {
    .timeout_ms = 5000,
    .idle_core_mask = (1 << 1),  // Bitmask: Bit 1 is Core 1. We leave Bit 0 (Core 0) unset.
    .trigger_panic = true,
  };
  esp_task_wdt_reconfigure(&twdt_config);

  driver.init();
  driver1.init();
  motor.linkDriver(&driver);
  motor1.linkDriver(&driver1);



  xTaskCreatePinnedToCore(
    Left_task,         // Function to implement the task
    "Left Motor",      // Name of the task
    10000,             // Stack size in words (10k is usually plenty)
    NULL,              // Task input parameter
    5,                 // Priority of the task (1 is higher than 0)
    &motorTaskHandle,  // Task handle
    0                  // Core where the task should run (Core 0)
  );


  xTaskCreatePinnedToCore(
    Right_task,        // Function to implement the task
    "Right Motor",     // Name of the task
    10000,             // Stack size in words (10k is usually plenty)
    NULL,              // Task input parameter
    5,                 // Priority of the task (1 is higher than 0)
    &motorTaskHandle,  // Task handle
    0                  // Core where the task should run (Core 0)
  );



  STARTUPMSG();
}


void loop() {
  vTaskDelay(1);
  //motor.monitor();
  //motor1.monitor();
  //MavLink_RC();

  //motor.move(1);
  //motor1.move(1);






  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= telem) {
    previousMillis = currentMillis;
    //MAVLINK_HB();
    // MAVLINK_ESC_1();

    // if (DI1O == 1) { Mavlink_Telemetry(); }
    //if (DI1O == 2) { FCHBC(); }
    // if (DI1O == 3) { FOC_telemetry(); }
    // if (DI1O == 4) { sleepcheck(); }
    DI1O++;
    if (DI1O > 4) { DI1O = 1; }
    // Serial.print("                               DI1O  ");
    // Serial.println(DI1O);
  }
}
