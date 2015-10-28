#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "../headers/ftd2xx.h"

#define MAX_BUF 256

FT_HANDLE ftHandle;
FT_STATUS ftStatus;
ULONG baudRate = 250000;

DWORD bytesWritten;
char txBuffer[256] = {0xE7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40};

int ledParSetup();


int main(int argc, char* argv[])
{
    ledParSetup();

    int fd;
    char * myfifo = "/tmp/myfifo";
    char buf[MAX_BUF];
    while (1)
    {
        /* open, read, and display the message from the FIFO */
        fd = open(myfifo, O_RDONLY);
        read(fd, buf, MAX_BUF);
        printf("Received input.\n");
        ftStatus = FT_Write(ftHandle, buf, sizeof(buf), &bytesWritten);
        close(fd);
    }


    return 0;



    // Write data to the device
    ftStatus = FT_Write(ftHandle, txBuffer, sizeof(txBuffer), &bytesWritten);
    if (ftStatus != FT_OK)
    {
        printf("Error while writing to device. Exiting...\n");
        return 1;
    }

    FT_Close(ftHandle);
    return 0;
}

int ledParSetup()
{
    ftStatus = FT_Open(0, &ftHandle);
    if (ftStatus == FT_OK)
        printf("All OK!\n");
    else
    {
        printf("Error! Did you forget to remove ftdi_sio and usbserial?\n");
        return 1;
    }

    ftStatus = FT_ResetDevice(ftHandle);

    // Set BaudRate of the device
    ftStatus = FT_SetBaudRate(ftHandle, baudRate);
    if (ftStatus == FT_OK)
        printf("BaudRate successfully set to %d.\n", baudRate);
    else
    {
        printf("BaudRate could not be set. Exiting...\n");
        return 1;
    }

    ftStatus = FT_SetDataCharacteristics(ftHandle, FT_BITS_8, FT_STOP_BITS_2, FT_PARITY_NONE);
    if (ftStatus == FT_OK)
        printf("Bits per word: 8. Number of stop bits: 2. No parity.\n");
    else
    {
        printf("Couldn't set data characteristics. Exiting...\n");
        return 1;
    }

    ftStatus = FT_SetFlowControl(ftHandle, 0, 0, 0);
}