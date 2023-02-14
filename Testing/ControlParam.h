#ifndef CONTROL_PARAM_H
#define CONTROL_PARAM_H
#endif

class ControlParam{
  public:
    int setTemp;
    double  Kp;
    double  Ki;
    double  Kd;
    int timpIncalzire;
    int timpMentinere;
    int timpRacire;
    
    ControlParam();
    ControlParam(int, double, double, double, int, int, int);

    int getSetTemp();
    void setSetTemp(int);

    int getTmentinere();
    void setTmentinere(int);

    char* getString();  
};