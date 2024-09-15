#include <ESP32Servo.h>

#include "door.h"

Servo sg90;

int pos = 0;
int angle = 0;
int increment = 3;
int open_time = 0;

bool is_open = false;

void rotateServo()
{
    angle += increment;

    if (angle >= 180 || angle <= 0)
    {
        increment = -increment;
    }

    sg90.write(angle);
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
    sg90.write(pos);
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
        rotateServo();
        digitalWrite(LED, HIGH);
    }
    else
    {
        digitalWrite(LED, LOW);
    }

    open_time--;
}