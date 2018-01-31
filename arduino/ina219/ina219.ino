#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;

/*most of these code stolen from https://github.com/kriswiner/ESP8266/blob/master/BMP280_ESP8266.ino */

#define I2C_DISPLAY_ADDRESS 0x3c
#define I2C_DISPLAY_DEVICE 4
char i2c_dev[I2C_DISPLAY_DEVICE][32]; // Array on string displayed

#define LED_PIN   16

// I2C Pins Settings
//#define SDA_PIN   5
//#define SDC_PIN   4

#define SDA_PIN   12
#define SDC_PIN   14

/* ======================================================================
  Function: i2cScan
  Purpose : scan I2C bus
  Input   : specifc address if looking for just 1 specific device
  Output  : number of I2C devices seens
  Comments: -
  ====================================================================== */
uint8_t i2c_scan(uint8_t address = 0xff)
{
  uint8_t error;
  int nDevices;
  uint8_t start = 1 ;
  uint8_t end   = 0x7F ;
  uint8_t index = 0;
  char device[16];
  char buffer[32];

  if (address >= start && address <= end) {
    start = address;
    end   = address + 1;
    Serial.print(F("Searching for device at address 0x"));
    Serial.printf("%02X ", address);
  } else {
    Serial.println(F("Scanning I2C bus ..."));
  }

  nDevices = 0;
  for (address = start; address < end; address++ ) {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.printf("Device ");

      if (address == 0x40)
        strcpy(device, "TH02" );
      else if (address == 0x29 || address == 0x39 || address == 0x49)
        strcpy(device, "TSL2561" );
      else if (address == I2C_DISPLAY_ADDRESS || address == I2C_DISPLAY_ADDRESS + 1)
        strcpy(device, "OLED SSD1306" );
      else if (address == 0x64)
        strcpy(device, "ATSHA204" );
      else
        strcpy(device, "Unknown" );

      sprintf(buffer, "0x%02X : %s", address, device );
      if (index < I2C_DISPLAY_DEVICE) {
        strcpy(i2c_dev[index++], buffer );
      }

      Serial.println(buffer);
      nDevices++;
    }
    else if (error == 4)
    {
      Serial.printf("Unknow error at address 0x%02X", address);
    }

    yield();
  }
  if (nDevices == 0)
    Serial.println(F("No I2C devices found"));
  else
    Serial.printf("Scan done, %d device found\r\n", nDevices);

  return nDevices;
}

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
  
  Wire.begin(SDA_PIN, SDC_PIN);
  ina219.begin();
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //ina219.setCalibration_16V_400mA();
  i2c_scan();

  delay(1000); 

}

void loop() {
#if 1
  uint8_t count = 30;

  // put your main code here, to run repeatedly:
  while (count --) {
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
    delay(1000);
  }
#endif
  
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;

  busvoltage = ina219.getBusVoltage_V();
  shuntvoltage = ina219.getShuntVoltage_mV();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);

#if 0
  Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
  Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
  Serial.println("");
#endif

  Serial.print(current_mA);
  Serial.println("");
}
