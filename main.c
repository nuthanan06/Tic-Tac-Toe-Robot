//constants defined
const float squareDistance = 4.5;
const float hubAndBoardDistance = 10;

const float CONVERSION_RATE = ((2 * PI * (4)) / 360); //wheel radius in cm
const float fullUltrasonic = 45;
const int EMPTY = 0;
const int HUMAN = -1;
const int ROBOT = 1;
const int UNKNOWN = 2;
int board[9] = {EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY}; 
const int MAX_DEPTH = 7;
const int INF = 1000;

/*
Function Description: Calibrates the sensors on the robot. 
*/
void calibrateSensors() {
SensorType[S2] = sensorEV3_Ultrasonic;
	wait1Msec(50);
	SensorType[S2] = sensorEV3_Color;
	wait1Msec(50);
	SensorMode(S1) = modeEV3Color_Color;
	wait1Msec(50);
}

/* 
Function Description: checks whether a winner is found, returns the player that wins
*/
int checkIsWon(){
//checks the 2nd row, both diagonals, the middle column
	if (board[4] != EMPTY && 
((board[3] == board[4] && board[4] == board[5]) || 
(board[1] == board[4] && board[4] == board[7]) || 
(board[0] == board[4] && board[4] == board[8]) || 
(board[2] == board[4] && board[4] == board[6]))){
		return board[4];
	}
	//checks the first row and first column
	if (board[0] != EMPTY && 
((board[0] == board[1] && board[1] == board[2]) || 
(board[0] == board[3] && board[3] == board[6]))){
		return board[0];
	}
	//checks the last row and last column
	if (board[8] != EMPTY && 
((board[6] == board[7] && board[7] == board[8]) ||  
(board[2] == board[5] && board[5] == board[8]))){
		return board[8];
	}
	

//if all are empty just return 0
if (board[0] != EMPTY && board[1] != EMPTY && board[2] != EMPTY && board[3] != EMPTY && 
board[4] != EMPTY && board[5] != EMPTY && board[6] != EMPTY && board[7] != EMPTY && 
board[8] != EMPTY){
		return 0;
	}
	
	//returns unknown if the winner is undetermined (a constant defined at the start which is 2) 
	return UNKNOWN;

}

/* 
Function Description: passes in parameters such as specific statistics, and displays statistics, waits for the user to press enter before quitting
*/
void endGameProcedure(int playerTime, int robotTime, int playerMoves, int robotMoves, float totTime){
if (checkIsWon() == ROBOT){
		displayTextLine(0, "ROBOT WON");
	}else if (checkIsWon() == HUMAN){
		displayTextLine(0, "HUMAN WON");
	}else{
		displayTextLine(0, "DRAW");
	}
	displayTextLine(1, "STATISTICS");
	displayTextLine(2, "Total Player Time: %d", playerTime);
	displayTextLine(3, "Total Robot Time: %d", robotTime);
	displayTextLine(4, "Avg Player Time: %d", (int)playerTime/playerMoves);
	displayTextLine(5, "Avg Robot Time: %d", (int)robotTime/robotMoves);
	displayTextLine(6, "Total Game Time: %d", totTime);
	displayTextLine(7, "Thanks for playing!");
	displayTextLine(8, "Push enter to exit");
	while(!getButtonPress(buttonEnter)){
	}
	while(getButtonPress(buttonEnter)){
	}
}


