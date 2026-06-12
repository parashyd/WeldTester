#include <stdio.h>
#include <stdint.h>
#include "unistd.h"
#include <string.h>
#include "matrix_keypad.h"
#include "Ugpio.h"
#include <time.h> 
#include <sys/socket.h>         // for socket(), connect(), send()
#include <arpa/inet.h>          // for inet_pton(), htons()
#include <netinet/in.h>         // for sockaddr_in
#include <pthread.h>

typedef struct {
    int isAlphaNum;           // 0 for regular key, 1 for alphanumeric key (multi-character)
    int row;                  // Row index in the 7x7 matrix keypad
    int col;                  // Column index in the 7x7 matrix keypad
    int keyCount;             // Number of characters for this key (1 for regular keys, >1 for multi-function keys)
    char keyValues[MAX_KEYS]; // Array of key values (ASCII or special codes)
    const char *keyNames[MAX_KEYS]; // Human-readable names
} KeyMapping;


uint8_t Rows[8] = {136,135,137,0,1,5};              //6
uint8_t Cols[8] = {7,138,139,78,115,80};         //73

KeyMapping keyMappings[] = {
    {0, 0, 0, 0, {0}, {NULL}},         // free
    {0, 0, 1, 1, {CH_A}, {"1"}},       // CH_A
    {0, 0, 2, 1, {CH_B}, {"2"}},       // CH_B
    {0, 0, 3, 1, {FREEZE}, {"3"}},       // FREEZE
    {0, 0, 4, 1, {VELOCITY}, {"4"}},       // VEL
    {0, 0, 5, 1, {ZOOM}, {"5"}},       // ZOOM

    {0, 1, 0, 1, {CALSET}, {"6"}},       // CAL/GATE
    {0, 1, 1, 1, {0},   {NULL}},       // FREE
    {0, 1, 2, 1, {0}, {NULL}},         // FREE
    {0, 1, 3, 1, {AUDIO}, {"7"}},       // AUD ON/OFF
    {0, 1, 4, 1, {RUN}, {"8"}},       // RUN
    {0, 1, 5, 1, {SAVE}, {"9"}},     // SAVE : A

    {1, 2, 0, 4, {'1', 'A', 'B', 'C'}, {"1", "A", "B", "C", NULL}},
    {1, 2, 1, 4, {'2', 'D', 'E', 'F'}, {"2", "D", "E", "F", NULL}},
    {1, 2, 2, 4, {'3', 'G', 'H', 'I'}, {"3", "G", "H", "I", NULL}},
    {1, 2, 3, 4, {'4', 'J', 'K', 'L'}, {"4", "J", "K", "L", NULL}},
    {1, 2, 4, 4, {'5', 'M', 'N', 'O'}, {"5", "M", "N", "O", NULL}},
    {1, 2, 5, 5, {'6', 'P', 'Q', 'R', 'S'}, {"6", "P", "Q", "R", "S"}},
//    {1, 2, 6, 1, {0}, {NULL}},         // FREE

    {1, 3, 0, 4, {'7', 'T', 'U', 'V'}, {"7", "T", "U", "V", NULL}},
    {1, 3, 1, 5, {'8', 'W', 'X', 'Y', 'Z'}, {"8", "W", "X", "Y", "Z"}},
    {1, 3, 2, 1, {'9'}, {"9"}},
    {1, 3, 3, 1, {'0'}, {"0"}},
    {1, 3, 4, 2, {'.', '*'}, {".", "*"}},
    //{1, 3, 5, 2, {'\r', '\x14'}, {"Enter", "-"}}, // Enter, Ctrl-T
    {1, 3, 5, 3, {'-', '_', ' '}, {"-", "_", "space"}}, // Enter, Ctrl-T

    {0, 4, 0, 1, {UP}, {"10"}},    // UP arrow : B
    {0, 4, 1, 1, {DOWN}, {"11"}},    // Down arrow : C
    {0, 4, 2, 1, {LEFT}, {"12"}},    // Left arrow : D
    {0, 4, 3, 1, {DAC}, {"13"}},    // DAC
    {0, 4, 4, 1, {SD_MODE}, {"14"}},    // S/D
    {0, 4, 5, 1, {OK}, {"15"}},    // OK

    {0, 5, 0, 1, {0}, {NULL}},        // FREE
    {0, 5, 1, 1, {BACKSPACE}, {"16"}},    // BACK : F
    {0, 5, 2, 1, {ESC}, {"17"}},    // ESC : 10
    {0, 5, 3, 1, {DEC}, {"18"}},    // - : 11
    {0, 5, 4, 1, {INC}, {"19"}},    // + : 12
    {0, 5, 5, 1, {RIGHT}, {"20"}},    // Right arrow : 13


};

