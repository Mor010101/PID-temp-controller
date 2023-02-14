#ifndef PID_v1_h
#define PID_v1_h

class PID
{


  public:
  #define DIRECT  0
  #define REVERSE  1

  PID(double*, double*, int*,        
        double, double, double, int);    

  bool Compute();                      

  void SetOutputLimits(double, double);
	
  void SetTunings(double, double, double);         	 
                                        
	void SetControllerDirection(int);	  
  
  //Display functions
	double GetKp();						  
	double GetKi();						 
	double GetKd();						 
	int GetMode();						
	int GetDirection();           
	double GetITerm();					
	double GetDTerm();				
  int *mySetpoint;  			

  private:
	double dispKp;				// * we'll hold on to the tuning parameters in user-entered 
	double dispKi;				//   format for display purposes
	double dispKd;				//
    
	double kp;                  // * (P)roportional Tuning Parameter
  double ki;                  // * (I)ntegral Tuning Parameter
  double kd;                  // * (D)erivative Tuning Parameter

	int controllerDirection;

  double *myInput;              
  double *myOutput;             
			  
	double ITerm, lastInput, DTerm;  

	double outMin, outMax;
};
#endif

