#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>//sleep
#include <conio.h>//kbhit
#include <unistd.h>
#define ROW 6 //define board
#define COL 7
#define SIZE 3
 

int robotCheckWin(char *board,int bottom);//????????????????,??????? 1
int robotInputChess(char *board);//??????????,?????
void robotTakeTurn(int round, char *board, char *player);//?????????
int inputChess(void);//????,??????
int gameMode(char *board, int round); //??????
void printBoard(char *board); //????
void printWiningBoard(char *board);//????????????
int takeTurn(int round, char *board, char *player); //????
void savingWinnigPos(int a,int b,int c,int d); //????????
bool checkWin(int insertPos, char *board);
bool checkFour(char *board, int a, int b, int c, int d);
bool checkVertical(int insertPos, char *board);
bool checkHorizontal(int insertPos, char *board);
bool checkTilted(int insertPos, char *board);
bool tieCheck(char *board);
void saveFile(char *board);
int loadFile(char *boar, int round);
void animation(char *board, int insertPos, int position);



int insertPos = 0; //get final insert position in takeTurn function
int winningPos[4];//the four winning position
int position; //user enter number
int playMode;
char blank = ' ';

int main(void){
	char board[ROW * COL];  //??????
	char player[SIZE] = "OX"; //player's sign
	int round = 2; //first round number
	int playagain, i, tieTest = 1;
	
	/**
	 * 1.???????? : ?????AI;????????
	 * 2.????????????,????????
	 * ???????????
	 * ??????
	 * ??????
	**/
	
	do{
		system("CLS");
		gameMode(board, round); //????????
		system("CLS");
		printBoard(board); //print the first board
	
		if(playMode == 1 || playMode == 3){// new game : human VS. human
		
			if(playMode == 3){ //load game : human v.s human
				round = loadFile(board, round);//??????round,?????
			} 
			
			takeTurn(round, board, player); //player 1 ??

			while(checkWin(insertPos, board) != 1 && !tieCheck(board)){ //  win : 1
				++round; //next round
				round = takeTurn(round, board, player); //?????????????round
			}
			
			if(checkWin(insertPos, board) == 1 && position != 0){ //????
				system("CLS");
				printWiningBoard(board);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED);//print red
				printf("%45c", blank);
				printf("Player %d (%c) Wins!\n\n\n", round % 2 + 1, player[round % 2]); //win
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//print white
			}
			if(tieCheck(board) && checkWin(insertPos, board) != 1){ //??
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED);
				printf("%51c", blank);
				printf("Tie ! \n\n\n");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
		}
		
		else if (playMode == 2 || playMode == 4){ //new game :  human VS. robot  
		
			if(playMode == 4){ //load game : human v.s robot
				round = loadFile(board, round);
			}
			
			robotTakeTurn(round, board, player); 
			
			while(checkWin(insertPos, board) != 1 && tieCheck(board) != 1){ //  win : 1
		
				robotTakeTurn(round, board, player);
			}
			
			if(tieCheck(board) && checkWin(insertPos, board) != 1){
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED);
				printf("%51c", blank);
				printf("Tie ! \n\n\n");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
		}
		if(position != 0){
			printf("%40c", blank);
			printf("Enter (1) to play again\n");
			printf("%40c", blank);
			printf("Enter (0) to end the game\n");
			scanf("%d",&playagain);
			while(playagain != 1 && playagain != 0){
				printf("%40c", blank);
				printf("Enter 1 or 0\n");
				printf("%40c", blank);
				scanf("%d",&playagain);
			}
			
		}
		
	}while(playagain == 1);
	
	if(position == 0){ //??
		printf("\n%45c", blank);
		printf("Saving Complete!\n");
		printf("\n%45c", blank);
		printf("Thanks for playing.\n\n\n");
	}
	else{
		printf("\n%43c Thanks for playing.\n\n\n", blank);
	}
	
	
	system("pause");
	return 0;
}

int gameMode(char *board, int round){
	int gameMode, newMode;
	
	printf("\n\n\n\n\n\n\n");
	printf("%42c", blank);
	printf("Welcome to Connect Four !\n\n", blank);
	printf("%45c", blank);
	
	printf("\n\n");
	printf("%45c", blank);
	printf("******************\n\n");
	printf("%45c", blank);
	printf("Human VS. Human: 1\n %43c Human VS. Robot: 2\n\n", blank);
	printf("%45c", blank);
	printf("******************\n");

	scanf("%d", &gameMode);
	
	while(gameMode != 1 && gameMode != 2 ){
		printf("%48c", blank);
		printf("Enter 1 or 2 \n\n");
		scanf("%d", &gameMode);
	}
	system("CLS");
	
	printf("\n\n\n\n\n\n\n\n\n\n\n");
	printf("%45c", blank);
	printf("******************\n\n");
	printf("%48c", blank);
	printf("New Game : 1 \n %46c Load Game : 2\n\n", blank);
	printf("%45c", blank);
	printf("******************\n");
	printf("%45c", blank);
	scanf("%d", &newMode);
	
	
	while(newMode != 1 && newMode != 2 ){
		printf("Enter 1 or 2 \n");
		scanf("%d", &newMode);
	}
	
	if(gameMode == 1 && newMode == 1){//New Game : Human VS. Human
		memset(board,' ', ROW * COL); //innitialize array with blank
		playMode = 1;
	}
	else if(gameMode == 2 && newMode == 1){//New Game : Human VS. Robot
		memset(board,' ', ROW * COL); //innitialize array with blank
		playMode = 2;
	}
	else if (gameMode == 1 && newMode == 2){ //Loading Game : human v.s human
		playMode = 3;                  
		return loadFile(board, round); //return old board and old round
	}
	else if (gameMode == 2 && newMode == 2){ //loadding Game : human V.s robot
		playMode = 4;
		return loadFile(board, round);
	}
}