#define NUM_KEYS (sizeof(keyMappings) / sizeof(KeyMapping))
static int lastRow = -1;
static int lastCol = -1;
static int pressCount = 0;
static time_t lastPressTime = 0;


void* KeypadThread(void*)
{
    // cpu_set_t cpuset;
    // CPU_ZERO(&cpuset);
    // CPU_SET(1, &cpuset);
    // pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);


    uint8_t row = 0, col = 0;
    // Keypad_init();
    printf("Start typing on keypad...\n");

    while (1) {
        if (Scan_Keys(&row, &col)) 
        {
            row -= 1;
            col -= 1;
            processKey(row, col);
            usleep(300000); // Debounce
        }
    }

    return NULL;
}

void processKey(uint8_t row, uint8_t col) 
{
    time_t now = time(NULL);
    char output = '\0';

    if (row == lastRow && col == lastCol && (now - lastPressTime) < 3) {
        pressCount++;
    } else {
        pressCount = 0;
    }

    lastPressTime = now;
    lastRow = row;
    lastCol = col;

    for (uint8_t i = 0; i < NUM_KEYS; i++) {
    if (keyMappings[i].row == row && keyMappings[i].col == col) {
        if (keyMappings[i].keyCount == 0) {
            // Free key, do nothing
            return;
        }

        int index = pressCount % keyMappings[i].keyCount;
        output = keyMappings[i].keyValues[index];
       // const char *label = keyMappings[i].keyNames[index];
        sendCharToWindows(output);
        //printf("Pressed: %s (code: 0x%02X)\n", label ? label : "?", (unsigned char)output);
        
        break;
    }
}


    if (output) {
        printf("Pressed: %c\n", output);
    } else {
        printf("Unmapped key at (%d,%d)\n", row, col);
    }

}

uint8_t Scan_Keys(uint8_t *Key_Row, uint8_t *Key_Col)
{
  uint8_t row;
  for (uint8_t col = 1; col <= TOTAL_COLUMNS; col++) 
  {
        SelectCol(col);  // Activate one column
        usleep(1000);    // Short delay for signal to settle

         row = GetRow();
        if (row != FALSE) 
        {
            *Key_Row = row;
            *Key_Col = col;
            printf("key Detected\n");
            return TRUE;  // Key detected
        }
    }
   // printf("Key not detected\n");
    return FALSE; // No key press detected
	      
}

void Keypad_init(void)
{

  //Row pins as input, column pins as output.
  
    for (int i = 0; i < TOTAL_ROWS; i++)
    {
        gpio_export(Rows[i]);
        gpio_set_dir(Rows[i], "in");
    }
    
    for (int i = 0; i < TOTAL_COLUMNS; i++)
    {
        gpio_export(Cols[i]);
        gpio_set_dir(Cols[i], "out");
    }

}


void SelectCol(uint8_t column) //column 
{
    uint8_t tmp = 0;
    for (tmp = 1; tmp < TOTAL_COLUMNS+1; tmp++) 
    {
        if (tmp == column) 
            gpio_Write(Cols[tmp - 1], 1);          // setting the  specific column to high
        else
            gpio_Write(Cols[tmp - 1], 0);
    }
}


uint8_t GetRow(void)
{
    uint8_t tmp = 0;
	
    for (tmp = 0; tmp < TOTAL_ROWS; tmp++)
    {
	
        if (gpio_Read(Rows[tmp]) == TRUE)    // gets out the row which is pressed by checking whether it is high or not.
        {
            return (tmp + 1);
        }
    }
    return FALSE;
}


void sendCharToWindows(char c) 
{
    int sockfd;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8888);  // Same port as Qt app
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);


    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        return;
    }

    send(sockfd, &c, 1, 0);  // Send 1 character
    close(sockfd);
}




