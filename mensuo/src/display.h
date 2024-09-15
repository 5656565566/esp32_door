#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class Display {
public:
    Display();
    void init();  // 初始化显示器
    void clear(); // 清空显示内容
    void setText(const char* text); // 设置显示内容
    void show();  // 显示内容

private:
    Adafruit_SSD1306 display;
    char displayText[64]; // 最大支持显示的字符串长度
};

#endif
