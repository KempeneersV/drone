#include <avr/interrupt.h>
#include "timer.h"

#define TIMER1_PRESCALER 64
#define TIMER1_COUNT_PER_MS (F_CPU / TIMER1_PRESCALER / 1000)

// Counter for number of ms since startup
volatile static uint32_t timerMillis = 0;

// Timer1 initialization
void initTimer()
{
    // CTC mode (Clear Timer on Compare Match) with OCR1A as the comparison register
    TCCR1B |= (1 << WGM12);
    
    // Set prescaler to 64
    TCCR1B |= (1 << CS11) | (1 << CS10);
    
    // Load comparison value to achieve interrupt every millisecond
    OCR1A = TIMER1_COUNT_PER_MS - 1;
    
    // Enable Timer1 compare match interrupt
    TIMSK1 |= (1 << OCIE1A);
    
    // Enable global interrupts
    sei();
}

// Delay function in milliseconds
void delay(uint32_t ms)
{
    uint32_t start = millis();
    while (millis() - start < ms) {
        // Wait
    }
}

// Function to obtain the number of milliseconds elapsed since startup
uint32_t millis()
{
    uint32_t m;
    uint8_t oldSREG = SREG;
    cli();
    m = timerMillis;
    SREG = oldSREG;
    return m;
}

// Function to obtain the number of microseconds elapsed since startup
uint32_t micros()
{
    uint32_t us;
    uint8_t oldSREG = SREG;
    cli();
    us = timerMillis * 1000 + TCNT0 / (F_CPU / 1000000L);
    SREG = oldSREG;
    return us;
}

ISR(TIMER1_COMPA_vect)
{
    cli();
    // Increment timerMillis variable
    timerMillis++;
    sei();
}