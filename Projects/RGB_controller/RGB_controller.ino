/*1.1_Write_by_ImaSoft_11.12.20
* Четверг     - v1.0  от 10.12.20, Произведена глубокая модернизация портированного проекта
* пятница     - v1.1  от 11.12.20, Произвел_оптимизацию_кода_добавил_восстановление_режима_после_перезагрузки_контроллера
* 
*/

#define NAME "RGB_controller"
#define VERSION "1,1"
//-------------------------------------------------------------//
#include <avr/wdt.h>                                           //Стандартная_библиотека_WDT_таймера
#include <PWM.h>                                               //В_Arduino_по_умолчанию_используется_таймер-0(5-6выводы) частота 976Гц(Менять_ее_нельзя),_все_остальные_по_497Гц,_это_Таймер-1_и_Таймер-2,их_можно_использовать_как_хотим 
#include <EEPROM.h>                                            //Стандартная_библиотека_для_работы_с_EEPROM_памятью
//-------------------------------------------------------------//

//---Дерективы_отладки--После_отдадки_все_за_комментировать----//
#define Debug_Serial                                           //Разрешить_использование_для_отладки_последавательный_порт
//#define Debug_Serial_LED                                     //Разрешить_вывод_информации_в_потоке_Elapsed Time
//---PINS------------------------------------------------------//
#define  pinBooton           2                                 //Вход_кнопки_выбора_режима_работы
#define  Red1                3                                 //
#define  Green1              5                                 //
#define  Blue1               6                                 //
#define  Red2                9                                 //
#define  Green2              10                                //
#define  Blue2               11                                //
//---LED-------------------------------------------------------//
#define LEDPIN               13                                //PIN13_подключенный_светодиод(Плата_UNO)
#define TimeLED             350//ms                            //Время_интервала_мигания_первого_светодиода_в_миллисекундах
//---Переменные------------------------------------------------//
#define  x                   8                                 //Колличество_программ
#define  Speed1              5                                 //ms
#define  Speed2              5                                 //ms
#define  Speed3              5                                 //ms
#define  Speed4              5                                 //ms
#define  Speed5              5                                 //ms
#define  Speed6              5                                 //ms
#define  Speed7              5                                 //ms
byte     r=0, g=0, b=0, i=0;                                   //
//---EEPROM----------------------------------------------------//
#define DATAEEPROMCURMODE    0                                 //Ячейка_Byte(ADDR:0)_памяти_EEPROM_значение_байта_номера_текущего_режима_работы
//---Разрешить_прерывание_от_компаратора_OCR0A-----------------//
#define STARTTIMERCOMPA {OCR0A = 0x7;TIMSK0 |= _BV(OCIE0A);}   //Используем_прерывание_от_компаратора_OCR0A_таймера-0,_расчетное_время_вызова:1/(16000000(частота_генератора_ЦПУ)/128(делитель_по_умолчанию)/256(полное_время_таймера_TCN0L))=0,002048мСб_так_как_таймер_TCN0L_не_сбрасывается_в_ноль_по_достижению_значения_компаратора_то_время_генерации_прерывания_будет_равно_времени_полного_цикла_таймера_TCN0L
//-------------------------------------------------------------//


//***START_ReadEEPROM_Byte*************************************//
uint8_t ReadEEPROM_Byte(uint8_t addr)                          //
{                                                              //
    return eeprom_read_byte((uint8_t*)addr);                   //
}                                                              //
//***END_ReadEEPROM_Byte***************************************//

//***START_WriteEEPROM_Byte************************************//
void WriteEEPROM_Byte(uint8_t addr, uint8_t data)  //сохранить в EEPROM
{                                                              //
    eeprom_write_byte((uint8_t*)addr, data);                   //
}                                                              //
//***END_WriteEEPROM_Byte**************************************//


