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
    uint16_t startTime_;        // czas początku danego impulsu = czasowi realTime_ poprzedniego impulsu
    ShapeKind elementType_;     // typ przebiegu (stały, skokowy, liniowy, sinusoida)
    uint16_t timeInterval_;     // dla wszystkich czas trwania a okres sinusoidy dla sinusa
    int16_t pressureDiff_;      // zmiana ciśnienia w zadanym interwale czasowym, dla skokowego  (todo: wyjaśnić czy dla jednego schodka czy dla interwału) a min ciśnienie dla sinusa
    uint8_t stepsNumber_;       // liczba schodków dla skokowego, liczba okresów dla sinusoidy
    uint16_t pressureMax_;      // maksymalne ciśnienie w przebiegu sinusoidalnym
    uint16_t realTime_;         // czas końca bieżącego impulsu liczony od początku
};

#endif // TYPES_H
