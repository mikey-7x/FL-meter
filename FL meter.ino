#include <FreqCount.h>//https://github.com/PaulStoffregen/FreqCount/archive/master.zip
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

double pi = 3.1415926535;
double pipi = pi * pi;
int aco=1<<ACO;


void setup()
 {
  lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
//initialize comparator
  DDRD=0;
  PORTD=0;
  ACSR=0;
  ADCSRB=0;
  Serial.begin(9600);
  
  pinMode(4,INPUT);
  pinMode(3, OUTPUT);
  pinMode(2,INPUT);
  FreqCount.begin(1000);
}
unsigned long f;
float f0;
int x,n=3,r,p;


void loop()
{
p=digitalRead(2);
if(p==HIGH)
{mikey();}
else
{mikey2();}
}


void mikey(){
if(digitalRead(4)==HIGH)
{n++;x=0;delay(100);}
lcd.setCursor(0,1);
if(n==1){x++;
if(x==1){FreqCount.begin(100);}r=-1;lcd.print("T=0.1 s");
Serial.print("T=0.1 s\n");}
if(n==2){x++;if(x==1){FreqCount.begin(10000);}r=1;
lcd.print("T=10 s");
Serial.print("T=10 s\n");}
if(n==3){x++;
if(x==1){FreqCount.begin(1000);}r=0;
lcd.print("T=1 s");
 Serial.print("T=1 s\n");}
if(n>3)
{n=1;} 
lcd.setCursor(0,0);
lcd.print("F=");
Serial.print("F=");
if(f>=1000000 && n==3)
{f0=f/1000000.0;lcd.print(f0,6+r);
Serial.print(f0,6+r);
lcd.print(" MHz");
Serial.print(" MHz ");}
if(f<1000000 && n==3){f0=f/
1000.0;
lcd.print(f0,3+r);
Serial.print(f0,3+r);       
lcd.print(" kHz");
Serial.print(" kHz ");}
if(f>=100000 && n==1)
{f0=f/100000.0;lcd.print(f0,6+r);
Serial.print(f0,3+r);        
lcd.print(" MHz");
Serial.print(" MHz ");}
if(f<100000 && n==1)
{f0=f/100.0;
lcd.print(f0,3+r);
Serial.print(f0,3+r);
lcd.print(" kHz");
Serial.print(" kHz ");}
if(f>=10000000 && n==2)
{f0=f/10000000.0;lcd.print(f0,6+r);
Serial.print(f0,6+r);
lcd.print(" MHz");
Serial.print(" MHz ");}
if(f<10000000 && n==2){f0=f/
10000.0;lcd.print(f0,3+r);
Serial.print(f0,3+r);
lcd.print(" kHz");
Serial.print(" kHz ");}

if(FreqCount.available()) 
{ 
f=FreqCount.read(); 
lcd.setCursor(10,1);
lcd.print("***");
Serial.print(" *** ");
}
delay(50);
lcd.clear();
}

void mikey2(){
unsigned long timeout=1000000;
unsigned long t=0;
unsigned long total;
  
digitalWrite(3, HIGH); 
delay(100);        //charge the inductor.
digitalWrite(3,LOW);

//next three while loops wait for full wave to start and finish
//aco is a bit within ACSR register that represents output of the comparator

while((ACSR & aco)&& t<timeout)
{t++;}


unsigned long start = micros();
while(!(ACSR & aco)&& t<timeout)
{t++;}
while((ACSR & aco)&& t<timeout)
{t++;}
total=micros()-start-4;

double C=2.2;      //capacitor is in uF 
double T=total;    // T is in us
double total2 = (T*T) / ( 4.0 * pipi * C);
total2*=1;      //convert to mH

if(t>=timeout)
{
lcd.setCursor(4,0);
lcd.print("timeout");
Serial.println("timeout \n");
}
else
{
lcd.setCursor(4,0);
lcd.print(total2,3);
lcd.print("uH");
Serial.print(total2,3);
Serial.println("uH \n");
delay(1);
}
}