void animation(char *board, int insertPos, int position){
	int i, j, tempPos;
	char temp;
	
	temp = board[insertPos];
	board[insertPos] = ' ';
	
	for(i = position - 1; i <= insertPos; i += 7){
		board[i] = temp;
		printBoard(board);
		Sleep(40);
		board[i] = ' ';
		
		system("CLS");
	}
	board[insertPos] = temp;
}

void printBoard(char *board){
	size_t i;
	printf("\n\n\n\n\n");
	printf("%40c", blank);
	puts("--------Connect Four---------\n");
	printf("%40c", blank);
	for(i = 0; i < ROW * COL; ++i){
		if(i % COL == 0 && i != 0){  //change line every seven elements in one row
			printf("|\n");
			printf("%40c", blank);
			puts("-----------------------------");
			printf("%40c", blank);

		}
		if(i == insertPos){
				printf("|", board[i]);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_BLUE);//print blue
				printf(" %c ", board[i]); //print board with winning position
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//print white
		}
		else{
			printf("| %c ", board[i]); //print |board[0]|board[1]|board[2]|...
		}
		
	}
	printf("|\n"); //print the last | 
	printf("%40c", blank);
	puts("-----------------------------");
	printf("%40c", blank);
	puts("  1   2   3   4   5   6   7\n");
}

void printWiningBoard(char *board){
	size_t i;
	printf("\n\n\n\n\n");
	printf("%40c", blank);
	puts("--------Connect Four---------\n");
	printf("%40c", blank);
	for(i = 0; i < ROW * COL; ++i){
		if(i % COL == 0 && i != 0){  //change line every seven elements in one row
			printf("|\n");
			printf("%40c", blank);
			puts("-----------------------------");
			printf("%40c", blank);
		}
		
		if(i==winningPos[0]||i==winningPos[1]||i==winningPos[2]||i==winningPos[3]){
			printf("|", board[i]);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED);//print red
			printf(" %c ", board[i]); //print board with winning position
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//print white
		}
		
		else{
			printf("| %c ", board[i]); //print board without winning position
		}
		
	}
	printf("|\n"); //print the last | 
	printf("%40c", blank);
	puts("-----------------------------");
	printf("%40c", blank);
	puts("  1   2   3   4   5   6   7\n\n");
}

	/**
	 * 1.????????1 ~ 7, 0??
	 * ??????????????
	 * ?????????
	 * ???????????
	 * ?????????
	 * */

int takeTurn(int round, char *board, char *player){
	int bottom, test = 0;
	
	do{
		printf("%30c", blank);
		printf("player %d (%c) : Enter 1 ~ 7, 0 to Save Current Game\n", round % 2 + 1, player[round % 2]); //player one go first
		position = inputChess(); //???????????
		if(position == -1){
			round++; //?????????round
		}

	
	}while(position == -1);//?round????
	
	
	if(position > 0){ //?????
		while(test == 0){  //check whether insertion is complete, if complete, test = 1
			for(bottom = position + 34; bottom >= position - 1; bottom -= 7){ //check whether the position in board is blank from bottom
				if(board[bottom] == ' '){
					board[bottom] = player[round % 2]; //insert player sign to board
					test = 1; //insert complete
					insertPos = bottom; //get final insert position store in global variable
					system("CLS"); //clear screen
					animation(board, insertPos, position);
					printBoard(board); //print new board
					
					break; //end for loop
				}
			else if(bottom == position - 1 && board[bottom] != ' '){ //???????
				printf("\n");
				printf("Column full! Enter other column.\n");
				position = inputChess(); //????
				}
			}
		}
	}
	else if(position == 0){
		saveFile(board); //save record to file
	}
	return round;
}

void savingWinnigPos(int a,int b,int c,int d){
	winningPos[0] = a;
	winningPos[1] = b;
	winningPos[2] = c;
	winningPos[3] = d;
	}

bool tieCheck(char *board){
	int i, tieTest = 1;
	
	for(i = 0; i < COL; ++i){
		if(board[i] == ' '){
			tieTest = 0;
			return tieTest;
		}
	}
	return tieTest;
}

bool checkWin(int insertPos, char *board){
	if(checkVertical(insertPos, board) || checkHorizontal(insertPos, board) || checkTilted(insertPos, board) || position == 0){
		return 1; //???????????????
	}
	else{
		return 0;
	}
}

