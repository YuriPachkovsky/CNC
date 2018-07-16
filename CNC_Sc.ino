#include <MyMenu.h>
#include <U8g2lib.h>
#include <SPI.h>

//--ПИНЫ ЭНКОДЕРА--
#define CLK 2
#define DT 3          // пины енкодера
#define SW 4
//--ПИНЫ ЭНКОДЕРА--
boolean DT_now, DT_last,SW_state;
int ch; 
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* CS=*/ 10, /* reset=*/ 8);

#define NUM_ITEMS 14 //length of items array include submenu headers

MItm items[NUM_ITEMS] = {
  MItm("CNC Menu",0,0), //main header always 0,0
  MItm("x y z ",1),
  MItm("Exec file",2),
  MItm("Go home",100),
  MItm(" <<BACK",0),
  MItm("x y z ",1,0),
  MItm("X",110),
  MItm("Y",120),
  MItm("Z",130), 
  MItm(" <<BACK",0),
  MItm("Exec file",2,0), 
  MItm("sd card",140),
  MItm("Wifi",7),
  MItm(" <<BACK",0),
};
void menuCallback(int idx);

Menu menu(items,NUM_ITEMS,&u8g2,&menuCallback);

void setup() {
  //set encoder pins
  pinMode (CLK, INPUT);
  pinMode (DT, INPUT);
  pinMode(SW, INPUT_PULLUP);
  DT_last = digitalRead(CLK);         // читаем начальное положение CLK
  
  //initialize u8g2
  u8g2.begin();
  delay(100);
  u8g2.clearBuffer();
  u8g2.drawStr(0,20,"test");
  u8g2.sendBuffer();
  delay(5000);
  //show main menu on screen
  menu.goMain();
}

void loop() {
  ch = encoderTick();
  switch(ch){
    case 1:
    {
      menu.goDown();
      delay(100); 
      break;
    }
    case 2:
    {
      menu.goUp();
      delay(100); 
      break;  
    }
    case 0:
    {
      menu.goNext();                  // выбор
      delay(100); 
      break;
    }
  }
  delay(100);
}

// Логика работы для пунктов
void menuCallback(int idx){
  //do something according to index of item selected
  switch(idx)
  {
    case 100:
    {
     // GoHome();
      break;
    }  
    default:
      break;
  }
  
  menu.goLast(); //return to last viewed menu
}

//1 вниз -1 вверх 0 нажатие
int encoderTick() {          
  DT_now = digitalRead(CLK);          // читаем текущее положение CLK
  SW_state = !digitalRead(SW);        // читаем положение кнопки SW
  if(!SW_state)
    return 0;
  if (DT_now != DT_last) {            // если предыдущее и текущее положение CLK разные, значит был поворот
    if (digitalRead(DT) != DT_now) {  // если состояние DT отличается от CLK, значит крутим по часовой стрелке
      return 1;                     // вправо акей вниз
    } else {                          // если совпадают, значит против часовой
      return -1;                     //влево акей вверх
    }
  }
  DT_last = DT_now;                   // обновить значение
}


