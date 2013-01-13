//
//  keyboard.h
//  aspclient
//
//  Created by Douglas Frazer on 1/3/13.
//  Copyright 2013 Douglas Frazer. All rights reserved.
//

#ifndef aspclient_keyboard_h
#define aspclient_keyboard_h

typedef void (*KEYBOARD_EVENT)(void);

void Keyboard_Init();
void Keyboard_Update(float DeltaTime);
void Keyboard_Deinit();

void Keyboard_KeyPressed(unsigned char c, int x, int y);
void Keyboard_KeyUp(unsigned char c, int x, int y);

void Keyboard_RegisterEvent(unsigned char c, KEYBOARD_EVENT Event);

#endif
