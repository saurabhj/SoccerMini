/*  ============================================================
 *	The "SOCCER mini" game                                     *
 *	Written by Gaurang Sinha & Saurabh Jain                    *
 *	TE COMPUTER STUDENTS,                                      *
 *	MES College of Engineering,                                *
 *	Wadia College Campus,                                      *
 *	Pune - 411001                                              *
 *                                                             *
 *	Entry to the "Game Programming Competition in C / C++"     *
 *															   *
 * 	Written in Borland's Turbo C 3.0 in 2005                   *
 *  Code best viewed in the Turbo C 3.0 Editor                 *
 *                         with a tab spacing of 4             *
 *  ============================================================
*/

//////////////////////////////////////////////////////////////////////////
//			    				INCLUDE FILES							//
//////////////////////////////////////////////////////////////////////////
#include <iostream.h>
#include <conio.h>
#include <graphics.h>
#include <dos.h>
#include <bios.h>
#include <stdlib.h>
#include <stdio.h>
#include <alloc.h>
#include <string.h>
#include <math.h>
#include <fstream.h>
#include "mouse.h"
#include "music.h"

//////////////////////////////////////////////////////////////////////////
//			    				# DEFINITIONS							//
//////////////////////////////////////////////////////////////////////////
#define Yellow_Team 1
#define Red_Team    2
#define TRUE	    1
#define FALSE	    2
#define UP	    	0
#define DOWN	    1
#define NORMAL	    2
#define LEFT	    0
#define RIGHT       1
#define UP_KEY		0x48
#define DOWN_KEY	0x50
#define LEFT_KEY	0x1E	//'a'
#define RIGHT_KEY	0x1F	//'s'
#define ESC_KEY		0x1B
#define ENTER_KEY	0x1C
#define ACTIVE	    1
#define INACTIVE	0
#define BALLTIMEDELAY 50
// ( approx 3 seconds )
#define ONE_PLAYER  1
#define TWO_PLAYER  2

//////////////////////////////////////////////////////////////////////////
//			    		PROTOTYPE DECLARATIONS							//
//////////////////////////////////////////////////////////////////////////
void initialize(void);
void ShowIntro(void);
void Draw_Table(void);
void GamePlay(int);
void draw_all(void);		// Draws all players on screen
void Refresh_Table(void);
void DDA(int x1, int y1, int x2, int y2);
void play_goal_seq(void);
void Main_Menu(void);
void Option_Menu(void);
void Show_TShirts(int, int, int);
void Display_Help(void);
void Game_Over_Screen(void);
void Computer_AI(void);
int Graphics_Accept(int x, int y);
int Get_AI_Level(void);
void CustomizeGame(void);
void SetPlayerDefaults(void);
int ChooseTwoPlayerMode(void);

//////////////////////////////////////////////////////////////////////////
//			    			CLASS DECLARATIONS							//
//////////////////////////////////////////////////////////////////////////
class _ball {
	public:
		float x_pos;	//Current X Position
		float y_pos;	//Current Y Position
		int direction;	//Direction up or down
		int side;		//Side left or right
		int speed;		//Speed of ball

		void draw_ball(void);
}Ball;

typedef class _player {
	public:
		int x_player;	//X Position
		int y_player;	//Y Position
		int state;		//direction in which he is pointing
		int check_collision(int TEAM);
		void draw_player(int x, int y, int COL);
}player;

class stick {
	private:
		int x_stick;
		int y_stick;
		int length;
		int team;
	public:
		int no_of_players;
		player Players[5];

		void set_no_of_players(int no, int team_no);
		void get_stick_pos(int *x, int *y);
		void set_stick_pos(int y);
		void draw_stick(void);
}Yellow[4] , Red[4];

typedef class _Button {
	private:
	public:
		int	x1, x2, y1, y2, X_text, Y_text;
		char caption[20];
		void draw_normal(int xx1,int yy1,int xx2,int yy2,char *str);
		void draw_pressed(void);
		void draw_unpressed(void);
}Win_Button;

//////////////////////////////////////////////////////////////////////////
//			    	   GLOBAL VARIABLE DECLARATIONS						//
/////////////////////////////////////////////////////////////////////////
const X_Center = 315;		//Our Board Center X
const Y_Center = 240;		// "   "     "	   Y

int delaytime = 1;
float X_inc = 1;			//X increment for Ball
float Y_inc = 1;			//Y increment for Ball
int RED_GOAL = 0;			//No Of GOALS RED has scored
int YELLOW_GOAL = 0;		//No Of GOALS YELLOW has scored
int BallKickTime = 0;		//For Deacceleration of the Ball
int CPUASSIST = 5;			//For Ball Kick A.I.
int MAXSPEED = 6;			//Default Maximum Ball Speed
int MINSPEED = 2;			//Default Minimum Ball Speed
int STARTSPEED = 3;			//Default Start Ball Speed
int MAXGOALS = 5;			//No Of goals to win
int AI_Level = 5;			//CPU A.I. Level ( 1 player mode)
int Song_No = 10;			//Default Background Track

int YPlayers[4];							// Defines the no. of players
int RPlayers[4];							// on each stick
// [0] Goalie is always constant = 1
// [1] = Defender ; [2] = Midfielders ; [3] = Strikers

union REGS input, output;

//////////////////////////////////////////////////////////////////////////
//			       			MAIN FUNCTION								//
//////////////////////////////////////////////////////////////////////////
int
main(void) {
	initialize(); cleardevice(); ShowIntro(); Main_Menu(); getch(); return 0;
}

//////////////////////////////////////////////////////////////////////////
//			 			INITIALISATION FUNCTION							//
//////////////////////////////////////////////////////////////////////////
void
initialize(void) {
	int gd=DETECT, gm, error, i, j, k;
	char path[80], c;

	ifstream g("config.ini");			//Stream For File Input
	g.getline(path, 80);
	g.close();

	initgraph(&gd, &gm, path);			// Tring to initialise graphics
	error = graphresult();
	while (error != grOk) {				//Error In Initializing Graphics
		clrscr();
		// If BGI file not found, ask user for path
		cout<<"Graphics error: "<<grapherrormsg(error);
		cout<<"\n\nDo You Wish To Manually Specifiy Path (y/n) : ";
		cin>>c;
		if(c == 'y' || c == 'Y') {
			cout<<"\n\nEnter Path For BGI Directory (E.g. C:\\TC\\BGI) : ";
			cin>>path;
			gd = DETECT;
			initgraph(&gd, &gm, path);
			error = graphresult();
		}
		else {
			exit(0);
		}
	}

	ofstream sv("config.ini");
	sv<<path;
	sv.close();

	setviewport(0, 0, 639, 479, 0);
	Mouse.initmouse();					// Initialisng Mouse
}

