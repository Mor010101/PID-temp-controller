#include "Arduino.h"
#include "string.h"
#include "DataPointDTO.h"
#include "ControlParam.h"

DataPoint x;
ControlParam y(60,1.45,1.3,2.1,30,60,20);
int V;

void loop()
{
  V = analogRead(A1);
  Serial.println(V);
}

int main()
{
  
  x.setTemp(34.5);
  x.setTime(1);

  Serial.begin(9600);
  //Serial.println(x.getString());

  while(true){
    loop();
  }
  return 0;
}
