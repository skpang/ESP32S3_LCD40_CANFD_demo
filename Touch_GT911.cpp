/*
I2C device found at address 0x14  !
I2C device found at address 0x20  !
*/
#include "Touch_GT911.H"
#include <Wire.h> 
uint8_t touch_addr  = 0x5d;  //CTP IIC ADDRESS
//uint8_t touch_addr  = 0x14;  //CTP IIC ADDRESS

TouchLocation touchLocations[5];

unsigned char  GTP_CFG_DATA[] =
{

0x4F,0xE0,0x01,0xE0,0x01,0x05,0x35,0x00,0x01,0xC8,
0x28,0x0F,0x50,0x3C,0x03,0x05,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x18,0x1A,0x1E,0x14,0x85,0x25,0x0A,
0xEA,0xEC,0xB5,0x06,0x00,0x00,0x00,0x20,0x21,0x10,
0x00,0x01,0x00,0x0F,0x00,0x2A,0x00,0x00,0x01,0x50,
0x32,0xDC,0xFA,0x94,0xD0,0x02,0x08,0x00,0x00,0x04,
0x80,0xDE,0x00,0x80,0xE4,0x00,0x80,0xEA,0x00,0x7F,
0xF0,0x00,0x7F,0xF6,0x00,0x7F,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x14,0x12,0x10,0x0E,0x0C,0x0A,0x08,0x06,
0x04,0x02,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0x21,0x20,0x1F,0x1E,0x1D,0x00,0x02,0x04,
0x06,0x08,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xAA,0x01
};

void inttostr(uint16_t value,uint8_t *str);
uint8_t gt911_Send_Cfg(uint8_t * buf,uint16_t cfg_len);
uint8_t writeGT911TouchRegister( uint16_t regAddr,uint8_t *val, uint16_t cnt);
uint8_t readGT911TouchAddr( uint16_t regAddr, uint8_t * pBuf, uint8_t len );

uint32_t dist(const TouchLocation & loc);
uint32_t dist(const TouchLocation & loc1, const TouchLocation & loc2);
bool sameLoc( const TouchLocation & loc, const TouchLocation & loc2 );

uint8_t buf[80];


uint8_t gt911_Send_Cfg(uint8_t * buf,uint16_t cfg_len)
{
  uint8_t ret;
  uint8_t retry=0;

	ret = writeGT911TouchRegister(0x8047,buf,cfg_len);
	return ret;
}


uint8_t writeGT911TouchRegister( uint16_t regAddr,uint8_t *val, uint16_t cnt)
{	
  uint16_t i=0;
  uint8_t ret;

  Wire.beginTransmission(touch_addr);
  Wire.write( regAddr>>8 );  // register 0
  Wire.write( regAddr);  // register 0 
	for(i=0;i<cnt;i++,val++)//data
	{		
       Wire.write( *val );  // value
      
	}
  ret = Wire.endTransmission(); 
  return(ret);
}

uint8_t readGT911TouchAddr( uint16_t regAddr, uint8_t * pBuf, uint8_t len )
{
  Wire.beginTransmission(touch_addr);
  Wire.write( regAddr>>8 );  // register 0
  Wire.write( regAddr);  // register 0  
  uint8_t retVal = Wire.endTransmission();
  
  uint8_t returned = Wire.requestFrom(touch_addr, len);    // request 1 bytes from slave device #2
  
  uint8_t i;
  for (i = 0; (i < len) && Wire.available(); i++)
  
  {
    pBuf[i] = Wire.read();
  }
  
  return i;
}

uint8_t readGT911TouchLocation( TouchLocation * pLoc, uint8_t num )
{
  uint8_t retVal;
  uint8_t i;
  uint8_t k;
  uint8_t  ss[1];
  
  do
  {  
   //   Serial.println(" readGT911TouchLocation do ");
    if (!pLoc) break; // must have a buffer
    if (!num)  break; // must be able to take at least one
     ss[0]=0;
      readGT911TouchAddr( 0x814e, ss, 1);
      uint8_t status=ss[0];

    if ((status & 0x0f) == 0) break; // no points detected
    uint8_t hitPoints = status & 0x0f;
    
    //Serial.print("number of hit points = ");
   // Serial.println( hitPoints );
    
     uint8_t tbuf[40]; uint8_t tbuf1[32];uint8_t tbuf2[16];  
    readGT911TouchAddr( 0x8150, tbuf, 40);
    readGT911TouchAddr( 0x8150+32, tbuf1, 32);
    
      if(hitPoints<=4)
            {   
              for (k=0,i = 0; (i <  4*8)&&(k < num); k++, i += 8)
              {
                pLoc[k].x = tbuf[i+1] << 8 | tbuf[i+0];
                pLoc[k].y = tbuf[i+3] << 8 | tbuf[i+2];
              }   
            }
        if(hitPoints>4)   
            {  
               for (k=0,i = 0; (i <  4*8)&&(k < num); k++, i += 8)
              {
                pLoc[k].x = tbuf[i+1] << 8 | tbuf[i+0];
                pLoc[k].y = tbuf[i+3] << 8 | tbuf[i+2];
              }               
              
              for (k=4,i = 0; (i <  4*8)&&(k < num); k++, i += 8)
              {
                pLoc[k].x = tbuf1[i+1] << 8 | tbuf1[i+0];
                pLoc[k].y = tbuf1[i+3] << 8 | tbuf1[i+2];
              }   
            } 
            
                
            
    
    retVal = hitPoints;
    
  } while (0);
  
    ss[0]=0;
    writeGT911TouchRegister( 0x814e,ss,1); 
  
  return retVal;
}

void GT911_init(void)
{
  uint8_t ret;
  uint8_t buff[3];

  Serial.println("GT911_init sending config");

  ret=gt911_Send_Cfg((uint8_t*)GTP_CFG_DATA,sizeof(GTP_CFG_DATA));
//	writeGT911TouchRegister(0x8047,(uint8_t*)GTP_CFG_DATA,186);
  
  readGT911TouchAddr( 0x8140, buff, 2);
  Serial.printf("Product ID: %x %x ",buff[0],buff[1]);
  
  readGT911TouchAddr( 0x8142, buff, 2);
  Serial.printf("%x %x \n",buff[0],buff[1]);
 
  readGT911TouchAddr( 0x8144, buff, 2);
  Serial.printf("Firmware version: %x %x \n",buff[0],buff[1]);

}