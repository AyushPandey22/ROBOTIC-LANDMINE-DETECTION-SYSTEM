#include <TinyGPS++.h>
#include <SoftwareSerial.h>
/* Create object named bt of the class SoftwareSerial */
SoftwareSerial GPS_SoftSerial(8, 9);/* (Rx, Tx) */
/* Create an object named gps of the class TinyGPSPlus */
TinyGPSPlus gps; 

volatile float minutes, seconds;
volatile int degree, secs, mins;
   
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (GPS_SoftSerial.available()) /* Encode data read from GPS while data is available on serial port */
      gps.encode(GPS_SoftSerial.read());
    /* Encode basically is used to parse the string received by the GPS and to store it in a buffer so that information can be extracted from it */
  } while (millis() - start < ms);
}

void DegMinSec( double tot_val)   /* Convert data in decimal degrees into degrees minutes seconds form */
{  
  degree = (int)tot_val;
  minutes = tot_val - degree;
  seconds = 60 * minutes;
  minutes = (int)seconds;
  mins = (int)minutes;
  seconds = seconds - minutes;
  seconds = 60 * seconds;
  secs = (int)seconds;
}

void locL(void)
{
       smartDelay(1000);  /* Generate precise delay of 1ms */
     
        unsigned long start;
        double lat_val, lng_val, alt_m_val;
        uint8_t hr_val, min_val, sec_val;
        bool loc_valid, alt_valid, time_valid;
        lat_val = gps.location.lat(); /* Get latitude data */
        loc_valid = gps.location.isValid(); /* Check if valid location data is available */
        lng_val = gps.location.lng(); /* Get longtitude data */
        alt_m_val = gps.altitude.meters();  /* Get altitude data in meters */
        alt_valid = gps.altitude.isValid(); /* Check if valid altitude data is available */
        hr_val = gps.time.hour(); /* Get hour */
        min_val = gps.time.minute();  /* Get minutes */
        sec_val = gps.time.second();  /* Get seconds */
        time_valid = gps.time.isValid();  /* Check if valid time data is available */
        
        if (!loc_valid)/// if location not valid
        //send dummy location
        {      
         
          delay(500);
          Serial.println("AT+CIPSEND=0,46\r\n");
          delay(500);
          Serial.print("http://maps.google.com/?q=");               
          Serial.println("26.810903,80.902473"   );//TEXT MODE
          delay(1000); 
          delay(2000);

          delay(100);
          GPS_SoftSerial.println("ATE0");
          delay(1000);
          GPS_SoftSerial.println("AT+CMGF=1");//TEXT MODE
          delay(1000);
          GPS_SoftSerial.println("AT+CMGS=\"+919648369831\"\r"); //replace x by your number
          delay(100);   
          GPS_SoftSerial.print("http://maps.google.com/?q="); 
          delay(200);                 
          GPS_SoftSerial.println("26.810903,80.902473" );
          delay(100);               
          GPS_SoftSerial.println((char)26);// CTRL+Z COMMAND
          delay(2000);
          
        }
        else
        /// location valid send original location
        {
          delay(500);
          Serial.println("AT+CIPSEND=0,49\r\n");
          delay(500);
          Serial.print("http://maps.google.com/?q=");
 
          Serial.print(lat_val, 6);
         
          Serial.print(",");
         
          Serial.println(lng_val, 6);
         
          Serial.println("      ");



          delay(100);
          GPS_SoftSerial.println("ATE0");
          delay(1000);
          GPS_SoftSerial.println("AT+CMGF=1");//TEXT MODE
          delay(1000);
          GPS_SoftSerial.println("AT+CMGS=\"+919648369831\"\r"); //replace x by your number
          delay(200); 
          GPS_SoftSerial.print("http://maps.google.com/?q="); 
          delay(100);   
          GPS_SoftSerial.print(lat_val, 6); 
          delay(200); 
          GPS_SoftSerial.println("," );  
          delay(200);              
          GPS_SoftSerial.println(lng_val, 6);
          delay(100);               
          GPS_SoftSerial.println((char)26);// CTRL+Z COMMAND
          delay(2000);
          
            
        }

       
  }



unsigned char Buff[250];
unsigned char BuffIndex;

int x_out= A0;
int x_adc_value; 

#include <LiquidCrystal.h>
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int in11 = A1;//m1
const int in12 = A2;//m1

const int in21 = A3;//m2
const int in22 = A4;//m2

const int in31 = A5;//m3


