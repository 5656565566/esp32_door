#include <ESP32Servo.h>

#include "door.h"

Servo sg90;

int pos = 0;
int angle = 0;
int open_time = 0;

bool is_open = false;

void rotateServo(int target_angle)
{
    pos = target_angle;

    if (target_angle != pos)
    {
        sg90.write(pos);
    }
}

void open_door()
{
    open_time = 1 * 400;
    is_open = true;
}

void close_door()
{
    open_time = 0;
    is_open = false;
}

void init_door()
{
    sg90.attach(SG90);
    sg90.write(0);
    pinMode(LED, OUTPUT);
}

void loop_door()
{

    if (open_time == 0)
    {
        is_open = false;
    }

    if (is_open)
    {
        rotateServo(180);
        digitalWrite(LED, HIGH);
    }
    else
    {
        rotateServo(0);
        digitalWrite(LED, LOW);
    }

    open_time--;
}