bool checkFour(char *board, int a, int b, int c, int d){
	if(board[a] == board[b] && board[b] == board[c] && board[c] == board[d] 
		&& d < COL * ROW && d >= 0 && a >= 0 && a < COL * ROW && board[a] != ' '){ // a:start, d:end must between 0 ~ 41
		return 1; //??????
	}
	else{
		return 0;
	}
}

bool checkVertical(int insertPos, char *board){ //?????
	int start, add, i = 1, j = 1;
	
	for(start = 0; i <= 3; start += 7, ++i){
		for(add = start; j <= 7; ++add, ++j){
			if(checkFour(board, add, add + 7, add + 7*2, add + 7*3)){
				savingWinnigPos(add, add + 7, add + 7*2, add + 7*3);
				
				return 1;
			}
		}
		j = 1;
	}
	return 0;
}

bool checkHorizontal(int insertPos, char *board){
	int start, add, i = 1, j = 1;
	
	for(start = 0; i <= 6; start += 7, ++i){
		for(add = start; j <= 4; add++, j++){
			if(checkFour(board, add, add + 1, add + 2, add + 3) == 1){
			
			//check form the last left side to last right side
			savingWinnigPos(add, add + 1, add + 2, add + 3);
			return 1;
			}
		}
			j = 1;
	}
	return 0;
}
bool checkTilted(int insertPos, char *board){
	int start, i = 1, j = 1 , add;
	
	for(start = 0; i <= 3; start += 7, ++i){ //check negative slop
		for(add = start; j <= 4; add++, j++ ){
			if(checkFour(board, add, add + 8, add + 8*2, add + 8*3) == 1){
				savingWinnigPos(add, add + 8, add + 8*2, add + 8*3);
			
				return 1;
			}
		}
		j = 1;
	}
	i = 1;
	j = 1;
	
	for(start = 3; i <= 3; start += 7, ++i){ //check positive slope
		for(add = start; j <= 4; add++, j++){
			if(checkFour(board, add, add + 6, add + 6*2, add + 6*3)){
				savingWinnigPos(add, add + 6, add + 6*2, add + 6*3);
				
				return 1;
			}
		}
		j = 1;
	} 
	return 0;
}

void saveFile(char *board){
	FILE *file;
	
	file = fopen("record.txt", "w");
	fwrite(board, sizeof(char), COL * ROW, file); //write board to file
	fclose(file);
}

int loadFile(char *board, int round){
	int i = 0, count = 0;
	FILE *file;
	
	if(access("record.txt", R_OK) != -1){ //check the file is exist
		file = fopen("record.txt", "r");
	
		while((fscanf(file,"%c",&board[i])) != EOF) //scanf and check EOF
	        {
	            i++;
	        }
	        
	    fclose(file);
	    
	    for(i = 0; i < COL*ROW; ++i){
	    	if(board[i] != ' '){
	    		count++;
			}
		}
		if(count % 2 == 1 || count == 1){
			return 3; //player two play
		}
	}
	else{
		printf("%37c", blank);
		printf("Record Not Found. New Game Start !\n\n");
		memset(board,' ', ROW * COL);
		return 2;
	}
	
	
} 

int inputChess(void){//???? 
	int sec;//???? 
	int position=-1;//?????(1~7)  
	char input=-1;//????  
    printf("%30c", blank);
    for(sec=10;sec>0;sec--){//???????? 
    	Sleep(1000);
    	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED);//?? 
    	printf("%d  ",sec);
    	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//??
    	
    	if (kbhit()){//????????? 
			input=getche();
			while(input<48||input>55){
				printf("%45c", blank);
    			printf("\nERROR, enter1~7 or 0 to save the file\n");
    			input=getche();
    			if(input>=48&&input<=55){
    				break;//??,?????
				}
			} 
			if(input >= 48 && input<=55){
    			break;//??,?????
			}
		}
	}

    
	if(input==-1){
		printf("\nThe time is up\n");
    	printf("Take turn!!\n\n");
		return position;//??????,???-1
	}
	else if(input!=-1){
		position=input-48;//?char???int 
		return position;//????? 
	}
	
	
}

