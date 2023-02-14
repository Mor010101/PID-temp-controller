#include <Arduino.h>

#include "PID_v1R.h"

PID::PID(double* Input, double* Output, int* Setpoint,
        double Kp, double Ki, double Kd, int ControllerDirection)
{
	
    myOutput = Output;
    myInput = Input;
    mySetpoint = Setpoint;
	
	  PID::SetOutputLimits(0, 255);				//default output limit corresponds to 
												                //the arduino pwm limits

    PID::SetControllerDirection(ControllerDirection);
    PID::SetTunings(Kp, Ki, Kd);

}
 
bool PID::Compute()
{
   unsigned long now = millis();         
  
      /*Compute all the working error variables*/
	    double input = *myInput;
      double error = *mySetpoint - input;
      ITerm+= (ki * error);
      if(ITerm > outMax) ITerm= outMax;
      else if(ITerm < outMin) ITerm= outMin;
      double dInput = (input - lastInput);
	    DTerm = kd*dInput;
 
      /*CompPID Output*/
      double output = kp * error + ITerm- kd * dInput;
      
	  if(output > outMax) output = outMax;
      else if(output < outMin) output = outMin;
	  *myOutput = output;
	  
      /*Remember some variables for next time*/
      lastInput = input;

	  return true;
   
   //else return false;
}

void PID::SetTunings(double Kp, double Ki, double Kd)
{
   if (Kp<0 || Ki<0 || Kd<0) return;
 
   dispKp = Kp; dispKi = Ki; dispKd = Kd;
   

   kp = Kp;
   ki = Ki;
   kd = Kd;
 
  if(controllerDirection == REVERSE)
   {
      kp = (0 - kp);
      ki = (0 - ki);
      kd = (0 - kd);
   }
}
  

void PID::SetOutputLimits(double Min, double Max)
{
   if(Min >= Max) return;
   outMin = Min;
   outMax = Max;
}


void PID::SetControllerDirection(int Direction)
{
   if(Direction !=controllerDirection)
   {
	  kp = (0 - kp);
      ki = (0 - ki);
      kd = (0 - kd);
   }   
   controllerDirection = Direction;
}

double PID::GetKp(){ return  dispKp; }
double PID::GetKi(){ return  dispKi;}
double PID::GetKd(){ return  dispKd;}
int PID::GetDirection(){ return controllerDirection;}
double PID::GetITerm() {return ITerm;} // added
double PID::GetDTerm() {return DTerm;} // added

