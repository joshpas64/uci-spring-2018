#include <avr/io.h>
#include <stdio.h>
#include "avr.h"
#include "lcd.h"


#define KEYPAD_ROW_COUNT 4
#define KEYPAD_COLUMN_COUNT 4

#define NUM_MELODIES 2
#define NOTES_PER_MELODY 8

#define UNIQUE_NOTES 14

#define A1 440
#define A2 880
#define W 1000
#define H 500
#define Q 250
#define E 125


#define LINE_SIZE 16

#define A_INDEX 0
#define A_SHARP_INDEX 1
#define B_INDEX 2 
#define B_SHARP_INDEX 3
#define C_INDEX 4
#define C_SHARP_INDEX 5
#define D_INDEX 6
#define D_SHARP_INDEX 7
#define E_INDEX 8
#define E_SHARP_INDEX 9
#define F_INDEX 10
#define F_SHARP_INDEX 11
#define G_INDEX 12
#define G_SHARP_INDEX 13


#define A_NOTE 440
#define A_SHARP_NOTE 466
#define B_NOTE 493
#define B_SHARP_NOTE 523
#define C_NOTE 554
#define C_SHARP_NOTE 587
#define D_NOTE 622
#define D_SHARP_NOTE 659
#define E_NOTE 880
#define E_SHARP_NOTE 739
#define F_NOTE 783
#define F_SHARP_NOTE 830
#define G_NOTE 880
#define G_SHARP_NOTE 932





int is_keypressed(int row,int column);
int get_key();
void wait_avr2(unsigned int usec);
void wait_avr3(unsigned int usec);
unsigned int calculateWait(int frequency);
int parseFrequency(unsigned int wait);


typedef struct note {
	unsigned int f;
	unsigned int d;
} note;

void play_birthday2();
void play_birthday();
void play_song(note* melody,int size);
void play_note(unsigned int frequency,unsigned int duration);
void test_sound(unsigned int usec);
void display_song(note* melody,int index);

void ini_melodies();


static const unsigned int frequencies[UNIQUE_NOTES] = {440,466,493,523,554,587,622,659,698,739,783,830,880,932};
static const unsigned int durations[UNIQUE_NOTES] = {2,2,2,2,2,2,2,2};

note reference_notes[UNIQUE_NOTES];

note melody1[8];
note melody2[4];

note* melodies[NUM_MELODIES];

static const int melodySizes[NUM_MELODIES] = {8,4};


static char line1[LINE_SIZE];
static char line2[LINE_SIZE];

void main(void){
	ini_avr();
	ini_lcd();

	SET_BIT(DDRB,3);

	ini_melodies();

	const char* opener = "PICK A SONG";
	clr_lcd();
	pos_lcd(0,0);
	puts_lcd2(opener);

	while(1){
		int code = get_key();
		if(code){
			//unsigned int wait = 100;
			int melodyIndex = 0;
			switch(code){
				case 1:
					melodyIndex = 0;
					break;
				case 2:
					melodyIndex = 1;
					break;
				case 3:
					melodyIndex = 2;
					break;
				case 4:
					break;
				case 5:
					break;
				case 6:
					break;
				case 7:
					break;
				case 8:
					break;
				case 9:
					break;
				case 10:
					break;
				case 11:
					break;
				case 12:
					break;
				case 13:
					break;
				case 14:
					break;
				case 15:
					break;
				case 16:
					break;
				default:
					break;
			}

			display_song(melodies[melodyIndex],melodyIndex);
			if(melodyIndex == 1)
				play_birthday();
			else if(melodyIndex == 2)
				play_birthday2();
			else
				test_sound(114);
		} 
	}
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


void wait_avr2(unsigned int usec){
	TCCR0 = 2;

	while(usec--){
		TCNT0 = (unsigned char) (256 - (XTAL_FRQ / 8) * 0.00005);
		//TCNT0 = (unsigned char) (256 - (XTAL_FRQ / 8) * 0.0000005);
		SET_BIT(TIFR,TOV0);
		WDR();
		while(!GET_BIT(TIFR,TOV0));
	}

	TCCR0 = 0;
}

void wait_avr3(unsigned int usec){
	TCCR0 = 2;

	while(usec--){
		TCNT0 = (unsigned char) (256 - (XTAL_FRQ / 8) * 0.000005);
		SET_BIT(TIFR,TOV0);
		WDR();
		while(!GET_BIT(TIFR,TOV0));
	}

	TCCR0 = 0;
}


void test_sound(unsigned int usec){
	wait_avr2(usec);
	SET_BIT(PORTB,3);
	wait_avr2(usec);
	CLR_BIT(PORTB,3);
}

unsigned int calculateWait(int frequency){
	float period = (1.0 / frequency) / 2.0;
	unsigned int wait = (unsigned int) (round(period * 100000));
	return wait;
}

int parseFrequency(unsigned int wait){
	float recip_freq = (float) (wait * 0.00001 * 2);
	int freq = (int) (1 / recip_freq);
	return freq;
}

void play_birthday(){


	play_note(G_NOTE,2);
	play_note(F_NOTE,2);
	play_note(E_NOTE,2);
	play_note(C_NOTE,1);
	play_note(C_NOTE,1);
	play_note(B_NOTE,2);
	play_note(G_NOTE,2);
	play_note(A_NOTE,2);
	play_note(G_NOTE,4);

}

void play_birthday2(){
	play_note(C_NOTE,2);
	play_note(E_NOTE,1);
	play_note(F_SHARP_NOTE,4);
	play_note(D_NOTE,1);


	play_note(G_NOTE,2);
	play_note(C_NOTE,1);
	play_note(A_NOTE,1);
	play_note(D_NOTE,2);
	

}

void play_song(note* melody,int size){
	int i;

	for(i = 0; i < 8; i++){
		play_note(melody[i].f,melody[i].d);
	}
}


void play_note(unsigned int frequency,unsigned int duration){
	int i;
	unsigned int period = (unsigned int) ((100000) / frequency);

	unsigned int tHigh = period / 2;
	unsigned int tLow = period / 2;

	unsigned int cycles = frequency * duration;
	for(i = 0; i < cycles; i++){
		SET_BIT(PORTB,3);
		wait_avr3(tHigh);
		CLR_BIT(PORTB,3);
		wait_avr3(tLow);
	}
}


void ini_melodies(){
	melody1[0] = reference_notes[G_INDEX];
	melody1[1] = reference_notes[A_INDEX];
	melody1[2] = reference_notes[B_INDEX];
	melody1[3] = reference_notes[C_INDEX];
	melody1[4] = reference_notes[D_INDEX];
	melody1[5] = reference_notes[E_INDEX];
	melody1[6] = reference_notes[F_SHARP_INDEX];
	melody1[7] = reference_notes[G_INDEX];

	melody2[0] = reference_notes[G_INDEX];
	melody2[1] = reference_notes[C_INDEX];
	melody2[2] = reference_notes[A_INDEX];
	melody2[3] = reference_notes[D_INDEX];

	melodies[0] = melody1;
	melodies[1] = melody2;
}



void display_song(note* melody,int index){
	clr_lcd();
	snprintf(line1,LINE_SIZE,"Melody %02d",(index % NUM_MELODIES) + 1);
	snprintf(line2,LINE_SIZE,"Has %02d Notes",melodySizes[index % NUM_MELODIES]);
	pos_lcd(0,0);
	puts_lcd2(line1);
	pos_lcd(1,0);
	puts_lcd2(line2);
}

