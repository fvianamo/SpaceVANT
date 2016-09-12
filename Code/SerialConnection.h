
#ifndef SERIALCONNECTION_H
#define SERIALCONNECTION_H

#include "libs/serialib.h"
#include <string>

class SerialConnection {
public:
    SerialConnection(char*, int, bool msg = false);
    virtual ~SerialConnection();
    void getProprities();
    void write(const char *string);
    void read(char *buffer, int maxNbBits, char endMsg ,unsigned int timeOutMS);
    int peek();
    
private:
    serialib conn;
    char* port;
    int baudRate;
    
};

#endif /* SERIALCONNECTION_H */