void robotTakeTurn(int round, char *board, char *player){
	int bottom, test = 0, i, tieTest = 1;
	
	//human move
	printf("%30c", blank);
	printf("player (%c) : Enter 1 ~ 7, 0 to Save Current Game\n", player[round % 2]); //player go first
	position = inputChess(); 
	
	
	if(position > 0){//check human move
		while(test == 0){  //check whether insertion is complete, if complete, test = 1
			for(bottom = position + 34; bottom >= position - 1; bottom -= 7){ //check whether the position in board is blank from bottom
				if(board[bottom] == ' '){
					board[bottom] = player[round % 2]; //insert player sign to board
					test = 1; //insert complete
					insertPos = bottom; //get final insert position store in global variable
					system("CLS"); //clear screen
					animation(board, insertPos, position);
					printBoard(board); //print new board
					
					break; //end for loop
				}
				else if(bottom == position - 1 && board[bottom] != ' '){ //column full, insertion fail, test = 0
					printf("\n");
					printf("%45c", blank);
					printf("Column full! Enter other column.\n");
					position = inputChess();
					}
				}
			}
		}
	else if(position == 0){
		saveFile(board); //save record to file
	}
	
	if(checkWin(insertPos, board) == 1 && position != 0){//if human wins
		system("CLS");
		printWiningBoard(board);

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED);//print red
		printf("%45c", blank);
		printf("Player 1 (O) Wins!\n\n\n"); //win
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//print white
		}
	
	if(checkWin(insertPos, board)!= 1 && position != 0){// if human didn't win
		round++;
		position = robotInputChess(board);//robot move
	
		if(position > 0){//check robot move
			test=0;
			while(test == 0){  //check whether insertion is complete, if complete, test = 1
				for(bottom = position + 34; bottom >= position - 1; bottom -= 7){ //check whether the position in board is blank from bottom
					if(board[bottom] == ' '){
						board[bottom] = player[round % 2]; //insert player sign to board
						test = 1; //insert complete
						insertPos = bottom; //get final insert position store in global variable
						system("CLS");
						animation(board, insertPos, position);
						printBoard(board); //print new board
						
						break; //end for loop
					}
				}
			}
		}
	
		if(checkWin(insertPos, board) == 1 && position != 0){//if robot wins
			system("CLS");
			printWiningBoard(board);
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED);//print red
				printf("%45c", blank);
				printf("Robot (X)  Wins!\n\n\n"); //win
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//print white
			}		
		}		
	}
	//system("pause");
}

