#include <M5StickC.h>

#define I2C_ADR_HTU21D 0x40
#define reset 0xFE
#define CMD_MESURE_TEMP 0xE3
#define CMD_MESURE_HUMID 0xE5
byte readbuffer[6];

int humid = 0;
int humidBlack = 0;
float dispTemp = 0.0;

void setup() 
{
    Serial.begin(115200);
    Serial.println("\nStart");
    Wire.begin(0, 26, 10000); //SDA(G0), SCL(G26)
    Wire.beginTransmission(I2C_ADR_HTU21D);
    Wire.write(reset);
    Wire.endTransmission();
    delay(100);

    M5.begin();
    M5.Axp.ScreenBreath(8);
    
    M5.Lcd.setRotation(1);
    M5.Lcd.fillScreen(TFT_BLACK);
    
    // frame(humidity progress bar)
    M5.Lcd.drawLine(7, 28, 7, 73, WHITE);
    M5.Lcd.drawLine(7, 73, 38, 73, WHITE);
    M5.Lcd.drawLine(38, 73, 38, 28, WHITE);
    M5.Lcd.drawLine(7, 28, 38, 28, WHITE);
}

void loop() 
{

    Wire.beginTransmission(I2C_ADR_HTU21D);
        Wire.write(CMD_MESURE_TEMP);
    Wire.endTransmission();
    delay(100);
    Wire.requestFrom(I2C_ADR_HTU21D,3);
    readbuffer[0] = Wire.read();
    readbuffer[1] = Wire.read();
    readbuffer[2] = Wire.read(); 
    int Temp0 = ( readbuffer[0] << 8 | readbuffer[1] ) ; 
    float Temp = -46.85 + 175.72 * Temp0 * 0.00001525878L;
    Serial.print("Temp ");Serial.println(Temp,1);
    Wire.beginTransmission(I2C_ADR_HTU21D);
    Wire.write(CMD_MESURE_HUMID);
    Wire.endTransmission();
    delay(100);
    Wire.requestFrom(I2C_ADR_HTU21D,3);
    readbuffer[0] = Wire.read();
    readbuffer[1] = Wire.read();
    readbuffer[2] = Wire.read(); 
    uint16_t RH0 = readbuffer[0] << 8 | readbuffer[1] ; 
    float RH = -6 + 125.0 * RH0 * 0.00001525878L;
    Serial.print(" ");Serial.print(RH,0);Serial.println(" RH");
    Serial.println("--- ");
    dispTemp = Temp; //temperature
     
//    M5.Lcd.fillRect(45, 30, 135, 50, BLACK);
//    M5.Lcd.setCursor(60, 30, 1);
    M5.Lcd.fillRect(40, 30, 125, 50, BLACK);
    M5.Lcd.setCursor(50, 30, 1);
    M5.Lcd.setTextFont(4);
    if(dispTemp>0){
      M5.Lcd.printf("+%.1f 'C\r\n",dispTemp);
    }else if(dispTemp<0){
      M5.Lcd.printf("-%.1f 'C\r\n",dispTemp);
    }else{
      M5.Lcd.println(" 0.0 'C\r\n");
    }
  
    humid = (int)(RH);
    humidBlack = map((100-humid),0, 100, 0,50);
  
    //humidity[%]
    M5.Lcd.setCursor(6, 0, 2);
    M5.Lcd.setTextFont(2);
    M5.Lcd.printf("%3d%%\r\n",humid);
   
    //humidity progress bar
    M5.Lcd.fillRect(8, 29, 30, 44, GREEN);
    M5.Lcd.fillRect(8, 29, 30, humidBlack - 6, BLACK);
    delay(2000);
}
