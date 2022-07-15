#include <Wire.h>
#include "rgb_lcd.h"
#define MAX_CHAR 256

rgb_lcd lcd;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  lcd.begin(16, 2);
  lcd.setRGB(0, 255, 255);
}

//Converting alphabets to ASCII
int *toASCII(char string[]){
  //Serial.println(string);
  int Size = strlen(string);
  int *Array = (int*)malloc(sizeof(int)*(Size+10));
  for(int i=0; i<=Size;i++){
    if((int)string[i]>=33 && (int)string[i]<=64){
      for (int j=i;j<=Size-1;j++){
        string[j] = string[j+1];
      }
    }
    Array[i] = string[i];
    //Serial.println(Array[i]);
  }
  return Array;
}

void loop() {
  // put your main code here, to run repeatedly:
  String value;
  char store[MAX_CHAR];
  char shifted[MAX_CHAR];
  int *string;
  int shift;
  int page;
  int previous;
  while(Serial.available()==0){
    //shift = (analogRead(A0)-511)/38;
    shift = 0;
    Serial.println(shift);
    //lcd.clear();
    //lcd.setCursor(0,0);
    //lcd.print(shift);
    delay(100);
  }
  lcd.clear();
  value = Serial.readString();
  value.toUpperCase();
  value.toCharArray(store,MAX_CHAR); //String -> char
  Serial.println(store);
  string = toASCII(store);
  for(int i=0;i<=strlen(store)-2 ;i++){
    string[i] += shift; //Shifts the letter
    if (string[i]>90){
      string[i] -= 25; //if 'Z' goes to 'A'
    }
    if(string[i] == 32+shift){
      string[i] -= shift;  
    } else if(string[i]<65){
      string[i] += 26;
    }
    shifted[i] = string[i];
    //Serial.print(shifted[i]);
    if(i<16){
      lcd.setCursor(i,0);
    } else{
      lcd.setCursor(i-16,1);
    }
    lcd.print(shifted[i]);
  }
  while(Serial.available()==0){
    page = analogRead(A0)/515;
    if(page != previous){ lcd.clear();}
    for(int i=0; i<=31 && i+page*32 <= strlen(store)-2; i++){
        if(i<16){
          lcd.setCursor(i,0);
        } else{
          lcd.setCursor(i-16,1);
        }
        lcd.print(shifted[i+page*32]);
      }
    previous = page;
  }
  free(string);
}
