#include "card.h"

MFRC522 mfrc522(RC_NSS, RC_RST);
Preferences preferences;
int save = 0;

void init_mfrc()
{
    SPI.begin(RC_SCK, RC_MISO, RC_MOSI, RC_NSS);
    mfrc522.PCD_Init();
    pinMode(TIP_LED, OUTPUT);
}

void storeUIDToPreferences()
{
    String uidString = "";
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
        uidString += String(mfrc522.uid.uidByte[i], HEX);
    }

    if (uidString != "")
    {
        save = 0;
        digitalWrite(TIP_LED, LOW);

    }

    preferences.begin("card");
    preferences.putString("uid", uidString);
    Serial.println("Card UID stored in Preferences: " + uidString);
    preferences.end();
}

void clearUID()
{
    for (byte i = 0; i < MFRC522::MF_KEY_SIZE; i++)
    {
        mfrc522.uid.uidByte[i] = 0;
    }
    mfrc522.uid.size = 0;
}

bool isStoredCard()
{
    preferences.begin("card");
    String storedUID = preferences.getString("uid", "");
    preferences.end();
    if (storedUID == "")
    {
        return false;
    }

    String currentUID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
        currentUID += String(mfrc522.uid.uidByte[i], HEX);
    }

    if (storedUID == currentUID)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void RESTMfrc(){
    mfrc522.PCD_Reset();
    mfrc522.PCD_AntennaOff();
    mfrc522.PCD_AntennaOn();
}

void loop_mfrc()
{
    RESTMfrc();
    save --;
    if (!mfrc522.PICC_IsNewCardPresent())
    {
        // return; // If no card is present
    }

    // Select one of the cards
    if (!mfrc522.PICC_ReadCardSerial())
    {
        // return; // If reading failed
    }
    // Print card UID to serial monitor

    if (mfrc522.uid.size)
    {
        Serial.print("\nCard UID: ");
        if (save > 0)
        {
            storeUIDToPreferences();
        }
        else if (isStoredCard())
        {
            add_new_record(2);
            open_door();
        }
    }
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX); // Print each byte of the UID in HEX
    }
    clearUID();

    mfrc522.PICC_HaltA();
}

void save_card()
{
    save = 77 * 60;
    digitalWrite(TIP_LED, HIGH);
}

void clear_card()
{
    preferences.begin("card");
    preferences.clear();
    preferences.end();
}