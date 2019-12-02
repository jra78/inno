
//valo ohjaus array/ light control
//tunti ja minuutti/hour and minute
volatile int valoOn[1];
volatile int valoOff[1];

//pumppu ohjaus array/pump array
//delaytime and pump time in minutes
volatile int pumppu[1];

//master on/off
volatile bool master = true;


//for I2C & time
#include <Wire.h>


//for RTC & date
#include <DS1307RTC.h>
#include <Time.h>


//For LCD
//#include <LiquidCrystal_I2C.h>

//LiquidCrystal_I2C lcd (0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address and initializing of pins

boolean relays[10] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};      //relays array, [0] is first relay and [5] is if 12V is send to output of first relay. [1] and [6] are for second relay ans so on



//serial to paraller pins
int data = 3;
int clk = 2;
int clr = 6;
//needed for RTC library/initialise RTC
tmElements_t tm;
//tallenna tämän hetken aika muuttujiin
//RTC.read(tm);
unsigned int vminuutit = tm.Minute;
unsigned int vtunnit = tm.Hour;



void setup() {
  //tallenna tämän hetken aika muuttujiin
  RTC.read(tm);
  vminuutit = tm.Minute;
  vtunnit = tm.Hour;

  // put your setup code here, to run once:

  //serial to paraller pins
  pinMode(clk, OUTPUT);
  pinMode(data, OUTPUT);
  pinMode(clr, OUTPUT);
}

void loop() {
  //valo ohjaus
  RTC.read(tm);
  if (master == true && valoOn[0] == tm.Hour && valoOn[1] == tm.Minute) {
    relays[0] = 1;

  }
  if (master == true && valoOff[0] == tm.Hour && valoOff[1] == tm.Minute) {
    relays[0] = 0;

  }
  //pumppu ohjaus
  //käynnistetään pumppu, jos aikaa on kulunut viiveen verran
  if (master == true && relays [1] == 0) {
    if ((vminuutit + pumppu[0]) >= 60) {
      unsigned int valiMin;
      unsigned int valiTun;

      valiMin = (vminuutit + pumppu[0]) - 60;
      valiTun = vtunnit + 1;
      if (valiTun > 23) {
        valiTun = 0;
      }
      if (master == true && valiTun == tm.Hour && valiMin == tm.Minute) {
        relays[1] = 1;
        vminuutit = tm.Minute;
        vtunnit = tm.Hour;
      }
      else if (master == true && vtunnit == tm.Hour && (vminuutit + pumppu[0]) == tm.Minute) {
        relays[1] = 1;
        vminuutit = tm.Minute;
        vtunnit = tm.Hour;
      }
    }
  }
  //sammutettaan pumppu, jos aikaa kulunut viiveen verran
  if (master == true && relays [1] == 1) {
    if ((vminuutit + pumppu[1]) >= 60) {
      unsigned int valiMin;
      unsigned int valiTun;

      valiMin = (vminuutit + pumppu[1]) - 60;
      valiTun = vtunnit + 1;
      if (valiTun > 23) {
        valiTun = 0;
      }
      if (master == true && valiTun == tm.Hour && valiMin == tm.Minute) {
        relays[1] = 0;
        vminuutit = tm.Minute;
        vtunnit = tm.Hour;
      }
      else if (master == true && vtunnit == tm.Hour && (vminuutit + pumppu[1]) == tm.Minute) {
        relays[1] = 0;
        vminuutit = tm.Minute;
        vtunnit = tm.Hour;
      }
    }

  }
//releiden asetus
setRelays();

  //turn off everything if something is wrong
  //ota kommentointi pois!
  /*
    if (master == false){
    relays[0] = 0;
    relays[1] = 0;
    relays[2] = 0;
    relays[3] = 0;
    relays[4] = 0;
    relays[5] = 0;
    relays[6] = 0;
    relays[7] = 0;
    relays[8] = 0;
    relays[9] = 0;
    setRelays();

    }
  */
}
void tick()
{
  delay (1);                                //remember to remove this line !!!
  digitalWrite(clk, HIGH);
  digitalWrite(clk, LOW);
}


void setRelays()
{
  // digitalWrite(clr, LOW);  //clear old relay status & activate them
  //delay (50);
  digitalWrite(clr, HIGH);


  digitalWrite (data, relays[9]);
  tick();
  digitalWrite (data, relays[8]);
  tick();
  digitalWrite (data, relays[7]);
  tick();
  digitalWrite (data, relays[6]);
  tick();
  digitalWrite (data, relays[5]);
  tick();
  digitalWrite (data, relays[4]);
  tick();
  digitalWrite (data, relays[3]);
  tick();
  digitalWrite (data, relays[2]);
  tick();
  digitalWrite (data, relays[1]);
  tick();
  digitalWrite (data, relays[0]);
  tick();


}


void relaytest()
{
  relays[0] = 1;
  relays[1] = 1;
  relays[2] = 1;
  relays[3] = 1;
  relays[4] = 1;
  relays[5] = 1;
  relays[6] = 1;
  relays[7] = 1;
  relays[8] = 1;
  relays[9] = 1;

  setRelays();
  //  lcd.clear();
  //  lcd.print ("All relays ON!");
  delay (1000);
  relays[0] = 0;
  relays[1] = 0;
  relays[2] = 0;
  relays[3] = 0;
  relays[4] = 0;
  relays[5] = 0;
  relays[6] = 0;
  relays[7] = 0;
  relays[8] = 0;
  relays[9] = 0;
  setRelays();

  delay (1000);

  int i;

  for (i = 0 ; i < 10; i++)
  {
    relays[i] = 1;

    setRelays();
    //    lcd.clear();
    //    lcd.print ("Testing relay nro");
    //    lcd.setCursor (0,1);
    //    lcd.print (i);

    delay (300);
    relays[i] = 0;

    setRelays();
  }

  relays[0] = 1;
  relays[1] = 1;
  relays[2] = 1;
  relays[3] = 1;
  relays[4] = 1;
  relays[5] = 1;
  relays[6] = 1;
  relays[7] = 1;
  relays[8] = 1;
  relays[9] = 1;
  setRelays();


}

bool setClock(int tunnit, int minuutit, int pv, int kk, int vuosi) {

  tm.Hour = tunnit;
  tm.Minute = minuutit;
  tm.Second = 0;
  tm.Day = pv;
  tm.Month = kk;
  tm.Year = vuosi;

  if (RTC.set)
    return true;
  else
    return false;
}
