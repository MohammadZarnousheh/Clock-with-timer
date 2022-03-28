#include "RTClib.h"
#include <EEPROM.h>
#include <LiquidCrystal.h>
//*********************************************Config RTC
RTC_DS1307 rtc;
//*********************************************Config Pin_Keypad
const int PIN_SW_DOWN = 3;
const int PIN_SW_SET = 4;
const int PIN_SW_UP = 2;
//*********************************************Config Pin_Relay
const int Relay =  8;
//*********************************************Config Pin_LCD

//const int LCD_Pin_RS = 5;
//const int LCD_Pin_E = 6;
//const int LCD_Pin_D4 = 7;
//const int LCD_Pin_D5 = 8;
//const int LCD_Pin_D6 = 9;
//const int LCD_Pin_D7 = 10;

LiquidCrystal lcd( 12, 13, A0, A1, A2, A3);
//*********************************************Config Value
char daysOfTheWeek[7][12] = {"[Su]", "[Mo]", "[Tu]", "[We]", "[Th]", "[Fr]", "[Sa]"};
boolean key = 0;
int _hour, _minute;
int _year, _month, _day;
boolean key_SET , key_DOWN, key_UP;
int hourRelay_on;
int minuteRelay_on;
int hourRelay_off;
int minuteRelay_off;
//*********************************************Subprogram setup()
void setup() {
  rtc.begin();
  pinMode(PIN_SW_DOWN, INPUT_PULLUP);
  pinMode(PIN_SW_SET, INPUT_PULLUP);
  pinMode(PIN_SW_UP, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(Relay, OUTPUT);
  EEPROM.get(1, hourRelay_on);
  EEPROM.get(4, minuteRelay_on);
  EEPROM.get(8, hourRelay_off);
  EEPROM.get(12, minuteRelay_off);
  if (! rtc.isrunning()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  lcd.begin(16, 2);
  lcd.clear();
  //  lcd.setCursor(0, 0);
  lcd.print("MyElectro");
  //  lcd.setCursor (0, 1);
  //  lcd.print("MyElectro");
  delay(2000);
  lcd.clear();
}
//*********************************************Subprogram loop()
void loop() {
  DateTime now = rtc.now();
  lcd.setCursor(0, 0);
  lcd.print(now.hour());
  lcd.print(":");
  lcd.print(now.minute());
  lcd.print(":");
  lcd.print(now.second());
  lcd.print("   ");
  lcd.setCursor(12, 0);
  lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
  lcd.setCursor(0, 1);
  lcd.print(now.year());
  lcd.print("/");
  lcd.print(now.month());
  lcd.print("/");
  lcd.print(now.day());
  lcd.print("   ");

  if (hourRelay_on == now.hour()) {
    if (minuteRelay_on == now.minute()) {
      digitalWrite(Relay, HIGH);
    }
  }
  if (hourRelay_off == now.hour()) {
    if (minuteRelay_off == now.minute()) {
      digitalWrite(Relay, LOW);
    }
  }



  key_UP = digitalRead(PIN_SW_UP);
  key_SET = digitalRead(PIN_SW_SET);
  key_DOWN = digitalRead(PIN_SW_DOWN);
  if (key == 0 && key_SET == 0 && key_DOWN == 1 && key_UP == 1) {
    key = 1;
    _hour = now.hour();
    _minute = now.minute();
    _year = now.year();
    _month = now.month();
    _day = now.day();
    int sensorVal = digitalRead(7);
    if (sensorVal == LOW) {
      SET_hour();
    } else {
      SET_hourRelay_on();
    }
  }
  if (key == 1 && key_SET == 1 && key_DOWN == 1 && key_UP == 1) {
    key = 0;
  }
}
//*********************************************Subprogram SET_hour()
void SET_hour() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("> SET Hour");
  lcd.setCursor (0, 1);
  lcd.print("Hour = ");
  while (1) {
    lcd.setCursor (7, 1);
    lcd.print(_hour);
    lcd.print("   ");
    key_UP = digitalRead(PIN_SW_UP);
    key_SET = digitalRead(PIN_SW_SET);
    key_DOWN = digitalRead(PIN_SW_DOWN);
    if (key == 0 && key_SET == 1 && key_DOWN == 0 && key_UP == 1) {
      key = 1;
      if (_hour == 0) {
        _hour = 23;
      } else {
        _hour--;
      }
    }
    if (key == 0 && key_SET == 1 && key_DOWN == 1 && key_UP == 0) {
      key = 1;
      if (_hour == 23) {
        _hour = 0;
      } else {
        _hour++;
      }
    }
    if (key == 0 && key_SET == 0 && key_DOWN == 1 && key_UP == 1) {
      key = 1;
      break;
    }
    if (key == 1 && key_SET == 1 && key_DOWN == 1 && key_UP == 1) {
      key = 0;
    }
  }
  SET_minute();
}
//*********************************************Subprogram SET_minute()
void SET_minute() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("> SET Minute");
  lcd.setCursor (0, 1);
  lcd.print("Minute = ");
  while (1) {
    lcd.setCursor (9, 1);
    lcd.print(_minute);
    lcd.print("   ");
    key_UP = digitalRead(PIN_SW_UP);
    key_SET = digitalRead(PIN_SW_SET);
    key_DOWN = digitalRead(PIN_SW_DOWN);
    if (key == 0 && key_SET == 1 && key_DOWN == 0 && key_UP == 1) {
      key = 1;
      if (_minute == 0) {
        _minute = 59;
      } else {
        _minute--;
      }
    }
    if (key == 0 && key_SET == 1 && key_DOWN == 1 && key_UP == 0) {
      key = 1;
      if (_minute == 59) {
        _minute = 0;
      } else {
        _minute++;
      }
    }
    if (key == 0 && key_SET == 0 && key_DOWN == 1 && key_UP == 1) {
      key = 1;
      break;
    }
    if (key == 1 && key_SET == 1 && key_DOWN == 1 && key_UP == 1) {
      key = 0;
    }
  }
  SET_year();
}
//*********************************************Subprogram SET_year()
void SET_year() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("> SET Year");
  lcd.setCursor (0, 1);
  lcd.print("Year = ");
  while (1) {
    lcd.setCursor (7, 1);
    lcd.print(_year);
    lcd.print("   ");
    key_UP = digitalRead(PIN_SW_UP);
    key_SET = digitalRead(PIN_SW_SET);
    key_DOWN = digitalRead(PIN_SW_DOWN);
    if (key == 0 && key_SET == 1 && key_DOWN == 0 && key_UP == 1) {
      key = 1;
      if (_year == 2014) {
        _year = 2030;
      } else {
        _year--;
      }
    }
    if (key == 0 && key_SET == 1 && key_DOWN == 1 && key_UP == 0) {
      key = 1;
      if (_year == 2030) {
        _year = 2014;
      } else {
        _year++;
      }
    }
    if (key == 0 && key_SET == 0 && key_DOWN == 1 && key_UP == 1) {
      key = 1;
      break;
    }
    if (key == 1 && key_SET == 1 && key_DOWN == 1 && key_UP == 1) {
      key = 0;
    }
  }
  SET_month();
}
//*********************************************Subprogram SET_month()
void SET_month() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("> SET Month");
  lcd.setCursor (0, 1);
  lcd.print("Month = ");
  while (1) {
    lcd.setCursor (8, 1);
    lcd.print(_month);
    lcd.print("   ");
    key_UP = digitalRead(PIN_SW_UP);
    key_SET = digitalRead(PIN_SW_SET);
    key_DOWN = digitalRead(PIN_SW_DOWN);
    if (key == 0 && key_SET == 1 && key_DOWN == 0 && key_UP == 1) {
      key = 1;
      if (_month == 1) {
        _month = 12;
      } else {
        _month--;
      }
    }
    if (key == 0 && key_SET == 1 && key_DOWN == 1 && key_UP == 0) {
      key = 1;
      if (_month == 12) {
        _month = 1;
      } else {
        _month++;
      }
    }
    if (key == 0 && key_SET == 0 && key_DOWN == 1 && key_UP == 1) {
      key = 1;
      break;
    }
    if (key == 1 && key_SET == 1 && key_DOWN == 1 && key_UP == 1) {
      key = 0;
    }
  }
  SET_day();
}
//*********************************************Subprogram SET_day()
void SET_day() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("> SET Day");
  lcd.setCursor (0, 1);
  lcd.print("Day = ");
  while (1) {
    lcd.setCursor (6, 1);
    lcd.print(_day);
    lcd.print("   ");
    key_UP = digitalRead(PIN_SW_UP);
    key_SET = digitalRead(PIN_SW_SET);
    key_DOWN = digitalRead(PIN_SW_DOWN);
    if (key == 0 && key_SET == 1 && key_DOWN == 0 && key_UP == 1) {
      key = 1;
      if (_day == 1) {
        _day = 31;
      } else {
        _day--;
      }
    }
    if (key == 0 && key_SET == 1 && key_DOWN == 1 && key_UP == 0) {
      key = 1;
      if (_day == 31) {
        _day = 1;
      } else {
        _day++;
      }
    }
    if (key == 0 && key_SET == 0 && key_DOWN == 1 && key_UP == 1) {
      key = 1;
      break;
    }
    if (key == 1 && key_SET == 1 && key_DOWN == 1 && key_UP == 1) {
      key = 0;
    }
  }

  rtc.adjust(DateTime(_year, _month, _day, _hour, _minute, 0));
  //SET_hourRelay_on();
  Save();
}