int robotInputChess(char *board){
	int i=0,j=0,k=2;
	int bottom,input;
	
	/*
		????:
		1.?????????????(3???4?) 
		2.?????????????(3???4?) 
		3.???????????????????(2???3?) 
		4.?????????,????(??????????)
		5.??????????,????????????? 
	*/
	
	
	//????(X)????????? 
	for(i=0;i<=6;i++){//checking vertical is three or not 
		for(j=i+7;j<=(i+21);j=j+7){
			if(board[j]=='X'&&board[j]==board[j+7]&&board[j+7]==board[j+14]&&board[j-7]==' '){
				input=j%7+1;
				return input;
			}
		}
	}
	
	for(i=1;i<=5;i++){//checking tilted is three or not
		for(j=i+7;j<=(i+28);j=j+7){
			if(i>=1&&i<=4&&j>13&&j<35){// XOOO_ ???  j>13:??????? 
				if(board[j]=='X'&&board[j]==board[j-6]&&board[j]==board[j+6]){
					if(board[j-12]==' '&&board[j-5]!=' '){
						input=(j-12)%7+1;
						return input;
					}
				}
			}
			if(i>=2&&i<=5&&j<28&&j>=7){// _OOOX ???  j<28:???????	j>=21 : ???????? !!
				if(board[j]=='X'&&board[j]==board[j-6]&&board[j]==board[j+6]){
					if(board[j+12]==' '&&((j<21&&board[j+19]!=' ')||j>=21)){
						input=(j+12)%7+1;
						return input;
					}
				}
			}
			if(i>=1&&i<=4&&j>13&&j<35){// XOO_OX ???  j>13:??????? 
				if(board[j]=='X'&&board[j]==board[j+6]&&board[j]==board[j-12]){
					if(board[j-6]==' '&&board[j+1]!=' '){
						input=(j-6)%7+1;
						return input;
					}
				}
			}
			if(i>=2&&i<=5&&j<28&&j>=7){// XO_OOX ???  j>13:??????? 
				if(board[j]=='X'&&board[j]==board[j+6]&&board[j]==board[j-12]){
					if(board[j+6]==' '&&board[j+13]!=' '){
						input=(j-6)%7+1;
						return input;
					}
				}
			}
			if(i>=2&&i<=5&&j>13){// _OOOX ???  j>13:??????? 
				if(board[j]=='X'&&board[j]==board[j-8]&&board[j]==board[j+8]){
					if(board[j-16]==' '&&board[j-9]!=' '){
						input=(j-16)%7+1;
						return input;
					}
				}
			}
			if(i>=1&&i<=4&&j<28&&j>6){// XOOO_ ???  j<28:???????	j>=21 : ???????? !!
				if(board[j]=='X'&&board[j]==board[j-8]&&board[j]==board[j+8]){
					if(board[j+16]==' '&&((j<21&&board[j+23]!=' ')||j>=21)){
						input=(j+16)%7+1;
						return input;
					}
				}
			}
			if(i>=1&&i<=4&&j>13&&j<35){// XOO_OX ???  j>13:??????? 
				if(board[j]=='X'&&board[j]==board[j-8]&&board[j]==board[j+16]){
					if(board[j+8]==' '&&board[j+15]!=' '){
						input=(j+15)%7+1;
						return input;
					}
				}
			}
			if(i>=2&&i<=5&&j<35&&j>13){// XO_OOX ???  j>13:??????? 
				if(board[j]=='X'&&board[j]==board[j+8]&&board[j]==board[j-16]){
					if(board[j-8]==' '&&board[j-1]!=' '){
						input=(j-8)%7+1;
						return input;
					}
				}
			}
		}
	}
	
	for(i=1;i<=5;i++){//checking horizontal is three or not,  j>=35 : ???????? !!
		for(j=i;j<=(i+35);j=j+7){
			
			if(i>=1&&i<=4){// XOOO_ 
				if(board[j]=='X'&&board[j]==board[j+1]&&board[j]==board[j-1]){
					if(board[j+2]==' '&&((j<35&&board[j+9]!=' ')||j>=35)){
						input=(j+2)%7+1;
						return input;
					}
				}
			}
			if(i>=2&&i<=5){// _OOOX 
				if(board[j]=='X'&&board[j]==board[j+1]&&board[j]==board[j-1]){
					if(board[j-2]==' '&&((j<35&&board[j+5]!=' ')||j>=35)){
						input=(j-2)%7+1;
						return input;
					}
				}
			}
			if(i>=1&&i<=4){// XOO_OX 
				if(board[j]=='X'&&board[j]==board[j-1]&&board[j]==board[j+2]){
					if(board[j+1]==' '&&((j<35&&board[j+8]!=' ')||j>=35)){
						input=(j+1)%7+1;
						return input;
					}
				}
			}
			if(i>=2&&i<=5){// XO_OOX 
				if(board[j]=='X'&&board[j]==board[j-2]&&board[j]==board[j+1]){
					if(board[j-1]==' '&&((j<35&&board[j+6]!=' ')||j>=35)){
						input=(j-1)%7+1;
						return input;
					}
				}
			}
		}
	}
	
	
	//????(O)????????? 
	for(i=0;i<=6;i++){//checking vertical is three or not 
		for(j=i+7;j<=(i+21);j=j+7){
			if(board[j]=='O'&&board[j]==board[j+7]&&board[j+7]==board[j+14]&&board[j-7]==' '){
				input=j%7+1;
				return input;
			}
		}
	}
	
	for(i=1;i<=5;i++){//checking tilted is three or not
		for(j=i+7;j<=(i+28);j=j+7){
			if(i>=1&&i<=4&&j>13&&j<35){// XOOO_ ???  j>13:??????? 
				if(board[j]=='O'&&board[j]==board[j-6]&&board[j]==board[j+6]){
					if(board[j-12]==' '&&board[j-5]!=' '){
						input=(j-12)%7+1;
						return input;
					}
				}
			}
			if(i>=2&&i<=5&&j<28&&j>=7){// _OOOX ???  j<28:???????	j>=21 : ???????? !!
				if(board[j]=='O'&&board[j]==board[j-6]&&board[j]==board[j+6]){
					if(board[j+12]==' '&&((j<21&&board[j+19]!=' ')||j>=21)){
						input=(j+12)%7+1;
						return input;
					}
				}
			}
			if(i>=1&&i<=4&&j>13&&j<35){// XOO_OX ???  j>13:??????? 
				if(board[j]=='O'&&board[j]==board[j+6]&&board[j]==board[j-12]){
					if(board[j-6]==' '&&board[j+1]!=' '){
						input=(j-6)%7+1;
						return input;
					}
				}
			}
			if(i>=2&&i<=5&&j<28&&j>=7){// XO_OOX ???  j>13:??????? 
				if(board[j]=='O'&&board[j]==board[j+6]&&board[j]==board[j-12]){
					if(board[j+6]==' '&&board[j+13]!=' '){
						input=(j-6)%7+1;
						printf("r");//test
					}
				}
			}
			if(i>=2&&i<=5&&j>13){// _OOOX ???  j>13:??????? 
				if(board[j]=='O'&&board[j]==board[j-8]&&board[j]==board[j+8]){
					if(board[j-16]==' '&&board[j-9]!=' '){
						input=(j-16)%7+1;
						return input;
					}
				}
			}
			if(i>=1&&i<=4&&j<28&&j>6){// XOOO_ ???  j<28:???????	j>=21 : ???????? !!
				if(board[j]=='O'&&board[j]==board[j-8]&&board[j]==board[j+8]){
					if(board[j+16]==' '&&((j<21&&board[j+23]!=' ')||j>=21)){
						input=(j+16)%7+1;
						return input;
					}
				}
			}
			if(i>=1&&i<=4&&j>13&&j<35){// XOO_OX ???  j>13:??????? 
				if(board[j]=='O'&&board[j]==board[j-8]&&board[j]==board[j+16]){
					if(board[j+8]==' '&&board[j+15]!=' '){
						input=(j+15)%7+1;
						return input;
					}
				}
			}
			if(i>=2&&i<=5&&j<35&&j>13){// XO_OOX ???  j>13:??????? 
				if(board[j]=='O'&&board[j]==board[j+8]&&board[j]==board[j-16]){
					if(board[j-8]==' '&&board[j-1]!=' '){
						input=(j-8)%7+1;
						return input;
					}
				}
			}
		}
	}
	
	for(i=1;i<=5;i++){//checking horizontal is three or not,  j>=35 : ???????? !!
		for(j=i;j<=(i+35);j=j+7){
			
			if(i>=1&&i<=4){// XOOO_ 
				if(board[j]=='O'&&board[j]==board[j+1]&&board[j]==board[j-1]){
					if(board[j+2]==' '&&((j<35&&board[j+9]!=' ')||j>=35)){
						input=(j+2)%7+1;
						return input;
					}
				}
			}
			if(i>=2&&i<=5){// _OOOX 
				if(board[j]=='O'&&board[j]==board[j+1]&&board[j]==board[j-1]){
					if(board[j-2]==' '&&((j<35&&board[j+5]!=' ')||j>=35)){
						input=(j-2)%7+1;
						return input;
					}
				}
			}
			if(i>=1&&i<=4){// XOO_OX 
				if(board[j]=='O'&&board[j]==board[j-1]&&board[j]==board[j+2]){
					if(board[j+1]==' '&&((j<35&&board[j+8]!=' ')||j>=35)){
						input=(j+1)%7+1;
						return input;
					}
				}
			}
			if(i>=2&&i<=5){// XO_OOX 
				if(board[j]=='O'&&board[j]==board[j-2]&&board[j]==board[j+1]){
					if(board[j-1]==' '&&((j<35&&board[j+6]!=' ')||j>=35)){
						input=(j-1)%7+1;
						return input;
					}
				}
			}
			
			//???????????????? 
			//checking horizontal is two or not
			if(board[j]!=' '&&board[j]==board[j+1]&&board[j-1]==' '&&board[j+2]==' '&&((j<35&&board[j+6]!=' '&&board[j+9]!=' ')||(j>=35))){// _OO_
				if(i<=4&&board[j+2]==' '&&((j<35&&board[j+9]!=' ')||j>=35)){// XOO_ 
					input=(j+2)%7+1;
					return input;
				}
				
				if(i>=2&&board[j-1]==' '&&((j<35&&board[j+6]!=' ')||j>=35)){// _OOX
					input=(j-1)%7+1;
					return input;
				}
			}
			
			if(board[j-1]!=' '&&board[j+1]==board[j-1]&&board[j-2]==' '&&board[j+2]==' '&&((j<35&&board[j+6]!=' '&&board[j+8]!=' ')||(j>=35))){// _O_O_
				if(i>=2&&i<=4&&board[j]==' '&&((j<35&&board[j+7]!=' ')||j>=35)){
					input=j%7+1;
					return input;
				}
			}
		}
	}
	
	for(i=1;i<=5;i++){//checking tilted is two or not
		for(j=i+7;j<=(i+28);j=j+7){
			
			if(board[j]!=' '&&board[j]==board[j-6]&&board[j-12]==' '&&board[j+6]==' '){// _OO_ ???
				if(i>=1&&i<=4&&j>13&&board[j-12]==' '&&board[j-5]!=' '){// XOO_ ???
					input=(j-12)%7+1;
					return input;
				}
				
				if(i>=2&&i<=5&&board[j+6]==' '&&((j<28&&board[j+16]!=' ')||j>=28)){// _OOX ???
					input=(j+6)%7+1;
					return input;
				}
			}
			
			if(board[j-6]!=' '&&board[j-6]==board[j+6]&&j>=14&&j<35&&board[j-12]==' '&&board[j+12]==' '){// _O_O_ ???
				if(board[j]==' '&&board[j+7]!=' '){
					input=j%7+1;
					return input;
				}
			}
			
			if(board[j]!=' '&&board[j]==board[j-8]&&board[j-16]==' '&&board[j+8]==' '){// _OO_ ???
				if(j>13&&board[j-16]==' '&&board[j-9]!=' '){// _OOX ???
					input=(j-16)%7+1;
					return input;
				}
				
				if(j<35&&board[j+8]==' '&&((j<28&&board[j+15]!=' ')||j>=28)){// XOO_ ???
					input=(j+8)%7+1;
					return input;
				}
			}
			
			if(board[j-8]!=' '&&board[j-8]==board[j+8]&&j>=14&&j<35&&board[j+16]==' '&&board[j-16]==' '){// _O_O_ ???
				if(board[j]==' '&&board[j+7]!=' '){
					input=j%7+1;
					return input;
				}
			}
		}
	}
	
	if(board[3]==' '){//???? 
		input=4;
		if(robotCheckWin(board,input)==0){//?????????? 
			return input;
		}
		
	}
	else if(board[3]!=' '){//????
		for(i=4;i!=7;i++){ 
			if(board[i]==' '){
				input=i+1;
				if(robotCheckWin(board,input)==0){//?????????? 
					return input;
				}
			}
			
			
			else if(board[i]!=' '){//?????,???? 
				j=i;
				j=j-k;
				if(board[j]==' '){
					input=j+1;
					if(robotCheckWin(board,input)==0){//?????????? 
						return input;
					}
				}
			}
			k=k+2;//??????? 
		}	
	}
	
	//??????? 
	printf("yogamama\n");//test
	k=2;//initialize k=2
	if(board[3]==' '){//???? 
		input=4;
		return input;	
	}
	else if(board[3]!=' '){//????
		for(i=4;i!=7;i++){ 
			if(board[i]==' '){
				input=i+1;
				return input;
			}
			
			
			else if(board[i]!=' '){//?????,???? 
				j=i;
				j=j-k;
				if(board[j]==' '){
					input=j+1;
					return input;
				}
			}
			k=k+2;//??????? 
		}	
	}
	
	//???? 
	else{
		return -1;//??-1,position <0 ??????? 
	}
}

