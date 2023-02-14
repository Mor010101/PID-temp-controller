#include "ControlParam.h"
#include <stdio.h>
#include <string.h>
#include <Arduino.h>

ControlParam::ControlParam(){}

ControlParam::ControlParam(int SetTemp, double Kp, double Ki, double Kd, int Tincalzire, int Tmentinere, int Tracire){
    this->setTemp = SetTemp;
    this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;
    this->timpIncalzire = Tincalzire;
    this->timpMentinere = Tmentinere;
    this->timpRacire = Tracire;
}

int ControlParam::getSetTemp(){
    return this->setTemp;
}

void ControlParam::setSetTemp(int Temp){
    this->setTemp = Temp;
}

int ControlParam::getTmentinere(){
  return this->timpMentinere;
}

void ControlParam::setTmentinere(int Tmentinere){
  this->timpMentinere = Tmentinere;
}

char* ControlParam::getString(){
  char serialData[64];
  char aux[16];
  char K[5];

  sprintf(serialData,"%d;", this->setTemp);
  dtostrf(this->Kp,-1,3, K); //converts double to string
  strcat(K,";");
  strcat(serialData,K);
  dtostrf(this->Ki,-1,3, K); //converts double to string
  strcat(K,";");
  strcat(serialData,K);
  dtostrf(this->Kd,-1,3, K); //converts double to string
  strcat(K,";");
  strcat(serialData,K);
  sprintf(aux,"%d;", this->timpIncalzire);
  strcat(serialData,aux);
  sprintf(aux,"%d;", this->timpMentinere);
  strcat(serialData,aux);
  sprintf(aux,"%d", this->timpRacire);
  strcat(serialData,aux);

  Serial.println(serialData);
  return serialData;
}