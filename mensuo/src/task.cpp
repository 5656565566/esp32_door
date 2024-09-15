#include <Arduino.h>
#include <Keypad.h>
#include <Preferences.h>

#include "const.h"
#include "display.h"
#include "door.h"
#include "data.h"


Display display;

class PasswordManager
{
private:
    char *buffer;         // 用于存储字符串的缓冲区
    size_t maxLength;     // 最大字符串长度
    size_t currentLength; // 当前字符串长度

public:
    // 构造函数：初始化字符串管理器，设置最大长度
    PasswordManager(size_t maxLen)
    {
        maxLength = maxLen;
        buffer = new char[maxLength + 1]; // 分配空间，多1是为了存储结尾的'\0'
        currentLength = 0;
        buffer[0] = '\0'; // 初始化为空字符串
    }

    // 析构函数：释放内存
    ~PasswordManager()
    {
        delete[] buffer;
    }

    // 添加字符，成功返回 true，失败返回 false
    bool addChar(char c)
    {
        if (currentLength < maxLength)
        {
            buffer[currentLength++] = c;
            buffer[currentLength] = '\0'; // 添加 '\0' 作为字符串结尾
            return true;
        }
        else
        {
            return false; // 超过最大长度
        }
    }

    // 返回当前存储的字符串
    const char *getString()
    {
        return buffer;
    }

    // 清空字符串
    void clearString()
    {
        currentLength = 0;
        buffer[0] = '\0';
    }

    // 删除末尾的最后一个字符
    bool removeLastChar()
    {
        if (currentLength > 0)
        {
            currentLength--;
            buffer[currentLength] = '\0'; // 调整字符串结尾
            return true;
        }
        return false; // 字符串为空，无法删除
    }

    // 比较输入的字符串与存储的字符串是否一致
    bool compareString(const char *inputString)
    {
        return strcmp(buffer, inputString) == 0;
    }
};

const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};

PasswordManager stringManager(4);

byte rowPins[ROWS] = {KEY_7, KEY_6, KEY_5, KEY_4};
byte colPins[COLS] = {KEY_3, KEY_2, KEY_1};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void keypadEvent(KeypadEvent key)
{
    switch (keypad.getState())
    {
    case PRESSED:
        if (key == '#')
        {
            stringManager.removeLastChar();
            break;
        }
        if (key == '*')
        {
            Preferences preferences;
            preferences.begin("password", false);
            if (stringManager.compareString(preferences.getString("password", "xxxx").c_str()))
            {
                stringManager.clearString(); // 密码正确
                add_new_record(1);
                open_door();
            }
            else
            {
                stringManager.clearString(); // 密码错误
            }
            preferences.end();
            break;
        }
        stringManager.addChar(key);
        break;

    case RELEASED:
        if (key == '*')
        {
            break;
        }
        break;

    case HOLD:
        if (key == '*')
        {
            break;
        }
        if (key == '#')
        {
            close_door();
            break;
        }
        stringManager.addChar(key);
        break;
    }
}

void init_keypad()
{
    keypad.addEventListener(keypadEvent);
}

void loop_keypad()
{
    char key = keypad.getKey();
}

void init_display()
{
    display.init();
}

void loop_display()
{
    display.clear();
    display.setText(stringManager.getString());
    display.show();
}