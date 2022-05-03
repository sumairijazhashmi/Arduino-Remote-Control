#include <EEPROM.h>

#include <IRremote.hpp>


const int FAN_PIN = 7;
const int PUMP_PIN = 6;

int receiver = 11;
IRrecv irrecv(receiver);
decode_results results;

bool fan_on = false;
bool pump_on = false;

void setup() {
  // put your setup code here, to run once:

  delay(300); // Needed otherwise setup runs twice on a reset with usb
  
  Serial.begin(9600);

  Serial.println("Started Arduino");
  
  irrecv.enableIRIn();

  restore_state();
  pinMode(FAN_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);
  
}

void restore_state()
{
  EEPROM.get(0,fan_on);
  EEPROM.get(10,pump_on);

  if (pump_on == true)
  {
    Serial.println("GOT pump on");
  }
  else
  {
    Serial.println("GOT pump off");
  }

  if (fan_on == true)
  {
    Serial.println("GOT fan on");
  }
  else
  {
    Serial.println("GOT fan off");
  }
  

  if (fan_on == true)
  {
    turn_on_fan();
  }
  else
  {
    turn_off_fan();
  }

  if (pump_on == true)
  {
    turn_on_water_pump();
  }
  else
  {
    turn_off_water_pump();
  }
}


void turn_on_fan()
{
  digitalWrite(FAN_PIN, LOW);
  fan_on = true;
  EEPROM.put(0, fan_on);  
}

void turn_off_fan()
{
  digitalWrite(FAN_PIN, HIGH);
  fan_on = false;
  EEPROM.put(0, fan_on);
}

void turn_on_water_pump()
{
  digitalWrite(PUMP_PIN, LOW);
  pump_on = true;
  EEPROM.put(10, pump_on);
  
}

void turn_off_water_pump()
{
  digitalWrite(PUMP_PIN, HIGH);
  pump_on = false;
  EEPROM.put(10, pump_on);  
}

void turn_off_all()
{    
  turn_off_fan();
  turn_off_water_pump();
}

void turn_on_all()
{
  turn_on_fan();
  turn_on_water_pump();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (irrecv.decode())
  {

    uint16_t command = irrecv.decodedIRData.command;

    if (command == 18)
    {
      Serial.println("Volume Up - Turn On Fan");
      turn_on_fan();

    }
    else if (command == 19)
    {
      Serial.println("Volume Down - Turn Off Fan");
      turn_off_fan();
    }
    else if (command == 16)
    {
      Serial.println("Program Up - Turn On Pump");
      turn_on_water_pump();
    }
    else if (command == 17)
    {
      Serial.println("Program Down - Turn Off Pump");
      turn_off_water_pump();
    }
    else if (command == 21)
    {
      Serial.println("Power OFF - Turn everything");
      turn_off_all();
    }
    else if (command == 96)
    {
      Serial.println("Power ON - Turn on everything");
      turn_on_all();
    }

    
    else
    {
      Serial.println(command);
    }

    //Serial.println(results.value,HEX);
    delay(250);
    irrecv.resume();
  }

}
