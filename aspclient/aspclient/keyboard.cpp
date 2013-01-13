//
//  keyboard.cpp
//  aspclient
//
//  Created by Douglas Frazer on 1/3/13.
//  Copyright 2013 Douglas Frazer. All rights reserved.
//

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include "common_include.h"
#include "keyboard.h"
#include "network.h"

#define UP_ARROW    72
#define LEFT_ARROW  75
#define DOWN_ARROW  80
#define RIGHT_ARROW 77

static const uint32_t NUM_EVENT_HANDLERS = 26;

KEYBOARD_EVENT RegisteredEvents[NUM_EVENT_HANDLERS]; // for now, only the a-z are supported 

void Keyboard_Init()
{
    zero(&RegisteredEvents);
}

void Keyboard_Update(float DeltaTime)
{
}

void Keyboard_Deinit()
{
}

void Keyboard_RegisterEvent(unsigned char c, KEYBOARD_EVENT Event)
{
    assert(c >= 'a' && c <= 'z');
    uint32_t Index = (uint32_t)(c - 'a');
    if(RegisteredEvents[Index] == NULL) {
        RegisteredEvents[Index] = Event;
    } else {
        printf("Error function already registered to that key ('%c')\n", c);
    }
}

void Keyboard_KeyPressed(unsigned char c, int x, int y)
{
    printf("[KB] got keypress %c (%d %d)\n", c, x, y);
    uint32_t Index = (uint32_t)(c - 'a');
    if(Index >= 0 && Index < NUM_EVENT_HANDLERS && RegisteredEvents[Index] != NULL) {
        RegisteredEvents[Index]();
    }
    
}

void Keyboard_KeyUp(unsigned char c, int x, int y)
{
    printf("[KB] got key release %c (%d %d)\n", c, x, y);
}
