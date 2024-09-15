#ifndef CARD_H
#define CARD_H

#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Preferences.h>

#include "const.h"
#include "data.h"
#include "door.h"

void init_mfrc();
void loop_mfrc();
void clear_card();
void save_card();

#endif // CARD_H