/* 
Function Description: passes a settings array by reference, and asks the user questions on who starts first, and what colour the user wants 
*/
void startUp(bool* settings) {
    while (!getButtonPress(buttonUp) && (!getButtonPress(buttonDown))) {
        displayTextLine(5, "Who will start first?");
        displayTextLine(7, "Up Button --> Robot.");
        displayTextLine(8, "Down Button --> You (Player)");
    }

    settings[1] = true; //players starts first
    if (getButtonPress(buttonUp)) {
        settings[1] = false;
    }

    while (getButtonPress(buttonUp) || getButtonPress(buttonDown)) {}

    while (!getButtonPress(buttonUp) && (!getButtonPress(buttonDown))) {
        displayTextLine(5, "What colour pieces is the player?");
        displayTextLine(7, "Up Button --> Red.");
        displayTextLine(8, "Down Button --> Blue");
    }

    settings[2] = true; // player red
    if (getButtonPress(buttonDown)) {
        settings[2] = false;
    }

    while (getButtonPress(buttonUp) || getButtonPress(buttonDown)) {}

    string color = "blue";
    if (settings[2]) {
        color = "red";
    }

    int counter = 0;
    displayTextLine(5,"Please fill the dropper with");
    displayTextLine(6, "the 5 robot pieces.");
    displayTextLine(7, "");
    displayTextLine(8, "");
    while (counter < 5) {
wait1Msec(1000);
  	if (SensorValue[S2] < fullUltrasonic){
	    counter++;
	 }
	    else {
	    	counter = 0;
    	}
    }
    displayTextLine(5,"");
    displayTextLine(6, "");
}

int max(int a, int b){
	return (a >= b) ? a : b;
}
int min(int a, int b){
	return (a <= b) ? a : b;
}


/*
Description: This is a helper function for the getBestPosition function. It recursively iterates through the game tree and evaluates each position. The algorithm implements alpha beta pruning for efficiency. For the general algorithm, see report references.
*/
int miniMax(int depth, int currentPlayer, int alpha, int beta){
int winner = checkIsWon();
	if (depth == 0 || winner != UNKNOWN){
		if(winner == UNKNOWN){
			return 0;
		}
		return winner;
	}
	if (currentPlayer == ROBOT){ //want to maximize
		int maxEval = -INF;
		for (int i = 0; i < 9; i++){
			if (board[i] == EMPTY){
				board[i] = ROBOT;
				maxEval = max(maxEval, miniMax(depth - 1, HUMAN, alpha, beta));
				board[i] = EMPTY;
				alpha = max(maxEval, alpha);
				if (beta < alpha){
					return maxEval;
				}
			}
		}
		return maxEval;
	}else{
		int minEval = INF;
		for (int i = 0; i < 9; i++){
			if (board[i] == EMPTY){
				board[i] = HUMAN;
				minEval = min(minEval, miniMax(depth - 1, ROBOT, alpha, beta));
				board[i] = EMPTY;
				beta = min(minEval, beta);
				if (alpha > beta){
					return minEval;

				}
			}
		}
		return minEval;

	}

}

/*
Description: After each index is evaluated with the miniMax function, this function compares their scores and returns the index with the best score.
*/
int getBestPosition(int depth){
	int alpha = -INF;
	int beta = INF;
	int winner = checkIsWon();
	if (depth == 0 || winner != UNKNOWN){
		return winner;
	}
	int max = -INF;
	int maxMove = -1;
	for (int i = 0; i < 9; i++){
		if (board[i] == EMPTY){
			board[i] = ROBOT;
			int eval = miniMax(depth - 1, HUMAN, alpha, beta);
			if (eval > max){
				max = eval;
				maxMove = i;
			}
			board[i] = EMPTY;
		}
	}
	return maxMove;
}




