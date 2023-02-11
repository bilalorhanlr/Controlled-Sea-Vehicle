#include "Arduino.h"
#include "LoRa_E220.h"

LoRa_E220 e220ttl(16, 17, &Serial2, 15, 21, 19, UART_BPS_RATE_9600);
void setup() {
  // put your setup code here, to run once:
  
  pinMode(34,INPUT); // patonsiyometre
  pinMode(35, INPUT); // tek eksen x
  pinMode(25, INPUT);  // switch
  pinMode(33,INPUT); // 2 eksen x
  pinMode(32,INPUT); // 2 eksen y
  Serial.begin(9600);
  delay(1000);

  e220ttl.begin();
  Serial.println("Hi, I'm going to send message!");
}


int git(int olan, int istenilen)
{ 
  if(olan - istenilen < 0)
    return(olan + 1);
   else if(olan - istenilen > 0)
    return(olan -1);
}
int last_pos_x = 9;
int last_pos_y = 9;
int start_hiz = 9;
int mid_start_fast = 9;
char start_mid_case = 'i';
int hiz; 

void loop() {
  // put your main code here, to run repeatedly:
  int mid_fast = analogRead(34) / 200;  // potasyometre 
  int fast = analogRead(35) / 200;                      
  int on_off = digitalRead(25);                 
  int xPozisyon = analogRead(33) / 200;
  int yPozisyon = analogRead(32) / 200;

    struct Signal{
    char left_right_power[10];
    char up_down_power[10];
    char hiz[10];
    char on_off[10];
    char mid_power[10];
  }data;
  if(on_off == 1)
  {
    start_mid_case = 'i' ;
  }
  else
  {
    start_mid_case = 'g';
  }


    String str_start_hiz = String(fast);
    char start_hiz_char[10];  
    str_start_hiz.toCharArray(start_hiz_char,10);

    String str_last_pos_x = String(xPozisyon);
    char start_last_pos_x[10];  
    str_last_pos_x.toCharArray(start_last_pos_x,10);

    String str_last_pos_y = String(yPozisyon);
    char start_last_pos_y[10];  
    str_last_pos_y.toCharArray(start_last_pos_y,10);

    String str_last_pos_mid = String(mid_fast);
    char start_last_pos_mid[10];  
    str_last_pos_mid.toCharArray(start_last_pos_mid,10);

    for(int i = 0; i < 10; i++)
    {
      data.hiz[i] = start_hiz_char[i];
      data.left_right_power[i] = start_last_pos_x[i];
      data.up_down_power[i] = start_last_pos_y[i];
      data.mid_power[i] = start_last_pos_mid[i];
      data.on_off[i] = start_mid_case;
    }
    Serial.print("hiz :")
    Serial.println(fast);
    
    Serial.print("x_poz :")
    Serial.println(xPozisyon);

    Serial.print("yPozisyon :")
    Serial.println(yPozisyon);

    Serial.print("mid_fast :")
    Serial.println(mid_fast);
    
    ResponseStatus rs = e220ttl.sendBroadcastFixedMessage(23, &data, sizeof(Signal));
    Serial.println(rs.getResponseDescription());

  if (e220ttl.available()>1) 
  {
      // read the String message
        Serial.println("Message received!");
        // read the String message
        ResponseStructContainer rc = e220ttl.receiveMessage(sizeof(Signal));
    
    // Is something goes wrong print error
    if (rc.status.code!=1){
      Serial.println(rc.status.getResponseDescription());
      
    }
    else
    {
      // Print the data received
      Serial.println(rc.status.getResponseDescription());
      Serial.println("gelen veri - ");
      struct Signal data = *(Signal *)rc.data;
    }
  }
  
  if (Serial.available()) {
    String input = Serial.readString();
    ResponseStatus rs = e220ttl.sendBroadcastFixedMessage(23, input);
    // Check If there is some problem of succesfully send
    Serial.println(rs.getResponseDescription());
  }
   delay(200);
  
}
