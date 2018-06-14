/*
 * Project2.c
 *
 * Created: 4/27/2018 12:56:22 PM
 * Author : kenag
 
 */ 

#include <avr/io.h>
#include <stdio.h>
#include "avr.h"
#include "lcd.h"

#define DATE_SIZE 13
#define TIME_SIZE 14

#define KEYPAD_ROW_COUNT 4
#define KEYPAD_COLUMN_COUNT 4

/*
	1: Increment Months
	2: Increment Days
	3: Increment Years
	A: Increment Hours
	7: Increment Minutes
	8: Increment Seconds
	9: Decrement Years
	B: Start Timer
	C: Switch between regular and military time
*/


int is_keypressed(int row,int column);
int get_key();

static volatile int days = 28;
static volatile int years = 2016;
static volatile int months = 2;
static volatile int hours = 23;
static volatile int minutes = 59;
static volatile int seconds = 45;
static volatile int useMilitary = 1;

static char time_str[TIME_SIZE];
static char date_str[DATE_SIZE];


void outputTime();
void incrementTime();
void incrementMonth();
int isLeapYear();
void verifyTime();
void verifyDays();

int main(void){
	ini_avr();
	ini_lcd();
	
	int needsTicking = 0;	
	outputTime();

	while(1){
		wait_avr(100);
		int code = get_key();
		if(code){
			//needsTicking = 0;
			if(code != 8)
				needsTicking = 0;
			else
				needsTicking = 1;
				
			if(code == 12){
				useMilitary = !(useMilitary);	
			}
			
			switch(code){
				case 1:
					months++;
					break;
				case 2:
					days++;
					break;
				case 3:
					years++;
					break;
				case 4:
					hours++;
					break;
				case 9:
					minutes++;
					break;
				case 10:
					seconds++;
					break;
				case 11:
					years--;
					break;
				default:
					break;
			}
			
			verifyTime();
			clr_lcd();
			outputTime();
			wait_avr(100);
		}
		if(needsTicking){
			outputTime();
			wait_avr(1000);
			incrementTime();
		}
		
	}
	return 0;
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




void outputTime(){
	snprintf(date_str,DATE_SIZE,"%02d/%02d/%04d",months,days,years);
	
	if(!useMilitary){
		int tempHour = hours < 12 ? hours : (hours % 12);
		tempHour = tempHour == 0 ? 12 : tempHour;
		snprintf(time_str,TIME_SIZE,"%02d:%02d:%02d %s",tempHour,minutes,seconds,hours < 12 ? "AM" : "PM");
	} else {
		snprintf(time_str,TIME_SIZE,"%02d:%02d:%02d",hours,minutes,seconds);	
	}

	pos_lcd(0,0);
	puts_lcd2(date_str);
	pos_lcd(1,0);
	puts_lcd2(time_str);
	
}

void incrementTime(){
	if(seconds < 60){
		seconds++;
		if(seconds >= 60){
			seconds = 0;
			minutes++;
		}
	}

	if(minutes >= 60){
		minutes = 0;
		hours++;
	}

	if(hours >= 24){
		hours = 0;
		days++;
	}

	if(days >= 28){
		incrementMonth();
	}

	if(months >= 13){
		months = 1;
		years++;
	}
}


void incrementMonth(){
	if(days > 28 && months == 2 && !(isLeapYear())){
			days = 1;
			months++;
		} else if(days > 29 && months == 2){
			days = 1;
			months++;	
		} else if(days > 30 && (months == 4 || months == 6 || months == 9 || months == 11)){
			days = 1;
			months++;
		} else if(days > 31){
			days = 1;
			months++;
	}
}

int isLeapYear(){
	return years % 4 == 0 && (years % 100 != 0 || years % 400 == 0);
}


void verifyTime(){
	if(years < 0){
		years = 2018;
	}


	if(months > 12){
		months = 1;
	}

	if(hours > 23){
		hours = 0;
	}

	if(minutes > 59){
		minutes = 0;
	}

	if(seconds > 59){
		seconds = 0;
	}

	if(days > 28){
		verifyDays();
	}
}

void verifyDays(){
	if(months == 2){
		if(days > 29 && isLeapYear()){
				days = 1;
			} else if(!isLeapYear() && days > 28){
				days = 1;
			}
		} else if(months == 4 || months == 6 || months == 9 || months == 11){
			if(days > 30){
				days = 1;
			}
		} else {
		if(days > 31){
			days = 1;
		}
	}
}