int robotCheckWin(char *board,int input){//????????????????
	int i=0,j=0,k=1;
	int bottom,humanWins=0;
	
	//??????
	for(bottom = input + 34; bottom >= input - 1; bottom -= 7){ //check whether the position in board is blank from bottom
		if(board[bottom] == ' '){
			board[bottom] = 'X'; //insert player sign to board						
			break; //end for loop
		}
	}
	
	//??????
	for(i=0;i<=6;i++){//checking vertical is three or not 
		for(j=i+7;j<=(i+21);j=j+7){
			if(board[j]!=' '&&board[j]==board[j+7]&&board[j+7]==board[j+14]&&board[j-7]==' '){
				if(board[j]=='O'){//check if human will win or not
					humanWins=1;
					board[bottom] = ' '; //??!!
					return humanWins;
				}
			}
		}
	}
	
	for(i=1;i<=5;i++){//checking tilted is three or not
		for(j=i+7;j<=(i+28);j=j+7){
			if(i>=1&&i<=4&&j>13&&j<35){// XOOO_ ???  j>13:??????? 
				if(board[j]!=' '&&board[j]==board[j-6]&&board[j]==board[j+6]){
					if(board[j-12]==' '&&board[j-5]!=' '){
						if(board[j]=='O'){//check if human will win or not
							humanWins=1;
							board[bottom] = ' '; //??!!
							return humanWins;
						}
					}
				}
			}
			if(i>=2&&i<=5&&j<28&&j>=7){// _OOOX ???  j<28:???????	j>=21 : ???????? !!
				if(board[j]!=' '&&board[j]==board[j-6]&&board[j]==board[j+6]){
					if(board[j+12]==' '&&((j<21&&board[j+19]!=' ')||j>=21)){
						if(board[j]=='O'){//check if human will win or not
							humanWins=1;
							board[bottom] = ' '; //??!!
							return humanWins;
						}
					}
				}
			}
			if(i>=1&&i<=4&&j>13&&j<35){// XOO_OX ???  j>13:??????? 
				if(board[j]!=' '&&board[j]==board[j+6]&&board[j]==board[j-12]){
					if(board[j-6]==' '&&board[j+1]!=' '){
						if(board[j]=='O'){//check if human will win or not
							humanWins=1;
							board[bottom] = ' '; //??!!
							return humanWins;
						}
					}
				}
			}
			if(i>=2&&i<=5&&j<28&&j>=7){// XO_OOX ???  j>13:??????? 
				if(board[j]!=' '&&board[j]==board[j+6]&&board[j]==board[j-12]){
					if(board[j+6]==' '&&board[j+13]!=' '){
						if(board[j]=='O'){//check if human will win or not
							humanWins=1;
							board[bottom] = ' '; //??!!
							return humanWins;
						}
					}
				}
			}
			if(i>=2&&i<=5&&j>13&&j<35){// _OOOX ???  j>13:??????? 
				if(board[j]!=' '&&board[j]==board[j-8]&&board[j]==board[j+8]){
					if(board[j-16]==' '&&board[j-9]!=' '){
						if(board[j]=='O'){//check if human will win or not
							humanWins=1;
							board[bottom] = ' '; //??!!
							return humanWins;
						}
					}
				}
			}
			if(i>=1&&i<=4&&j<28&&j>6){// XOOO_ ???  j<28:???????	j>=21 : ???????? !!
				if(board[j]!=' '&&board[j]==board[j-8]&&board[j]==board[j+8]){
					if(board[j+16]==' '&&((j<21&&board[j+23]!=' ')||j>=21)){
						if(board[j]=='O'){//check if human will win or not
							humanWins=1;
							board[bottom] = ' '; //??!!
							return humanWins;
						}
					}
				}
			}
			if(i>=1&&i<=4&&j<28&&j>6){// XOO_OX ???  j>13:??????? 
				if(board[j]!=' '&&board[j]==board[j-8]&&board[j]==board[j+16]){
					if(board[j+8]==' '&&board[j+15]!=' '){
						if(board[j]=='O'){//check if human will win or not
							humanWins=1;
							board[bottom] = ' '; //??!!
							return humanWins;
						}
					}
				}
			}
			if(i>=2&&i<=5&&j>13&&j<35){// XO_OOX ???  j>13:??????? 
				if(board[j]!=' '&&board[j]==board[j+8]&&board[j]==board[j-16]){
					if(board[j-8]==' '&&board[j-1]!=' '){
						if(board[j]=='O'){//check if human will win or not
							humanWins=1;
							board[bottom] = ' '; //??!!
							return humanWins;
						}
					}
				}
			}
		}
	}
	
	for(i=1;i<=5;i++){//checking horizontal is three or not,  j>=35 : ???????? !!
		for(j=i;j<=(i+35);j=j+7){
			
			if(i>=1&&i<=4){// XOOO_ 
				if(board[j]!=' '&&board[j]==board[j+1]&&board[j]==board[j-1]){
					if(board[j+2]==' '&&((j<35&&board[j+9]!=' ')||j>=35)){
						if(board[j]=='O'){//check if human will win or not
							humanWins=1;
							board[bottom] = ' '; //??!!
							return humanWins;
						}
					}
				}
			}
			if(i>=2&&i<=5){// _OOOX 
				if(board[j]!=' '&&board[j]==board[j+1]&&board[j]==board[j-1]){
					if(board[j-2]==' '&&((j<35&&board[j+5]!=' ')||j>=35)){
						if(board[j]=='O'){//check if human will win or not
							humanWins=1;
							board[bottom] = ' '; //??!!
							return humanWins;
						}
					}
				}
			}
			if(i>=1&&i<=4){// XOO_OX 
				if(board[j]!=' '&&board[j]==board[j-1]&&board[j]==board[j+2]){
					if(board[j+1]==' '&&((j<35&&board[j+8]!=' ')||j>=35)){
						if(board[j]=='O'){//check if human will win or not
							humanWins=1;
							board[bottom] = ' '; //??!!
							return humanWins;
						}
					}
				}
			}
			if(i>=2&&i<=5){// XO_OOX 
				if(board[j]!=' '&&board[j]==board[j-2]&&board[j]==board[j+1]){
					if(board[j-1]==' '&&((j<35&&board[j+6]!=' ')||j>=35)){
						if(board[j]=='O'){//check if human will win or not
							humanWins=1;
							board[bottom] = ' '; //??!!
							return humanWins;
						}
					}
				}
			}
			
			//checking horizontal is two or not
			if(board[j]!=' '&&board[j]==board[j+1]&&board[j-1]==' '&&board[j+2]==' '){// _OO_
				if(i<=4&&board[j+2]==' '&&((j<35&&board[j+9]!=' ')||j>=35)){// XOO_ 
					if(board[j]=='O'){//check if human will win or not
						humanWins=1;
						board[bottom] = ' '; //??!!
						return humanWins;
					}
				}
				
				if(i>=2&&board[j-1]==' '&&((j<35&&board[j+6]!=' ')||j>=35)){// _OOX
					if(board[j]=='O'){//check if human will win or not
						humanWins=1;
						board[bottom] = ' '; //??!!
						return humanWins;
					 }
				}
			}
			
			if(i<=4&&i>=2&&board[j-1]!=' '&&board[j+1]==board[j-1]&&board[j-2]==' '&&board[j+2]==' '){// _O_O_
				if(board[j]==' '&&((j<35&&board[j+7]!=' ')||j>=35)){
					if(board[j]=='O'){//check if human will win or not
						humanWins=1;
						board[bottom] = ' '; //??!!
						return humanWins;
					}
				}
			}
		}
	}
	
	for(i=1;i<=5;i++){//checking tilted is two or not
		for(j=i+7;j<=(i+28);j=j+7){
			
			if(board[j]!=' '&&board[j]==board[j-6]&&board[j-12]==' '&&board[j+6]==' '){// _OO_ ???
				if(i>=1&&i<=4&&j>13&&board[j-12]==' '&&board[j-5]!=' '){// XOO_ ???
					if(board[j]=='O'){//check if human will win or not
						humanWins=1;
						board[bottom] = ' '; //??!!
						return humanWins;
					}
				}
				
				if(i>=2&&i<=5&&board[j+6]==' '&&((j<28&&board[j+16]!=' ')||j>=28)){// _OOX ???
					if(board[j]=='O'){//check if human will win or not
						humanWins=1;
						board[bottom] = ' '; //??!!
						return humanWins;
					}
				}
			}
			
			if(board[j-6]!=' '&&board[j-6]==board[j+6]&&j>=14&&j<35&&board[j-12]==' '&&board[j+12]==' '){// _O_O_ ???
				if(board[j]==' '&&board[j+7]!=' '){
					if(board[j]=='O'){//check if human will win or not
						humanWins=1;
						board[bottom] = ' '; //??!!
						return humanWins;
					}
				}
			}
			
			if(board[j]!=' '&&board[j]==board[j-8]&&board[j-16]==' '&&board[j+8]==' '){// _OO_ ???
				if(j>13&&board[j-16]==' '&&board[j-9]!=' '){// _OOX ???
					if(board[j]=='O'){//check if human will win or not
						humanWins=1;
						board[bottom] = ' '; //??!!
						return humanWins;
					}
				}
				
				if(j<35&&board[j+8]==' '&&((j<28&&board[j+15]!=' ')||j>=28)){// XOO_ ???
					if(board[j]=='O'){//check if human will win or not
						humanWins=1;
						board[bottom] = ' '; //??!!
						return humanWins;
					}
				}
			}
			
			if(board[j-8]!=' '&&board[j-8]==board[j+8]&&j>=14&&j<35&&board[j+16]==' '&&board[j-16]==' '){// _O_O_ ???
				if(board[j]==' '&&board[j+7]!=' '){
					if(board[j]=='O'){//check if human will win or not
						humanWins=1;
						board[bottom] = ' '; //??!!
						return humanWins;
					}
				}
			}
		}
	}
	
	//??????,??? 0
	board[bottom] = ' '; //??!!
	if(humanWins==0){//if human won't win
		printf("0\n");//test
		return humanWins;
	}
}

