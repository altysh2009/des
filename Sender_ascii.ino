#include <Keypad.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
SoftwareSerial BTserial(2, 3); // RX | TX
LiquidCrystal lcd(12, 11, A0, A1, A2, A3);
#define Password_Lenght 16 
char Data[Password_Lenght]; 
byte data_count = 0,master_count = 0;
char customKey;
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'A','E','I','O'},
  {'U','S','T','M'},
  {'N','R','P','H'},
  {'L','C','B','W'}
};
byte rowPins[ROWS] = {0, 13, 4, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 7,8, 9}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup() {
  // put your setup code here, to run once:
    lcd.begin(16, 2);
    pinMode(10,OUTPUT);
Serial.begin(9600);
    BTserial.begin(9600);  
    digitalWrite(10, LOW);
    }
String text="";
 String key="";
const   uint8_t pc1[56] PROGMEM = {56, 48, 40, 32, 24, 16,  8,
                 0, 57, 49, 41, 33, 25, 17,
                 9,  1, 58, 50, 42, 34, 26,
                 18, 10,  2, 59, 51, 43, 35,
                 62, 54, 46, 38, 30, 22, 14,
                  6, 61, 53, 45, 37, 29, 21,
                 13,  5, 60, 52, 44, 36, 28,
                 20, 12,  4, 27, 19, 11,  3  };

 const   uint8_t  pc2[48] PROGMEM ={ 13, 16, 10, 23,  0,  4,
                 2, 27, 14,  5, 20,  9,
                 22, 18, 11,  3, 25,  7,
                 15,  6, 26, 19, 12,  1,
                40, 51, 30, 36, 46, 54,
                 29, 39, 50, 44, 32, 47,
                43, 48, 38, 55, 33, 52,
                45, 41, 49, 35, 28, 31   };
const  uint8_t  shiftround[16] PROGMEM ={1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1}   ;

const PROGMEM uint8_t  IP[64] PROGMEM = {57, 49, 41, 33, 25, 17, 9,  1,
              59, 51, 43, 35, 27, 19, 11, 3,
              61, 53, 45, 37, 29, 21, 13, 5,
              63, 55, 47, 39, 31, 23, 15, 7,
              56, 48, 40, 32, 24, 16, 8,  0,
              58, 50, 42, 34, 26, 18, 10, 2,
              60, 52, 44, 36, 28, 20, 12, 4,
              62, 54, 46, 38, 30, 22, 14, 6 };
const PROGMEM uint8_t E_Box[48] PROGMEM = {31,  0,  1,  2,  3,  4,
                  3,  4,  5,  6,  7,  8,
                  7,  8,  9, 10, 11, 12,
                 11, 12, 13, 14, 15, 16,
                 15, 16, 17, 18, 19, 20,
                 19, 20, 21, 22, 23, 24,
                 23, 24, 25, 26, 27, 28,
                 27, 28, 29, 30, 31,  0};