/* 
Function Description: drops a piece based on the best position determined by the previous algorithm
*/
void pieceDrop(int moveNumber, int bestPosition) {
    // moves a piece from dropper onto board

   nMotorEncoder[motorD] = 0;
    motor[motorD] = -25;
    if (moveNumber < 2) {
        while (abs(nMotorEncoder[motorD]) < 720) {}
    } else {
        while (abs(nMotorEncoder[motorD]) < 360) {}
    }
    motor[motorD] = 0;
    int factor = 0;
    //determines how far the robot needs to move back to make its move depending on position	
    nMotorEncoder[motorC] = 0;
    motor[motorC] = 10;
    if (bestPosition == 1 || bestPosition == 4 || bestPosition == 7) {
        factor = 1;
    } else if (bestPosition == 0 || bestPosition == 3 || bestPosition == 6) {
        factor = 2;
    }
    while (abs(nMotorEncoder[motorC])*(CONVERSION_RATE)<squareDistance*factor-squareDistance*0.2) {}
    motor[motorC] = 0;
    //moves table and drops piece based on the column it needs to be in
    nMotorEncoder[motorA] = 0;
    nMotorEncoder[motorB] = 0;
    if (bestPosition == 0 || bestPosition == 1 || bestPosition == 2) {
        motor[motorA] = -25;
        while (abs(nMotorEncoder[motorA])  < 700) {}
        motor[motorA] = 0;
        motor[motorB] = 25;
        while (abs(nMotorEncoder[motorB]) < 180) {}
        motor[motorB] = 0;

        nMotorEncoder[motorA] = 0;
        motor[motorA] = 25;
        while (abs(nMotorEncoder[motorA]) < 700) {}
        motor[motorA] = 0;
    } else if (bestPosition == 6 || bestPosition == 7 || bestPosition == 8) {
        motor[motorA] = 25;
        while (abs(nMotorEncoder[motorA]) < 730) {}
        motor[motorA] = 0;
        motor[motorB] = 25;
        while (abs(nMotorEncoder[motorB]) < 180) {}
        motor[motorB] = 0;

        nMotorEncoder[motorA] = 0;
        motor[motorA] = -25;
        while (abs(nMotorEncoder[motorA]) < 730) {}
        motor[motorA] = 0;
    } else {
        motor[motorB] = 25;
        while (abs(nMotorEncoder[motorB]) < 180) {}
        motor[motorB] = 0;
        motor[motorA] = 25;
        while (abs(nMotorEncoder[motorA]) < 700) {}
        motor[motorA] = 0;
        nMotorEncoder[motorA] = 0;
        motor[motorA] = -25;
        while (abs(nMotorEncoder[motorA]) < 700) {}
        motor[motorA] = 0;
    }

    nMotorEncoder[motorB] = 0;
    motor[motorB] = -25;
    while (abs(nMotorEncoder[motorB]) < 180) {}
    motor[motorB] = 0;

    // returns to homebase
    nMotorEncoder[motorC] = 0;
    motor[motorC] = 10;
    while (abs(nMotorEncoder[motorC]) * CONVERSION_RATE < 
  ((2 - factor) * squareDistance + hubAndBoardDistance + squareDistance*0.2)) {}
    motor[motorC] = 0;
    board[bestPosition] = ROBOT;
    displayTextLine(5, "Please adjust pieces so");
    displayTextLine(6, "that the coloured face is up.");
    wait1Msec(5000);
}

/* 
Function Description: scans the board for where the player’s pieces are using the colour sensor
*/
void boardScan(int* getRobotTime, int* getPlayerTime, int* getPlayerMove, bool* settings){


	// Wait for button press to start
	clearTimer(T1);
	displayTextLine(4, "Press enter when piece placed.");
	while(!getButtonPress(ENTER_BUTTON)){};
	displayTextLine(4, "");
	while(getButtonPress(ENTER_BUTTON)){};
	displayTextLine(2, "");
	displayTextLine(3, "");
	displayTextLine(5, "");
	displayTextLine(6, "");
	*getPlayerTime += (int)time100(T1)/10;
	(*getPlayerMove)++;
	// Drive to the top row
	clearTimer(T1);
	nMotorEncoder[motorC] = 0;
	motor[motorC] = -10;
	while(abs(nMotorEncoder[motorC]) * CONVERSION_RATE < hubAndBoardDistance){}
	motor[motorC] = 0;

	nMotorEncoder[motorA] = 0;
	motor[motorA] = -25;
	while (abs(nMotorEncoder[motorA]) < 700) {}
	motor[motorA] = 0;

	// Scan each row
	for(int row = 0; row < 3; row++) {
		// Scan all three squares in the current row
		for(int col = 0; col < 3; col++) {
			// Take a color reading
			int index = col * 3 + row;
			wait1Msec(1000);
			if (settings[2]) {
				if (SensorValue[S1] == (int)colorRed) {
					board[index] = HUMAN;
					displayTextLine(5,"RED %d", index);
				}else{
					displayTextLine(5,"BLUE OR NOTHING %d", index);
				}
			}
			else {
				if (SensorValue[S1] == (int)colorBlue) {
					board[index] = HUMAN;
					displayTextLine(5,"BLUE %d", index);
				}	else{
				displayTextLine(5,"RED OR NOTHING %d", index);
				}
			}

			if (!(col == 2)) {
				nMotorEncoder[motorA] = 0;
				motor[motorA] = 25;
				while (abs(nMotorEncoder[motorA]) < 700) {}
				motor[motorA] = 0;
			}

			// Move to the next square within the row, if not at the end of the row
		}

		// Return motorB to the original position after scanning the row
		nMotorEncoder[motorA] = 0;
		motor[motorA] = -50;
		while (abs(nMotorEncoder[motorA]) < 1400) {}
		motor[motorA] = 0;

		// Move down to the next row, if not at the last row
		if (row < 2) {
			nMotorEncoder[motorC] = 0;
			motor[motorC] = -10;
			while(abs(nMotorEncoder[motorC]) * CONVERSION_RATE < squareDistance){};
			motor[motorC] = 0;
		}

	}

	nMotorEncoder[motorA] = 0;
	motor[motorA] = 25;
	while (abs(nMotorEncoder[motorA]) < 700) {}
	motor[motorA] = 0;
	*getRobotTime += time100(T1)/10;
	displayTextLine(5,"");
}

