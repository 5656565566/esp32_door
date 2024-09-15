#include <Arduino.h>
#include <TaskScheduler.h>

#include "const.h"
#include "easy_webserver.h"
#include "door.h"
#include "task.h"
#include "data.h"
#include "card.h"

WebServerManager webServer;

Scheduler runner;

Task task1(25, TASK_FOREVER, &loop_door);
Task task2(1, TASK_FOREVER, &loop_display);
Task task3(1000 * 60, TASK_FOREVER, &loopsyncTime);
Task task4(15, TASK_FOREVER, &loop_mfrc);

void setup()
{

    init_door();
    init_keypad();
    init_display();
    init_mfrc();

    runner.init();

    runner.addTask(task1);
	runner.addTask(task2);
    runner.addTask(task3);
    runner.addTask(task4);

    task1.enable();
    task2.enable();
    task3.enable();
    task4.enable();

    Serial.begin(115200);
    webServer.begin();  // 启动WiFi和Web服务器
    loopsyncTime();

    runner.startNow();
}

void loop()
{
    loop_keypad();
    webServer.handleClient();  // 处理客户端请求
    runner.execute();
}
