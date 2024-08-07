/*
 * Meiro 6 library
 *
 * Copyright (C) Yasuhiro ISHII
 */

#include <Meiro.h>

void Meiro::backgroundMain(void)
{
    static bool sw1_1d = false;
    static bool sw2_1d = false;
    static uint32_t sw1_pressed_time = 0;
    static uint32_t sw2_pressed_time = 0;
    bool communicationMode = false;
    uint32_t timeModeEntered = 0;

    while (1)
    {
        bool sw1 = isSw1Push();
        bool sw2 = isSw2Push();
        bool sw1_short_pressed = false;
        bool sw2_short_pressed = false;
        bool sw1_long_pressed = false;
        bool sw2_long_pressed = false;

        if (sw1 && !sw1_1d)
        {
            sw1_pressed_time = millis();
        }

        if (sw2 && !sw2_1d)
        {
            sw2_pressed_time = millis();
        }

        if (!sw1 && sw1_1d)
        {
            if ((millis() - sw1_pressed_time) > BUTTON_LONG_PRESS_THRESH)
            {
                sw1_long_pressed = true;
            }
            else
            {
                sw1_short_pressed = true;
            }
        }

        if (!sw2 && sw2_1d)
        {
            if ((millis() - sw2_pressed_time) > BUTTON_LONG_PRESS_THRESH)
            {
                sw2_long_pressed = true;
            }
            else
            {
                sw2_short_pressed = true;
            }
        }

        if (sw1_long_pressed)
        {
            Serial.println("SW1 LONG");
        }
        if (sw2_long_pressed)
        {
            Serial.println("SW2 LONG");
        }
        if (sw1_short_pressed)
        {
            Serial.println("SW1 SHORT");
        }
        if (sw2_short_pressed)
        {
            Serial.println("SW2 SHORT");
        }

        {
            if (!communicationMode)
            {
                if (sw1_long_pressed)
                {
                    Serial.println("Entered to Wireless mode");
                    communicationMode = true;
                    timeModeEntered = millis();
                    privilegedMode = true;
                    for (int i = 0; i < MAX_FLOORS; i++)
                    {
                        setLedColorInternal(i, 0, 0, 255);
                    }
                    startBle();
                }
            }
            else
            {
                if ((sw1_long_pressed) || (!bleCallbacks->isAccessed() && ((millis() - timeModeEntered) > (60 * 1000))))
                {
                    Serial.println("Leave from Wireless mode");
                    communicationMode = false;
                    privilegedMode = false;
                    restoreLedColors();
                    stopBle();
                }
            }
        }

        sw1_1d = sw1;
        sw2_1d = sw2;

        vTaskDelay(10 / portTICK_RATE_MS);
    }
}

void Meiro::startBle(void)
{
    BLEDevice::init("Meiro 6");
    BLEServer *pServer = BLEDevice::createServer();
    pBleService = pServer->createService(SERVICE_UUID);
    BLECharacteristic *pCharacteristic =
        pBleService->createCharacteristic(CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

    pCharacteristic->setValue("Meiro 6");
    bleCallbacks = new BleCallbacks(this);
    pCharacteristic->setCallbacks(bleCallbacks);
    pBleService->start();
    // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
};

void Meiro::stopBle(void)
{
    BLEDevice::stopAdvertising();
    pBleService->stop();
    delete bleCallbacks;
    bleCallbacks = NULL;
};

BleCallbacks::BleCallbacks(Meiro *m)
{
    meiro = m;
}

void BleCallbacks::onWrite(BLECharacteristic *pCharacteristic)
{
    String value = pCharacteristic->getValue();
    Serial.printf("Value len = %d\n", value.length());

    if (value.length() > 0)
    {
        if (value.length() == 5)
        { // 評価内容が間違っているので、あとで直す
            switch (value[0])
            {
            case COMMAND_SET_COLOR:
                Serial.println("COMMAND_SET_COLOR");
                meiro->setLedColorInternal(value[1], value[2], value[3], value[4]);
                break;
            default:
                break;
            }
        }
    }
    accessed = true;
}