//***START_offRGB**********************************************//
void offRGB(){                                                 //
r=0;g=0;b=0;                                                   //
analogWrite(Red1,LOW);analogWrite(Green1,LOW);analogWrite(Blue1,LOW);
analogWrite(Red2,LOW);analogWrite(Green2,LOW);analogWrite(Blue2,LOW);
}                                                              //
//***END_offRGB************************************************//



//***START_button2*********************************************//
void button2(){wdt_reset();                                    //
  if (digitalRead(pinBooton)==LOW){                            //
    delay(250);i++;offRGB();                                   //
    if (i>x){i=0;}                                             //
    WriteEEPROM_Byte(DATAEEPROMCURMODE,i);                     //
    #ifdef Debug_Serial                                        //
    Serial.print("Mode-");                                     //
    Serial.println(i);                                         //
    #endif                                                     //
  }                                                            //
 }                                                             //
//***END_button2***********************************************//


//***START_program1********************************************//
void program1(){                                               //
  while ((r < 255) & (i==1)) {button2();r++;                   //
    analogWrite(Red1,r);analogWrite(Red2,r);delay(Speed1);}    //
  while ((b > 0) & (i==1)) {button2(); b--;                    //
    analogWrite(Blue1,b);analogWrite(Blue2,b);delay(Speed1);}  //
  while ((g < 255) & (i==1)) {button2();g++;                   //
    analogWrite(Green1,g);analogWrite(Green2,g);delay(Speed1);}//
  while ((r > 0) & (i==1)) {button2();r--;                     //
    analogWrite(Red1,r);analogWrite(Red2,r);delay(Speed1);}    //
  while ((b < 255) & (i==1)) {button2();b++;                   //
    analogWrite(Blue1,b);analogWrite(Blue2,b);delay(Speed1);}  //
  while ((g > 0) & (i==1)) {button2();g--;                     //
    analogWrite(Green1,g);analogWrite(Green2,g);delay(Speed1);}//
 }                                                             //
//***END_program1**********************************************//


//***START_program2********************************************//
void program2() {                                              //
  while ((r < 255) & (i==2)) {button2();r++;                   //
    analogWrite(Red1, r);analogWrite(Red2, r);delay(Speed2);}  //
  while ((r > 0) & (i==2)) {button2();r--;                     //
    analogWrite(Red1, r);analogWrite(Red2, r);delay(Speed2);}  //
  while ((r < 255) & (i==2)) {button2(); r++;                  //
    analogWrite(Red1, r);analogWrite(Red2, r);                 //
    analogWrite(Blue1, r);analogWrite(Blue2, r);delay(Speed2);}//
  while ((r > 0) & (i==2)) {button2(); r--;                    //
    analogWrite(Red1, r);analogWrite(Red2, r);                 //
    analogWrite(Blue1, r);analogWrite(Blue2, r);delay(Speed2);}//
  while ((b < 255) & (i==2)) {button2(); b++;                  //
    analogWrite(Blue1, b);analogWrite(Blue2, b);delay(Speed2);}//
  while ((b > 0) & (i==2)) {button2(); b--;                    //
    analogWrite(Blue1, b);analogWrite(Blue2, b);delay(Speed2);}//
  while ((b < 255) & (i==2)) {button2(); b++;                  //
    analogWrite(Blue1, b);analogWrite(Blue2, b);               //
    analogWrite(Green1, b);analogWrite(Green2, b);delay(Speed2);}
  while ((b > 0) & (i==2)) {button2(); b--;                    //
    analogWrite(Blue1, b);analogWrite(Blue2, b);               //
    analogWrite(Green1, b);analogWrite(Green2, b);delay(Speed2);}
  while ((g < 255) & (i==2)) {button2(); g++;                  //
    analogWrite(Green1, g);analogWrite(Green2, g);delay(Speed2);}
  while ((g > 0) & (i==2)) {button2(); g--;                    //
    analogWrite(Green1, g);analogWrite(Green2, g);delay(Speed2);}
  while ((g < 255) & (i==2)) {button2(); g++;                  //
    analogWrite(Green1, g);analogWrite(Green2, g);             //
    analogWrite(Red1, g);analogWrite(Red2, g);delay(Speed2);}  //
  while ((g > 0) & (i==2)) {button2(); g--;                    //
    analogWrite(Green1, g);analogWrite(Green2, g);             //
    analogWrite(Red1, g);analogWrite(Red2, g);delay(Speed2);}  //
}                                                              //
//***END_program2**********************************************//


