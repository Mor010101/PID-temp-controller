#include "PID_v1R.h"  // similar to standard PID_v1, this custom library is required for full functionality
#include <avr/power.h>
#include "DataPointDTO.h"
#include <Arduino.h>

#define CLK_DIV 8    // 8
// define 2 exceptions (temp and pid). TEMP_ALARM is fatal - program end.
#define TEMP_ALARM_MIN 0 // exceeds min temp limit [F]
#define TEMP_ALARM_MAX 200 // exceeds max temp limit [F]

/* PID limits should reflect actual physical capability. */
#define PID_MIN  -255  // exceeds min PID limit
#define PID_MAX  255  // exceeds max PID limit

#define TEMP_CORRECTION_OFFSET -0.0 // Thermocouple fudge factor [Fahrenheit]

// PWM on 2 pins (+/-) using Timer 2 (16 bits)
int PWM_Pin_plus = 3;
int PWM_Pin_minus = 11;

// PID variables we connect to
double Setpoint = 60, Input = 60, Output, PV; // degrees Fahrenheit, Celsius or anything else
double PIDSAMPLETIME_MS = 1000 ;  // milliseconds
double Kp = 0.1, Ki = 0.3, Kd = 0.5, K = 3; // K is fudge factor for Output Gain or Thermal Resistance

unsigned long stepN;


DataPoint data(0,0);
  

void t_PID(double *T, unsigned long *t);

//MODIFY PID to take an object of type ControlParam

PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);



int Vo;
float R1 = 10000;
float logR2, R2, Tcalc, Tc, Tf;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

void init_timer0()
{
  cli();
//set timer0 interrupt at 2kHz
  TCCR0A = 0;// set entire TCCR0A register to 0
  TCCR0B = 0;// same for TCCR0B
  TCNT0  = 0;//initialize counter value to 0
  // set compare match register for 1kHz increments
  OCR0A = 250;// = (16*10^6) / (1*64) - 1 (must be <256)
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set CS01 and CS00 bits for 64 prescaler
  TCCR0B |= (1 << CS01) | (1 << CS00);   
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);
  sei();//allow interrupts
}



int main()
{
  clock_prescale_set(clock_div_8);

  // PID setup
  myPID.SetMode(AUTOMATIC);
  myPID.SetSampleTime(PIDSAMPLETIME_MS);
  myPID.SetOutputLimits(PID_MIN, PID_MAX);   // True PID heating and cooling. Should reflect actual physical limits
 
  // phase-correct PWM proportional output,  Timer 2 on pins 3, 11
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM20);
  //TCCR2B = _BV(CS22);
  //TCCR2B = TCCR2B & B11111000 | B00000001;    // set timer 2 divisor to     1 for PWM frequency of 31372.55 Hz
  //TCCR2B = TCCR2B & B11111000 | B00000010;    // set timer 2 divisor to     8 for PWM frequency of  3921.16 Hz
  //TCCR2B = TCCR2B & B11111000 | B00000011;    // set timer 2 divisor to    32 for PWM frequency of   980.39 Hz
  TCCR2B = TCCR2B & B11111000 | B00000100;    // set timer 2 divisor to    64 for PWM frequency of   490.20 Hz  <-- default
  //TCCR2B = TCCR2B & B11111000 | B00000101;    // set timer 2 divisor to   128 for PWM frequency of   245.10 Hz
  //TCCR2B = TCCR2B & B11111000 | B00000110;    // set timer 2 divisor to   256 for PWM frequency of   122.55 Hz
  //TCCR2B = TCCR2B & B11111000 | B00000111;    // set timer 2 divisor to  1024 for PWM frequency of    30.64 Hz
  pinMode(PWM_Pin_plus,  OUTPUT);
  pinMode(PWM_Pin_minus, OUTPUT);

  // Serial
  Serial.begin(9600);  // Change Serial Monitor tool datarate to 2400
  Serial.println("Arduino PID Temperature Control");


  init_timer0();
}