//////////////////////////////////////////////////////////////////////////
//			 			DRAW TABLE FUNCTION								//
//////////////////////////////////////////////////////////////////////////
void
Draw_Table(void) {
	int x, y, size;
	setfillstyle(1, GREEN);
	bar(155, 30, 485, 450);				//Main Board (Green Part)

	setcolor(YELLOW);
	rectangle(144, 19, 496, 461);		//OuterMost Border
	rectangle(143, 18, 497, 462);		//OuterMost Border

	setcolor(WHITE);
	setlinestyle(0, 1, 1);
	rectangle(154, 29, 486, 451);		//Inner Border
	ellipse(315, 240, 0, 360, 25, 20);	//Center Circle
	line(155, 240, 485, 240);			//Center Line
	rectangle(232, 30, 408, 125);		//Upper Outer D
	rectangle(276, 30, 364, 96);		//Upper Inner D
	rectangle(232, 355, 408, 450);		//Lower Outer D
	rectangle(276, 384, 364, 450);		//Lower Inner D

	//Draw Goal Nets
	setcolor(WHITE);
	setfillstyle(XHATCH_FILL, WHITE);
	rectangle(276, 30, 364, 20);					//For YELLOW's Goal
	bar(277, 29, 363, 21);							//
	rectangle(276, 450, 364, 460);					//
	bar(277, 451, 363, 459);						//

	setcolor(BLACK);								//
	setlinestyle(0, 1, 3);							//
	line(276, 450, 364, 450);						// Lower Goal
	line(276, 30, 364, 30);							// Upper Goal

	Yellow[0].set_no_of_players(YPlayers[0], Yellow_Team);	//Set No of
	Yellow[1].set_no_of_players(YPlayers[1], Yellow_Team);	// Players for
	Yellow[2].set_no_of_players(YPlayers[2], Yellow_Team);	// each team
	Yellow[3].set_no_of_players(YPlayers[3], Yellow_Team);	//
	Red[0].set_no_of_players(RPlayers[0], Red_Team);		// Goalie
	Red[1].set_no_of_players(RPlayers[1], Red_Team);		// Defenders
	Red[2].set_no_of_players(RPlayers[2], Red_Team);		// Mid Field
	Red[3].set_no_of_players(RPlayers[3], Red_Team);		// Offence

	Yellow[0].set_stick_pos(74);		//Set Y Positions for each
	Yellow[0].draw_stick();				// stick on the board and then
	Yellow[1].set_stick_pos(118);		// draw each stick
	Yellow[1].draw_stick();				//
	Red[3].set_stick_pos(162);			//
	Red[3].draw_stick();				//
	Yellow[2].set_stick_pos(206);		//
	Yellow[2].draw_stick();				//
	Red[2].set_stick_pos(274);			//
	Red[2].draw_stick();				//
	Yellow[3].set_stick_pos(318);		//
	Yellow[3].draw_stick();				//
	Red[1].set_stick_pos(362);			//
	Red[1].draw_stick();				//
	Red[0].set_stick_pos(406);			//
	Red[0].draw_stick();				//

	//Draw T-Shirts For Each Team And Display Score
	Show_TShirts(575, 80, YELLOW);
	Show_TShirts(575, 318, RED);

	// Quit if user clicks mouse
	while(!(kbhit())) {
		Mouse.getmousepos();
		if((Mouse.Button &1) == 1) {
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//				STICK : SETS THE NUMBER OF PLAYERS ON STICK				//
//////////////////////////////////////////////////////////////////////////
void
stick :: set_no_of_players(int no, int team_no) {
	no_of_players = no;
	team = team_no;
}

//////////////////////////////////////////////////////////////////////////
//				STICK : SET STICK POSITION ON FIELD						//
//////////////////////////////////////////////////////////////////////////
void
stick :: set_stick_pos(int y) {
	x_stick = 100;
	y_stick = y;
	length = 432;
}

//////////////////////////////////////////////////////////////////////////
//				STICK : GETS THE POSITION OF THE STICK					//
//////////////////////////////////////////////////////////////////////////
void
stick :: get_stick_pos(int *x, int *y) {
	*x = x_stick;
	*y = y_stick;
}

//////////////////////////////////////////////////////////////////////////
//		 STICK : DRAWS THE STICK WITH PLAYERS AT DEFAULT LOCATION		//
//////////////////////////////////////////////////////////////////////////
void
stick :: draw_stick(void) {
	int disp;

	disp = length/(no_of_players + 1);
	setfillstyle(1, DARKGRAY);
	bar(x_stick, y_stick, x_stick+length, y_stick);

	for(int i=0 ; i<no_of_players ; i++) {
		if(team == Yellow_Team) {
			Players[i].state=2;
			Players[i].draw_player(x_stick + (disp*(i+1)), y_stick, YELLOW);
		}
		else {
			Players[i].state=2;
			Players[i].draw_player(x_stick + (disp*(i+1)), y_stick, RED);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//			   PLAYER : CHECKS COLLISION FOR PLAYER						//
//////////////////////////////////////////////////////////////////////////
/*
 *	Here we basically make a imaginary square around both the ball and the
 *	player to detect collision.
 */
int
_player :: check_collision(int TEAM) {
	int ball_top, ball_bottom;
	int ball_left, ball_right;
	int player_top, player_bottom;
	int player_left, player_right;
	int KickAssist = 0;

	ball_top = Ball.y_pos - 5;
	ball_bottom = Ball.y_pos + 5;
	ball_left = Ball.x_pos - 5;
	ball_right = Ball.x_pos + 5;

	player_top = y_player - 5;
	player_bottom = y_player + 5;
	player_left = x_player - 5;
	player_right = x_player + 5;

	// Checking for non collision cases
	if(ball_bottom < player_top){
		return 0;
	}
	if(ball_top > player_bottom){
		return 0;
	}
	if(ball_right < player_left){
		return 0;
	}
	if(ball_left > player_right){
		return 0;
	}

	// If the program gets till here, means some collision has
	// been detected ... so running the collision detection code
	// AI for Player Kick Direction & Side

	// Avoiding the side line collision ***
	if(Ball.x_pos < 170) {
		Ball.x_pos = Ball.x_pos + 5;
	}
	if(Ball.x_pos > 470) {
		Ball.x_pos = Ball.x_pos - 5;
	}

	if(TEAM == YELLOW) {
		if(y_player == 318) {			//Check If Strikers
			KickAssist = 4*CPUASSIST;	// Using the AI to generate Accuracy
			DDA(x_player, y_player, (230+KickAssist)+random((176-KickAssist)), 450);
			if(x_player >= 320) {
				Ball.side = LEFT;
			}
			else {
				Ball.side = RIGHT;
			}
		}
		else if(y_player == 206) {	//Check If Mid-Field
				if(state == LEFT) { // Player is moving to the left
					DDA(x_player, y_player, random(x_player), 30);
					Ball.side = LEFT;
				}
				else if(state == RIGHT){ // Player is moving to the right
					DDA(x_player, y_player, x_player + random((640 - x_player)), 30);
					Ball.side = RIGHT;
				}
				else {						// Player is standing straight
					KickAssist = 18*CPUASSIST;	// Using the AI to generate Accuracy
					DDA(x_player, y_player, (55 + KickAssist) + random((530 - (2*KickAssist))), 30);
				}
		}
		else {							//Else If Defenders
			if(state == NORMAL) {
				Ball.side = random(2);	//Randomly Put Which Side the
			}
			else {
				Ball.side = state;
			}
			X_inc = 1;
			Y_inc = 1;
		}
		Ball.direction = DOWN;
	}
	else {								//If TEAM = RED
		if(y_player == 162) {			//Check If Strikers
			KickAssist = 4*CPUASSIST;	// Using the AI to generate Accuracy
			DDA(x_player, y_player, (230+KickAssist)+random((176-KickAssist)), 30);
			if(x_player >= 320) {
				Ball.side = LEFT;
			}
			else {
				Ball.side = RIGHT;
			}
		}
		else if(y_player == 274) {		//Check If Mid-Field
			if(state == LEFT) {			//Player is moving to the left
				DDA(x_player, y_player, random(x_player), 450);
				Ball.side = LEFT;
			}
			else if(state == RIGHT) {	// Player is moving to the right
				DDA(x_player, y_player, x_player + random((640 - x_player)), 450);
				Ball.side = RIGHT;
			}
			else {	// Player is standing straight
				KickAssist = 18*CPUASSIST;	// Using the AI to generate Accuracy
				DDA(x_player, y_player, (55 + KickAssist) + random((530 - (2*KickAssist))), 450);
			}
		}
		else {							//Else if Defenders
			if(state == NORMAL) {
				Ball.side = random(2);	//Randomly Put Which Side the
			}
			else {
				Ball.side = state;
			}
			X_inc = 1;
			Y_inc = 1;
		}
		Ball.direction = UP;
	}
	// When player kicks the ball, make ball speed = MAX
	if((x_player < 475) && (x_player > 165)) {
		Ball.speed = MAXSPEED;
		BallKickTime = 0;			// Reset ball no kick delay checking
	}
	sound(800);
	delay(5);
	nosound();
	return 1;
}

//////////////////////////////////////////////////////////////////////////
//		FUNCTION : DRAWS PLAYERS ON BOARD AT SPECIFIED POSITION			//
//////////////////////////////////////////////////////////////////////////
void
_player :: draw_player(int x, int y, int COL) {
	static int Step = 0;
	x_player = x;
	y_player = y;
	setfillstyle(0, COL);
	bar(x-5, y-5, x+5, y+5);				// Red box
	setlinestyle(0, 1, 1);
	setcolor(BLACK);
	switch(state) {
		case NORMAL:
			setfillstyle(0, BLACK);			//Player Body
			bar(x-5, y-5, x+5, y+5);		//Basic BOX
			setlinestyle(0, 1, 1);			//
			setcolor(BLACK);
			if(COL == YELLOW) {             //
				bar(x-5, y+5, x-2, y+9);	// Draw Both Legs Forward
				bar(x+2, y+5, x+5, y+9);	//
			}
			else {
				bar(x-5, y-5, x-2, y-9);	//Draw Both Legs Forward
				bar(x+2, y-5, x+5, y-9);	//
			}
			break;
		case LEFT:                          //
			setfillstyle(0, BLACK);         //
			bar(x-5, y-5, x+5, y+5);		//Basic BOX
			setlinestyle(0, 1, 1);			//Set Line Style
			setcolor(BLACK);
			if(Step == 0){
				bar(x-5, y+5, x-9, y+2);	//Draw Leg Left
				bar(x+5, y-2, x+9, y-5);	//Draw Leg Right
				Step = 1;
			}
			else {
				bar(x+5, y+5, x+9, y+2);	//Draw Leg Left
				bar(x-5, y-2, x-9, y-5);	//Draw Leg Right
				Step = 0;
			}
			break;
		case RIGHT:
			setfillstyle(0, BLACK);
			bar(x-5, y-5, x+5, y+5);		//Basic BOX
			setlinestyle(0, 1, 1);			//Set Line Style
			setcolor(BLACK);
			if(Step == 0){
				bar(x+5, y+5, x+9, y+2);	//Draw Leg Left
				bar(x-5, y-2, x-9, y-5);	//Draw Leg Right
				Step = 1;
			}
			else{
				bar(x-5, y+5, x-9, y+2);	//Draw Leg Left
				bar(x+5, y-2, x+9, y-5);	//Draw Leg Right
				Step = 0;
			}
			break;
	}
	setcolor(COL);
	setlinestyle(0, 1, 3);
	rectangle(x-5, y-5, x+5, y+5);
}

//////////////////////////////////////////////////////////////////////////
//                  FUNCTION : DRAWS THE BALL ON THE FIELD				//
//////////////////////////////////////////////////////////////////////////
void
_ball :: draw_ball(void) {
	int x = x_pos;
	int y = y_pos;

	setcolor(WHITE);
	setfillstyle(1, WHITE);
	ellipse(x, y, 0, 360, 5, 5);
	fillellipse(x, y, 5, 5);
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
////		    		 MAIN GAMEPLAY FUNCTION						  ////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void
GamePlay(int MODE){
	int toss, i, j;
	char d;
	unsigned char key_count=0, c=0, prev;
	Ball.x_pos = X_Center;
	Ball.y_pos = Y_Center;
	Ball.speed = STARTSPEED;

	CurrentSong = Notes[Song_No];
	count = s = 0;

	randomize();
	toss = random(100);

	if(toss <= 50){
		Ball.direction = UP;
	}
	else{
		Ball.direction = DOWN;
	}

	int NormRedStep = 0, NormYelStep = 0;

	if(Song_No != 10)
		StartSong();							//Starting to play song

	// MAIN GAME LOOP BEGINS HERE !
	while(1){
		Mouse.getmousepos();

		// Checking to see and decrement ball speed
		BallKickTime++;
		if(BallKickTime >= BALLTIMEDELAY){
			if(Ball.speed > MINSPEED){
				Ball.speed --;
			}
			BallKickTime = 0;
		}

		// Resetting the feet position of the Red Players
		for(i=0 ; i<4 ; i++) {
			for(j=0 ; j<Red[i].no_of_players ; j++) {
				if(Red[i].Players[j].state != NORMAL) {
					if(NormRedStep == 5) {
						Red[i].Players[j].state = NORMAL;
						NormRedStep = 0;
					}
					else{
						NormRedStep ++;
					}
				}
			}
		}

		// Moving the red players
		if((Mouse.Button & 1) == 1) {			//Check Mouse Movement
			for(int i=0 ; i<4 ; i++) {
				for(int j=0 ; j<Red[i].no_of_players ; j++) {
					if(Red[0].Players[0].x_player >= 237) {
						Red[i].Players[j].state = LEFT;
						Red[i].Players[j].x_player -= 5;
					}
				}
			}
		}
		else if((Mouse.Button&2) == 2) {		   //For RIGHT Movement
			for(int i=0 ; i<4 ; i++) {
				for(int j=0 ; j<Red[i].no_of_players ; j++) {
					if(Red[0].Players[0].x_player <= 403) {
						Red[i].Players[j].state = RIGHT;
						Red[i].Players[j].x_player += 5;
					}
				}
			}
		}
		//Reset Red Goaly To Center Of Defenders ***
		if(RPlayers[1] > 0) {
			Red[0].Players[0].x_player = (Red[1].Players[0].x_player
							+ Red[1].Players[RPlayers[1]-1].x_player) / 2;
		}

		// Resetting the yellow players to normal feet position
		for(i=0 ; i<4 ; i++) {
			for(j=0 ; j<Yellow[i].no_of_players ; j++) {
				if(Yellow[i].Players[j].state != NORMAL){
					if(NormYelStep == 5){
						Yellow[i].Players[j].state = NORMAL;
						NormYelStep = 0;
					}
					else{
						NormYelStep ++;
					}
				}
			}
		}
		if(MODE == TWO_PLAYER) {			//Check For 2 Player Mode
			input.h.ah = 0x0B;				//Check For Key Press
			int86(0x21, &input, &output);	//
			if(output.h.al != 0x00) {		//If key press Accept Character

				c = inportb(0x60);

				if(c==prev) {				//check if buffer needs to be
					key_count++;			//flushed
					if(key_count == 20) {
						input.x.ax = 0x0C00;			//Flush keyboard Buffer
						int86(0x21, &input, &output);	//
					}
				}
				else {
					prev = c;
					key_count=0;
				}
				//Reset Yellow Goaly To Center Of Defenders ***
				if(YPlayers[1] > 0) {		// Defenders > 1
					Yellow[0].Players[0].x_player = (Yellow[1].Players[0].x_player
							+ Yellow[1].Players[YPlayers[1]-1].x_player) / 2;
				}

				if(c == LEFT_KEY) {		//For LEFT KEY
					for(int i=0 ; i<4 ; i++) {
						for(int j=0 ; j<Yellow[i].no_of_players ; j++) {
							if(Yellow[0].Players[0].x_player >= 237) {
								Yellow[i].Players[j].state = LEFT;
								Yellow[i].Players[j].x_player -= 5;
							}
						}
					}
				}
				else if(c == RIGHT_KEY) {		   //For RIGHT KEY
					for(int i=0 ; i<4 ; i++) {
						for(int j=0 ; j<Yellow[i].no_of_players ; j++) {
							if(Yellow[0].Players[0].x_player <= 403) {
								Yellow[i].Players[j].state = RIGHT;
								Yellow[i].Players[j].x_player += 5;
							}
						}
					}
				}
				else if(c == 0x01) {			//If ESC Pressed
					StopSong();
					cleardevice();
					while(kbhit())				// Clearing keybd buffer
						getch();
					Mouse.showmouseptr();
					ShowIntro();
					return;
				}
			}
		}
		else if(MODE == ONE_PLAYER) {	//Check For 1 Player Mode
			if(kbhit()){      			// Give user option to quit
				if(getch() == ESC_KEY){
					StopSong();
					cleardevice();
					while(kbhit())				// Clearing keybd buffer
						getch();
					ShowIntro();
					return;
				}
			}
			Computer_AI();
			//Reset Yellow Goalie To Center Of Defenders
			if(YPlayers[1] > 0) {
				Yellow[0].Players[0].x_player = (Yellow[1].Players[0].x_player
						+ Yellow[1].Players[YPlayers[1]-1].x_player) / 2;
			}
		}

		// Sidelines Collision detection
		if(((Ball.x_pos <= 170)) || ((Ball.x_pos >= 470))) {
			if(Ball.side == LEFT) {
				Ball.x_pos = Ball.x_pos + 6;
				Ball.side = RIGHT;
			}
			else{
				Ball.x_pos = Ball.x_pos - 6;
				Ball.side = LEFT;
			}
		}

		if(Ball.direction == UP){
			Ball.y_pos -= (Y_inc * Ball.speed);
		}
		else{
			Ball.y_pos += (Y_inc * Ball.speed);
		}
		if(Ball.side == LEFT){
			Ball.x_pos -= (X_inc * Ball.speed);
		}
		else{
			Ball.x_pos += (X_inc * Ball.speed);
		}

		Ball.draw_ball();

		// Player Collision detection
		for(i=0;i<4;i++){
			for(j=0;j<Yellow[i].no_of_players;j++){
				if(Yellow[i].Players[j].x_player > 160 &&
					Yellow[i].Players[j].x_player < 480 ){
					if(Yellow[i].Players[j].check_collision(YELLOW))
						break;
				}
			}
		}

		for(i=0;i<4;i++){
			for(j=0;j<Red[i].no_of_players;j++){
				if(Red[i].Players[j].x_player > 160 &&
						Red[i].Players[j].x_player < 480 ){
					if(Red[i].Players[j].check_collision(RED))
						break;
				}
			}
		}

		//Goal Detection
		if((!(Ball.y_pos > 45)) || (!(Ball.y_pos < 440))) {
			if((Ball.x_pos > 276) && (Ball.x_pos < 364)) {
				if(Ball.y_pos < 74) {
					//Goal Detected In YELLOW's GOAL
					RED_GOAL++;
					input.x.ax = 0x0C00;			//Flush keyboard Buffer
					int86(0x21, &input, &output);	//
					play_goal_seq();
					if(RED_GOAL == MAXGOALS) {
						StopSong();
						Game_Over_Screen();
						cleardevice();
						ShowIntro();
						return;
					}
					else {
						// Restarting sound engine
						count = s = 0;
						CurrentSong = Notes[Song_No];
						if(Song_No != 10)
							StartSong();

						cleardevice();
						Draw_Table();
						Ball.x_pos = X_Center;
						Ball.y_pos = Y_Center;
					}
				}
				else if(Ball.y_pos > 406) {
					//Goal Detected In RED's GOAL
					YELLOW_GOAL++;
					input.x.ax = 0x0C00;			//Flush keyboard Buffer
					int86(0x21, &input, &output);	//
					play_goal_seq();
					// Restarting Sound Engine
					count = s = 0;
					CurrentSong = Notes[Song_No];
					if(Song_No != 10)
						StartSong();

					if(YELLOW_GOAL == MAXGOALS) {
						StopSong();
						Game_Over_Screen();
						cleardevice();
						while(kbhit())				// Clearing keybd buffer
							getch();
						ShowIntro();
						return;
					}
					else {
						cleardevice();
						Draw_Table();
						Ball.x_pos = X_Center;
						Ball.y_pos = Y_Center;
					}
				}
			}
			if(Ball.direction == UP){
				Ball.direction = DOWN;
			}
			else{
				Ball.direction = UP;
			}
		}

		while(inportb(0x3da)&8);			//Waits for scan line to retrace
		while(!(inportb(0x3da)&8));			// and during retrace draws stuff
		Refresh_Table();
		draw_all();
	}
}

//////////////////////////////////////////////////////////////////////////
//			    		  DRAW ALL PLAYERS FUNCTION						//
//////////////////////////////////////////////////////////////////////////
void
draw_all(void){
	for(int j = 0; j < 4; j++){
		for(int i = 0 ; i < Yellow[j].no_of_players ; i++) {
			//Check To See If The Player Is On The Table
			//And Not To Draw Him If He Is Not
			if((Yellow[j].Players[i].x_player >= 160 ) &&
				(Yellow[j].Players[i].x_player <= 480)) {
					Yellow[j].Players[i].draw_player(Yellow[j].Players[i].x_player,
							Yellow[j].Players[i].y_player, YELLOW);
			}
		}
	}
			//Check To See If The Player Is On The Table
			//And Not To Draw Him If He Is Not
	for(j = 0; j < 4; j++){
		for(int i = 0 ; i < Red[j].no_of_players ; i++) {
			if((Red[j].Players[i].x_player >= 160 ) &&
				(Red[j].Players[i].x_player <= 480)) {
				Red[j].Players[i].draw_player(Red[j].Players[i].x_player,
						Red[j].Players[i].y_player, RED);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//			    		  REFRESH TABLE FUNCTION						//
//////////////////////////////////////////////////////////////////////////
void
Refresh_Table(void){
	setfillstyle(1, GREEN);
	bar(155, 30, 485, 450);				//Main Board (Green Part)

	setcolor(WHITE);
	setlinestyle(0, 1, 1);
	rectangle(154, 29, 486, 451);		//Inner Border
	ellipse(315, 240, 0, 360, 25, 20);	//Center Circle
	line(155, 240, 485, 240);			//Center Line
	rectangle(232, 30, 408, 125);		//Upper Outer D
	rectangle(276, 30, 364, 96);		//Upper Inner D
	rectangle(232, 355, 408, 450);		//Lower Outer D
	rectangle(276, 384, 364, 450);		//Lower Inner D

	setcolor(BLACK);
	setlinestyle(0, 1, 3);
	line(276, 450, 364, 450);			//Lower Goal
	line(276, 30, 364, 30);				//Upper Goal

	// Drawing the bars on the screen
	setfillstyle(1, DARKGRAY);
	int val = 74;
	for(int i=0 ; i<4 ; i++){
		bar(100, val , 532, val );
		val = val + 44;
	}

	val = 274;
	for(i=0 ; i<4 ; i++){
		bar(100, val , 532, val );
		val = val + 44;
	}
}

//////////////////////////////////////////////////////////////////////////
//			    				DDA FUNCTION							//
//					Digital Differential Analyzer Algorithm				//
//////////////////////////////////////////////////////////////////////////
void
DDA(int x1, int y1, int x2, int y2) {
	int Dx, Dy, LL, i;
	Dx = abs(x2 - x1);		//Find Difference In x
	Dy = abs(y2 - y1);		//Find Difference In y
	if(Dx >= Dy)			//Find Greater & make it Line Length
		LL = Dx;
	else
		LL = Dy;
	X_inc = (float)(Dx) / (float)(LL);	//Find minumum increment in x
	Y_inc = (float)(Dy) / (float)(LL);	//Find minimum increment in y
}

//////////////////////////////////////////////////////////////////////////
//			    		PLAY GOAL SEQUENCE FUNCTION						//
//////////////////////////////////////////////////////////////////////////
void
play_goal_seq(void) {
	StopSong();
	cleardevice();
	delay(300);
	while(kbhit())						// Clearing Input Stream
		getch();						//  ( Keyboard Buffer )

	char *Goal = "           !!! GOAL !!!";
	char back;
	char ShowText[13];

	cin.clear();						// Making sure its clean :)

	// Drawing the legs of the board
	setfillstyle(XHATCH_FILL, WHITE);
	bar3d(74, 227, 94, 300, 15, 0);
	bar3d(525, 227, 545, 300, 15, 0);
	setfillstyle(SOLID_FILL, BLACK);
	bar(64, 154, 566, 226);				// Clipping and fill up bar3d

	// Border for title bar
	setcolor(WHITE);
	setlinestyle(SOLID_LINE, 0, 4);
	rectangle(67, 157, 563, 223);		// Inner border
	rectangle(64, 154, 566, 226);		// Outer border

	// Drawing rectangle around the scores
	setcolor(BLUE);
	setlinestyle(0, 0, 2);
	rectangle(140, 290, 470, 420);

	// Drawing T Shirts and showing score
	Show_TShirts(180, 370, YELLOW);
	Show_TShirts(380, 370, RED);

	int COL = BLACK;
	// Drawing the flashing title and the title bar
	settextstyle(DEFAULT_FONT, 0, 6);
	CurrentSong = Notes[5];					// Loading song
	s = count = 0;							// resetting values of variables
	StartSong();
	while(!kbhit()){
		Mouse.getmousepos();				// Code to exit incase user
		if((Mouse.Button & 1) == 1) {		// clicks mouse button
			StopSong();
			return;
		}
		for(int i=0;i<13;i++){
			ShowText[i] = Goal[i];
		}
		ShowText[13] = '\0';
		back = Goal[0];
		for(i=0;i<23;i++){
			Goal[i] = Goal[i+1];
		}
		Goal[22] = back;

		outtextxy(84, 170, ShowText);
		setcolor(WHITE);
		line(566, 154, 566, 226);		// Drawing back erased line
		delay(10);
		setcolor(COL);
		outtextxy(84, 170, ShowText);
		setcolor(WHITE);
		line(566, 154, 566, 226);		// Drawing back erased line
		delay(20);
		setfillstyle(HATCH_FILL, YELLOW);
		bar(70, 160, 560, 220);
		delay(60);
		setcolor(BLACK);
	}
	StopSong();						// Ending song
	getch();
}

//////////////////////////////////////////////////////////////////////////
//			    			SHOW INTRO FUNCTION							//
//////////////////////////////////////////////////////////////////////////
void
ShowIntro(void){
	cleardevice();

	Mouse.hidemouseptr();
	settextstyle(DEFAULT_FONT, 0, 1);
	setcolor(LIGHTBLUE);
	outtextxy(300, 90, " GAURANG & SAURABH present ");

	setfillstyle(1, GREEN);
	bar(0, 70, 195, 490);				//Main Board (Green Part)

	setcolor(WHITE);
	setlinestyle(0, 1, 3);
	rectangle(-10, 69, 196, 491);		//Inner Border
	ellipse(15, 280, 0, 360, 25, 20);	//Center Circle
	line(0, 280, 195, 280);				//Center Line
	rectangle(-10, 70, 108, 165);		//Upper Outer Square D
	rectangle(-10, 60, 64, 136);		//Upper Inner Square D
	rectangle(-10, 395, 108, 490);		//Lower Outer Square D
	rectangle(-10, 424, 64, 500);		//Lower Inner Square D
	sector(0, 165, 180, 360, 77, 60);	// Upper Circular Square D
	sector(0, 395, 0, 180, 77, 60);		// Bottom Circluar Square D

	setcolor(BLACK);
	setlinestyle(0, 1, 3);
	line(0, 490, 64, 490);				//Lower Goal
	line(0, 70, 64, 70);				//Upper Goal

	//Draw Goal Nets
	setcolor(WHITE);
	setfillstyle(XHATCH_FILL, WHITE);
	rectangle(-10, 70, 64, 60);			//For YELLOW's Goal
	bar(-10, 69, 63, 61);				//
	rectangle(-10, 490, 64, 500);		//
	bar(-10, 491, 63, 499);

	// Drawing Football at screen bottom
	setfillstyle(SOLID_FILL, WHITE);
	pieslice(600, 479, 75, 180, 150);
	floodfill(639, 479, WHITE);

	// Drawing line to show speed
	setlinestyle(SOLID_LINE, 0, 2);
	line(390, 340, 500, 340);
	line(410, 360, 460, 360);
	line(300, 380, 480, 380);
	line(370, 400, 460, 400);
	line(390, 420, 450, 420);
	line(300, 430, 430, 430);
	line(350, 450, 450, 450);
	line(280, 470, 450, 470);

	// Logo on the ball
	setcolor(BLACK);
	settextstyle(DEFAULT_FONT, 0, 1);
	outtextxy(515, 420, "S O C C E R ");
	outtextxy(515, 435, "  m i n i  ");

	// Drawing Menu
	setfillstyle(SOLID_FILL, LIGHTBLUE);
	setcolor(WHITE);
	setlinestyle(SOLID_LINE, 1, 3);
	rectangle(249, 199, 421, 323);      		// Drawing the outer border
	bar(250, 200, 420, 322);					// Middle color fill
	settextstyle(DEFAULT_FONT, 0, 2);
	setcolor(BLUE);
	outtextxy(250, 215, " GAME MENU ");
	settextstyle(DEFAULT_FONT, 0, 1);
	setcolor(WHITE);
	outtextxy(275, 245, " 1 Player Game ");
	outtextxy(275, 260, " 2 Player Game ");
	outtextxy(275, 275,"Gameplay Options");
	outtextxy(275, 290, "  Instructions ");
	outtextxy(275, 305, " Quit the Game ");

	// Drawing title bar flashing text
	setfillstyle(SOLID_FILL, BLACK);
	bar(140, 110, 630, 170);

	// Border for title bar
	setlinestyle(SOLID_LINE, 0, 4);
	rectangle(139, 109, 631, 171);
	CurrentSong = Notes[7]; 			// Assigning song to be played
	s = count = 0;						// Resetting values of s & Count
	StartSong();						// Starting the song
	Mouse.showmouseptr();
	Mouse.setmousepos(335, 247);
}

//////////////////////////////////////////////////////////////////////////
//			    			SHOW MAIN MENU FUNCTION						//
//////////////////////////////////////////////////////////////////////////
void
Main_Menu(void) {
	char *Title = "          SOCCER mini";
	char back;
	char ShowText[12];
	int COL = 0;
	char c;

	// Drawing the flashing title and the title bar
	settextstyle(DEFAULT_FONT, 0, 6);
	Mouse.showmouseptr();
	while(1) {
		// Keyboard Interfacing ....
		if(kbhit()){
			c = getch();
			// Clearing all red rectangles drawn ...
			setcolor(LIGHTBLUE);
			rectangle(251, 240, 419, 255);
			rectangle(251, 255, 419, 270);
			rectangle(251, 270, 419, 285);
			rectangle(251, 285, 419, 300);
			rectangle(251, 300, 419, 315);

			if(c == ESC_KEY) {
				StopSong();				// Stopping song
				exit(0);
			}
			else if(c == '1') {			//Single Player Mode
				Mouse.hidemouseptr();
				setcolor(RED);
				rectangle(251, 240, 419, 255);
				delay(500);
				cleardevice();
				setcolor(WHITE);
				settextstyle(0, 0, 1);

				//Accept AI Difficulty Level !!!
				AI_Level = Get_AI_Level();
				Mouse.hidemouseptr();
				cleardevice();
				if(AI_Level != -1) {
					if(AI_Level < 2) {
						AI_Level = 2;
					}
					else if(AI_Level > 7) {
						AI_Level = 7;
					}
					//Go To Single Player Mode !!!
					StopSong();		// Stopping Title Song
					SetPlayerDefaults();	// Setting default positions of
					cleardevice();		    //		players
					delay(1000);	// To prevent the user from starting the
									// the game by mistake
					YELLOW_GOAL = RED_GOAL = 0;
					Draw_Table();
					GamePlay(ONE_PLAYER);
					Mouse.showmouseptr();
				}
				else {
					StopSong();
					cleardevice();
					ShowIntro();
					Mouse.showmouseptr();
				}
			}
			else if(c == '2') {
				//Go To Two Player Mode !!!
				Mouse.hidemouseptr();
				setcolor(RED);
				rectangle(251, 255, 419, 270);
				delay(500);
				StopSong();				// Stopping title track
				int i = ChooseTwoPlayerMode();
				if(i != -1) {			// ESC KEY
					if(i == 0) {		// Default Settings
						SetPlayerDefaults();
					}
					else if(i == 1) { 	// Advanced Settings
						CustomizeGame();
					}
					cleardevice();
					delay(1000); // To prevent user from starting the game
								// by mistake
					YELLOW_GOAL = RED_GOAL = 0;
					Mouse.hidemouseptr();
					Draw_Table();
					GamePlay(TWO_PLAYER);
					Mouse.showmouseptr();
					Mouse.showmouseptr();
				}
				else {
					StopSong();
					cleardevice();
					ShowIntro();
					Mouse.showmouseptr();
				}
			}
			else if(c == '3') {
				//Go To Options Screen
				Mouse.hidemouseptr();
				setcolor(RED);
				rectangle(251, 270, 419, 285);
				delay(500);
				StopSong();					// Stopping Sound
				Option_Menu();
				Mouse.showmouseptr();
			}
			else if(c == '4') {
				//Go To Instruction Screen
				Mouse.hidemouseptr();
				setcolor(RED);
				rectangle(251, 285, 419, 300);
				delay(500);
				StopSong();					// Stopping sound
				cleardevice();
				Display_Help();
				Mouse.showmouseptr();
			}
			else if(c == '5') {
				//Quit Game
				Mouse.hidemouseptr();
				setcolor(RED);
				rectangle(251, 300, 419, 315);
				delay(500);
				StopSong();				// Stopping title track
				exit(0);
			}
		}

		//Mouse Interfacing ....
		Mouse.getmousepos();
		if(Mouse.X > 250 && Mouse.X < 420) {
			// 1 Player Mode
			if(Mouse.Y > 240 && Mouse.Y < 255) {
				setcolor(LIGHTBLUE);
				rectangle(251, 255, 419, 270);
				rectangle(251, 270, 419, 285);
				rectangle(251, 285, 419, 300);
				rectangle(251, 300, 419, 315);
				Title = "      One Player Game";
				setcolor(RED);
				Mouse.hidemouseptr();
				rectangle(251, 240, 419, 255);
				Mouse.showmouseptr();
				if((Mouse.Button & 1) == 1) {
					Mouse.hidemouseptr();
					cleardevice();
					setcolor(WHITE);
					settextstyle(0, 0, 1);
					//Accept AI Difficulty Level !!!
					AI_Level = Get_AI_Level();
					Mouse.hidemouseptr();
					cleardevice();
					if(AI_Level != -1) {
						if(AI_Level < 2) {
							AI_Level = 2;
						}
						else if(AI_Level > 7) {
							AI_Level = 7;
						}
						//Go To Single Player Mode !!!
						StopSong();		// Stopping Title Song
						SetPlayerDefaults();
						cleardevice();
						delay(1000);	// To prevent the user from starting the
										// the game by mistake
						YELLOW_GOAL = RED_GOAL = 0;
						Draw_Table();
						GamePlay(ONE_PLAYER);
						Mouse.showmouseptr();
					}
					else {
						StopSong();
						cleardevice();
						ShowIntro();
						Mouse.showmouseptr();
					}
				}
			}
			// 2 Player Mode
			else if(Mouse.Y > 255 && Mouse.Y < 270) {
				setcolor(LIGHTBLUE);
				rectangle(251, 240, 419, 255);
				rectangle(251, 270, 419, 285);
				rectangle(251, 285, 419, 300);
				rectangle(251, 300, 419, 315);
				Title = "      Two Player Game";
				setcolor(RED);
				Mouse.hidemouseptr();
				rectangle(251, 255, 419, 270);
				Mouse.showmouseptr();
				if((Mouse.Button & 1) == 1) {
					//Go To Two Player Mode !!!
					Mouse.hidemouseptr();
					StopSong();		// Stopping title track
					int i = ChooseTwoPlayerMode();
					if(i != -1) {			// ESC KEY
						if(i == 0) {		// Default Settings
							SetPlayerDefaults();
						}
						else if(i == 1) { 	// Advanced Settings
							CustomizeGame();
						}
						cleardevice();
						delay(1000);// To prevent user from starting
									//	the game by mistake
						YELLOW_GOAL = RED_GOAL = 0;
						Draw_Table();
						GamePlay(TWO_PLAYER);
						Mouse.showmouseptr();
					}
					else {
						StopSong();
						cleardevice();
						ShowIntro();
						Mouse.showmouseptr();
					}
				}
			}
			// Gameplay Options
			else if(Mouse.Y > 270 && Mouse.Y < 285) {
				setcolor(LIGHTBLUE);
				rectangle(251, 240, 419, 255);
				rectangle(251, 255, 419, 270);
				rectangle(251, 285, 419, 300);
				rectangle(251, 300, 419, 315);
				Title = " Change Game Settings";
				setcolor(RED);
				Mouse.hidemouseptr();
				rectangle(251, 270, 419, 285);
				Mouse.showmouseptr();
				if((Mouse.Button & 1) == 1) {
					//Go To Options Screen
					Mouse.hidemouseptr();
					StopSong();					// Stopping Sound
					Option_Menu();
					Mouse.showmouseptr();
				}
			}
			// Instructions and help
			else if(Mouse.Y > 285 && Mouse.Y < 300) {
				setcolor(LIGHTBLUE);
				rectangle(251, 240, 419, 255);
				rectangle(251, 255, 419, 270);
				rectangle(251, 270, 419, 285);
				rectangle(251, 300, 419, 315);
				Title = "       Help & Credits";
				setcolor(RED);
				Mouse.hidemouseptr();
				rectangle(251, 285, 419, 300);
				Mouse.showmouseptr();
				if((Mouse.Button & 1) == 1) {
					//Go To Instruction Screen
					Mouse.hidemouseptr();
					StopSong();					// Stopping sound
					cleardevice();
					Display_Help();
					Mouse.showmouseptr();
				}
			}
			// Exit
			else if(Mouse.Y > 300 && Mouse.Y < 315) {
				setcolor(LIGHTBLUE);
				rectangle(251, 240, 419, 255);
				rectangle(251, 255, 419, 270);
				rectangle(251, 270, 419, 285);
				rectangle(251, 285, 419, 300);
				Title = "        Return to DOS";
				setcolor(RED);
				Mouse.hidemouseptr();
				rectangle(251, 300, 419, 315);
				Mouse.showmouseptr();
				if((Mouse.Button & 1) == 1) {
					//Quit Game
					StopSong();				// Stopping title track
					exit(0);
				}
			}
		}
		// Refresh red bars drawn earlier
		if((Mouse.Y < 200 || Mouse.Y > 320) ||
			(Mouse.X < 250 || Mouse.X > 420)) {
				setcolor(LIGHTBLUE);
				Title = "          SOCCER mini";
				rectangle(251, 240, 419, 255);
				rectangle(251, 255, 419, 270);
				rectangle(251, 270, 419, 285);
				rectangle(251, 285, 419, 300);
				rectangle(251, 300, 419, 315);
		}

		//Rotating Text
		COL = COL % 16;
		settextstyle(DEFAULT_FONT, 0, 6);
		for(int i=0 ; i<11 ; i++) {
			ShowText[i] = Title[i];
		}
		ShowText[11] = '\0';

		back = Title[0];
		for(i=0 ; i<21 ; i++) {
			Title[i] = Title[i+1];
		}
		Title[20] = back;

		outtextxy(150, 120, ShowText);
		delay(10);
		setcolor(COL);
		outtextxy(150, 120, ShowText);
		delay(20);
		setfillstyle(HATCH_FILL, WHITE);
		bar(140, 110, 630, 170);
		delay(60);
		setcolor(BLACK);
		COL ++;
	}
}

//////////////////////////////////////////////////////////////////////////
//			    			OPTION MENU FUNCTION						//
//////////////////////////////////////////////////////////////////////////
void
Option_Menu(void) {
	int Temp_CPUAssist = CPUASSIST;
	int Temp_StartSpeed = STARTSPEED;
	int Temp_MaxSpeed = MAXSPEED;
	int Temp_MinSpeed = MINSPEED;
	int Temp_Goal = MAXGOALS;
	char temp[3];
	char c;
	int SongFlag = 0;
	int Original_Song = Song_No;		//Save original song no
	player TempPlayers[5];
	cleardevice();

	// Typing out the text on screen
	setfillstyle(SOLID_FILL, LIGHTBLUE);		 // Main Fill Bar
	bar(0, 0, 530, 90);
	setcolor(BLACK);
	settextstyle(DEFAULT_FONT, 0, 2);
	outtextxy(11, 62,  " Gameplay Options ");	// Shadow Text
	setcolor(BLUE);
	outtextxy(10, 60,  " Gameplay Options ");	// Actual Text
	setfillstyle(SOLID_FILL, BLUE);
	bar(0, 90, 530, 390);						// Background bar
	setfillstyle(SOLID_FILL, CYAN);
	bar(0, 390, 530, 479);						// Last bar
	setfillstyle(SOLID_FILL, GREEN);
	bar(531, 0, 639, 479);						// Side green bar

	setcolor(WHITE);
	setlinestyle(DASHED_LINE, 0, 3);
	line(530,   0, 530, 479);					// Straight bottom line
	line(  0,  90, 529,  90);					// First Horz line
	line(  0, 390, 529, 390);					// Second Horz Line
	line(  0,   0,   0, 479);					// LHS Vert Line
	line(  0,   0, 529,   0);					// Upper Horz Line
	line(  0, 479, 529, 479);					// Botton Horz Line

	// Drawing objects on the RHS green field
	setfillstyle(XHATCH_FILL, WHITE);
	bar(531, 0, 639, 20);
	setcolor(WHITE);
	setlinestyle(SOLID_LINE, 0, 3);
	line(531, 20, 639, 20);
	TempPlayers[0].draw_player(550, 80, RED);
	TempPlayers[1].draw_player(590, 85, YELLOW);
	TempPlayers[2].draw_player(630, 130, RED);
	TempPlayers[3].draw_player(580, 220, YELLOW);
	TempPlayers[4].draw_player(540, 420, RED);
	setlinestyle(DOTTED_LINE, 0, 3);
	setcolor(WHITE);
	line(545, 414, 625, 136);
	line(635, 124, 545, 86);
	line(555, 74, 590, 21);

	settextstyle(0, 0, 1);
	outtextxy(10, 107, "Enter CPU Assist Mode ( 0 - low ... 10 - high ) : ");
	outtextxy(10, 157, "    Enter start speed of the ball ( default 3 ) : ");
	outtextxy(10, 207, "      Enter max speed of the ball ( default 6 ) : ");
	outtextxy(10, 257, "      Enter min speed of the ball ( default 2 ) : ");
	outtextxy(10, 307, "               Enter race to goal ( default 5 ) : ");
	outtextxy(10, 357, " Select Game Play Song : ");

	// Painting all the text boxes interior black
	setfillstyle(SOLID_FILL, BLACK);
	for(int i=102;i<=302;i = i+50) {
		bar(422, i, 498, (i+17));
	}
	bar(240, 350, 460, 371);				//Clear The Song Text Box

	// Filling in the default values in the textboxes ...
	settextstyle(DEFAULT_FONT, 0, 1);
	setcolor(WHITE);

	itoa(CPUASSIST, temp, 10);    			// CPUASSIST default value
	outtextxy(432, 107, temp);
	itoa(STARTSPEED, temp, 10);    			// STARTSPEED default value
	outtextxy(432, 157, temp);
	itoa(MAXSPEED, temp, 10);    			// MAXSPEED default value
	outtextxy(432, 207, temp);
	itoa(MINSPEED, temp, 10);    			// MINSPEED default value
	outtextxy(432, 257, temp);
	itoa(MAXGOALS, temp, 10);    			// MAXGOALS default value
	outtextxy(432, 307, temp);

	outtextxy(252, 357, Songs[Song_No]);	// Music to play

	setcolor(BLACK);
	outtextxy(540, 450, "S O C C E R ");
	outtextxy(540, 465, "  m i n i   ");

	setcolor(LIGHTGRAY);
	setlinestyle(SOLID_LINE, 0, 3);

	rectangle(420, 100, 500, 121);			//TextBox For CPU Assist
	rectangle(420, 150, 500, 171);			//TextBox For Start Speed
	rectangle(420, 200, 500, 221);			//TextBox For Max Speed
	rectangle(420, 250, 500, 271);			//TextBox For Min Speed
	rectangle(420, 300, 500, 321);			//TextBox For Max Goals
	rectangle(240, 350, 460, 371);			//TextBox For Song Slection

	Win_Button OK, CANCEL, Left_Song, Right_Song;
	Left_Song.draw_normal(221, 350, 236, 371, "<");
	Right_Song.draw_normal(465, 350, 480, 371, ">");
	OK.draw_normal(400, 400, 500, 440, "OK");
	CANCEL.draw_normal(200, 400, 300, 440, "CANCEL");
	Mouse.showmouseptr();
	while(1) {
		if(kbhit()) {
			c = getch();
			if(c == ESC_KEY) {
				Mouse.hidemouseptr();
				cleardevice();
				while(kbhit()) {				// Clearing keybd buffer
					getch();
				}
				if(SongFlag == 1) {
					StopSong();
				}
				Song_No = Original_Song;		//Restore Original Song No
				ShowIntro();
				return;
			}
		}
		Mouse.getmousepos();
		if((Mouse.Button & 1) == 1) {
			if(Mouse.X > 420 && Mouse.Y > 100 &&	//TextBox For CPU Assist
				Mouse.X < 500 && Mouse.Y < 121) {
				setfillstyle(1, BLACK);
				Mouse.hidemouseptr();
				bar(422, 102, 498, 118);			//Clear Text Box
				Temp_CPUAssist = Graphics_Accept(432, 117);	//Accept New Values
				if(Temp_CPUAssist == 0) {			// No value entered
					setcolor(WHITE);
					bar(422, 102, 498, 118);   //Clear Text Box
					itoa(0, temp, 10);
					outtextxy(432, 107, temp);
				}
				else if(Temp_CPUAssist == -1) { // User has hit ESC Key
					bar(422, 102, 498, 118);   //Clear Text Box
					itoa(CPUASSIST, temp, 10);
					setcolor(WHITE);
					outtextxy(432, 107, temp);
				}
				Mouse.showmouseptr();
			}
			else if(Mouse.X > 430 && Mouse.Y > 150 &&//TextBox For Start Speed
					Mouse.X < 500 && Mouse.Y < 171) {
					setfillstyle(1, BLACK);
					Mouse.hidemouseptr();
					bar(432, 152, 498, 168);        //Clear Text Box
					Temp_StartSpeed = Graphics_Accept(432, 167);
					if(Temp_StartSpeed == 0 || Temp_StartSpeed == -1) {
													// No value entered
						bar(432, 152, 498, 168);    //Clear Text Box
						itoa(STARTSPEED, temp, 10); // Display default
						setcolor(WHITE);			// value
						outtextxy(432, 157, temp);
					}
					Mouse.showmouseptr();
			}
			else if(Mouse.X > 430 && Mouse.Y > 200 &&//TextBox For Max Speed
					Mouse.X < 500 && Mouse.Y < 221) {
					setfillstyle(1, BLACK);
					Mouse.hidemouseptr();
					bar(432, 202, 498, 218);        //Clear Text Box					bar(432, 202, 498, 218);		// Clear textbox
					Temp_MaxSpeed = Graphics_Accept(432, 217);
					if(Temp_MaxSpeed == 0 || Temp_MaxSpeed == -1) {
													// No value entered
						bar(432, 202, 498, 218);    //Clear Text Box
						itoa(MAXSPEED, temp, 10);   // Display default
						setcolor(WHITE);			// value
						outtextxy(432, 207, temp);
					}
					Mouse.showmouseptr();
			}
			else if(Mouse.X > 430 && Mouse.Y > 250 &&//TextBox For Min Speed
					Mouse.X < 500 && Mouse.Y < 271) {
					setfillstyle(1, BLACK);
					Mouse.hidemouseptr();
					bar(432, 252, 498, 268);        //Clear Text Box					bar(432, 252, 498, 268);		//Clear Text Box
					Temp_MinSpeed = Graphics_Accept(432, 267);
					if(Temp_MinSpeed == 0 || Temp_MinSpeed == -1) {
													// No value entered
						bar(432, 252, 498, 268);    //Clear Text Box
						itoa(MINSPEED, temp, 10);   // Display default
						setcolor(WHITE);			// value
						outtextxy(432, 257, temp);
					}
					Mouse.showmouseptr();
			}
			else if(Mouse.X > 430 && Mouse.Y > 300 &&//TextBox For Max Goals
					Mouse.X < 500 && Mouse.Y < 321) {
					setfillstyle(1, BLACK);
					Mouse.hidemouseptr();
					bar(432, 302, 498, 318);        //Clear Text Box					bar(432, 302, 498, 318);		//Clear Text Box
					Temp_Goal = Graphics_Accept(432, 317);
					if(Temp_Goal == 0 || Temp_Goal == -1) {
													// No value entered
						bar(432, 302, 498, 318);    //Clear Text Box
						itoa(MAXGOALS, temp, 10);   // Display default
						setcolor(WHITE);			// value
						outtextxy(432, 307, temp);
					}
					Mouse.showmouseptr();
			}
			else if(Mouse.X > 400 && Mouse.Y > 400 &&	//OK BUTTON
					Mouse.X < 500 && Mouse.Y < 440) {
						Mouse.hidemouseptr();
						OK.draw_pressed();
						Mouse.showmouseptr();
					while((Mouse.Button&1) == 1) {
						Mouse.getmousepos();
					}
					OK.draw_unpressed();
					Mouse.getmousepos();	// Checking to see if mouse is
											// still on the button

				   if(Mouse.X > 400 && Mouse.Y > 400 &&
					Mouse.X < 500 && Mouse.Y < 440) {

						if(Temp_CPUAssist >= 0 && Temp_CPUAssist < 11) {
							CPUASSIST = Temp_CPUAssist;
						}
						else if(Temp_CPUAssist > 10) {
							CPUASSIST = 10;
						}

						if(Temp_StartSpeed > 0 && Temp_StartSpeed <= 15) {
							STARTSPEED = Temp_StartSpeed;
						}


						if(Temp_MaxSpeed > 0 && Temp_MaxSpeed <= 15) {
							MAXSPEED = Temp_MaxSpeed;
						}

						if(Temp_MinSpeed > 0 && Temp_MinSpeed <= Temp_MaxSpeed) {
							MINSPEED = Temp_MinSpeed;
						}

						if(Temp_Goal > 0) {
							MAXGOALS = Temp_Goal;
						}
						else {
							MAXGOALS = 1;
						}
						Mouse.hidemouseptr();
						if(SongFlag == 1) {
							StopSong();
						}
						cleardevice();
						ShowIntro();
						return;
				   }
			}
			else if(Mouse.X > 200 && Mouse.Y > 400 &&	//CANCEL BUTTON
					Mouse.X < 300 && Mouse.Y < 440) {
					Mouse.hidemouseptr();
					CANCEL.draw_pressed();
					Mouse.showmouseptr();
					while((Mouse.Button&1) == 1) {
						Mouse.getmousepos();
					}
					CANCEL.draw_unpressed();
					Mouse.getmousepos();	// Checking to see if mouse is
											// still on the cancel button
					if(Mouse.X > 200 && Mouse.Y > 400 &&
					Mouse.X < 300 && Mouse.Y < 440) {
						Mouse.hidemouseptr();
						if(SongFlag == 1) {
							StopSong();
						}
						Song_No = Original_Song;
						cleardevice();
						ShowIntro();
						return;
					}
			}
			// Code for the left Song button press
			else if(Mouse.X > 221 && Mouse.Y > 350 &&
					Mouse.X < 236 && Mouse.Y < 371) {	//Left Song Button
					Mouse.hidemouseptr();
					Left_Song.draw_pressed();
					Mouse.showmouseptr();
					while((Mouse.Button&1) == 1) {
						Mouse.getmousepos();
					}
					Mouse.hidemouseptr();
					Left_Song.draw_unpressed();
					Mouse.showmouseptr();
					Mouse.getmousepos();	// Checking to see if mouse is
											// still on the cancel button
					if(Mouse.X > 221 && Mouse.Y > 350 &&
					Mouse.X < 236 && Mouse.Y < 371) {
						if(Song_No > 0) {
							Song_No --;
						}
						else {
							Song_No = 10;
						}
						setfillstyle(SOLID_FILL, BLACK);
						bar(242, 352, 458, 369);	//Clear The Song Text Box
						settextstyle(0, 0, 1);
						setcolor(WHITE);
						outtextxy(252, 357, Songs[Song_No]);
						if(SongFlag == 1) {
							StopSong();
						}
						CurrentSong = Notes[Song_No];	//Set Song Notes
						s = 0;						  	//Reset Array Pointer
						StartSong();
						SongFlag = 1;
					}
			}
			// Code for the Right Song Button Press
			else if(Mouse.X > 465 && Mouse.Y > 350 &&
			Mouse.X < 480 && Mouse.Y < 371) {	//Right Song Button
					Mouse.hidemouseptr();
					Right_Song.draw_pressed();
					Mouse.showmouseptr();
					while((Mouse.Button&1) == 1) {
						Mouse.getmousepos();
					}
					Mouse.hidemouseptr();
					Right_Song.draw_unpressed();
					Mouse.showmouseptr();
					Mouse.getmousepos();	// Checking to see if mouse is
										// still on the cancel button
					if(Mouse.X > 465 && Mouse.Y > 350 &&
					Mouse.X < 480 && Mouse.Y < 371) {
						if(Song_No < 10) {
							Song_No++;
						}
						else {
							Song_No = 0;
						}
						setfillstyle(SOLID_FILL, BLACK);
						bar(242, 352, 458, 369);	//Clear The Song Text Box
						settextstyle(0, 0, 1);
						setcolor(WHITE);
						outtextxy(252, 357, Songs[Song_No]);
						if(SongFlag == 1) {
							StopSong();
						}
						CurrentSong = Notes[Song_No];	//Set Song Notes
						s = 0;							//Reset Array Pointer
						StartSong();
						SongFlag = 1;
					}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//			    			TSHIRT DRAWING FUNCTION						//
//////////////////////////////////////////////////////////////////////////
void
Show_TShirts(int X, int Y, int COL){
	//Draw T-Shirts For Each Team And Display Score
	setlinestyle(0, 0, 1);
	settextstyle(DEFAULT_FONT, 0, 3);
	char score[3];

	setcolor(COL);					//Drawing Shirts For YELLOW Team
	setfillstyle(SOLID_FILL, COL);	//
	moveto(X, Y);					//
	linerel(0, -40);				//
	linerel(-10, 10);				//        ____  ____
	linerel(-10, -10);				//		 /    \/    \
	linerel(15, -15);				//	   /  /|      |\  \
	linerel(20, 0);		  			//	   \/  |      |  \/
	linerel(5, 5);					//         |      |
	linerel(5, -5);					//         |      |
	linerel(20, 0);					//         |______|
	linerel(15, 15);				//      (x, y)
	linerel(-10, 10);				//
	linerel(-10, -10);				//
	linerel(0, 40);					//
	linerel(-40, 0);				//
	floodfill(X+10, Y-10, COL);
	if(COL == YELLOW) {
		itoa(YELLOW_GOAL, score, 10);
	}
	else {
		itoa(RED_GOAL, score, 10);
	}
	outtextxy(X+10, Y+10, score);
}

//////////////////////////////////////////////////////////////////////////
//			    	  INSTRUCTIONS DISPLAY FUNCTION						//
//////////////////////////////////////////////////////////////////////////
void
Display_Help(void){
	int NextPage = 0;

	cleardevice();
	setfillstyle(SOLID_FILL, BLACK);		// The two left & right margins
	bar(0, 0, 60, 479);						// green margins
	bar(580, 0, 639, 479);
	setfillstyle(SOLID_FILL, GREEN);     	// Middle dark blue bands
	bar(60, 0, 580, 90);
	bar(60, 205, 580, 235);
	bar(60, 405, 580, 479);
	setfillstyle(SOLID_FILL, LIGHTBLUE);	// Light blue bands having text
	bar(60, 90, 580, 205);
	bar(60, 235, 580, 405);

	setlinestyle(USERBIT_LINE, 511, 3);
	setcolor(WHITE);
	line(59, 0, 59, 479); 					// Left margin line
	line(581, 0, 581, 479);					// Right margin line
	line(60, 90, 580, 90);					// First line
	line(60, 206, 580, 206);				// Second line
	line(60, 235, 580, 235);				// Third line
	line(60, 406, 580, 406);				// Fourth line

	// THE INSTRUCTIONS PAGE

	settextstyle(DEFAULT_FONT, 0, 3);
	setcolor(BLACK);
	outtextxy(45, 32, " Gameplay Instructions ");	// Double imposing
	setcolor(WHITE);								// text to get shadow
	outtextxy(43, 30, " Gameplay Instructions ");	// effect
	setcolor(WHITE);

	settextstyle(DEFAULT_FONT, 0, 1);
	setcolor(BLACK);
	outtextxy(231, 101, " Single Player Mode ");
	setcolor(WHITE);
	outtextxy(230, 100, " Single Player Mode ");

	settextstyle(DEFAULT_FONT, 0, 1);
	setcolor(WHITE);
	outtextxy(65, 120, " Team : ");
	setcolor(BLACK);
	outtextxy(131, 122, "RED");
	setcolor(WHITE);
	outtextxy(130, 121, "RED");
	outtextxy(65, 135, " Input Device : 2 button mouse ");
	outtextxy(65, 160, " In this mode, the user plays Vs the computer (YELLOW Team)");
	outtextxy(65, 175, " Use the left mouse button to move the players to the left and");
	outtextxy(65, 190, " the right mouse button to move the players to the right.");

	settextstyle(DEFAULT_FONT, 0, 1);
	setcolor(BLACK);
	outtextxy(231, 241, " Two Player Mode ");
	setcolor(WHITE);
	outtextxy(230, 240, " Two Player Mode ");
	settextstyle(DEFAULT_FONT, 0, 1);
	setcolor(WHITE);
	outtextxy(65, 265, " Input Device : 2 button mouse (RED) and Keyboard (YELLOW)");
	outtextxy(65, 285, " In this mode, two players play Vs each other");
	setcolor(BLACK);
	outtextxy(66, 311, " TEAM RED : ");
	setcolor(WHITE);
	outtextxy(65, 310, " TEAM RED : ");
	outtextxy(65, 325, " Use the left mouse button to move the players to the left and");
	outtextxy(65, 340, " the right mouse button to move the players to the right.");
	setcolor(BLACK);
	outtextxy(66, 361, " TEAM YELLOW : ");
	setcolor(WHITE);
	outtextxy(65, 360, " TEAM YELLOW : ");
	outtextxy(65, 375, " Use the 'a' key to move the players to the left");
	outtextxy(65, 390, " the 's' key to move the players to the right.");

	Mouse.showmouseptr();

	Win_Button NEXT;
	NEXT.draw_normal(450, 440, 550, 470, "Next >>");
	while(!kbhit()) {
		Mouse.getmousepos();
		if(NextPage == 1) {
			break;
		}
		if(Mouse.X > 450 && Mouse.X < 550 && Mouse.Y > 440 && Mouse.Y < 470) {
			if((Mouse.Button & 1) == 1) {
				Mouse.hidemouseptr();
				NEXT.draw_pressed();
				Mouse.showmouseptr();
				while(Mouse.Button & 1 == 1) {
					Mouse.getmousepos();
				}
				Mouse.hidemouseptr();
				NEXT.draw_unpressed();
				Mouse.showmouseptr();
				if(((Mouse.Button & 1) == 0) &&
				(Mouse.X > 450 && Mouse.X < 550 && Mouse.Y > 440 && Mouse.Y < 470)) {
					Mouse.hidemouseptr();
					NextPage = 1;
				}
			}
		}
	}
	cleardevice();

	// THE CREDITS PAGE

	while(kbhit())
		getch();
	setfillstyle(SOLID_FILL, LIGHTBLUE);		// Background Color
	bar(0, 0, 639, 479);
	setfillstyle(SOLID_FILL, BLUE);				// Title bar
	bar(0, 0, 639, 60);
	setfillstyle(SOLID_FILL, GREEN);			// Left border
	bar(0, 60, 130, 479);

	setcolor(WHITE);
	setlinestyle(USERBIT_LINE, 511, 3);			// Left Line
	line(130, 60, 130, 479);
	setlinestyle(USERBIT_LINE, 511, 3);			// Top Line
	line(0, 60, 639, 60);

	settextstyle(DEFAULT_FONT, 0, 2);
	setcolor(BLACK);
	outtextxy(10, 32, "Credits");
	setcolor(WHITE);
	outtextxy(8, 30, "Credits");

	settextstyle(DEFAULT_FONT, 0, 1);
	setcolor(BLACK);
	outtextxy(6, 81, "Programming");
	setcolor(WHITE);
	outtextxy(5, 80, "Programming");
	outtextxy(140, 80, "Saurabh Jain");
	outtextxy(140, 95, "Gaurang Sinha");
	outtextxy(140, 110, "( M.E.S College of Engineering, Pune )");

	setcolor(BLACK);
	outtextxy(6, 161, "Special Thanks");
	setcolor(WHITE);
	outtextxy(5, 160, "Special Thanks");
	outtextxy(140, 160, "All the teachers at the Dept. of Computer Engg., MESCOE");

	setcolor(BLACK);
	outtextxy(6, 211, "Beta Testing");
	setcolor(WHITE);
	outtextxy(5, 210, "Beta Testing");
	outtextxy(140, 210, "Darkcloud - Rishi Agarwal");
	outtextxy(140, 225, "Godfather - Nithin Satyamoorthy");
	outtextxy(140, 240, "     MakD - Mayank Tripathi");
	outtextxy(140, 255, "       VJ -  Jomy John");
	outtextxy(140, 270, "     Dena - Abhishek Venkatkrishnan");
	outtextxy(140, 285, "And all the other people who we are happily forgetting :)");

	setcolor(BLACK);
	outtextxy(6, 336, "Dedications");
	setcolor(WHITE);
	outtextxy(5, 335, "Dedications");
	outtextxy(140, 335, "Google : For the SeArCh and the mAiL");

	Win_Button OK;
	OK.draw_normal(500, 410, 600, 440, "OK");
	Mouse.showmouseptr();
	while(!kbhit()) {
		Mouse.getmousepos();
		if(Mouse.X > 500 && Mouse.X < 600 && Mouse.Y > 410 && Mouse.Y < 440) {
			if((Mouse.Button & 1) == 1) {
				Mouse.hidemouseptr();
				OK.draw_pressed();
				Mouse.showmouseptr();
				while(Mouse.Button & 1 == 1) {
					Mouse.getmousepos();
				}
				Mouse.hidemouseptr();
				OK.draw_unpressed();
				Mouse.showmouseptr();
				if(((Mouse.Button & 1) == 0) &&
				(Mouse.X > 500 && Mouse.X < 600 && Mouse.Y > 410 && Mouse.Y < 440)) {
					Mouse.hidemouseptr();
					ShowIntro();
					return;
				}
			}
		}
	}
	Mouse.hidemouseptr();
	ShowIntro();
	return;
}

//////////////////////////////////////////////////////////////////////////
//			    		GAME OVER SCREEN DISPLAY 						//
//////////////////////////////////////////////////////////////////////////
void
Game_Over_Screen(void) {
	StopSong();
	cleardevice();
	delay(500);							//Half sec delay to avoid wrong
	char *Game = "GAME OVER";			//click
	count = s = 0;
	CurrentSong = Notes[1];
	StartSong();
	setcolor(WHITE);
	setfillstyle(XHATCH_FILL, WHITE);
	bar3d(74, 227, 94, 300, 15, 0);		// First Leg
	bar3d(505, 227, 525, 300, 15, 0);	// Second Leg
	setfillstyle(SOLID_FILL, BLACK);
	bar(64, 154, 546, 226);				// Clipping and fill up bar3d

	// Border for title bar
	setcolor(WHITE);
	setlinestyle(SOLID_LINE, 0, 4);
	rectangle(67, 157, 543, 223);		// Inner border
	rectangle(64, 154, 546, 226);		// Outer border

	setcolor(BLUE);
	setlinestyle(0, 0, 2);
	rectangle(140, 290, 470, 420);
	Show_TShirts(180, 370, YELLOW);
	Show_TShirts(380, 370, RED);
	settextstyle(DEFAULT_FONT, 0, 2);
	outtextxy(200, 250, " Final Scores ");

	int COL = BLACK;
	// Drawing the flashing title and the title bar
	settextstyle(DEFAULT_FONT, 0, 6);
	while(!kbhit()){
		Mouse.getmousepos();
		if((Mouse.Button & 1) == 1) {
			StopSong();
			return;
		}
		outtextxy(84, 170, Game);
		setcolor(WHITE);
		delay(10);
		setcolor(COL);
		outtextxy(84, 170, Game);
		setcolor(WHITE);
		delay(20);
		setfillstyle(HATCH_FILL, YELLOW);
		bar(70, 160, 540, 220);
		delay(60);
		setcolor(BLACK);
	}
	getch();
	Mouse.showmouseptr();
	StopSong();
}

//////////////////////////////////////////////////////////////////////////
//			    			COMPUTER AI			 						//
//////////////////////////////////////////////////////////////////////////
void
Computer_AI(void) {
	int Value=0;		//Contains Displacement Value After Calculation
	int New_State;		//State Of The Player After Calculation
	int Stick_No;		//Stick On Which Calculation To Be Performed
	int Player_No;		//Player On Which Calculation To Be Performed
	if(Ball.y_pos >= 318) {				//Check If Striker
		Stick_No = 3;
		Player_No = 1;
	}
	else if(Ball.y_pos >= 206) {		//Check If Mid-Field
		Stick_No = 2;
		Player_No = 2;
	}
	else if(Ball.y_pos >  50) {			//Check If Goaly Or Defenders
		Stick_No = 0;
		Player_No = 0;
	}

	//
	//	New AI with foresight ...
	//
	float X_Cross = Ball.x_pos;			//Current X Position for Ball
	//Stores The Distance Between Stick and ball
	float Count = fabs(Ball.y_pos - Yellow[Stick_No].Players[Player_No].y_player);
	while(Count > 5) {					//While ball has not reached the stick
		Count = Count - Y_inc;
		if(Ball.side == LEFT) {			//Increment Or Decrement According
			X_Cross -= (X_inc);			// to side
		}								//
		else{							//
			X_Cross += (X_inc);			//
		}
	}
	//If The Player Is At The Point Of Intersection
	if(((X_Cross+(5*(7-AI_Level))) >= Yellow[Stick_No].Players[Player_No].x_player)&&
		((X_Cross-(5*(7-AI_Level))) <= Yellow[Stick_No].Players[Player_No].x_player)) {
		Value = 0;
		New_State = NORMAL;
	}
	//Else if the player is to the LEFT of the Point
	else if(X_Cross > Yellow[Stick_No].Players[Player_No].x_player) {
		Value = AI_Level;
		New_State = RIGHT;
	}
	//Else if the player is to the RIGHT of the Point
	else if(X_Cross < Yellow[Stick_No].Players[Player_No].x_player) {
		Value = -AI_Level;
		New_State = LEFT;
	}

	//Update YELLOW Player Positions
	for(int i=0 ; i<4 ; i++) {
		for(int j=0 ; j<Yellow[i].no_of_players ; j++) {
			if(Value > 0) {				//Check For Right Boundary
				if(Yellow[0].Players[0].x_player <= 403) {
					Yellow[i].Players[j].state = New_State; //Put State
					Yellow[i].Players[j].x_player += Value;	//Put Position
				}
			}
			else {						//Check For Left Boundary
				if(Yellow[0].Players[0].x_player >= 237) {
					Yellow[i].Players[j].state = New_State;	//Put State
					Yellow[i].Players[j].x_player += Value;	//
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//			       BUTTON : DRAW NORMAL BUTTON							//
//////////////////////////////////////////////////////////////////////////
void
_Button :: draw_normal(int xx1, int yy1, int xx2, int yy2, char *str) {
	int lngth=0;
	x1 = xx1;						//Copy All Values To Class
	y1 = yy1;						// Variables
	x2 = xx2;                       //
	y2 = yy2;						//
	strcpy(caption, str);			//

	setlinestyle(0,1,1);
	lngth = strlen(caption) * 4;	//Calculate Length Of Strings In Pixels
	X_text = (x2 - x1) / 2 + x1 - lngth;	//Calculate X Offset
	Y_text = (y2 - y1) / 2 + y1;			//Calculate Y Offset
	draw_unpressed();						//Draw A Normal Button
	settextstyle(0, 0, 1);					//Set Text Font & Size
	outtextxy(X_text, Y_text, caption);		//Write Text Onto Button
}

//////////////////////////////////////////////////////////////////////////
//			       BUTTON : DRAW UNPRESSED BUTTON						//
//////////////////////////////////////////////////////////////////////////
void
_Button :: draw_unpressed(void) {
	setlinestyle(SOLID_LINE, 1, 1);			//
	setfillstyle(1, LIGHTGRAY);				//
	bar(x1, y1, x2, y2);					//Draw The Base Of The Button
	setcolor(WHITE);
	line(x1, y1, x2, y1);					//Draw A White Line Above & Left
	line(x1, y1, x1, y2);					//
	setcolor(BLACK);
	line(x1, y2, x2, y2);                   //Draw A Black Line Down & Right
	line(x2, y1, x2, y2);					//
	outtextxy(X_text, Y_text, caption);		//Write Text Onto Button
}

//////////////////////////////////////////////////////////////////////////
//			       BUTTON : DRAW PRESSED BUTTON							//
//////////////////////////////////////////////////////////////////////////
void
_Button :: draw_pressed(void) {
	setlinestyle(SOLID_LINE, 1, 1);
	setfillstyle(SOLID_FILL, LIGHTGRAY);
	bar(x1, y1, x2, y2);					//Draw base Of The Button
	setcolor(BLACK);
	line(x1, y1, x2, y1);					//Draw A Black Line Up & Right
	line(x1, y1, x1, y2);					//
	setcolor(WHITE);
	line(x1, y2, x2, y2);					//Draw A White Line Down & Left
	line(x2, y1, x2, y2);					//
	outtextxy(X_text, Y_text, caption);		//Write Text Onto Button
}

//////////////////////////////////////////////////////////////////////////
//			    		GRAPHICS ACCEPT FUNCTION						//
//////////////////////////////////////////////////////////////////////////
int
Graphics_Accept(int x, int y) {
	char c=0, temp[2], ans[5]={'\0'};
	int ln=0;
	settextstyle(0, 0, 1);
	setcolor(WHITE);
	setlinestyle(0, 0, 1);
	while((c != 13)&&(ln < 4)) {		//While Not Enter
		setcolor(WHITE);
		line(x, y, x, y-10);				//Draw A Cursor
		if(kbhit()) {						//Check for key press
			c = getch();					//get the key value
			if(c == 13) {					//If Return <-'
				setcolor(BLACK);
				line(x, y, x, y-10);		//Clear the cursor
				return (atoi(ans));			//Return Integer Value
			}
			else if(c == ESC_KEY) {
				setcolor(BLACK);
				line(x, y, x, y-10);
				return (-1);				//Return Integer Value
			}
			else if(c == 8) {				//If BackSpace <-
				if(ln != 0) {
					ans[ln-1] = '\0';		//put NULL at previous location
					setcolor(BLACK);
					line(x, y, x, y-10);	//Clear the cursor
					setfillstyle(1, BLACK);	//
					bar(x-7, y-10, x+7, y);	//Clear The Character
					x = x - 7;				//Move cursor back
					ln--;					//decrement count
				}
			}
			else {
				temp[0] = c;				//make character to string
				temp[1] = '\0';				//
				setcolor(WHITE);
				outtextxy(x, y-10, temp);	//display it on screen
				ans[ln] = c;				//put it in the array
				ln++;						//increment count
				setcolor(BLACK);
				line(x, y, x, y-10);		//clear cursor
				x = x + 7;					//Move cursor in front
			}
		}
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
//		   	FUNCTION TO GET AI LEVEL OF CPU IN ONE PLAYER MODE			//
//////////////////////////////////////////////////////////////////////////
int
Get_AI_Level(void) {
	char *Desc;
	char c;

	cleardevice();
	delay(1000);				// One sec delay to avoid wrong click

	// Drawing Menu
	setfillstyle(SOLID_FILL, GREEN);
	setcolor(WHITE);
	setlinestyle(SOLID_LINE, 1, 3);
	rectangle(199, 99, 401, 291);      		// Drawing the outer border
	bar(200, 100, 400, 290);				// Middle color fill
	settextstyle(DEFAULT_FONT, 0, 2);
	setcolor(BLACK);
	outtextxy(212, 117, " Choose AI ");
	outtextxy(212, 142, "   level   ");
	setcolor(WHITE);
	outtextxy(210, 115, " Choose AI ");
	outtextxy(210, 140, "   level   ");
	settextstyle(DEFAULT_FONT, 0, 1);
	setcolor(WHITE);
	outtextxy(225, 195, " The Warmup Games  ");
	outtextxy(225, 210, "   The Playoffs    ");
	outtextxy(225, 225, "  The Real Thing   ");
	outtextxy(225, 240, "The Premier Leagues");
	outtextxy(225, 255, "   In your Face    ");
	outtextxy(225, 270, "     Dream On      ");

	// The description box
	setfillstyle(SOLID_FILL, BLUE);
	bar(50, 350, 530, 400);
	setcolor(WHITE);
	setlinestyle(SOLID_LINE, 0, 3);
	rectangle(49, 349, 531, 401);
	setcolor(LIGHTBLUE);
	rectangle(50, 350, 532, 402);

	setlinestyle(SOLID_LINE, 0, 3);
	Mouse.showmouseptr();
	while(1) {
		if(kbhit()){
			c = getch();
			// Clearing all the green rectangles drawn ...
			setcolor(GREEN);
			rectangle(201, 190, 399, 205);
			rectangle(201, 205, 399, 220);
			rectangle(201, 220, 399, 235);
			rectangle(201, 235, 399, 250);
			rectangle(201, 250, 399, 265);
			rectangle(201, 265, 399, 280);

			// Level 1
			if(c == ESC_KEY) {				// Go back to main menu
				StopSong();
				return -1;
			}
			else if(c == '1') {				//Single Player Mode
				setcolor(RED);
				Mouse.hidemouseptr();
				rectangle(201, 190, 399, 205);
				delay(500);
				Mouse.showmouseptr();
				return 2;
			}
			// Level 2
			else if(c == '2') {
				setcolor(RED);
				Mouse.hidemouseptr();
				rectangle(201, 205, 399, 220);
				delay(500);
				Mouse.showmouseptr();
				return 3;
			}
			// Level 3
			else if(c == '3') {
				setcolor(RED);
				Mouse.hidemouseptr();
				rectangle(201, 220, 399, 235);
				delay(500);
				Mouse.showmouseptr();
				return 4;
			}
			// Level 4
			else if(c == '4') {
				setcolor(RED);
				Mouse.hidemouseptr();
				rectangle(201, 235, 399, 250);
				delay(500);
				Mouse.showmouseptr();
				return 5;
			}
			// Level 5
			else if(c == '5') {
				setcolor(RED);
				Mouse.hidemouseptr();
				rectangle(201, 250, 399, 265);
				delay(500);
				Mouse.showmouseptr();
				return 6;
			}
			// Level 6
			else if(c == '6') {
				setcolor(RED);
				Mouse.hidemouseptr();
				rectangle(201, 265, 399, 280);
				delay(500);
				Mouse.showmouseptr();
				return 7;
			}
		}

		Mouse.getmousepos();
		// Refresh red bars of menu
		if(Mouse.X < 200 || Mouse.X > 400 ||
			Mouse.Y < 190 || Mouse.Y > 280) {
			Desc = " Choose a level of difficulty ";
			setcolor(GREEN);
			Mouse.hidemouseptr();
			rectangle(201, 190, 399, 205);
			rectangle(201, 205, 399, 220);
			rectangle(201, 220, 399, 235);
			rectangle(201, 235, 399, 250);
			rectangle(201, 250, 399, 265);
			rectangle(201, 265, 399, 280);
			Mouse.showmouseptr();
		}

		if(Mouse.X > 200 && Mouse.X < 400) {
			// Warmup Games - Level = 2
			if(Mouse.Y > 190 && Mouse.Y < 205) {
				setcolor(GREEN);
				rectangle(201, 208, 399, 220);
				rectangle(201, 220, 399, 235);
				rectangle(201, 235, 399, 250);
				rectangle(201, 250, 399, 265);
				rectangle(201, 265, 399, 280);

				Desc = " Win Everytime ...";
				setcolor(RED);
				Mouse.hidemouseptr();
				rectangle(201, 190, 399, 205);
				Mouse.showmouseptr();
				if((Mouse.Button & 1) == 1) {
					return 2;
				}
			}
			// The playoff games - Level = 3
			if(Mouse.Y > 205 && Mouse.Y < 220) {
				setcolor(GREEN);
				rectangle(201, 190, 399, 205);
				rectangle(201, 220, 399, 235);
				rectangle(201, 235, 399, 250);
				rectangle(201, 250, 399, 265);
				rectangle(201, 265, 399, 280);

				Desc = " Some competition, but not enough !";
				setcolor(RED);
				Mouse.hidemouseptr();
				rectangle(201, 205, 399, 220);
				Mouse.showmouseptr();
				if((Mouse.Button & 1) == 1) {
					return 3;
				}
			}

			// Normal Mode - Level = 4
			else if(Mouse.Y > 220 && Mouse.Y < 235) {
				setcolor(GREEN);
				rectangle(201, 190, 399, 205);
				rectangle(201, 205, 399, 220);
				rectangle(201, 235, 399, 250);
				rectangle(201, 250, 399, 265);
				rectangle(201, 265, 399, 280);

				Desc = " The way its meant to be played ...";
				setcolor(RED);
				Mouse.hidemouseptr();
				rectangle(201, 220, 399, 235);
				Mouse.showmouseptr();
				if((Mouse.Button & 1) == 1) {
					return 4;
				}
			}
			// The Premier Leagues - Level = 5
			else if(Mouse.Y > 235 && Mouse.Y < 250) {
				setcolor(GREEN);
				rectangle(201, 190, 399, 205);
				rectangle(201, 205, 399, 220);
				rectangle(201, 220, 399, 235);
				rectangle(201, 250, 399, 265);
				rectangle(201, 265, 399, 280);

				Desc = " Step up ... play with the big boys ";
				setcolor(RED);
				Mouse.hidemouseptr();
				rectangle(201, 235, 399, 250);
				Mouse.showmouseptr();
				if((Mouse.Button & 1) == 1) {
					return 5;
				}
			}
			// In your face - Level = 6
			else if(Mouse.Y > 250 && Mouse.Y < 265) {
				setcolor(GREEN);
				rectangle(201, 190, 399, 205);
				rectangle(201, 205, 399, 220);
				rectangle(201, 220, 399, 235);
				rectangle(201, 235, 399, 250);
				rectangle(201, 265, 399, 280);

				Desc = " Try and get the ball to the other end ...";
				setcolor(RED);
				Mouse.hidemouseptr();
				rectangle(201, 250, 399, 265);
				Mouse.showmouseptr();
				if((Mouse.Button & 1) == 1) {
					return 6;
				}
			}
			// Dream On - Level = 7
			else if(Mouse.Y > 265 && Mouse.Y < 280) {
				setcolor(GREEN);
				rectangle(201, 190, 399, 205);
				rectangle(201, 205, 399, 220);
				rectangle(201, 220, 399, 235);
				rectangle(201, 235, 399, 250);
				rectangle(201, 250, 399, 265);

				Desc = " The Texas Chainsaw Massacre ... up close and personal !";
				setcolor(RED);
				Mouse.hidemouseptr();
				rectangle(201, 265, 399, 280);
				Mouse.showmouseptr();
				if((Mouse.Button & 1) == 1) {
					return 7;
				}
			}
		}
		while(inportb(0x3da) & 8);
		while(!(inportb(0x3da) & 8));
		setfillstyle(SOLID_FILL, BLUE);
		bar(60, 355, 525, 395);
		setcolor(WHITE);
		outtextxy(60, 372, Desc);
	}
}

//////////////////////////////////////////////////////////////////////////
//		   		FUNCTION TO GET ADVANCED PLAYER POSITIONS				//
//////////////////////////////////////////////////////////////////////////
void
CustomizeGame(void) {
	int Temp_Red[4], Red_Sum = 0;
	int Temp_Yellow[4], Yellow_Sum = 0;
	int Complete = 0;
	char Ans;

	YPlayers[0] = 1;
	RPlayers[0] = 1;
	Temp_Red[0] = Temp_Yellow[0] = 1;		// Only 1 goalie allowed
	Mouse.hidemouseptr();
	while(Complete != 1) {
		clearviewport();

		cout<<"\n\n\t Advanced Options for re-aligining Players on Board \n\n"
			<<" Maximum no. of players per team is 10 + 1"
			<<"\n However, a team CAN have less than 10 players \n\n";
		cout<<" Enter number of players for Yellow Defence ( max 5 ) : ";
		cin>>Temp_Yellow[1];
		cout<<" Enter number of players for Yellow Midfield ( max 5 ) : ";
		cin>>Temp_Yellow[2];
		cout<<" Enter number of players for Yellow Offence ( max 5 ) : ";
		cin>>Temp_Yellow[3];

		cout<<"\n\n Enter number of players for Red Defence ( max 5 ) : ";
		cin>>Temp_Red[1];
		cout<<" Enter number of players for Red Midfield ( max 5 ) : ";
		cin>>Temp_Red[2];
		cout<<" Enter number of players for Red Offence ( max 5 ) : ";
		cin>>Temp_Red[3];

		// Finding out the total no. of players
		Red_Sum = 0;
		for(int i=0;i<4;i++) {
			if(Temp_Red[i] > 5) {
				Complete = 3;
			}
			Red_Sum = Red_Sum + Temp_Red[i];
		}

		// Finding out the total no. of players
		Yellow_Sum = 0;
		for(i=0;i<4;i++) {
			if(Temp_Yellow[i] > 5) {
				Complete = 3;
			}
			Yellow_Sum = Yellow_Sum + Temp_Yellow[i];
		}

		if(Complete == 3) {
			cout<<"\n\n One of the player values enetered, exceeds 5 \n";
		}

		if(Red_Sum > 11) {
			cout<<"\n\n Number of players entered for Red team exceeding 11";
			Complete = 2;
		}

		if(Yellow_Sum > 11) {
			cout<<"\n\n Number of players entered for Yellow team exceeding 11";
			Complete = 2;
		}

		if(Complete == 2 || Complete == 3) {
			Complete = 0;
			cout<<"\n Hit any key to use default values or (R) to re-enter ...";
			Ans = getche();
			if(Ans != 'r' && Ans != 'R') {
				break;
			}
		}
		else {
			cout<<"\n Hit (R) to re-enter values or any other key to accept "
				<<"	these values ... ";
			Ans = getche();
			if(Ans != 'r' && Ans != 'R') {
				Complete = 1;
			}
		}
	}

	if(Complete == 1) {
		for(int i=0;i<4;i++) {
			YPlayers[i] = Temp_Yellow[i];
		}

		for(i=0;i<4;i++) {
			RPlayers[i] = Temp_Red[i];
		}
	}
	else {
		SetPlayerDefaults();
	}
}

//////////////////////////////////////////////////////////////////////////
//		   			FUNCTION TO SET PLAYER POSITION DEFAULTS			//
//////////////////////////////////////////////////////////////////////////
void
SetPlayerDefaults(void) {
		YPlayers[0] = 1;							// Goalie
		YPlayers[1] = 2;							// Defence
		YPlayers[2] = 5;							// Midfield
		YPlayers[3] = 3;							// Offence

		RPlayers[0] = 1;							// Goalie
		RPlayers[1] = 2;							// Defence
		RPlayers[2] = 5;							// Midfield
		RPlayers[3] = 3;							// Offence
}

//////////////////////////////////////////////////////////////////////////
//			       LETS USER CHOOSE MODE IN 2 PLAYER GAME				//
//////////////////////////////////////////////////////////////////////////
int
ChooseTwoPlayerMode(void) {
	char c;
	cleardevice();
	setfillstyle(SOLID_FILL, GREEN);
	setcolor(WHITE);
	setlinestyle(SOLID_LINE, 1, 3);
	rectangle(199, 99, 391, 231);      		// Drawing the outer border
	bar(200, 100, 390, 230);				// Middle color fill
	settextstyle(DEFAULT_FONT, 0, 2);
	setcolor(BLACK);
	outtextxy(212, 117, " Choose 2 ");
	outtextxy(212, 142, "Player Mode");
	setcolor(WHITE);
	outtextxy(210, 115, " Choose 2 ");
	outtextxy(210, 140, "Player Mode");
	settextstyle(DEFAULT_FONT, 0, 1);
	setcolor(WHITE);
	outtextxy(225, 195, " Default Settings  ");
	outtextxy(225, 210, " Advanced Settings ");
	setlinestyle(SOLID_LINE, 0, 3);
	Mouse.showmouseptr();
	while(1) {
		// Keyboard Interfacing ...
		if(kbhit()){
			c = getch();
			// Clearing all the green rectangles drawn ...
			setcolor(GREEN);
			rectangle(201, 190, 389, 205);
			rectangle(201, 205, 389, 220);

			if(c == ESC_KEY) {				// Go back to main menu
				StopSong();
				return -1;
			}
			// Default Settings
			else if(c == '1') {				//Single Player Mode
				setcolor(RED);
				Mouse.hidemouseptr();
				rectangle(201, 190, 389, 205);
				delay(500);
				Mouse.showmouseptr();
				return 0;
			}
			// Advanced Settings
			else if(c == '2') {
				setcolor(RED);
				Mouse.hidemouseptr();
				rectangle(201, 205, 389, 220);
				delay(500);
				Mouse.showmouseptr();
				return 1;
			}
		}

		// Mouse Interfacing ...
		Mouse.getmousepos();
		// Refresh red bars of menu
		if(Mouse.X < 200 || Mouse.X > 400 ||
			Mouse.Y < 190 || Mouse.Y > 230) {
			setcolor(GREEN);
			Mouse.hidemouseptr();
			rectangle(201, 190, 389, 205);
			rectangle(201, 205, 389, 220);
			Mouse.showmouseptr();
		}
		// Setting the X clips
		if(Mouse.X > 200 && Mouse.X < 400) {
			// Default Settings
			if(Mouse.Y > 190 && Mouse.Y < 205) {
				setcolor(GREEN);
				rectangle(201, 208, 389, 220);
				setcolor(RED);
				Mouse.hidemouseptr();
				rectangle(201, 190, 389, 205);
				Mouse.showmouseptr();
				if((Mouse.Button & 1) == 1) {
					Mouse.hidemouseptr();
					return 0;
				}
			}
			// Advanced Settings
			else if(Mouse.Y > 205 && Mouse.Y < 220) {
				setcolor(GREEN);
				rectangle(201, 190, 389, 205);
				setcolor(RED);
				Mouse.hidemouseptr();
				rectangle(201, 205, 389, 220);
				Mouse.showmouseptr();
				if((Mouse.Button & 1) == 1) {
					Mouse.hidemouseptr();
					return 1;
				}
			}
		}
	while(inportb(0x3da) & 8);
	while(!(inportb(0x3da) & 8));
	}
}