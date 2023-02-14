#include "DataPointDTO.h"
#include <stdio.h>
#include <string.h>
#include <Arduino.h>

DataPoint::DataPoint(){}

DataPoint::DataPoint(double Temp, int Time){
    this->temp = Temp;
    this->time = Time;
}

double DataPoint::getTemp(){
    return this->temp;
}

void DataPoint::setTemp(double Temp){
    this->temp = Temp;
}

int DataPoint::getTime(){
    return this->time;
}

void DataPoint::setTime(int Time){
    this->time = Time;
}

char* DataPoint::getString(){
  char serialData[64];
  char Time[16];

  sprintf(Time,";%d;", this->time); //convert time to char and store
  
  dtostrf(this->temp,-1,3,serialData); //converts double to string
  strcat(serialData,Time);

  Serial.println(serialData);
  return serialData;
}