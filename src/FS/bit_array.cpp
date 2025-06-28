#include <vector>
#include <cstdint>

class BitArray {
    std::vector<uint8_t> data;

public:
    BitArray(size_t bitCount) {
        // Zaokrąglamy do pełnych bajtów
        size_t byteCount = (bitCount + 7) / 8;
        data.resize(byteCount, 0);
    }

    // Ustaw bit na 1
    void setBit(size_t bitIndex) {
        size_t byteIndex = bitIndex / 8;
        size_t bitPos = bitIndex % 8;
        data[byteIndex] |= (1 << bitPos);
    }

    // Wyczyść bit (ustaw na 0)
    void clearBit(size_t bitIndex) {
        size_t byteIndex = bitIndex / 8;
        size_t bitPos = bitIndex % 8;
        data[byteIndex] &= ~(1 << bitPos);
    }

    // Sprawdź, czy bit jest ustawiony (1)
    bool isBitSet(size_t bitIndex) const {
        size_t byteIndex = bitIndex / 8;
        size_t bitPos = bitIndex % 8;
        return (data[byteIndex] & (1 << bitPos)) != 0;
    }
};