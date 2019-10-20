
#include <WiFi.h>
#include <HTTPClient.h>


#define Start 0
#define End 1


const char* ssid     = "mayavi";
const char* password = "dcvtpqt2?";

const char* serverName = "http://example.com/post-esp-data.php";

const int potPin = 34;
const int LED = 2;
const int button = 5;
long int potVal=0;
int state = End;
int prevRes = 0 ;
int curRes =0;


void setup()
{
  
  pinMode(LED, OUTPUT);
  pinMode(button,INPUT_PULLUP);
 Serial.begin(9600);
delay(500);

 WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED)
  { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
}

// the loop function runs over and over again forever
void loop() 
{
  
    
  if(digitalRead(button) == LOW)
   {
    delay(50);
   if(digitalRead(button)==LOW)
   {
    if(state==End)
    {
      digitalWrite(LED,HIGH);
      state=Start;
      prevRes = readRes();
      sendData(1);
      Serial.println("Start");
      Serial.println(prevRes);
    }
     else
     {
        curRes =readRes();
         if(((prevRes*0.85)<= curRes)&&(curRes <= (prevRes*1.15)))
         {
          state=End;
          digitalWrite(LED,LOW);
          Serial.println("End");
          sendData(2);
         }
         else
         sendData(3);
          Serial.println(curRes);
     }
    
     delay(2000);
    }
   }
             
}
int readRes()
{
  unsigned int resSum = 0;
 // int smplRes=0;
  for(int i=0;i<15;i++)
    {
      delay(20);
    
      resSum=resSum+analogRead(potPin);
    }
    return resSum/15;
}

void sendData(int state)
{
  String strdata="";
  if(WiFi.status()!=WL_CONNECTED)
    Serial.println("WiFi not connected");
    else
    {
      HTTPClient http;
      http.begin("http://192.168.43.58:8090/post");
      http.addHeader("Content-Type", "text/plain");
      
      if(state==1)
      strdata= "Start";
      else if(state==2)
      strdata= "End";
      else
      strdata= "Error";
      
      int httpResponseCode = http.POST(strdata);
      if(httpResponseCode>0)
      {
        Serial.println(httpResponseCode);
        Serial.println(" Posted");
      }
      else
      {
        Serial.println("Error in posting");
      }
      
    }
}
