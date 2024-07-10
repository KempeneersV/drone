#include <stdint.h>

// Helper function to convert an integer to its ASCII representation
void int_to_ascii(int32_t num, char *buffer) {
    int i = 0;
    int is_negative = 0;

    // Handle negative numbers
    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    // Handle zero explicitly, otherwise, handle positive numbers
    do {
        buffer[i++] = num % 10 + '0';
        num /= 10;
    } while (num > 0);

    // If the number was negative, add the '-' sign
    if (is_negative) {
        buffer[i++] = '-';
    }

    // Reverse the buffer to get the correct ASCII representation
    for (int j = 0; j < i / 2; j++) {
        char temp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = temp;
    }

    // Null-terminate the string
    buffer[i] = '\0';
}

// Function to convert a float to its character representation with specified decimal places
void float_to_ascii(float num, char *buffer, int32_t decimal_places) {
    // Extract integer part
    int32_t integer_part = (int32_t)num;

    // Convert integer part to ASCII
    int_to_ascii(integer_part, buffer);

    // Find the position of the decimal point in the buffer
    int len = 0;
    while (buffer[len] != '\0') {
        len++;
    }

    // Add decimal point
    buffer[len] = '.';
    len++;

    // Extract fractional part
    float fractional_part = fabs(num - integer_part); // Use fabs to handle negative numbers correctly

    // Convert fractional part to ASCII
    for (int i = 0; i < decimal_places; i++) {
        fractional_part *= 10;
        int digit = (int)fractional_part;
        buffer[len + i] = digit + '0';
        fractional_part -= digit;
    }

    // Null-terminate the string
    buffer[len + decimal_places] = '\0';
}