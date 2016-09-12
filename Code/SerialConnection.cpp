
#include "SerialConnection.h"

SerialConnection::SerialConnection(char* port, int baudRate, bool msg) {
    int ret;
    ret = conn.Open(port, baudRate);
    
    if(ret!=1 && msg)
        std::cout << "connection failed!" << std::endl;
    else if(msg)
        std::cout << "connection opened successfully" << std::endl;
    
    this->port = port;
    this->baudRate = baudRate;
}

void SerialConnection::getProprities(){
    std::cout << "port: " << this->port << " baudRate: " << this->baudRate << std::endl;
}

void SerialConnection::write(const char* string){
    int ret;
    
    ret = this->conn.WriteString(string);
    
    if(ret == -1)
        std::cout << "Error while writing data!\n";
    
}

void SerialConnection::read(char *buffer, int maxNbBits, char endMsg, unsigned int timeOutMS){
    
    int ret = this->conn.ReadString(buffer, endMsg, maxNbBits, timeOutMS);
    
    switch (ret){
        case 0:
            std::cout << "Time out was reached\n";
            break;
        case -1:
            std::cout << "error while setting the Timeout\n";
            break;
        case -2:
            std::cout << "error while reading the byte\n";
            break;
        case -3:
            std::cout << "MaxNbBytes is reached\n";
            break;
    }
}

int SerialConnection::peek(){
    return this->conn.Peek();
}

SerialConnection::~SerialConnection() {
    conn.Close();
}

