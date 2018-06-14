#include <avr/io.h>
#include <stdio.h>
#include <limits.h>
#include "avr.h"
#include "lcd.h"


#define KEYPAD_ROW_COUNT 4
#define KEYPAD_COLUMN_COUNT 4

#define SAMPLING_PERIOD 500
#define READING_SIZE 5


#define LINE_SIZE 16


typedef struct stats {
	unsigned short min;
	unsigned short max;
	unsigned short current;
} stats;

char* make_reading(unsigned short sample);
int is_keypressed(int row,int column);
int get_key();

unsigned long trials;
unsigned long sum;
unsigned int skippedFirst;

static stats statistics;
static char line1[LINE_SIZE];
static char line2[LINE_SIZE];


unsigned short get_raw_sample();
void update_stats(unsigned short sample);
void show_stats();


void main(void){
	ini_avr();
	ini_lcd();


	CLR_BIT(ADMUX,7);
	SET_BIT(ADMUX,6);
	
	
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0);

	statistics.min = 1023;
	statistics.max = 0;
	statistics.current = 0;

	trials = 0;
	sum = 0;

	skippedFirst = 0;

	int needsSampling = 0;
	int reset = 0;
	
	while(1){
		int code = get_key();
		if(code){
			switch(code){
				case 1:
				//reset button
				needsSampling = 0;
				reset = 1;
				break;
				case 2:
				//start sampling
				needsSampling = 1;
				reset = 0;
				break;
				default:
				break;
			}
		}

		if(needsSampling){
			wait_avr(SAMPLING_PERIOD);
			unsigned short sample = get_raw_sample();

			update_stats(sample);
			show_stats();
		}

		if(reset){
			trials = 0;
			sum = 0;
			skippedFirst = 0;
			statistics.min = 1023;
			statistics.max = 0;
			statistics.current = 0;
			clr_lcd();
		}
	}
}


unsigned short get_raw_sample(){

	CLR_BIT(ADMUX,0);
	CLR_BIT(ADMUX,1);
	CLR_BIT(ADMUX,2);
	CLR_BIT(ADMUX,3);
	CLR_BIT(ADMUX,4);

	SET_BIT(ADCSRA,6);

	while(!GET_BIT(ADCSRA,4)){}

	SET_BIT(ADCSRA,4);

	return (ADC);
}

void update_stats(unsigned short sample){

	if(trials >= ULONG_MAX){
		trials = 0;
	}

	if(sum >= ULONG_MAX){
		sum = 0;
	}

	
	trials++;
	
	sum += sample;
	
	statistics.current = sample;
	
	if(sample < statistics.min){
		statistics.min = sample;
	}
	
	if(sample > statistics.max){
		statistics.max = sample;
	}
	
}

void show_stats(){
	clr_lcd();

	
	unsigned int conversion = (((unsigned long) (statistics.min)) * 5 * 100) / 1023;
	unsigned int digit1 = conversion / 100;
	unsigned int decimals1 = conversion % 100;
	
	unsigned int conversion2 = (((unsigned long) (statistics.max)) * 5 * 100) / 1023;
	unsigned int digit2 = conversion2 / 100;
	unsigned int decimals2 = conversion2 % 100;
	
	snprintf(line1,LINE_SIZE,"Mn:%d.%02d Mx:%d.%02d",digit1,decimals1,digit2,decimals2);
	
	conversion = (((unsigned long) (statistics.current)) * 500) / 1023;
	digit1 = conversion / 100;
	decimals1 = conversion % 100;
	
	conversion2 = (((unsigned long) (sum / trials)) * 500) / 1023;
	digit2 = conversion2 / 100;
	decimals2 = conversion2 % 100;

	snprintf(line2,LINE_SIZE,"V:%d.%02d A:%d.%02d",digit1,decimals1,digit2,decimals2);
	
	
	pos_lcd(0,0);
	puts_lcd2(line1);
	pos_lcd(1,0);
	puts_lcd2(line2);
	
}

int is_pressed(int r,int c){
	DDRC = 0;
	PORTC = 0;

	SET_BIT(DDRC,r);
	CLR_BIT(PORTC,r);

	CLR_BIT(DDRC,c + 4);
	SET_BIT(PORTC,c + 4);

	wait_avr(1);

	if(GET_BIT(PINC,c+4)){
		return 0;
	}

	return 1;
}


int get_key(){

	int row,col;
	for(row = 0; row < KEYPAD_ROW_COUNT;row++){
		for(col = 0; col < KEYPAD_COLUMN_COUNT; col++){
			if(is_pressed(row,col)){
				int code = row * KEYPAD_COLUMN_COUNT + col + 1;
				return code;
			}
		}
	}

	return 0;
}
