#include <avr/io.h>
#include <stdio.h>
#include "avr.h"
#include "lcd.h"

#define KEYPAD_ROW_COUNT 4
#define KEYPAD_COLUMN_COUNT 4

#define A1 440
#define A2 880
#define W 1000
#define H 500
#define Q 250
#define E 125

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

#define PASSWORD_SIZE 15
#define LINE_SIZE 16

void wait_avr3(unsigned int usec);

int is_keypressed(int row,int column);
int get_key();
char parse_key(int code);

int check_password();
void output_password(int promptBit,int* ptr);

void play_note(unsigned int frequency,unsigned int duration);
void sound_alarm();
void reset_input();

static char line1[LINE_SIZE];
static char line2[LINE_SIZE];

//By default are passwords and user input are initially empty.
static int password[PASSWORD_SIZE] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static int user_input[PASSWORD_SIZE] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

static int access_status; //If the user's password is correct
static int setting_password; //If the user is setting a new password

static int password_set;

static int inputting; //If the user is currently inputting a password to set or confirm

static int show_status;
static int index; //Cursor position of the set of digits that represent a user's input.

void main(void){
	ini_avr();
	ini_lcd();

	SET_BIT(DDRB,3);
	SET_BIT(DDRB,4);

	int promptBit = 1; //Status Bit that determines the top level prompt (Setting Password / Your Input)

	int alarmBit = 0; //By default the alarm is off

	index = 0; //Default index is 0, left most cursor
	setting_password = 1; //User must set a password on startup
	access_status = 1;

	inputting = 1;

	show_status = 0;

	//Display Introduction Prompt to LCD
	clr_lcd();
	pos_lcd(0,0);
	puts_lcd2("Plz enter an");
	pos_lcd(1,0);
	puts_lcd2("initial passwd");


	while(1){	
		int code = get_key(); //Get input code from keypad

		if(code){
			CLR_BIT(PORTB,3); //Turn of alarm so user can input a new password
			output_password(promptBit, setting_password ? password : user_input);

			//Form of button debouncing
			wait_avr(150); //Wait between button press so inputs are not registered more than once accidentally
			inputting = 1; //User is now inputting a password, turn off either LED or speaker.
			if(code == 13){
				clr_lcd(); //Clear LCD when a user chooses to either confirm a password or set a new one.

				//If the system is in a state where it is setting a new password
				if(setting_password){
					setting_password = !(setting_password); //Change setting password setting

					//State to allow user to set a new password (setting_password == 1)
					if(setting_password){
						clr_lcd();
						pos_lcd(0,0);
						puts_lcd2("Set your");
						pos_lcd(1,0);
						puts_lcd2("new passwd");
						promptBit = 1; //Change prompt setting

					} else { 
						//New user password is now saved into the system.
						clr_lcd();
						pos_lcd(0,0);
						puts_lcd2("Your passwd");
						pos_lcd(1,0);
						puts_lcd2("now saved!");
						promptBit = 0;
					}

					//wait_avr(1);
				} else { //When a user is trying to confirm a password and get access to the system
					int permission = check_password(); //Check the user's input with the saved password

					clr_lcd();
					pos_lcd(0,0);
					puts_lcd2("Checking Input");
					pos_lcd(1,0);
					puts_lcd2("with passwd");

					wait_avr(25);

					//Set access status flag to whether or not the passwords match
					if(permission){
						access_status = 1;
					} else {
						access_status = 0;
					}

					promptBit = 0;					
					inputting = 0; //User is done inputting, can now process and check status of the system.
				}
				
				index = 0; //Reset cursor position.
			} else if(code == 15) { //Clear user's input and the current password
				clr_lcd();
				int i;
				for(i = 0; i < PASSWORD_SIZE; i++){
					password[i] = 0;
					user_input[i] = 0;
				}
				setting_password = 1;
				inputting = 1;
				promptBit = 1;
				index = 0; //Reset cursor position
			} else if(code == 16){ //Button to clear user's current input (either a new password or a password to authenticate)
				
				//If the user is trying to set a new password, clear password screen.
				if(setting_password){
					int j;
					for(j = 0; j < PASSWORD_SIZE; j++){
						password[j] = 0; //Clear every password digit / item
					}
					output_password(promptBit,password);
				} else { //If user is trying to authenticate, clear user input screen.
					reset_input();
					output_password(promptBit,user_input);
				}
					
				index = 0; //Reset cursor position
				inputting = 1; //User is now restarting input
			} else {
				
				
				if(setting_password){
					password[index] = code; //Add button code to the password's next item
					output_password(promptBit,password); //Show new password
				} else {
					user_input[index] = code; //Add button code to the user's input (attempted authentication password)
					output_password(promptBit,user_input); //Show user's input
				}

				index = (index + 1) % PASSWORD_SIZE; //Move to the next cursor position, wrap around if at the end (bounded buffer)
			}
		} 

		
		if(!inputting){
			//When the user's inputted password matches
			if(!setting_password && access_status){
				alarmBit = 0; 
				
				//Display success prompt to LCD
				snprintf(line1,LINE_SIZE,"Success !");
				snprintf(line2,LINE_SIZE,"Access Granted");

				clr_lcd();
				pos_lcd(0,0);
				puts_lcd2(line1);
				pos_lcd(1,0);
				puts_lcd2(line2);

				CLR_BIT(PORTB,3); //Turn off alarm speaker
				SET_BIT(PORTB,4); //Turn on Success LED
			} else if(!setting_password && !access_status){ //When the password does not match

				//Display Access Denied Prompt to LCD
				snprintf(line1,LINE_SIZE,"Access Denied");
				snprintf(line2,LINE_SIZE,"Try Again!");

				clr_lcd();
				pos_lcd(0,0);
				puts_lcd2(line1);
				pos_lcd(1,0);
				puts_lcd2(line2);

				//output_password(password);

				CLR_BIT(PORTB,4); //Turn off access granted LED
				alarmBit = 1;
				sound_alarm(); //Turn on Speaker and sound alarm melody.

				
				//access_status = 1;
			}
			
			reset_input(); //Clear user input
			index = 0; //Reset cursor position
		}
	}

}

