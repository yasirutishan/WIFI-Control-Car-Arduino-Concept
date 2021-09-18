#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
//defining pins
#define ENR  D1          // Enables  motors Right
#define ENL  D2          // Enables motors Left
#define IN1  D3          // L298N in1 motors Right
#define IN2  D4 // L298N in2 motors Right
#define IN3  D5           // L298N in3 motors Left
#define IN4  D6           // L298N in4 motors Left          

String command;             //String to store web command "state"
int Speed = 450;         // initial speed 400-1024
int SpeedDiv = 3;         //value that speed will be divided by 

const char* ssid = "WiFi Car";// wifi ssid name
ESP8266WebServer server(80);//server port-80, 80 is http port

void setup() {
 
 pinMode(ENR, OUTPUT);//declaring pins as input or output in this case all are outputs
 pinMode(ENL, OUTPUT);  
 pinMode(IN1, OUTPUT);
 pinMode(IN2, OUTPUT);
 pinMode(IN3, OUTPUT);
 pinMode(IN4, OUTPUT); 
  
 Serial.begin(115200);//establishing serial communication between Arduino board and pc
  
  // Connecting WiFi
  
  WiFi.mode(WIFI_AP);//wifi mode set as wifi access point
  WiFi.softAP("WifiCar", "Password", 1, 0, 1);
  //WiFi.softAP(ssid, pwd, channel, hidden, max_connection)

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");//prints AP IP..... in serial monitor
  Serial.println(myIP);//ip address prints in arduino serial monitor
 
     // Starting WEB-server 
     server.on ( "/", HTTP_handleRoot );
     server.onNotFound ( HTTP_handleRoot );
     server.begin();    
}

void Forward(){ //function for car forward

      digitalWrite(IN1, LOW); 
//if this is turned high l298n bridge would switch the polarities(+,-) and motor will turn backward
      digitalWrite(IN2, HIGH);
//when this is turned high l298n bridge would switch the polarities(-,+) and motor will turn forward
      analogWrite(ENR, Speed);//enables l298N right side according to speed

      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      analogWrite(ENL, Speed);//enables l298N left side according to speed
  }

void Reverse(){ //fuction for reverse

      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(ENR, Speed);

      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(ENL, Speed);
  }

void Right(){ //fuction for turning right

//In this fuction when left side wheels are going backward the other side of wheel turns 
//backwards by doing that it gives the ability to turn in the same place(like in tanks)

      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(ENR, Speed);

      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      analogWrite(ENL, Speed);
  }

void Left(){//function for turning left
//In this fuction when right side wheels are going forward the other side of wheel turns backwards
//by doing that it gives the ability to turn in the same place(like in tanks)
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      analogWrite(ENR, Speed);

      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(ENL, Speed);
  }

void goAheadRight(){//fuction for turning right while moving
      
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      analogWrite(ENR, Speed/SpeedDiv);
      //speed get divided by speeddiv and reduces the speed of right side wheels
 
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      analogWrite(ENL, Speed);
   }

void goAheadLeft(){//fuction for turning left while moving;
      
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      analogWrite(ENR, Speed);

      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      analogWrite(ENL, Speed/SpeedDiv);
      //speed get divided by speeddiv and reduces the speed of left side wheels
 
  }

void goBackRight(){ //fuction for going right while reversing

      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(ENR, Speed/SpeedDiv);

      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(ENL, Speed);
  }

void goBackLeft(){ //fuction for going left while reversing

      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(ENR, Speed);

      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(ENL, Speed/SpeedDiv);
  }

void Stop(){  //stop

      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      analogWrite(ENR, 0);

      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      analogWrite(ENL, 0);
 }

void loop() {  
    server.handleClient();    
      command = server.arg("State"); //functioning the fuctions according to user input
      if (command == "F") Forward(); 
      else if (command == "B") Reverse();
      else if (command == "L") Left();
      else if (command == "R") Right();
      else if (command == "I") goAheadRight();
      else if (command == "G") goAheadLeft();
      else if (command == "J") goBackRight();
      else if (command == "H") goBackLeft();      
      else if (command == "1") Speed = 470; 
      else if (command == "2") Speed = 540;
      else if (command == "3") Speed = 680;
      else if (command == "4") Speed = 750;
      else if (command == "5") Speed = 1023;         
      else if (command == "S") Stop();
}
void HTTP_handleRoot() 
{
if(server.hasArg("State"))//when server recives a command
  {
       Serial.println(server.arg("State"));//prints present state in arduino serial monitor
  }
  server.send ( 200, "text/html", "" );//getting the command
  delay(1);
}
