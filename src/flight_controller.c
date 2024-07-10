#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    // set PORTB as output
    DDRB = DDRB | (1<< DDB5);

    // forever...
    while(1){
        // set PORTB
        PORTB = PORTB | (1 << PORTB5);

        // wait
        _delay_ms(1000);

        // unset PORTB5
        PORTB = PORTB & ~(1 << PORTB5);

        // wait somemore
        _delay_ms(1000);
    }
}