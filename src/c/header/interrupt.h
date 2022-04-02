#ifndef __INTERRUPT_H
#define __INTERRUPT_H

// REGISTERS
int AX, BX, CX, DX;
// REGISTERS HELPER UNIT
#define REG_H(r) (r >> 8)
#define REG_L(r) (r & 0xFF)
#define REG(h, l) ((h << 8) + l)
// INTERRUPTS ROUTINE
#define intr(n, a, b, c, d) AX = a; BX = b; CX = c; DX = d; interrupt(n, &AX, &BX, &CX, &DX);

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
// 0x0E00 + x = 0x0E0x -> AH = 0x0E/0xE, AL = 0x0x
#define AX_TELETYPE_WRITE(x) (0x0E00 | x)
// Write character at cursor pos with attr (ah=0x0A)
// The difference between teletype is that they dont move the cursor.
// It's basically just write character and that's it
// http://www.ctyme.com/intr/rb-0100.htm
#define AX_WRITE_CHAR(x) (0x0900 | x)

// Scroll Up (ah=0x06)
// http://www.ctyme.com/intr/rb-0096.htm
// 0x00 al clear the entire window
#define AX_VIDEO_SCROLLUP(x) (0x0600 | x)
// Character color write (bx)
// bh = BIOS color attribute
#define BX_VIDEO_COLOR(col) (col << 8)
// Default color is grey fg (0x0f) on black bg (0x00)
#define BX_VIDEO_COLOR_DEFAULT 0x0700

// Set cursor (ah=0x02)
// http://www.ctyme.com/intr/rb-0087.htm
#define AX_VIDEO_SETCURSOR 0x0200

// Get cursor (ah=0x03)
// http://www.ctyme.com/intr/rb-0088.htm
#define AX_VIDEO_GETCURSOR 0x0300

// =================
// B. Keyboard services
// =================
#define INT_KEYBOARD 0x16
// Get keystroke (ah=0x00)
// returns ah = BIOS scan code, al = ascii char
#define AX_KEYBOARD_READ 0x0000

// =================
// C. Read/Write services
// =================
#define INT_RW 0x13
// Read first sector
#define AX_READ 0x0201
// Write first sector
#define AX_WRITE 0x0301

#endif