//***START_program3********************************************//
void program3() {                                              //
  while ((g < 255) & (i==3)) {button2();g++;                   //
    analogWrite(Green1, g);analogWrite(Red2, g);               //
    analogWrite(Blue1, g);analogWrite(Green2, g);              //
    analogWrite(Red1, g);analogWrite(Blue2, g);delay(Speed3);} //
  while ((g > 0) & (i==3)) {button2();g--;                     //
    analogWrite(Green1, g);analogWrite(Red2, g);               //
    analogWrite(Blue1, g);analogWrite(Green2, g);              //
    analogWrite(Red1, g);analogWrite(Blue2, g);delay(Speed3);} //
}                                                              //
//***END_program3**********************************************//


//***START_program4********************************************//
void program4() {b=255;analogWrite(Blue1, b);analogWrite(Red2, b);
  while ((r < 255) & (i==4)) {button2();r++;                   //
    analogWrite(Red1, r);analogWrite(Green2, r);delay(Speed4);}//
  while ((b > 0) & (i==4)) {button2();b--;                     //
    analogWrite(Blue1, b);analogWrite(Red2, b);delay(Speed4);} //
  while ((g < 255) & (i==4)) {button2();g++;                   //
    analogWrite(Green1, g);analogWrite(Blue2, g);delay(Speed4);}
  while ((r > 0) & (i==4)) {button2();r--;                     //
    analogWrite(Red1, r);analogWrite(Green2, r);delay(Speed4);}//
  while ((b < 255) & (i==4)) {button2();b++;                   //
    analogWrite(Blue1, b);analogWrite(Red2, b);delay(Speed4);} //
  while ((g > 0) & (i==4)) {button2();g--;                     //
    analogWrite(Green1, g);analogWrite(Blue2, g);delay(Speed4);}
}                                                              //
//***END_program4**********************************************//


//***START_program5********************************************//
void program5(){                                               //
  while ((r < 255) & (i==5)) {button2();r++;                   //
    analogWrite(Red1, r);analogWrite(Green2, r);delay(Speed5);}//
  while ((r > 0) & (i==5)) {button2();r--;                     //
    analogWrite(Red1, r);analogWrite(Green2, r);delay(Speed5);}//
  while ((r < 255) & (i==5)) {button2();r++;                   //
    analogWrite(Red1, r);analogWrite(Green2, r);               //
    analogWrite(Green1, r);analogWrite(Blue2, r);delay(Speed5);}
  while ((r > 0) & (i==5)) {button2();r--;                     //
    analogWrite(Red1, r);analogWrite(Green2, r);               //
    analogWrite(Green1, r);analogWrite(Blue2, r);delay(Speed5);}
  while ((b < 256) & (i==5)) {button2();b++;                   //
    analogWrite(Green1, b);analogWrite(Blue2, b);delay(Speed5);}
  while ((b > 0) & (i==5)) {button2();b--;                     //
    analogWrite(Green1, b);analogWrite(Blue2, b);delay(Speed5);}
  while ((b < 256) & (i==5)) {button2();b++;                   //
    analogWrite(Green1, b);analogWrite(Blue2, b);              //
    analogWrite(Blue1, b);analogWrite(Red2, b);delay(Speed5);} //
  while ((b > 0) & (i==5)) {button2();b--;                     //
    analogWrite(Green1, b);analogWrite(Blue2, b);              //
    analogWrite(Blue1, b);analogWrite(Red2, b);delay(Speed5);} //
  while ((g < 255) & (i==5)) {button2();g++;                   //
    analogWrite(Blue1, g);analogWrite(Red2, g);delay(Speed5);} //
  while ((g > 0) & (i==5)) {button2();g--;                     //
    analogWrite(Blue1, g);analogWrite(Red2, g);delay(Speed5);} //
  while ((g < 255) & (i==5)) {button2();g++;                   //
    analogWrite(Blue1, g);analogWrite(Red2, g);                //
    analogWrite(Red1, g);analogWrite(Green2, g);delay(Speed5);}//
  while ((g > 0) & (i==5)) {button2();g--;                     //
    analogWrite(Blue1, g);analogWrite(Red2, g);                //
    analogWrite(Red1, g);analogWrite(Green2, g);delay(Speed5);}//
}                                                              //
//***END_program5**********************************************//