/* 
Function Description: Checks whether the move that was made is invalid based on comparing the previous state of the board vs the new state of the board.
*/
bool checkInvalidMove(int* prevState){
	int different = 0;
	for (int x = 0; x < 9; x++) {
		if (prevState[x] == EMPTY){
			if (board[x] != EMPTY){
				different++;
			}
		}else{
			if (prevState[x] != board[x]){
				return true;
			}
		}
	}

	return different != 1;
}


task main() {

	//declaring variables & robot is placed in the last row 
	int playerTime = 0;
	int robotTime = 0;
	int robotMoves = 0;
	int playerMoves = 0;
	int* obtainRobotTime;
	int* obtainPlayerTime;
	int* playerMoveGet = &playerMoves;
	obtainRobotTime = &robotTime;
	obtainPlayerTime = &playerTime;
	bool settings[3];


	calibrateSensors();

//moves robot back to the hub at the start to ensure the wheels are aligned 

	nMotorEncoder[motorC] = 0;
	motor[motorC] = 5;
	while (abs(nMotorEncoder[motorC]) * CONVERSION_RATE < 2 * squareDistance + hubAndBoardDistance) {}
	motor[motorC] = 0;
	wait1Msec(2000);

	//startUpFunction
	startUp(settings);
	int move = 0;

	while(checkIsWon() == UNKNOWN){
		//if players move
		if (settings[1]) {
			robotMoves++;
			int prevState[9];
			for (int i = 0; i < 9; i ++){
				prevState[i] = board[i];
			}
			boardScan(obtainRobotTime,obtainPlayerTime,playerMoveGet, settings);
			//checks if the move is invalid. 
			while(checkInvalidMove(prevState)){
				nMotorEncoder[motorC] = 0;
		    		motor[motorC] = 10;
		    		while (abs(nMotorEncoder[motorC]) * CONVERSION_RATE < 
      (2 * squareDistance + hubAndBoardDistance)) {}
		    		motor[motorC] = 0;
				displayTextLine(2, "You made an invalid move.");
				displayTextLine(3, "Correct it then push enter");
				for (int i = 0; i < 9; i ++){
					board[i] = prevState[i];
				}		
boardScan(obtainRobotTime,obtainPlayerTime,playerMoveGet, settings);
			}
			settings[1] = false;
		}
		//robots move
		else {
			if (move == 0) {
				nMotorEncoder[motorC] = 0;
				motor[motorC] = -10;
				while (abs(nMotorEncoder[motorC]) * CONVERSION_RATE < 
      (2 * squareDistance + hubAndBoardDistance)) {}
		   		motor[motorC] = 0;
			}
			clearTimer(T1);
			// finds best position
    			int bestPosition = 0;
	    		if (move != 0){
	    			bestPosition = getBestPosition(MAX_DEPTH);
	  		}
			pieceDrop(move, bestPosition);
			robotMoves++;
			robotTime += (int)time100(T1)/10;
			settings[1] = true;
		}
		move++;
	}
endGameProcedure(playerTime,robotTime,playerMoves,robotMoves,time100(T2));
}









