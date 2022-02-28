#ifndef __KERNEL_H
#define __KERNEL_H

#include "std_lib.h"
#include "std_type.h"

// MEMORY SEGMENT & ADDRESSING
// Video segment
#define VID_SEGMENT 0xB000

// INTERRUPT LIST
// =================
// A. Video services
// =================
#define INT_VIDEO 0x10
// Video Mode (ah=0x00)
// http://vitaly_filatov.tripod.com/ng/asm/asm_023.1.html
// al 0x03 = Text, 80x25, all but MDA, 16 bit fg + 8 bit bg colors
#define AX_VIDEO_MODE 0x0003

// Teletype write (ah=0x0E)
// http://www.ctyme.com/intr/rb-0106.htm
// al = characters to write
#define AX_TELETYPE_WRITE(x) ((0x0E << 8) + x)

// Scroll Up (ah=0x06)
// http://www.ctyme.com/intr/rb-0096.htm
// 0x00 al clear the entire window
#define AX_VIDEO_SCROLLUP 0x0600
// Video color
// bh = foreground color, bl = background color
#define BX_VIDEO_COLOR(fg, bg) ((fg << 8) + bg)
// Default color is grey fg (0x0f) on black bg (0x00)
#define BX_VIDEO_COLOR_DEFAULT 0x0F00

// Set cursor (ah=0x02)
// http://www.ctyme.com/intr/rb-0087.htm
#define AX_VIDEO_SETCURSOR 0x0200

// Default Constants
// Page number
#define PAGE_NUMBER 0
// Start cursor
#define START_CURSOR 0x0
// End cursor (DH h x DL w, for video mode 3h it will be 0x18 x 0x4f)
#define END_CURSOR 0x184F

// =================
// B. Keyboard services
// =================
#define INT_KEYBOARD 0x16
// Get keystroke (ah=0x00)
// returns ah = BIOS scan code, al = ascii char
#define AX_KEYBOARD_READ 0x0000

// Fungsi bawaan
extern void putInMemory(int segment, int address, byte b);
extern int interrupt(int int_number, int AX, int BX, int CX, int DX);
extern void makeInterrupt21();

void handleInterrupt21(int AX, int BX, int CX, int DX);

void printString(char *string);
void readString(char *string);
void clearScreen();
void printTitle();
void printChar();

#endif