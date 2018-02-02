#include <Wire.h>
#include <Adafruit_ssd1306syp.h>

/*most of these code stolen from https://github.com/kriswiner/ESP8266/blob/master/BMP280_ESP8266.ino */

#define I2C_DISPLAY_ADDRESS 0x3c
#define I2C_DISPLAY_DEVICE 4
char i2c_dev[I2C_DISPLAY_DEVICE][32]; // Array on string displayed

#define LED_PIN   16

#if 1
// I2C Pins Settings
#define SDA_PIN   5
#define SCL_PIN   4
#else
#define SDA_PIN   0
#define SCL_PIN   2
#endif

Adafruit_ssd1306syp display(SDA_PIN, SCL_PIN);

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
  delay(1000); // give some time to read the screen

  

  i2c_scan();
#if 0
  Wire.begin(SDA_PIN, SCL_PIN);
  pinMode(SDA_PIN, OUTPUT);
  pinMode(SCL_PIN, OUTPUT);
  
  digitalWrite(SDA_PIN, HIGH);
  digitalWrite(SCL_PIN, LOW);
#endif
  delay(1000); 

  display.initialize();//oled初始化

  display.setTextColor(WHITE);//设置oled文字颜色
  display.setTextSize(2);//设置oled文字大小
  display.setCursor(0, 24); //设置oled指针位置
  display.print("Hello!");//oled显示文字
  display.update();
  
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
