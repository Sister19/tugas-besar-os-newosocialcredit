#define true 1
#define false 0
#define length 128
#define endl printString("\r\n");

void printString(char* buffer);
void readString(char* buffer);
void clearScreen();
void printChar(char* c);
int len(char* buffer);
void printTitle();


int main(){
  interrupt(0x10, 0x0003, 0, 0, 0); //set video mode to 03
  printTitle();
  while (true){
    char buffer[length + 3];
    makeInterrupt21();
    endl;
    printString("Input string (~: clear): ");
    readString(buffer);
    //clear screen if tilde (idk)
    if (buffer[0] == '~'){
      clearScreen();
    }
    else{
      printString("\r\nYou inputted: ");
      printString(buffer);
      printString("\r\n");
    }
    // clearScreen();
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

void printStringColored(char *c, int baris, int kolom, int warna){
  /* Menulis string pada baris dan kolom dengan warna sesuai parameter fungsi */
  int i, b, alamat;
  i = 0;

  alamat = (baris-1)*80*2 + 2*kolom;
  while(c[i] != '\0'){
    b = alamat;
    putInMemory(0xB000, b+0x8000, c[i]);
    putInMemory(0xB000, b+0x8001, warna);
    i = i + 1;
    alamat = alamat + 2;
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

int len(char* buffer){
  int count = 0;
  while (buffer[count] != '\0' && buffer[count + 1] != '\r'){
    count++; //count string as long as its not null
  }
  return count;
}

void clearScreen(){
  /*interrupt for scrolling down (clearing screen you can say)
    16-bits register can be divided into two parts:
      ...X = |  ...H |  ...L |
    
    mode: 0x10 (write)
    AX, function code register (0x0600)
      AH = 0x06 (scroll up)
      AL = 0x00 (scroll up 0 lines, dependent to DX)
    BX, color register (0x0F00),
      BH: 0x0F (white color / foreground)
      BL: 0x00 (black color / background)
    CX, number of beginning row and column
      CH: upper row number, 0
      CL: left column number, 0
    DX, number of ending row and column
      DH: lower row number, 24 (0x18)
      DL: right column number, 79 (0x4F)
  */
  interrupt(0x10, 0x0600, 0x0F00, 0x0000, 0x184F);

  /* interrupt to reposition cursor, 0x02 = reposition cursor, 
  DX = 0 because the position we wanted is 0, 0 */

  interrupt(0x10, 0x0200, 0, 0, 0);

  /* message */
  printString("Screen cleared!");
}



void printTitle(){
  
  printStringColored(" ________   _______   ___       __   ________  ________      ",1,0,0x0A); endl;
  printStringColored("|\\   ___  \\|\\  ___ \\ |\\  \\     |\\  \\|\\   __  \\|\\   ____\\     ",2,0,0x0A); endl;
  printStringColored("\\ \\  \\ \\  \\ \\   __/|\\ \\  \\    \\ \\  \\ \\  \\|\\  \\ \\  \\___|_    ",3,0,0x0A); endl;
  printStringColored(" \\ \\  \\ \\  \\ \\  \\_|/_\\ \\  \\  __\\ \\  \\ \\  \\\\  \\ \\_____  \\   ",4,0,0x0A); endl;
  printStringColored("  \\ \\  \\ \\  \\ \\  \\_|\\ \\ \\  \\|\\__\\_\\  \\ \\  \\\\  \\|____|\\  \\  ",5,0,0x0A); endl;
  printStringColored("   \\ \\__\\ \\__\\ \\_______\\ \\____________\\ \\_______\\____\\_\\  \\ ",6,0,0x0A); endl;
  printStringColored("    \\|__| \\|__|\\|_______|\\|____________|\\|_______|\\_________\\",7,0,0x0A); endl;
  printStringColored("                                                 \\|_________|",8,0,0x0A); endl;
  endl;
  printStringColored("(tentative name because it's still kinda horrendous right now)",10,0,0x0D); endl;
  endl; 
  printStringColored("What you can CURRENTLY do right now, though: ",12,0,0x09); endl;
  printStringColored("1. Enter a string and the OS will output it again (yes)",13,0,0x0B); endl;
  printStringColored("2. Enter '~' to clear the terminal, sounds quite simple (because it is) ",14,0,0x0B); endl; endl;
}