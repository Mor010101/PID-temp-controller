#include<Arduino.h>
#include"PID_v1R.h"
#include"ControlParam.h"
#include"DataPointDTO.h"

#define PID_MIN -255
#define PID_MAX 255

int PWM_Pin_plus = 3;
int PWM_Pin_minus = 11;

ControlParam controlParam(30,0.1,0.3,0.5,0,1000,0);
DataPoint dataPoint(0,0);
double Input = 30, Output;
double Kp=0.1,Ki=0.3,Kd=0.5;
PID myPID(&Input, &Output, &controlParam.setTemp,Kp,Ki,Kd, DIRECT);


//constants for calculating temperature
int Vo;
float R1 = 10000;
float logR2, R2, Tcalc, Tc, Tf;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

int count = 0, page = 0, step = 0;

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

float voltage;

float getTemp()
{
    Vo =  analogRead(A0);
    voltage = Vo * (5.0/1023.0);    
    //Serial.print("V = ");
    //Serial.print(voltage);
    Vo = 1023 - Vo;
    R2 = R1 * (1023.0 / (float)Vo - 1.0);
    logR2 = log(R2);
    Tcalc = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
    Tcalc = Tcalc - 273.15;
    return Tcalc;
}

void PWM_out(int signal)
{
  if (signal > 0 ) {
    analogWrite(PWM_Pin_plus, signal);
    analogWrite(PWM_Pin_minus, 0);
  } else {
    analogWrite(PWM_Pin_minus, 0 - signal);
    analogWrite(PWM_Pin_plus, 0);
  }
}

void setDataPoint(double Temp)
{
  step++;
  dataPoint.setTemp(Temp);
  dataPoint.setTime(step);
  Serial.print(dataPoint.getString());
}

void setup(){
  Serial.begin(9600);

  myPID.SetOutputLimits(PID_MIN, PID_MAX);
  pinMode(PWM_Pin_plus,  OUTPUT); //change this
  pinMode(PWM_Pin_minus, OUTPUT);

  init_timer0();
}

void loop(){
  
}


ISR(TIMER0_COMPA_vect)
{
  count++;        //further divide interrupt rate
  page++;
  if(count > 100){
    Input = getTemp();      //set PID input to current temp
    //Serial.print(" CurrTemp = ");
    //Serial.print(Input);
    //Serial.print("  Output = ");

    if(page>500){ 
      setDataPoint(Input);
      page = 0;
    }

    myPID.Compute();
    //Serial.println(Output);
    Input = Input + Output; //update input
    count = 0;   
    

    if(Output < PID_MIN || Output > PID_MAX){
      Serial.println("PID error");
     // exit(1);
    }else{
    PWM_out(Output);
    }
  }
}

