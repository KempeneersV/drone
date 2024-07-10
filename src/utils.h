#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Convertit une valeur int16_t en une chaîne ASCII.
 *
 * @param value La valeur int16_t à convertir.
 * @param buffer Le buffer où stocker la chaîne ASCII résultante.
 * @param buffer_size La taille du buffer.
 */
void int_to_ascii(int32_t num, char *buffer);
void float_to_char(float num, char *buffer);

#endif // UTILS_H
