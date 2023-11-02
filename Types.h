#ifndef TYPES_H
#define TYPES_H

#include <cstdint>
enum class ShapeKind
{
    STABLE = 0,
    JUMP,
    LINE,
    SERIE,
    SINUS
};

struct PressureElement
{
    uint16_t startTime_;
    ShapeKind elementType_;
    uint16_t timeInterval_;     // okres sinusoidy dla sinusa
    uint16_t pressureDiff_;     // min dla sinusa
    uint8_t stepsNumber_;
    uint16_t pressureMax_;     // min dla sinusa
};

#endif // TYPES_H
