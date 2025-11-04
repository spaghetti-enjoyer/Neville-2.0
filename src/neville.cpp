#include "neville.hpp"
#include <iostream>

Neville::Neville(const std::string& port)
{
    try
    {
        serial.setPort(port);
        serial.setBaudrate(9600);

        serial.open();
        if (!serial.isOpen())
        {
            std::cerr << "Warning: Could not open serial port " << port << ". Continuing without serial.\n";
        }

    } catch (const std::exception& e)
    {
        std::cerr << "Serial error: " << e.what() << ". Continuing without serial.\n";
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
    return write_bytes(bytes);
}

int Neville::buzz()
{
    std::vector<uint8_t> bytes = {
        0xB3, 0x0D, 0x21, 'O', '2', 'C', '8', 'O', '3', 'C', '8', 'O', '4', 'C', '8'
    };
    return write_bytes(bytes);
}

int Neville::write_bytes(const std::vector<uint8_t> &bytes) 
{
    if (!serial.isOpen()) return 0;
    size_t written = serial.write(bytes);
    return (written == bytes.size());
}
