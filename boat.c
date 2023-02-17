
#include "LoRa_E220.h"
LoRa_E220 e220ttl(16, 17, &Serial2, 15, 21, 19, UART_BPS_RATE_9600); //  esp32 RX <-- e220 TX, esp32 TX --> e220 RX AUX M0 M1

int RPWM_Output_Left = 32;
int LPWM_Output_Left = 33; 

int RPWM_Output_Right = 26;
int LPWM_Output_Right = 27; 

int RPWM_Output_Mid = 14;
int LPWM_Output_Mid = 12; 
int RPWM_Output_Mid_Fb = 4; 
int LPWM_Output_Mid_Fb = 0; 

struct wants{
  int left_right_power_now;
  int up_down_power_now;
  int hiz_now;
  int mid_power_now;
}wanted;

void setup() {
  
  Serial.begin(9600);
  delay(500);

  pinMode(RPWM_Output_Left, OUTPUT);
  pinMode(LPWM_Output_Left, OUTPUT);

  pinMode(RPWM_Output_Right, OUTPUT);
  pinMode(LPWM_Output_Right, OUTPUT);

  pinMode(RPWM_Output_Mid,OUTPUT);
  pinMode(LPWM_Output_Mid,OUTPUT);
  
  pinMode(RPWM_Output_Mid_Fb,OUTPUT);
  pinMode(LPWM_Output_Mid_Fb,OUTPUT);

  e220ttl.begin();

  Serial.println("Start receiving!");
}
// joystick 1
int glob_left_right_power = 9;
int glob_left_or_right = -1;

int glob_up_down_power = 9;
int glob_up_or_down = -1;

int yon_change_okey = 1;

//joystick tek eksen
int glob_hiz = 9;

// switch
char glob_mid_yon = 'i';
char ucuncu_motor_yon ;

// potansiyometre
int glob_mid_hiz = 0;

struct Signal{
    char left_right_power[10];
    char up_down_power[10];
    char hiz[10];
    char on_off[10];
    char mid_power[10];
  }data;

int git(int olan, int istenilen)
{ 
  if(olan - istenilen < 0)
    return(olan + 1);
   else
    return(olan -1);
}

