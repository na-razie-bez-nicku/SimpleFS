#ifndef BIT_ARRAY
#define BIT_ARRAY

#include <vector>
#include <cstdint>

class BitArray
{
public:
    uint8_t byte;

    BitArray()
    {
        // Round to full bytes
        byte = 0x00;
    }

    // set bit to 1
    void setBit(size_t bitIndex)
    {
        size_t bitPos = bitIndex % 8;
        byte |= (1 << bitPos);
    }

    // clear bit (set to 0)
    void clearBit(size_t bitIndex)
    {
        size_t bitPos = bitIndex % 8;
        byte &= ~(1 << bitPos);
    }

    // Check bit is set (1)
    bool isBitSet(size_t bitIndex) const
    {
        size_t bitPos = bitIndex % 8;
        return (byte & (1 << bitPos)) != 0;
    }
};

#endif