//*********************************************Subprogram SET_hourRelay_on()
void SET_hourRelay_on() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("hour Relay on");
  lcd.setCursor (0, 1);
  lcd.print("hour = ");
  while (1) {
    lcd.setCursor (6, 1);
    lcd.print(hourRelay_on);
    lcd.print("   ");
    key_UP = digitalRead(PIN_SW_UP);
    key_SET = digitalRead(PIN_SW_SET);
    key_DOWN = digitalRead(PIN_SW_DOWN);
    if (key == 0 && key_SET == 1 && key_DOWN == 0 && key_UP == 1) {
      key = 1;
      if (hourRelay_on == 0) {
        hourRelay_on = 23;
      } else {
        hourRelay_on--;
      }
    }
    if (key == 0 && key_SET == 1 && key_DOWN == 1 && key_UP == 0) {
      key = 1;
      if (hourRelay_on == 23) {
        hourRelay_on = 1;
      } else {
        hourRelay_on++;
      }
    }
    if (key == 0 && key_SET == 0 && key_DOWN == 1 && key_UP == 1) {
      key = 1;
      break;
    }
    if (key == 1 && key_SET == 1 && key_DOWN == 1 && key_UP == 1) {
      key = 0;
    }
  }
  SET_minuteRelay_on();
}
//*********************************************Subprogram SET_minuteRelay_on()
void SET_minuteRelay_on() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("minute Relay on");
  lcd.setCursor (0, 1);
  lcd.print("minute = ");
  while (1) {
    lcd.setCursor (8, 1);
    lcd.print(minuteRelay_on);
    lcd.print("   ");
    key_UP = digitalRead(PIN_SW_UP);
    key_SET = digitalRead(PIN_SW_SET);
    key_DOWN = digitalRead(PIN_SW_DOWN);
    if (key == 0 && key_SET == 1 && key_DOWN == 0 && key_UP == 1) {
      key = 1;
      if (minuteRelay_on == 0) {
        minuteRelay_on = 59;
      } else {
        minuteRelay_on--;
      }
    }
    if (key == 0 && key_SET == 1 && key_DOWN == 1 && key_UP == 0) {
      key = 1;
      if (minuteRelay_on == 59) {
        minuteRelay_on = 1;
      } else {
        minuteRelay_on++;
      }
    }
    if (key == 0 && key_SET == 0 && key_DOWN == 1 && key_UP == 1) {
      key = 1;
      break;
    }
    if (key == 1 && key_SET == 1 && key_DOWN == 1 && key_UP == 1) {
      key = 0;
    }
  }
  SET_hourRelay_off();
}
//*********************************************Subprogram SET_hourRelay_off()
void SET_hourRelay_off() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("hour Relay off");
  lcd.setCursor (0, 1);
  lcd.print("hour = ");
  while (1) {
    lcd.setCursor (6, 1);
    lcd.print(hourRelay_off);
    lcd.print("   ");
    key_UP = digitalRead(PIN_SW_UP);
    key_SET = digitalRead(PIN_SW_SET);
    key_DOWN = digitalRead(PIN_SW_DOWN);
    if (key == 0 && key_SET == 1 && key_DOWN == 0 && key_UP == 1) {
      key = 1;
      if (hourRelay_off == 0) {
        hourRelay_off = 23;
      } else {
        hourRelay_off--;
      }
    }
    if (key == 0 && key_SET == 1 && key_DOWN == 1 && key_UP == 0) {
      key = 1;
      if (hourRelay_off == 23) {
        hourRelay_off = 1;
      } else {
        hourRelay_off++;
      }
    }
    if (key == 0 && key_SET == 0 && key_DOWN == 1 && key_UP == 1) {
      key = 1;
      break;
    }
    if (key == 1 && key_SET == 1 && key_DOWN == 1 && key_UP == 1) {
      key = 0;
    }
  }
  SET_minuteRelay_off();
}
//*********************************************Subprogram SET_minuteRelay_off()
void SET_minuteRelay_off() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("minute Relay off");
  lcd.setCursor (0, 1);
  lcd.print("minute = ");
  while (1) {
    lcd.setCursor (8, 1);
    lcd.print(minuteRelay_off);
    lcd.print("   ");
    key_UP = digitalRead(PIN_SW_UP);
    key_SET = digitalRead(PIN_SW_SET);
    key_DOWN = digitalRead(PIN_SW_DOWN);
    if (key == 0 && key_SET == 1 && key_DOWN == 0 && key_UP == 1) {
      key = 1;
      if (minuteRelay_off == 0) {
        minuteRelay_off = 59;
      } else {
        minuteRelay_off--;
      }
    }
    if (key == 0 && key_SET == 1 && key_DOWN == 1 && key_UP == 0) {
      key = 1;
      if (minuteRelay_off == 59) {
        minuteRelay_off = 1;
      } else {
        minuteRelay_off++;
      }
    }
    if (key == 0 && key_SET == 0 && key_DOWN == 1 && key_UP == 1) {
      key = 1;
      break;
    }
    if (key == 1 && key_SET == 1 && key_DOWN == 1 && key_UP == 1) {
      key = 0;
    }
  }
  Save();
}
//*********************************************Subprogram Save()
void Save() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(">    Save...    ");
  EEPROM.put(1, hourRelay_on);
  EEPROM.put(4, minuteRelay_on);
  EEPROM.put(8, hourRelay_off);
  EEPROM.put(12, minuteRelay_off);
  delay(2000);
  lcd.clear();
}
