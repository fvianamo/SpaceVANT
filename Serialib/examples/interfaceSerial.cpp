#include <stdio.h>
#include "serialib.h"

//Como compilar: g++ interfaceSerial.cpp serialib.cpp -o interfaceSerial
//Como executar: ./interfaceSerial

//Definicao de Portas serial utilizadas
#if defined (_WIN32) || defined( _WIN64)
#define         DEVICE_PORT             "COM1"                               // COM1 for windows
#endif

#ifdef __linux__
#define         DEVICE_PORT2             "/dev/ttyO2"                         // ttyS0 for linux
#define         DEVICE_PORT4             "/dev/ttyO4"                         // ttyS0 for linux
#endif

#define QTDBITSENVIADOS 	64	//Qtd de Bits enviados por vez
#define BAUDRATE 		115200  //Taxa de transferencia da comunicacao serial
#define TIMEOUT 		5000    //Tempo maximo (em ms) de espera de recebimento de dados


//Converte decimal em binario e imprime na tela
void printIntInBits(int n)
{
	int i = 0;
	for(i = 0; i < 8; i++)
	{ 
    		if (n & 1)
        		printf("1");
    		else
        		printf("0");
    		n >>= 1;
	}
}

//Imprime dados enviados na tela
void printBits(const char* message, char* Buf)
{
	int i;
	printf(message);
	printf("Char[i]: Decimal \t HexaDecimal \t BinarioReverso\n");
        for(i = 0; i < QTDBITSENVIADOS; i++)
	{   
        	printf("Char[%d]: %d \t\t %x \t\t ", i, (int) Buf[i], (int) Buf[i]);
		printIntInBits((int) Buf[i]);
		printf("\n");
	}
	printf("\n");
}

//Imprime diferencao entre bits enviados e recebidos (utilizado em teste)
void printDifBits(char* BufRec, char* BufSend)
{
        int i;
        printf("\nA diferenca dos bits enviados e recebidos sao: \n");
        for(i = 0; i < QTDBITSENVIADOS; i++)
        {
             	printf("Char[%d]: %d\n", i, ((int) BufRec[i] - (int) BufSend[i]));
        }
        printf("\n");
}

//Criar Funcoes enviar e receber
//Como esperar envio e qnd enviar (CTS e RTS)

int main()
{
    serialib LS2,LS4;      // Object of the serialib class
    int Ret;               // Used for return values
    char Buffer2[65] = "String inicial do Buffer utilizado na porta ttyO2 com 64 bits!!";
    char Buffer4[65] = {16,43,108,17,0,0,0,0, 
			98,169,74,14,0,0,0,0, 
			218,93,102,6,0,0,0,0, 
			0b01000110, 0b00001011, 0b11001110, 0b00101010, 0,0,0,0, 
                        0b00110000, 0b01100111, 0b00101000, 0b00100001, 0b00110110, 0b01101001, 0b01100000, 0b0000000,
			0b01110000, 0b11001101, 0b01001010, 0b00100010, 0b00001101, 0b00101100, 0b11000000, 0b0000000,
                        0b00101110, 0b11001001, 0b01001010, 0b00001110, 0,0,0,0,
                        0b00000110, 0b01110001, 0b11001100, 0b00101100, 0,0,0,0,
			0};
    printf("Buffer Inicial: %s\n", Buffer2); //Imprime estado inicial do buffer
	
    //Lista de comandos testados
    //Comando1:  00010000 00101011 01101100 00010001 00000000 00000000 00000000 0000000
    //Comando02: 01100010 10101001 01001010 00001110 00000000 00000000 00000000 0000000
    //Comando03: 11011010 01011101 01100110 00000110 00000000 00000000 00000000 0000000
    //Comando04: 01000110 00001011 11001110 00101010 00000000 00000000 00000000 0000000
    //Comando05: 00110000 01100111 00101000 00100001 00110110 01101001 01100000 0000000
    //Comando06: 01110000 11001101 01001010 00100010 00001101 00101100 11000000 0000000
    //Comando07: 00101110 11001001 01001010 00001110 00000000 00000000 00000000 0000000
    //Comando08: 00000110 01110001 11001100 00101100 00000000 00000000 00000000 0000000

    
    // Open serial port

    Ret=LS4.Open(DEVICE_PORT4,BAUDRATE);                                        // Open serial link at given bauds
    if (Ret!=1) {                                                           // If an error occured...
        printf ("Error while opening port. Permission problem ?\n");        // ... display a message ...
        return Ret;                                                         // ... quit the application
    }
    printf ("Serial port 4 opened successfully !\n");

	
    Ret=LS2.Open(DEVICE_PORT2,BAUDRATE);                                        // Open serial link at given bauds
    if (Ret!=1) {                                                           // If an error occured...
        printf ("Error while opening port. Permission problem ?\n");        // ... display a message ...
        return Ret;                                                         // ... quit the application
    }
    printf ("Serial port 2 opened successfully !\n");
	

    //Montar Comando


    // Write the command on the serial port

    Ret=LS4.WriteStringWithLength(Buffer4,QTDBITSENVIADOS);      // Send the command on the serial port
	if (Ret!=1) {                                                           // If the writting operation failed ...
        printf ("Error while writing data\n");                              // ... display a message ...
        return Ret;                                                         // ... quit the application.
    }
    printf ("Write operation for port 4 is successful : %s\n", Buffer4);
    printBits("\nOs bits enviados sao: \n", Buffer4); 			    //Imprime bits enviados

    
    // Read a string from the serial device
    Ret=LS2.ReadString(Buffer2,QTDBITSENVIADOS,TIMEOUT);                    // Read QTD/8 characters with a timeout of given miliseconds
                                                                            
    if (Ret>0)                                                              // If a string has been read from, print the string
        printf ("String read from serial port : %s\n",Buffer2);
    else
        printf ("TimeOut reached. No data received !\n");                   // If not, print a message.

    printBits("\nOs bits recebidos sao: \n", Buffer2);			    // Imprime bits recebidos
    printDifBits(Buffer2, Buffer4);					    // Imprime diferenca entre bits recebidos e enviados
    

    //Atera string recebida

    Buffer2[2] = 0;
    Buffer2[4] += 5;
    int i;
    for(i = 0; i < QTDBITSENVIADOS; i++)
    {
       Buffer4[i] = 255;
    }    


    // Retorna a string

    Ret=LS2.WriteStringWithLength(Buffer2,QTDBITSENVIADOS);      // Send the command on the serial port
        if (Ret!=1) {                                                           // If the writting operation failed ...
        printf ("Error while writing data\n");                              // ... display a message ...
        return Ret;                                                         // ... quit the application.
    }
    printf ("Write operation for port 2 is successful : %s\n", Buffer2);
    printBits("\nOs bits enviados sao: \n", Buffer2);


    // Read a string from the serial device
    Ret=LS4.ReadString(Buffer4,QTDBITSENVIADOS,TIMEOUT);                     // Read QTD/8 characters with a timeout of given miliseconds
                                                                            
    if (Ret>0)                                                              // If a string has been read from, print the string
        printf ("String read from serial port : %s\n",Buffer4);
    else
        printf ("TimeOut reached. No data received !\n");                   // If not, print a message.

    printBits("\nOs bits recebidos sao: \n", Buffer4);			    // Imprime bits recebidos
    printDifBits(Buffer4, Buffer2);					    // Imprime diferenca entre bits recebidos e enviados
    
    // Close the connection with the device

    LS4.Close();
    LS2.Close();

    return 0;
}
