#include "display.h"
#include "const.h"

Display::Display() : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1) {}

void Display::init() {
    Wire.setPins(SCREEN_SDA, SCREEN_SCL);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.display();
}

void Display::clear() {
    display.clearDisplay();
}

void Display::setText(const char* text) {
    strncpy(displayText, text, sizeof(displayText) - 1);
    displayText[sizeof(displayText) - 1] = '\0';
}

void Display::show() {
    display.clearDisplay();
    display.setTextSize(4);
    display.setCursor(0, 0);
    display.println(displayText);
    display.display();
}