void loop() {
  if (e220ttl.available()>1) {
      Serial.println("Message received!");
      // read the String message
      ResponseStructContainer rc = e220ttl.receiveMessage(sizeof(Signal));
      // Is something goes wrong print error
      if (rc.status.code!=1){
        Serial.println(rc.status.getResponseDescription());
      }else{
        // Print the data received
        
        Serial.println(rc.status.getResponseDescription());
        Serial.println("gelen veri - ");
        struct Signal data = *(Signal *) rc.data;

      if(1)
      {
        wanted.left_right_power_now = 0;
        wanted.up_down_power_now = 0;
        wanted.hiz_now = 0;
        wanted.mid_power_now = 0;
        for (int i = 3; i <= 4 && data.left_right_power[i] >= '0' && data.left_right_power[i] <= '9'; ++i)
        {
          wanted.left_right_power_now = wanted.left_right_power_now * 10 + data.left_right_power[i] - '0';    
        }
        for (int i = 3; i <= 4 && data.up_down_power[i] >= '0' && data.up_down_power[i] <= '9'; ++i)
        {
          wanted.up_down_power_now = wanted.up_down_power_now * 10 + data.up_down_power[i] - '0';    
        }
        for (int i = 3; i <= 4 && data.mid_power[i] >= '0' && data.mid_power[i] <= '9'; ++i)
        {
          wanted.mid_power_now = wanted.mid_power_now * 10 + data.mid_power[i] - '0';    
        }            
        for (int i = 3; i <= 4 && data.hiz[i] >= '0' && data.hiz[i] <= '9'; ++i)
        {
          wanted.hiz_now = wanted.hiz_now * 10 + data.hiz[i] - '0';    
        }
        ucuncu_motor_yon = data.on_off[4];      
      }

    
    int updown_yon;
    if(wanted.up_down_power_now > 9)
      updown_yon = 1;
    else if(wanted.up_down_power_now < 9)
      updown_yon = 0;
    else if(wanted.up_down_power_now == 9)
      updown_yon = -1;
    // yon degistir
    if(glob_up_or_down != updown_yon)
    {
      if(glob_hiz <= 9)
      {
        glob_up_or_down = updown_yon;
        glob_hiz = 9;
      }
      else
        glob_hiz -= 1;
    }
    if(glob_up_or_down == updown_yon)
    {
      glob_hiz = git(glob_hiz,wanted.hiz_now);
    }

    Serial.println("------Degerler------");
    Serial.print("glob_yon = ");
    Serial.println(glob_up_or_down);
    
    Serial.print("updown_yon = ");
    Serial.println(updown_yon);

    Serial.print("glob_hiz = ");
    Serial.println(glob_hiz);
    
    Serial.print("wanted hiz = ");
    Serial.println(wanted.hiz_now);


    int leftright_yon;
    if(wanted.left_right_power_now > 9)
      leftright_yon = 1;
    else if(wanted.left_right_power_now < 9)
      leftright_yon = 0;
    else if(wanted.left_right_power_now == 9)
      leftright_yon = -1;
    // yon degistir
    if(glob_left_or_right != leftright_yon)
    {
      if(glob_hiz <= 9)
      {
        glob_left_or_right = leftright_yon;
        glob_hiz = 9;
      }
      else
        glob_hiz -= 1;
    }
    if(glob_left_or_right == leftright_yon)
    {
      glob_hiz = git(glob_hiz,wanted.hiz_now);
    }

    // orta motor cop temizligi
    if(glob_mid_yon != ucuncu_motor_yon)
    {
      if(glob_mid_hiz <= 0)
        glob_mid_yon = ucuncu_motor_yon;
      else
        glob_mid_hiz -= 1;
    }
    if(glob_mid_yon == ucuncu_motor_yon)
    {
      glob_mid_hiz = git(glob_mid_hiz,wanted.mid_power_now);
    }
    if(glob_mid_yon == 'i')
    {
      int PWM = glob_mid_hiz  * 10;
      analogWrite(LPWM_Output_Left, PWM);
      analogWrite(RPWM_Output_Left, 0);
    }
    else if(glob_mid_yon == 'g')
    {
      int PWM = glob_mid_hiz  * 10;
      analogWrite(LPWM_Output_Left, 0);
      analogWrite(RPWM_Output_Left, PWM);        
    }    
       
  // state 1 => only sol    sol motor geri çalışıyor sağ motor ileri çalışıyor
  
  // state 2 => only sağ    sol motor ileri çalışıyor sağ motor geri çalışıyor
  
  // state 3 => only ileri  sol motor ileri çalışıyor sağ motor ileri çalışıyor

  // state 4 => only geri   sol motor geri  çalışıyor sağ motor geri çalışıyor

  // state 5 => durduğu     sol motor 0               sağ motor o

    /*Change states*/
    /* state 3 -> state 4 ------> sol motoru sıfırla yönü değiştir sağ motoru sıfırla yönü değiştir    
     * state 3 -> state 2 ------> sağ motor u sıfırla yönü değiştir                                           
     * state 3 -> state 1 ------> sol motor u sıfırla yönü değiştir
     * state 2 -> state 3 ------> sağ motoru sıfırla yönü değiştir.
     * state 2 -> state 1 ------> sol ve sağ motoru sıfırla her ikisininde yönünü değiştir.
     * 
     * state 1 -> state 2 ------> sol ve sağ motoru sıfırla her ikisininde yönünü değiştir.
     * state 1 -> state 3 ------> sol motoru sıfırla yönü değiştir
     */ 
     
      if(wanted.up_down_power_now == 9  && wanted.left_right_power_now  == 9)
      {
        Serial.println("durum 0");
        analogWrite(LPWM_Output_Left, 0);
        analogWrite(RPWM_Output_Left, 0);
        analogWrite(LPWM_Output_Right, 0);
        analogWrite(RPWM_Output_Right, 0);  
      }
      else if(wanted.up_down_power_now >= 9  && wanted.left_right_power_now  < 15)
      {
        Serial.println("durum 1");
        int PWM = (glob_hiz - 9) * 50;
        analogWrite(LPWM_Output_Left, PWM);
        analogWrite(RPWM_Output_Left, 0);
        analogWrite(LPWM_Output_Right, PWM);
        analogWrite(RPWM_Output_Right, 0); 
      }
      else if(wanted.up_down_power_now < 9 && wanted.left_right_power_now  < 15)
      {
        Serial.println("durum 2");
        int PWM = (glob_hiz - 9) * 50;
        analogWrite(LPWM_Output_Left,  0);
        analogWrite(RPWM_Output_Left,  PWM);
        analogWrite(LPWM_Output_Right, 0);
        analogWrite(RPWM_Output_Right, PWM); 
      }
      else if(wanted.up_down_power_now < 15 && wanted.left_right_power_now  > 9)
      {
        Serial.println("durum 3");
        int PWM = (glob_hiz - 9) * 50;
        analogWrite(LPWM_Output_Left, PWM);
        analogWrite(RPWM_Output_Left,  0);
        analogWrite(LPWM_Output_Right, 0);
        analogWrite(RPWM_Output_Right, PWM);
      }
      else if(wanted.up_down_power_now < 15 && wanted.left_right_power_now  < 9)
      {
        Serial.println("durum 4");
        int PWM = (glob_hiz - 9) * 50;
        analogWrite(LPWM_Output_Left, 0);
        analogWrite(RPWM_Output_Left, PWM);
        analogWrite(LPWM_Output_Right,PWM);
        analogWrite(RPWM_Output_Right, 0);
      }
    }
  }
 }