void setup()
{
 
     Serial.begin(115200);             
     GPS_SoftSerial.begin(9600);
 
     delay(4000);//wating esp power stabilization
     
     lcd.begin(16, 2);
     lcd.setCursor(0, 0);
     lcd.print("Landmine detec... ");
     lcd.setCursor(0, 1);
     lcd.print("    system ");
     delay(1000);
 
  pinMode (in11, OUTPUT);
  pinMode (in12, OUTPUT);
  pinMode (in21, OUTPUT);
  pinMode (in22, OUTPUT);
  pinMode (in31, OUTPUT);
  


 digitalWrite(in11, LOW); 
 digitalWrite(in12, LOW);
 digitalWrite(in21, LOW); 
 digitalWrite(in22, LOW);
 digitalWrite(in31, LOW); 
 
 
  Serial.println("ATE0\r\n");//TEXT MODE
  delay(1000);
  Serial.println("AT+CWMODE_DEF=2\r\n"); //
  delay(1000);
  Serial.println("AT+CIPMUX=1\r\n");
  delay(1000);
  Serial.println("AT+CIPSERVER=1,80\r\n");
  delay(1000);

  
    memset(Buff, '\0', 250);    // Initialize the string
    BuffIndex=5;
    


 
}


//  Where the Magic Happens
          void loop()
          {
  
               x_adc_value = analogRead(A0);
                 
              // Serial.println(x_adc_value);
              // delay(500);

               if(x_adc_value > 1000)//l
                    {
                      delay(200);
                    
                    if(x_adc_value > 1000)//2
                        {
                           delay(200);
                           if(x_adc_value > 1000)//3
                           {
                           
                          lcd.clear();
                          lcd.setCursor(0, 0);
                          lcd.print("  Landmine ");
                          lcd.setCursor(0, 1);
                          lcd.print("  detected ");
                         // Serial.println("STOP");
                         digitalWrite(in11, LOW); 
                         digitalWrite(in12, LOW);
                         digitalWrite(in21, LOW); 
                         digitalWrite(in22, LOW);

                         
                          delay(500);
                         
                          digitalWrite(in31,HIGH); 
                          delay(500);
                          digitalWrite(in31, LOW); 
                          delay(300);

                          digitalWrite(in11, LOW); 
                          digitalWrite(in12, HIGH);
                          digitalWrite(in21, LOW); 
                          digitalWrite(in22, HIGH); 
                          delay(2000);

                         digitalWrite(in11, LOW); 
                         digitalWrite(in12, LOW);
                         digitalWrite(in21, LOW); 
                         digitalWrite(in22, LOW);
                    
                          delay(1000);
          GPS_SoftSerial.println("ATE0");
          delay(1000);
          GPS_SoftSerial.println("AT+CMGF=1");//TEXT MODE
          delay(1000);
          GPS_SoftSerial.println("AT+CMGS=\"+919648369831\"\r"); //replace x by your number
          delay(200); 
          GPS_SoftSerial.print("landmine detected"); 
          delay(500); 
          GPS_SoftSerial.println((char)26);// CTRL+Z COMMAND
          delay(2000);


     
          lcd.clear();
          lcd.print("sending location");
          delay(200);
          locL();


          lcd.clear();
          lcd.print(" location sent");
          delay(2000);

           lcd.setCursor(0, 0);
          lcd.print("Landmine detec... ");
          lcd.setCursor(0, 1);
         lcd.print("    system ");
        

                        }
                        }
                        }

          if(Serial.available()>0)
              {
              Buff[BuffIndex] = Serial.read();
                  
                 
             if( (Buff[BuffIndex-2] == '#') && (Buff[BuffIndex-1] == 'U') )
              {

                //   Serial.println("u");
                   digitalWrite(in11, HIGH); 
                   digitalWrite(in12, LOW);
                   digitalWrite(in21, HIGH); 
                   digitalWrite(in22, LOW);       
                    
              }
            
                  
               if( (Buff[BuffIndex-2] == '#') && (Buff[BuffIndex-1] == 'D') )
              {
                   //Serial.println("d");
                   digitalWrite(in11, LOW); 
                   digitalWrite(in12, HIGH);
                   digitalWrite(in21, LOW); 
                   digitalWrite(in22, HIGH); 
                
              }


            if( (Buff[BuffIndex-2] == '#') && (Buff[BuffIndex-1] == 'L') )
                {
                  //  Serial.println("l");
                   digitalWrite(in11, HIGH); 
                   digitalWrite(in12, LOW);
                   digitalWrite(in21, LOW); 
                   digitalWrite(in22, HIGH); 
           
                }


                if( (Buff[BuffIndex-2] == '#') && (Buff[BuffIndex-1] == 'R') )
               {
                   // Serial.println("r");
                     digitalWrite(in11, LOW); 
                     digitalWrite(in12, HIGH);
                     digitalWrite(in21, HIGH); 
                     digitalWrite(in22, LOW); 
                 
          
               }


                if( (Buff[BuffIndex-2] == '#') && (Buff[BuffIndex-1] == 'S') )
                {
                
                     //////STOP
                     ////digitalWrite(relay, LOW);
                     digitalWrite(in11, LOW); 
                     digitalWrite(in12, LOW);
                     digitalWrite(in21, LOW); 
                     digitalWrite(in22, LOW); 
                           
                }


                 
               

                 
               else{}
               
              BuffIndex++;
              if(BuffIndex>250) 
              {
                  BuffIndex=5;
              }
          }


  
}
