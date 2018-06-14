// Name: Joshua Pascascio UCI ID#: 52192782
// Name:Kena Gindaba SID:14895132


#include <avr/io.h>
#include "avr.h"

int main(void) {
    /* Replace with your application code */
	SET_BIT(DDRB,0); //Set PORTB0 as output
	CLR_BIT(DDRB,2); //SET PINB2 as input
	
	while (1) {
		// When a button is pressed, the circuit has a path to ground with around 10k Ohms of resistance.
		// This means voltage will flow through the closed switch to ground rather than to the input pin (PINB2)

		//check if the switch is pushed. The switch is at pin 2.
		if(!(GET_BIT(PINB,2))){ 
			//Blink led at a rate 0.5s

			SET_BIT(PORTB,0); //sets the bit at postion 0 to 1 - We do at position 0 because that's where the led is!
			wait_avr(500); //0.5s delay
			CLR_BIT(PORTB,0); //set the bit at position 0 to zero.
			wait_avr(500); //0.5s delay
		} 
		
		else {
			CLR_BIT(PORTB,0); //If the button is not pressed keep the LED off.
		}
	} 
	
	return 0;
}

/*
PORTx is used for two things.
 1.When PORT is set to output port(DDRx is 1), It used to output data. 
 	e.d SET_BIT(DDRB,1) 
 		//now we can write an data into the correspoding bits in the register PORTx - PORTB in this case.
 		// This will change the state of the output PIN.
		SET_BIT(PORTB, 0) //the set_bit function here is used to write to PORTB 

 2.When PORT is congigured as an input it is used to activate/deactivate "PULL UP RESISTORS".

 	when DDRB is set to 0 as in the code above for example, the correspoding PORTB pins are used to 
 */