//***START_program6********************************************//
void program6(){                                               //
  while ((g < 255) & (i==6)) {button2(); g++;                  //
    analogWrite(Green1, g);analogWrite(Red2, g);               //
    analogWrite(Blue1, g);analogWrite(Green2, g);;delay(Speed6);}
  while ((g > 0) & (i==6)) {button2(); g--;                    //
    analogWrite(Green1, g);analogWrite(Red2, g);               // 
    analogWrite(Blue1, g);analogWrite(Green2, g);delay(Speed6);}
  while ((g < 255) & (i==6)) {button2(); g++;                  //
    analogWrite(Green1, g);analogWrite(Red2, g);               //
    analogWrite(Red1, g);analogWrite(Blue2, g);delay(Speed6);} //
  while ((g > 0) & (i==6)) {button2(); g--;                    //
    analogWrite(Green1, g);analogWrite(Red2, g);               //
    analogWrite(Red1, g);analogWrite(Blue2, g);delay(Speed6);} //
  while ((g < 255) & (i==6)) {button2(); g++;                  //
    analogWrite(Blue1, g);analogWrite(Green2, g);              //
    analogWrite(Red1, g);analogWrite(Blue2, g);delay(Speed6);} //
  while ((g > 0) & (i==6)) {button2(); g--;                    //
    analogWrite(Blue1, g);analogWrite(Green2, g);              //
    analogWrite(Red1, g);analogWrite(Blue2, g);delay(Speed6);} //
}                                                              //
//***END_program7**********************************************//


//***START_program7********************************************//
void program7() {                                              //
  byte Random1 = random (3, 7);                                //
  byte Random4 = random (8, 12);                               //
  byte Random2 = random (3, 7);                                //
  byte Random5 = random (8, 12);                               //
  byte Random3 = random (3, 7);                                //
  byte Random6 = random (8, 12);                               //
  while ((g < 255) & (i==7)) {button2();g++;                   //
    analogWrite(Random1, g);analogWrite(Random4, g);           //
    analogWrite(Random2, g);analogWrite(Random5, g);           //
    analogWrite(Random3, g);analogWrite(Random6, g);delay(Speed7);}
  while ((g > 0) & (i==7)) {button2(); g--;                    //
    analogWrite(Random1, g);analogWrite(Random4, g);           //
    analogWrite(Random2, g);analogWrite(Random5, g);           //
    analogWrite(Random3, g);analogWrite(Random6, g);delay(Speed7);}
}                                                              //
//***END_program7**********************************************//


//***START_program8********************************************//
void program8() {                                              //
  byte Random1 = random (3, 7);                                //
  byte Random4 = random (8, 12);                               // 
  byte Random2 = random (3, 7);                                //
  byte Random5 = random (8, 12);                               //
  byte Random3 = random (3, 7);                                //
  byte Random6 = random (8, 12);                               //
  byte SpeedR = random (2, 10);                                //
  while ((g < 255) & (i==8)) {button2();g++;                   //
    analogWrite(Random1, g);analogWrite(Random4, g);           //
    analogWrite(Random2, g);analogWrite(Random5, g);           //
    analogWrite(Random3, g);analogWrite(Random6, g);delay(SpeedR);}
  while ((g > 0) & (i==8)) {button2();g--;                     //
    analogWrite(Random1, g);analogWrite(Random4, g);           //
    analogWrite(Random2, g);analogWrite(Random5, g);           //
    analogWrite(Random3, g);analogWrite(Random6, g);delay(SpeedR);}
}                                                              //
//***END_program8**********************************************//