//Determine if user pressed a button on the keypad
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

//Button code collecting function from project 2
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


//Timer Function from Project 3
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

//Play note function from Project 3
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


void output_password(int promptBit,int* ptr){
	char keys[PASSWORD_SIZE];
	const char* prompt;
	
	switch(promptBit){
		case 1:
			prompt = "New Password";
			break;
		default:
			prompt = "Your Input:";
			break;
	}
	
	int i;

	for(i = 0; i < PASSWORD_SIZE; i++){
		keys[i] = parse_key(ptr[i]);
	}

	clr_lcd();
	pos_lcd(0,0);
	puts_lcd2(line1);
	snprintf(line1,LINE_SIZE,"%s",prompt);

	
	snprintf(line2,LINE_SIZE,
		"%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
		keys[0],
		keys[1],
		keys[2],
		keys[3],
		keys[4],
		keys[5],
		keys[6],
		keys[7],
		keys[8],
		keys[9],
		keys[10],
		keys[11],
		keys[12],
		keys[13],
		keys[14]
	);
	
	pos_lcd(1,0);
	puts_lcd2(line2);
}

char parse_key(int code){
	char key;
	if(code == 0){
		key = ' ';
		} else {
		switch(code){
			case 1:
			key = '1';
			break;
			case 2:
			key = '2';
			break;
			case 3:
			key = '3';
			break;
			case 4:
			key = 'A';
			break;
			case 5:
			key = '4';
			break;
			case 6:
			key = '5';
			break;
			case 7:
			key = '6';
			break;
			case 8:
			key = 'B';
			break;
			case 9:
			key = '7';
			break;
			case 10:
			key = '8';
			break;
			case 11:
			key = '9';
			break;
			case 12:
			key = 'C';
			break;
			case 14:
			key = '0';
			break;
			default:
			key = ' ';
			break;
		}
	}
	return key;
}

//Check if user input is the same as the password saved in memory.
int check_password(){
	int i;
	for(i = 0; i < PASSWORD_SIZE; i++){
		if(password[i] != user_input[i]){
			return 0;
		}
	}

	return 1;
}

//Clear user input cache
void reset_input(){
	int i;
	for(i = 0; i < PASSWORD_SIZE; i++){
		user_input[i] = 0;
	}
	
}

//Alarm melody for when user inputs an incorrect password
void sound_alarm(){
	play_note(G_NOTE,1);
	play_note(F_NOTE,1);
}