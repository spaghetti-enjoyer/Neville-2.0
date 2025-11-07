#include <serial/serial.h>

namespace motor
{
    constexpr uint8_t LEFT    = 0xc1;
    constexpr uint8_t RIGHT   = 0xc5;
}

namespace direction
{
    constexpr uint8_t FORWARD = 0x00;
    constexpr uint8_t REVERSE = 0x01;
}

// 0xc1 is LEFT forward
// 0xc2 is LEFT backwards
// 0xc5 is RIGHT forward
// 0xc6 is RIGHT backwards
// max speed is 0x7f

class Neville
{
    public: 
        Neville(const std::string &port);
        int activate_motor(uint8_t motor, uint8_t dir, uint8_t speed);
        int buzz();

    private:
        serial::Serial serial;
};
