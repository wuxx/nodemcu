#include <Wire.h>

#define LED_PIN   16

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("esp8266 setup!\r\n");
  pinMode(LED_PIN, OUTPUT);

  // Get some information abut the ESP8266

  uint32_t freeheap = ESP.getFreeHeap();
  Serial.print("Free Heap Size = "); Serial.println(freeheap);
  uint32_t chipID = ESP.getChipId();
  Serial.print("ESP8266 chip ID = "); Serial.println(chipID);
  uint32_t flashChipID = ESP.getFlashChipId();
  Serial.print("ESP8266 flash chip ID = "); Serial.println(flashChipID);
  uint32_t flashChipSize = ESP.getFlashChipSize();
  Serial.print("ESP8266 flash chip size = "); Serial.print(flashChipSize); Serial.println(" bytes");
  uint32_t flashChipSpeed = ESP.getFlashChipSpeed();
  Serial.print("ESP8266 flash chip speed = "); Serial.print(flashChipSpeed); Serial.println(" Hz");
  uint32_t getVcc = ESP.getVcc();
  Serial.print("ESP8266 supply voltage = "); Serial.print(getVcc); Serial.println(" volts");
  delay(4000); // give some time to read the screen
  
  //Wire.begin(SDA_PIN, SDC_PIN);

  delay(1000); 

}

void loop() {
  uint8_t count = 5;

  // put your main code here, to run repeatedly:
  while (count --) {
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
    delay(1000);
  }

}