//***START_SIGNAL**********************************************//Прерывание вызывается один раз в миллисекунду
unsigned long time = 0;                                        //
//-------------------------------------------------------------//
SIGNAL(TIMER0_COMPA_vect) {                                    //
//ISR(TIMER0_COMPA_vect) {                                     //
 //------------------------------------------------------------//
 TIMSK0 ^= _BV(OCIE0A);                                        //Запритить_прерывание_от_компаратора_OCR0A(В_PWM-6pin),_то_есть,_нельзя_использовать_одновременно_вывод_PWM_на_ножку-6_и_прерывание
 //------------------------------------------------------------//
 if (!(millis()%TimeLED)) {time ++;                           //
  digitalWrite(LEDPIN,!digitalRead(LEDPIN));                   //
  //Serial.println((millis()%1000));                           //
  if (!(time%4)) {                                             //
    #ifdef Debug_Serial_LED                                    //
    Serial.print("Elapsed Time-");Serial.print(time*TimeLED);Serial.println("ms");
    #endif                                                     //
  }
 }                                                             //
 //------------------------------------------------------------//
 TIMSK0 |= _BV(OCIE0A);                                        //Разрешить_прерывание_от_компаратора_OCR0A(В_PWM-6pin),_то_есть,_нельзя_использовать_одновременно_вывод_PWM_на_ножку-6_и_прерывание
 //------------------------------------------------------------//
}                                                              //
//***END_SIGNAL************************************************//







//***START_Setup***********************************************//
void setup() {                                                 //
  //---------------------------------------------------------- //  
  i = ReadEEPROM_Byte(DATAEEPROMCURMODE);                      //Чтение_c_EEPROM(ADDR:0)_значение_текущего_режима_работы
  if (i > x) {WriteEEPROM_Byte(DATAEEPROMCURMODE,0);i=0;}      //
  //-----------------------------------------------------------//
  #ifdef Debug_Serial                                          //
  Serial.begin(9600);                                          //
  Serial.println("============================");              //
  Serial.println(NAME);                                        //
  Serial.print("Version: ");                                   //
  Serial.println(VERSION);                                     //
  Serial.println("============================");              //
  Serial.print("Mode-");                                       //
  Serial.println(i);                                           //
  #endif                                                       //
  //---PINS----------------------------------------------------//
  pinMode(Red1,OUTPUT);pinMode(Green1,OUTPUT);pinMode(Blue1,OUTPUT);
  pinMode(Red2,OUTPUT);pinMode(Green2,OUTPUT);pinMode(Blue2,OUTPUT);
  pinMode(pinBooton,INPUT_PULLUP);                             //
  pinMode(LEDPIN, OUTPUT);                                     //
  //---Разрешить_прерывание_от_компаратора_OCR0A---------------//
  STARTTIMERCOMPA;                                             //
  //---WDT-----------------------------------------------------//
  wdt_enable (WDTO_8S);                                        //Активировать_WDT_таймер_на_период_8сек
  //wdt_disable();                                             //Rомманда_отключения_WDT_таймера
  //-----------------------------------------------------------//
  delay(1500);                                                 //
}                                                              //
//***END_Setup*************************************************//


//***START_loop************************************************//
void loop() {button2();                                        //
  if (i == 0){offRGB();}                                       //
  if (i == 1){program1();}                                     //
  if (i == 2){program2();}                                     //
  if (i == 3){program3();}                                     //
  if (i == 4){program4();}                                     //
  if (i == 5){program5();}                                     //
  if (i == 6){program6();}                                     //
  if (i == 7){program7();}                                     //
  if (i == 8){program8();}                                     //
  delay(1);                                                    //
}                                                              //
//***END_loop**************************************************//
