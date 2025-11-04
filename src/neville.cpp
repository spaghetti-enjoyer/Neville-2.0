#include "neville.hpp"
#include <iostream>

Neville::Neville(const std::string& port) : serial(port, 9600, serial::Timeout::simpleTimeout(1000)) 
{
    if (!serial.isOpen())
    {
        std::cerr << "Cannot initialize port\n";
    }
}

int Neville::activate_motor(uint8_t motor, uint8_t dir, uint8_t speed)
{
    if (speed > 0x7f) // has to be uint8 btw
    {
        std::cerr << "Invalid speed parameter. Expected a value between 0 and 127, got: " << static_cast<int>(speed) << std::endl;
        return 0;
    }
    uint8_t command = motor + dir;
    std::vector<uint8_t> bytes = { command, speed };
    return serial.write(bytes);
}

int Neville::buzz()
{
    std::vector<uint8_t> bytes = {
        0xB3, 0x0D, 0x21, 'O', '2', 'C', '8', 'O', '3', 'C', '8', 'O', '4', 'C', '8'
    };
    return serial.write(bytes);
}
