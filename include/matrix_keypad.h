#include <stdint.h>


#define TOTAL_ROWS 6
#define TOTAL_COLUMNS 6
#define TRUE 1
#define FALSE 0
#define MAX_KEYS 6*6



#define CH_A                    0x01
#define CH_B                    0x02
#define FREEZE                  0x03
#define VELOCITY                0x04
#define ZOOM                    0x05
#define CALSET                  0x06
#define AUDIO                   0x07
#define RUN                     0x08
#define SAVE                    0x09
#define UP                      0xA
#define DOWN                    0xB
#define LEFT                    0xC
#define DAC                     0xD
#define SD_MODE                 0xE
#define OK                      0xF
#define BACKSPACE               0x10
#define ESC                     0x11
#define DEC                     0x12
#define INC                     0x13
#define RIGHT                   0x14
#define SK_21                   0x15







void* KeypadThread(void*);
uint8_t Scan_Keys(uint8_t *Key_Row, uint8_t *Key_Col);
void Keypad_init(void);
void SelectCol(uint8_t column);
uint8_t GetRow(void);
void processKey(uint8_t row, uint8_t col);
void sendCharToWindows(char c);


