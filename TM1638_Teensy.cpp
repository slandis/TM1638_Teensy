/*
 * TM1638-Teensy/TM1638-Teensy.cpp
 * Copyright (C) 2019
 * Shaun Landis <slandis.gmail.com>
 *
 */

#include "TM1638_Teensy.h"

TM1638_Teensy::TM1638_Teensy(uint8_t data, uint8_t clock, uint8_t strobe, boolean active, uint8_t brightness) {
  this->data = data;
  this->clock = clock;
  this->strobe = strobe;

  this->brightness = brightness;
  this->active = active;

  pinMode(data, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(strobe, OUTPUT);

  digitalWrite(strobe, HIGH);
  digitalWrite(clock, HIGH);

  delayMicroseconds(CS_RISE_US);

  sendCommand(DISP_MODE | (active ? 8 : 0) | min(7, brightness));
  clearAll();
}

void TM1638_Teensy::setBrightness(uint8_t brightness) {
  this->brightness = min(7, brightness);

  sendCommand(DISP_MODE | (this->active ? 8 : 0) | this->brightness);
}

void TM1638_Teensy::setDisplay(boolean active) {
  this->active = active;

  sendCommand(DISP_MODE | (active ? 8 : 0) | this->brightness);
}

void TM1638_Teensy::clearDisplay() {
  sendCommand(DISP_MODE | (this->active ? 8 : 0) | this->brightness);
  sendCommand(WRITE_FIXED);

  digitalWrite(this->strobe, LOW);

  for (int i = 0; i < 8; i++) {
    sendByte((0xC0 | (i << 1)));
    sendByte(0x00);
  }

  digitalWrite(this->strobe, HIGH);
  delayMicroseconds(CS_RISE_US);

  sendCommand(DISP_MODE | (this->active ? 8 : 0) | this->brightness);
}

void TM1638_Teensy::clearLEDs() {
  sendCommand(DISP_MODE | (this->active ? 8 : 0) | this->brightness);
  sendCommand(WRITE_FIXED);

  digitalWrite(this->strobe, LOW);

  for (int i = 0; i < 8; i++) {
    sendByte(0xC0 | (1 + (i << 1)));
    sendByte(0x00);
  }

  digitalWrite(this->strobe, HIGH);
  delayMicroseconds(CS_RISE_US);

  sendCommand(DISP_MODE | (this->active ? 8 : 0) | this->brightness);
}

void TM1638_Teensy::clearAll() {
  sendCommand(DISP_MODE | (this->active ? 8 : 0) | this->brightness);
  sendCommand(WRITE_AUTO);

  digitalWrite(this->strobe, LOW);
  sendByte(0xC0);

  for (int i = 0; i < 16; i++) {
    sendByte(0x00);
  }

  digitalWrite(this->strobe, HIGH);
  delayMicroseconds(CS_RISE_US);

  sendCommand(DISP_MODE | (this->active ? 8 : 0) | this->brightness);
}

void TM1638_Teensy::sendByte(uint8_t value) {
  for (int i = 0; i <= 8; i++) {
    digitalWrite(this->clock, LOW);
    digitalWrite(this->data, (value & 0x01) ? HIGH : LOW);
    value >>= 1;
    digitalWrite(this->clock, HIGH);
  }
}

uint8_t TM1638_Teensy::receiveByte() {
  uint8_t r = 0;

  pinMode(this->data, INPUT);
  digitalWrite(this->data, HIGH);

  for (int i = 0; i < 8; i++) {
    r >>= 1;

    digitalWrite(this->clock, LOW);
    delayMicroseconds(READ_DELAY_US);

    if (digitalRead(this->data)) {
      r |= 0x80;
    }

    digitalWrite(this->clock, HIGH);
  }

  pinMode(this->data, OUTPUT);
  digitalWrite(this->data, LOW);

  return r;
}

void TM1638_Teensy::sendCommand(uint8_t value) {
  digitalWrite(this->strobe, LOW);
  sendByte(value);
  digitalWrite(this->strobe, HIGH);
  delayMicroseconds(CS_RISE_US);
}

void TM1638_Teensy::setDisplayAt(uint8_t position, uint8_t value) {
  sendCommand(DISP_MODE | (this->active ? 8 : 0) | this->brightness);
  sendCommand(WRITE_FIXED);

  digitalWrite(this->strobe, LOW);
  sendByte((0xC0 | (position << 1)));
  sendByte(value);
  digitalWrite(this->strobe, HIGH);
  delayMicroseconds(CS_RISE_US);

  sendCommand(DISP_MODE | (this->active ? 8 : 0) | this->brightness);
}

void TM1638_Teensy::setDisplay(uint8_t message[]) {
  for (int i = 0; i < 8; i++) {
    setDisplayAt(i, message[i]);
  }
}

void TM1638_Teensy::setLEDAt(uint8_t position, boolean active) {
  sendCommand(DISP_MODE | (this->active ? 8 : 0) | this->brightness);
  sendCommand(WRITE_FIXED);

  digitalWrite(this->strobe, LOW);
  sendByte(0xC0 | (1 + (position << 1)));
  sendByte((active ? 0x01 : 0x00));
  digitalWrite(this->strobe, HIGH);
  delayMicroseconds(CS_RISE_US);

  sendCommand(DISP_MODE | (this->active ? 8 : 0) | this->brightness);
}

void TM1638_Teensy::setLEDs(uint8_t mask) {
  for (int i = 7; i >= 0; i--) {
    setLEDAt(i, (mask & 0x1) ? true : false);
    mask >>= 1;
  }
}

uint8_t TM1638_Teensy::readKeys() {
  uint8_t r = 0;

  digitalWrite(this->strobe, LOW);
  sendByte(0x42);

  for (int i = 0; i < 4; i++) {
    r |= (receiveByte() << i);
  }
  digitalWrite(this->strobe, HIGH);
  delayMicroseconds(CS_RISE_US);

  return r;
}
