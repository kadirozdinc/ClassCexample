
#include <Arduino.h>
#include <lorawan.h>

// OTAA credentials
const char *devEui = "80B3D57ED004397D";
const char *appEui = "1300000000000013";
const char *appKey = "47521E11573093C237C7333983DD475C";

const unsigned long interval = 20 * 1000; // 10 s interval to send message
unsigned long previousMillis = 0;     // will store last time message sent
unsigned int counter = 0;             // message counter

char myStr[50];
char outStr[255];
byte recvStatus = 0;

#define LED 1

const sRFM_pins RFM_pins = {
    .CS = 7,
    .RST = 3,
    .DIO0 = 18,
    .DIO1 = 19,
    // .DIO2 = -1,
    // .DIO5 = -1,
};

void setup()
{
  // Setup loraid access
  Serial.begin(115200);
  while (!Serial)
    ;
  if (!lora.init())
  {
    Serial.println("RFM95 not detected");
    delay(5000);
    return;
  }
  pinMode(LED, OUTPUT);
  // Set LoRaWAN Class change CLASS_A or CLASS_C
  lora.setDeviceClass(CLASS_C);

  // Set Data Rate
  lora.setDataRate(SF7BW125);
  //lora.setTxPower1(0x0F); // set max power.

  // set channel to random
  lora.setChannel(MULTI);

  // Put OTAA Key and DevAddress here
  lora.setDevEUI(devEui);
  lora.setAppEUI(appEui);
  lora.setAppKey(appKey);

  // Join procedure
  bool isJoined;
  do
  {
    Serial.println("Joining...");
    isJoined = lora.join();

    // wait for 10s to try again
    delay(10000);
  } while (!isJoined);
  Serial.println("Joined to network");
}

void loop() {
  // Check interval overflow
  if(millis() - previousMillis > interval) {
    previousMillis = millis(); 

    sprintf(myStr, "Counter-%d", counter); 

    Serial.print("Sending: ");
    Serial.println(myStr);
    
    lora.sendUplink(myStr, strlen(myStr), 1, 1);
    counter++;
    delay(10);
   
  }

  recvStatus = lora.readData(outStr);
  if(recvStatus) {
    Serial.println(recvStatus);

    Serial.println(outStr);
  }
  
  // Check Lora RX
  lora.update();

    //  if(lora.readAck()) Serial.println("ack received");

 
}