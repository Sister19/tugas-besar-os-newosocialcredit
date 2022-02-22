// Kode kernel
// PENTING : FUNGSI PERTAMA YANG DIDEFINISIKAN ADALAH main(),
//   cek spesifikasi untuk informasi lebih lanjut

// TODO : Tambahkan implementasi kode C

#define true 1
#define false 0
#define length 128
#define endl printString("\r\n");
#include "header/kernel.h"
#include "header/std_type.h"


int main(){
  interrupt(0x10, 0x0003, 0, 0, 0); //set video mode to 03
  while (true){
    char buffer[length + 3];
    makeInterrupt21();
    endl;
    printString("Input string (~: clear): ");
    readString(buffer);
    printString("\r\nYou inputted: ");
    printString(buffer);
    printString("\r\n");
    }  
}


void handleInterrupt21(int AX, int BX, int CX, int DX) {
  switch (AX){
    case 0x0:
      printString(BX);
      break;
    case 0x1:
      readString(BX);
      break;
    default:
      printString("Invalid interrupt");
  }
}

void printString(char* buffer){
  int i = 0;
  // print char by char
  for(i = 0; buffer[i] != '\0'; i++){ // output selama ga \0
    interrupt(0x10, 0xE * 256 + buffer[i], 0, 0, 0);
  }
}

void readString(char* buffer){
  int index = 0;    
  while (true){
    int input = interrupt(0x16, 0, 0, 0, 0);  //0x16 used to scan keyboard input
    if (input == 0xD){              //if input is enter (0xD), fill the end of the buffer with 
      buffer[index] = '\0';         //null char, carriage return, and enter
      buffer[index] = 0x0;
      buffer[index + 1] = 0xD;
      buffer[index + 2] = 0xA;
      return;
    } else if (input == 0x8){       //delete last char (0xE is teletype)
      buffer[index] = 0x0;
      index--;
      interrupt(0x10, 0xE * 256 + 0x8, 0, 0, 0); // display character
      index++;
      interrupt(0x10, 0xE * 256 + 0x0, 0, 0, 0);
      index--;
      interrupt(0x10, 0xE * 256 + 0x8, 0, 0, 0);
    } else {
      buffer[index] = input;        //if input isn't the enter/backspace code, fill the buffer with the input
      interrupt(0x10, 0xE * 256 + input, 0, 0, 0); // display character
      index++;
    }
  }
}
