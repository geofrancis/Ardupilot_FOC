void FOC_SETUPL() {





  sensor.init();
  sensor.enableInterrupts(doA, doB, doC);
  motor.linkSensor(&sensor);
  driver.voltage_power_supply = 36;
  motor.voltage_sensor_align = 1;

  //motor.foc_modulation = FOCModulationType::SinePWM;
  //motor.foc_modulation = FOCModulationType::SpaceVectorPWM;  // Recommended for efficiency
  // motor.foc_modulation = FOCModulationType::Trapezoid_120;
  // motor.foc_modulation = FOCModulationType::Trapezoid_150;
  motor.controller = MotionControlType::torque;
  // set FOC loop to be used
  // MotionControlType::torque
  // MotionControlType::velocity
  // MotionControlType::angle
  // MotionControlType::angle_nocascade

  motor.useMonitoring(Serial);
  motor.init();
  motor.sensor_direction = Direction::CCW;  // CW or CCW
  motor.initFOC();
}



void FOC_SETUPR() {
  sensor1.init();
  sensor1.enableInterrupts(doA1, doB1, doC1);
  motor1.linkSensor(&sensor1);
  driver1.voltage_power_supply = 36;
  motor1.voltage_sensor_align = 1;
 // motor1.foc_modulation = FOCModulationType::SpaceVectorPWM;
  motor1.controller = MotionControlType::torque;
  motor1.useMonitoring(Serial);
  motor1.init();
  motor1.sensor_direction = Direction::CCW;  // CW or CCW
  motor1.initFOC();
}




float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void FOC_Speed() {


  if (leftoutputraw > (1500 + DZ)) { leftoutput = mapFloat(leftoutputraw, (1500 + DZ), 2000, 0, MAXRPM); }
  if (leftoutputraw < (1500 - DZ)) { leftoutput = mapFloat(leftoutputraw, (1500 - DZ), 1000, 0, -MAXRPM); }
  if (leftoutputraw < (1500 + DZ) && leftoutputraw > (1500 - DZ)) {
    leftoutput = 0;
  }

  if (rightoutputraw > (1500 + DZ)) { rightoutput = mapFloat(rightoutputraw, (1500 + DZ), 2000, 0, MAXRPM); }
  if (rightoutputraw < (1500 - DZ)) { rightoutput = mapFloat(rightoutputraw, (1500 - DZ), 1000, 0, -MAXRPM); }
  if (rightoutputraw < (1500 + DZ) && rightoutputraw > (1500 - DZ)) {
    rightoutput = 0;
  }
      motor.move(rightoutput);
      motor1.move(leftoutput);

  }







void FOC_telemetry() {

  targetL = motor.target;
  velocityL = motor.shaft_velocity;
  voltageqL = motor.voltage.q;
  currentqL = motor.current.q;

  targetR = motor1.target;
  velocityR = motor1.shaft_velocity;
  voltageqR = motor1.voltage.q;
  currentqR = motor1.current.q;

  Serial.println("");
 // Serial.print("targetL ");
 // Serial.println(targetL);
 // Serial.print("velocityL ");
 // Serial.println(velocityL);
  Serial.print("voltageqL ");
  Serial.println(voltageqL);
  Serial.print("currentqL ");
  Serial.println(currentqL);
 // Serial.print("target_velocity ");
 // Serial.println(target_velocity);
}