volatile int count = 10000, countDisp = 1000;

ISR(TIMER0_COMPA_vect)
{
  count++;
  countDisp++;

  if(count>=5){
  	Vo = 1023 - analogRead(A0);
    R2 = R1 * (1023.0 / (float)Vo - 1.0);
    logR2 = log(R2);
    Tcalc = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
    Tcalc = Tcalc - 273.15;
    PV = Tcalc;

    data.setTemp(Tcalc);
    data.setTime(count);
    Serial.print("OK");  
    

    Input = PV;

    if (Input <= TEMP_ALARM_MIN || Input >= TEMP_ALARM_MAX) { temp_alarm(Input); }
    bool x = myPID.Compute();
    if (Output <= PID_MIN || Output >= PID_MAX) { pid_error(); }
    PWM_OUT(K * Output);   // update the control outputs
    count = 0;
  }
  if(countDisp>=500){
    Serial.println("PV\t out\t Set\t compute");
    Serial.print(PV);
    Serial.print("\t");
    Serial.print(Output);
    Serial.print("\t");
    Serial.print(*myPID.mySetpoint);
    Serial.print("\t");
    Serial.println("\n");

    countDisp = 0;
  }
}

void t_PID(double *T, unsigned long *t) {
  unsigned long startTime = millis();
  unsigned long t_div = 60000;  // for accelerated testing, 1000ms/s * 60s/min = 60000 ms/min
  myPID.mySetpoint = T;

  while ((millis() - startTime) * CLK_DIV / t_div < *t) {
    Vo = 1023 - analogRead(A0);
    R2 = R1 * (1023.0 / (float)Vo - 1.0);
    logR2 = log(R2);
    Tcalc = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
    Tcalc = Tcalc - 273.15;
    PV = Tcalc;

    Input = PV;  // uncomment to use for live operation
    
    if (Input <= TEMP_ALARM_MIN || Input >= TEMP_ALARM_MAX) { temp_alarm(Input); }
    SER_update();  // update the displays
    
    if (Output <= PID_MIN || Output >= PID_MAX) { pid_error(); }
    Input = Input + K * Output;
    PWM_OUT(K * Output);   // update the control outputs
    stepN++;
    delay(PIDSAMPLETIME_MS / CLK_DIV);
  }  
}


void SER_update() {
  Serial.print("step\t sec.\t PV\t in\t out\t ITerm\t DTerm\n");
  Serial.print(stepN);
  Serial.print("\t");
  Serial.print(PIDSAMPLETIME_MS * stepN / 1000, 0);  // seconds
  Serial.print("\t");
  Serial.print(PV);
  Serial.print("\t");
  Serial.print(Input);
  Serial.print("\t");
  Serial.print(Output);
  Serial.print("\t");
  Serial.print(myPID.GetITerm());
  Serial.print("\t");
  Serial.print(myPID.GetDTerm());
  
  Serial.print("\t");
  Serial.print(*myPID.mySetpoint);
  Serial.println("\n");
}

void PWM_OUT(int signal) {
  if (signal > 0 ) {
    analogWrite(PWM_Pin_plus, signal);
    analogWrite(PWM_Pin_minus, 0);
  } else {
    analogWrite(PWM_Pin_minus, 255);
    analogWrite(PWM_Pin_plus, 0);
  }
}

void temp_alarm(double alarm_temp) {
  PWM_OUT(0);  // turn off forcing functions
  pinMode(PWM_Pin_plus, INPUT);  // disable output pins
  pinMode(PWM_Pin_minus, INPUT);
  Serial.print(alarm_temp);
  Serial.print("\nTemperature Alarm - Program End.\n");
  delay(100);
  exit(1);
}

void pid_error(){
  Serial.print("\n PID ERROR \n");
  delay(100);
  exit(1);
}

