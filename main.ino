#include<SoftwareSerial.h>
#include<Servo.h>
SoftwareSerial sr(2, 3);
char bullshit[50] = {0xBB, 0xB6, 0xD3, 0xAD, 0xB9, 0xE2, 0xC1, 0xD9};
Servo se;
void SYN_FrameInfo(uint8_t Music, uint8_t *HZdata)
{
  unsigned char Frame_Info[50];
  unsigned char HZ_Length;
  unsigned char ecc = 0;
  unsigned int i = 0;
  HZ_Length = strlen(HZdata);
  Frame_Info[0] = 0xFD;
  Frame_Info[1] = 0x00;
  Frame_Info[2] = HZ_Length + 3;
  Frame_Info[3] = 0x01;
  Frame_Info[4] = 0x00 | Music << 4;
  /*******************校验码计算***************************************/
  for (i = 0; i < 5; i++) //依次发送构造好的5个帧头字节
  {
    ecc = ecc ^ (Frame_Info[i]); //对发送的字节进行异或校验
  }

  for (i = 0; i < HZ_Length; i++) //依次发送待合成的文本数据
  {
    ecc = ecc ^ (HZdata[i]); //对发送的字节进行异或校验
  }
  /*******************发送帧信息***************************************/
  memcpy(&Frame_Info[5], HZdata, HZ_Length);
  Frame_Info[5 + HZ_Length] = ecc;
  for (i = 0; i < 5 + HZ_Length + 1; i++)
  {
    sr.write(Frame_Info[i]);
  }
}
void doit(){
  Serial.write("Here\n");
  SYN_FrameInfo(0, bullshit);
  for(int i=0;i<=180;i++)se.write(i), delay(5);
  for(int i=180;~i;i--)se.write(i), delay(5);
  se.write(90);
}
void setup(){
  sr.begin(9600);
  Serial.begin(9600);
  pinMode(5, OUTPUT), pinMode(4, INPUT), se.attach(5);
  doit();
}
bool laststat=false;
void loop(){
  Serial.write(laststat?"Yes":"No");
  if(!laststat&&digitalRead(4)==HIGH)doit();
  laststat=digitalRead(4);
}
