#ifndef DATA_POINT_DTO_H
#define DATA_POINT_DTO_H
#endif

class DataPoint{
  public:
    double temp;
    int time;
    
    DataPoint();
    DataPoint(double, int);

    double getTemp();
    void setTemp(double);

    int getTime();
    void setTime(int);

    char* getString();  
};