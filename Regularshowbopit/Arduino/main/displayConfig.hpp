#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C lcd;

inline void displayInit()
{
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Regular Show");
    lcd.setCursor(0, 1);
    lcd.print("Score: 0");
}

inline void displayScore(uint8_t score)
{
    lcd.setCursor(0, 1);
    lcd.print("Score:    ");
    lcd.setCursor(7, 1);
    lcd.print(score);
}

inline void displayMessage(const char *topLine, uint8_t score)
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(topLine);
    lcd.setCursor(0, 1);
    lcd.print("Score: ");
    lcd.print(score);
}

inline void displayStartingMessage()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Press Start");
}