const PROGMEM uint8_t Sbox[8][4][16] PROGMEM = {
                // S1
                {{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
                 {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
                 {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
                 {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},
                // S2
                {{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
                 {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
                 {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
                 {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},
                // S3
                {{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
                {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
                 {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
                 {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},
                // S4
                {{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
                 {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
                 {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
                 {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},
                // S5
                {{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
                 {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
                 {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
                 {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},
                // S6
                {{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
                 {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
                 {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
                 {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},
                //S7
                {{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
                 {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
                 {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
                 {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},
                // S8
                {{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
                 {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
                 {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
                 {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}};                               
const uint8_t P_Box[32]PROGMEM = {15, 6, 19, 20, 28, 11,27, 16,
                 0, 14, 22, 25,  4, 17, 30, 9,
                 1,  7, 23, 13, 31, 26,  2, 8,
                18, 12, 29,  5, 21, 10,  3, 24 };

const uint8_t IP_Inv[64]PROGMEM = {39,  7, 47, 15, 55, 23, 63, 31,
                  38,  6, 46, 14, 54, 22, 62, 30,
                  37,  5, 45, 13, 53, 21, 61, 29,
                  36,  4, 44, 12, 52, 20, 60, 28,
                  35,  3, 43, 11, 51, 19, 59, 27,
                  34,  2, 42, 10, 50, 18, 58, 26,
                  33,  1, 41,  9, 49, 17, 57, 25,
                  32,  0, 40,  8, 48, 16, 56, 24 };

 uint8_t INbox[64]; 
 String convertasciitobit(String text)
{String out="";
  for(int i=0; i<text.length(); i++){

   uint8_t myChar = text[i];
 
    for(int i=7; i>=4; i--){
      byte bytes = bitRead(myChar,i);
      out+=bytes;
      //Serial.println(bytes);
    }
     for(int i=3; i>=0; i--){
      byte bytes = bitRead(myChar,i);
      out+=bytes;
      //Serial.println(bytes);
    } 
      
}return out;
}      
 String converttobin(String c)
 {String out="";
  String in="";
  int x;
  String zeros="";
  String convert;
        // convert the incoming byte to a char
              // and add it to the string:
  for(int i=0;i<16;i++)
  {
  if (isDigit(c[i])) {
      in += c[i];
      x=in.toInt();
      ////Serial.println();
      convert=String(x,BIN);
      ////Serial.print(String(x,BIN)+' ');
      x=convert.length() ;
      for (int y=0;y<(4-x);y++)
       {zeros=zeros+"0";
      ////Serial.print(y);
       }
      out=out+zeros+convert;
      //Serial.print(freeRam () );
      convert="";
      zeros=""; 
      in="";
    }
  else if(c[i]=='a'||c[i]=='A')
     out=out+"1010"  ;
  else if(c[i]=='b'||c[i]=='B')
     out=out+"1011"  ;
  else if(c[i]=='c'||c[i]=='C')
     out=out+"1100"  ;
  else if(c[i]=='D'||c[i]=='d')
     out=out+"1101" ; 
  else if(c[i]=='e'||c[i]=='E')
     out=out+"1110" ; 
  else if(c[i]=='f'||c[i]=='F')
     out=out+"1111" ;      
  }
  return out;
 }
 //Converts Bits to Hexadecimal :
 //Save it To String :
void bittohex()
 {int b,pos=0;String out="";
 text="";
  for(int i=0;i<16;i++)
 { for(int y=0;y<4;y++)
  {
     b=(INbox[0+pos] <<3) + (INbox[1+pos] << 2) + (INbox[2+pos] << 1) + INbox[3+pos];
     out =String(b,HEX);
  }
  text+=out;
  pos+=4;
 }
 }
 //Generate The Key:
 
 uint8_t keys[48];
 void creatkey(String c,int h,uint8_t d='e')
 {
 
 uint8_t PC1_out[56];
 uint8_t cl;
 uint8_t cr;
 
 
//maping PC1:
for (int i=0;i<56;i++)
{int x=pgm_read_byte(&(pc1[i]));
 PC1_out[i]=c[x];
  }

if(d=='D')
h=15-h;
for (int i=0;i<16;i++)
{
//Suits the 16 round shifts:  
  for(int j=0;j< pgm_read_byte(&(shiftround[i]));j++)
  {
    
    
    cl=PC1_out[0];
   for(int k=0;k<27;k++)
  {
    PC1_out[k]=PC1_out[k+1];
  }
    
    PC1_out[27]=cl;
    cl=PC1_out[28];
     for(int k=0;k<27;k++)
  {
    PC1_out[k+28]=PC1_out[k+1+28];
  }
    PC1_out[27+28]=cl;
  }

 //PC2 maping :
  for(int q=0;q<48;q++)
  {
    int x=pgm_read_byte(&(pc2[q]));
    keys[q]=PC1_out[x]-48;

}
if(i==h)
break;

} }       
     int freeRam () 
    {
      extern int __heap_start, *__brkval; 
      int v; 
      return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
    }

uint8_t  RE[48];

    
uint8_t des(String c,char D='e') 
{
uint8_t qw=0;
                  
uint8_t   IP_out[64];/*L[32],R[32],*/
uint8_t list=0;
uint8_t pos=0;
String out="";
String row="" ;
String col="";
int b,a,val;
uint8_t  p[32];
//Doing Intial Permutation:
 for(int i=0;i<64;i++)
 {
   IP_out[i]= char(c[pgm_read_byte(&(IP[i]))])-48;
   }


for(int isa=0;isa<16;isa++)
{
  creatkey(key,isa,D);
  for(int q=0;q<48;q++)
 {
  //Doing E_Boxes:
     b=IP_out[pgm_read_byte(&(E_Box[q]))+32];
      a= (keys[q]);
  RE[q]= char(a^b);
 }

 sbox() ;
for(int pbox=0;pbox<32;pbox++)
{
  //Doing P_Box mapping:
 b=RE[pgm_read_byte(&(P_Box[pbox]))]; 
 a=IP_out[pbox];
 p[pbox]= char(a^b);
}
//Divide The Plain Text into two parts:
for(int left=0;left<32;left++)
{
IP_out[left]=IP_out[left+32];
}
for(int RIGH=0;RIGH<32;RIGH++)
{
IP_out[RIGH+32]=p[RIGH];
}
}
int cross;
for(int i=0;i<32;i++)
{

  cross=IP_out[i+32];
  IP_out[i+32]=IP_out[i];
  IP_out[i]=cross;
}
for(int i=0;i<64;i++)
{
    //Doing Inverse Permutation:
  INbox[i]=IP_out[pgm_read_byte(&(IP_Inv[i]))];
}

}
void sbox()
{
  uint8_t pos=0;
String out="";
String row="" ;
String col="";
int a,b,val,qw=0;
  for(int z=0;z<8;z++)

 {
  
  //fitting S_Boxes:
  a=(RE[0+pos]<<1)+RE[5+pos];
  
 
  b=(RE[1+pos] <<3) + (RE[2+pos] << 2) + (RE[3+pos] << 1) + RE[4+pos];
 
 val= pgm_read_byte(&(Sbox[z][a][b]));

 out =String(val,BIN);
 int rt=out.length() ;
String zeros="";
      for (int ya=0;ya<(4-rt);ya++)
       {zeros=zeros+"0";
       }

  pos+=6;

  row="";
  col="";
  out=zeros+out;
 
  zeros="";
  int w;
  for(w=0;w<4;w++)
  {RE[w+qw]=char(out[w])-48;
 
  }
 
  qw+=4;
 

 }
}
  

 
void checkcon()
{int re=1;
  while (re)
{
 BTserial.write('x');
 char xtc= char(BTserial.read());
 //Serial.print(xtc);
 if (xtc=='z')
 re=0;
 delay (20);
}
}
void senddata()
{
  for(int asd=0;asd<16;asd++){
  delay(5);
  BTserial.write(text[asd]);
 }
}
void serialFlush(){
    for (int i = 0; i < 10; i++)
 {
   while (BTserial.available() > 0)
   {
     char k = BTserial.read();
     //Serial.print(k);
    
     delay(20);
   }
   delay(20);
 }
} 
void loop() {
  digitalWrite(10, LOW);
   lcd.setCursor(0,0);
  lcd.print("Waiting   for");
  lcd.setCursor(0,1);
  lcd.print("  connection");
  delay(200);
   checkcon();
  
  serialFlush();
  digitalWrite(10, HIGH);
  Serial.print("connected");
  lcd.clear();
  delay(5);
  lcd.setCursor(0,0);
  lcd.print("connected");
  delay(1000);
  lcd.clear();
  delay(5);
lcd.setCursor(0,0);
lcd.print("Enter text");
     while(text.length()<8){
 text+= customKeypad.waitForKey();
 lcd.setCursor(0,1);
 lcd.print(text);}
 delay(500);
 lcd.clear();
 delay(5);
 lcd.setCursor(0,0);
lcd.print("Enter key");
    while(key.length()<8){
 key+= customKeypad.waitForKey();
 lcd.setCursor(0,1);
 lcd.print(key);}
 delay(500);
 lcd.clear();
 delay(5);
 Encrypt() ;
 lcd.setCursor(0,0);
 lcd.print(text);
 senddata();
 delay(600);
 int sdr=1;
 while(sdr)
 {int d=4;
  while(d>=0)
  {char xas=char(BTserial.read());
  lcd.setCursor(0,1);
    lcd.print("                     ");
    lcd.setCursor(0,1);
    delay(5);
  if (xas=='d')
  {
    sdr=0;
  d=-1;
  digitalWrite(10,HIGH);
  }
  else if(xas=='z')
  {
    lcd.setCursor(0,1);
    lcd.print("reconnected");
     senddata();
  }
  else 
  {BTserial.write('x');
    delay(20);
    }
     digitalWrite(10,LOW);
  //d-=1;
  delay(500);
 }
 lcd.setCursor(0,1);
 lcd.print("data not send");
 }
 /*
   for(int asd=0;asd<16;asd++){
  delay(5);
  BTserial.write(text[asd]);
 }
  */
Serial.print(text);
 lcd.setCursor(0,1);
 lcd.print("                    ");
    lcd.setCursor(0,1);
 delay(5);
 lcd.print("data send");
 delay(5000);
 lcd.clear();
 delay(5);
 lcd.setCursor(0,0);
 lcd.print("press any key to");
 lcd.setCursor(0,1);
 lcd.print( "    retry");
 char c=customKeypad.waitForKey();
 text="";
 key="";
 lcd.clear();
 delay(5);
 //digitalWrite(10, LOW);
  //digitalWrite(10, HIGH);
 }



void Encrypt(){
 text= convertasciitobit(text);
key=convertasciitobit(key);
des(text);
bittohex();
lcd.setCursor(0,0);
lcd.println(text);
}
