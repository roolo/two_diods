#include <IRremote.h>

int recvLED = 13;
int recvPin = 11;
int diodeOne = 2;
int diodeTwo = 3;

IRrecv irReciver(recvPin);
decode_results results;

unsigned long lastTime = 0;
  
void setup()
{
  pinMode(recvLED, OUTPUT); //Configue the recvLED pin as an OUTPUT pin
  digitalWrite(recvLED, LOW); //Leave the LED off as we start

  Serial.begin(9600); // Initialize the serial port with a baud rate of 9600 bps

  irReciver.enableIRIn(); // Start the receiver
  
  pinMode(diodeOne, OUTPUT);
  pinMode(diodeTwo, OUTPUT);
  
  digitalWrite(diodeOne, HIGH);
  digitalWrite(diodeTwo, HIGH);
}

void loop()
{
  unsigned long recv_value;
  if (irReciver.decode(&results)) {
    recv_value = results.value; 
    
    if (  recv_value != 0xFFFFFFFF ) //Ignore the 0x00 values recived as a result of pressing and holding a button on the remote for long
    {
      // Blink the LED each time a button is pressed
      digitalWrite(recvLED, HIGH);
      delay(10);
      digitalWrite(recvLED, LOW);  
      
      // Output the decoded hash value to th serial monitor. This is for debugging purposes only.
      Serial.println(recv_value, HEX);
      Serial.println(recv_value);

      if(recv_value == 0xFD08F7 || recv_value == 0x77E1E033) {
        lastTime = millis(); // Start power saving delay timer
        digitalWrite(diodeOne, HIGH);
        digitalWrite(diodeTwo, LOW);
      }
      
      if(recv_value == 0xFD8877 || recv_value == 0x77E11033) {
        lastTime = millis(); // Start power saving delay timer
        digitalWrite(diodeTwo, HIGH);
        digitalWrite(diodeOne, LOW);
      }
      
      if(recv_value == 0x77E12033) {
        lastTime = millis(); // Start power saving delay timer
        digitalWrite(diodeOne, HIGH);
        digitalWrite(diodeTwo, HIGH);
      }
      
            
            
            
    }
    irReciver.resume(); // Read the next value
  }
  
  // To save power switch off all segments if 5 seconds has elapsed since 
  if ( millis() - lastTime >= 5000 )
  {
    digitalWrite(diodeOne, LOW);
    digitalWrite(diodeTwo, LOW);
      
    lastTime = 0; // Reset the power saving delay timer to zero '0'   
  }
}
