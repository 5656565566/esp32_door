#include "easy_webserver.h"

WebServerManager::WebServerManager() : server(80) {}

String html_tip(String msg)
{
    String page = "<html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>智能门锁控制面板</title></head><body>";
    page += "<div>";
    page += msg;
    page += "</div>";
    page += "<button onclick=\"location.href='/'\">返回</button><br>";
    page += "</body></html>";
    return page;
}

void WebServerManager::begin()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected!");
    Serial.println(WiFi.localIP());

    server.on("/", [this]()
              { handleRoot(); });
    server.on("/open-door", [this]()
              { handleOpenDoor(); });
    server.on("/change-password", [this]()
              { handleChangePassword(); });
    server.on("/add-card", [this]()
              { handleAddNewCard(); });
    server.on("/clear-cards", [this]()
              { handleClearCards(); });

    server.on("/records", [this]()
              { handleRecords(); });

    server.begin();
    Serial.println("Server started");
}

void WebServerManager::handleClient()
{
    server.handleClient();
}

bool WebServerManager::isAuthenticated()
{
    if (!server.authenticate(username.c_str(), password.c_str()))
    {
        server.requestAuthentication(); // 如果认证失败，请求输入用户名和密码
        return false;
    }
    return true;
}

void WebServerManager::handleRoot()
{
    if (!isAuthenticated())
    {
        return; // 如果没有通过身份验证，则返回
    }

    String page = "<html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>智能门锁控制面板</title></head><body>";
    page += "<h1>智能门锁控制面板</h1>";
    page += "<button onclick=\"location.href='/open-door'\">开门</button><br>";
    page += "<input type='text' id='new-password' placeholder='输入新的开门密码'>";
    page += "<button onclick=\"location.href='/change-password?newpass='+document.getElementById('new-password').value\">修改密码</button><br>";
    page += "<button onclick=\"location.href='/add-card'\">认证新卡片</button><br>";
    page += "<button onclick=\"location.href='/clear-cards'\">清空卡片认证</button><br>";
    page += "<button onclick=\"location.href='/records'\">查看历史开门记录</button>";
    page += "</body></html>";

    server.send(200, "text/html", page);
}

void WebServerManager::handleOpenDoor()
{
    if (!isAuthenticated())
    {
        return;
    }
    Serial.println("Opening door...");
    add_new_record(0);
    open_door();
    server.send(200, "text/html", html_tip("门已打开"));
}

void WebServerManager::handleChangePassword()
{
    if (!isAuthenticated())
    {
        return;
    }

    if (server.hasArg("newpass"))
    {
        String newPassword = server.arg("newpass");

        if (newPassword.length() != 4)
        {
            server.send(400, "text/html", html_tip("密码长度不为4"));
            return;
        }

        Preferences preferences;
        preferences.begin("password", false);
        preferences.putString("password", newPassword);

        Serial.println("Password changed to: " + newPassword);
        server.send(200, "text/html", html_tip("密码修改为" + newPassword + "成功"));
        preferences.end();
    }
    else
    {
        server.send(400, "text/html", html_tip("未输入密码"));
    }
}

void WebServerManager::handleAddNewCard()
{
    if (!isAuthenticated())
    {
        return;
    }
    Serial.println("Adding new card...");
    save_card();
    server.send(200, "text/html", html_tip("请把新卡片放置到读卡器上, 黄灯熄灭读取成功"));
}

void WebServerManager::handleClearCards()
{
    if (!isAuthenticated())
    {
        return;
    }
    clear_card();
    Serial.println("Clearing all cards...");
    server.send(200, "text/html", html_tip("所有已经认证的卡片已经清空"));
}

void WebServerManager::handleRecords()
{
    if (!isAuthenticated())
    {
        return;
    }
    Serial.println("Read Records...");
    server.send(200, "text/html", html_tip(read_record()));
}