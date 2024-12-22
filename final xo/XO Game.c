#include "./headers/tm4c123gh6pm.h"
#include "./headers/Nokia5110.h"
#include "./headers/TExaS.h"
#include "./headers/GameArt.h"
#include "./headers/XO Game.h"
#include "./headers/Timer.h"
#include "./headers/LED.h"

unsigned long Timer;

int playedCell, currentCell,Xwin = 0, Owin = 0;

char turn,GameArray[9];

extern unsigned char Sw1, Sw2;

void GameIntro(void)
{
	Nokia5110_SetCursor(1, 2);			   // set cursor at first X place on the screen and the 3rd y to be nearly in the middle
	Nokia5110_OutString(" XO GAME");	   // write this string to screen
	Timer2(50);				// 5 seconds
	Nokia5110_Clear(); // clear the whole screen
	Nokia5110_SetCursor(5, 2);
	Nokia5110_OutString("X");
	Nokia5110_SetCursor(4, 4);
	Nokia5110_OutString("Start");
	Set_Led_Pin();
	Timer2(50);			// 5 seconds
	Clear_Led_Pin();
	Nokia5110_Clear(); // clear the whole screen
}

void GameInitialization()
{
	int i;
	playedCell = 0;  // make all cells available to play at
	turn = 'X';	  // first player is always X
	currentCell = -1; // no cells are hovered or selected
	for ( i = 0; i < 9; i++)
		GameArray[i] = ' '; // erase all drawings in cells matrix

	Set_Led_2();
	
}

void DrawClearGameMatrix()
{
	Nokia5110_ClearBuffer();
	// square is defined in gameArt to make the shape of the 9 squares of the game
	Nokia5110_PrintBMP(0, (SHEIGHT - 1), square, 0); // Nokia5110_printBMP(x_cor , y_cor , pointer_to_image (square or VL) , threshold(0 or 1))
	Nokia5110_DisplayBuffer();
	Timer2(1);

	Nokia5110_PrintBMP((SHEIGHT - 1) + 3, (SHEIGHT - 1), square, 0);
	Nokia5110_DisplayBuffer();
	Timer2(1);

	Nokia5110_PrintBMP(2 * (SHEIGHT - 1) + 6, (SHEIGHT - 1), square, 0);
	Nokia5110_DisplayBuffer();
	Timer2(1);

	Nokia5110_PrintBMP(0, 2 * (SHEIGHT - 1) + 3, square, 0);
	Nokia5110_DisplayBuffer();
	Timer2(1);

	Nokia5110_PrintBMP((SHEIGHT - 1) + 3, 2 * (SHEIGHT - 1) + 3, square, 0);
	Nokia5110_DisplayBuffer();
	Timer2(1);

	Nokia5110_PrintBMP(2 * (SHEIGHT - 1) + 6, 2 * (SHEIGHT - 1) + 3, square, 0);
	Nokia5110_DisplayBuffer();
	Timer2(1);

	Nokia5110_PrintBMP(0, 3 * (SHEIGHT - 1) + 6, square, 0);
	Nokia5110_DisplayBuffer();
	Timer2(1);

	Nokia5110_PrintBMP((SHEIGHT - 1) + 3, 3 * (SHEIGHT - 1) + 6, square, 0);
	Nokia5110_DisplayBuffer();
	Timer2(1);

	Nokia5110_PrintBMP(2 * (SHEIGHT - 1) + 6, 3 * (SHEIGHT - 1) + 6, square, 0);
	Nokia5110_DisplayBuffer();
	Timer2(1);

	Nokia5110_DisplayBuffer();

	displayStatus(); // to show whose player turn is it now
}


void RunGame()
{
	int row, col;
	int lastRow, lastCol, lastCell;

	if (Sw1)
	{
		Sw1 = 0;
		lastCell = currentCell; // store the cell location before moving to next one
		currentCell++;
		if (currentCell == 9) currentCell = 0;   // loop back to the beginning
												
		row = currentCell / 3; 
		col = currentCell % 3;

		// while hovered
		if (GameArray[currentCell] == 'X')
		{
			Nokia5110_PrintBMP(col * (SHEIGHT - 1) + 3 * col, (row + 1) * (SHEIGHT - 1) + 3 * row, selectedX, 0);
		}
		else if (GameArray[currentCell] == 'O')
		{
			Nokia5110_PrintBMP(col * (SHEIGHT - 1) + 3 * col, (row + 1) * (SHEIGHT - 1) + 3 * row, selectedO, 0);
		}
		else
		{
			Nokia5110_PrintBMP(col * (SHEIGHT - 1) + 3 * col, (row + 1) * (SHEIGHT - 1) + 3 * row, selectedSquare, 0);
		}

		// while unhovered get the last indicies and draw previous art
		if (lastCell == 8)
		{
			lastRow = 2;
			lastCol = 2;
			lastCell = 8;
		}
		else
		{
			lastRow = (currentCell - 1) / 3;
			lastCol = (currentCell - 1) % 3;
		}

		if (GameArray[lastCell] == 'X')
		{
			Nokia5110_PrintBMP(lastCol * (SHEIGHT - 1) + 3 * lastCol, (lastRow + 1) * (SHEIGHT - 1) + 3 * lastRow, X, 0);
		}
		else if (GameArray[lastCell] == 'O')
		{
			Nokia5110_PrintBMP(lastCol * (SHEIGHT - 1) + 3 * lastCol, (lastRow + 1) * (SHEIGHT - 1) + 3 * lastRow, O, 0);
		}
		else
		{
			Nokia5110_PrintBMP(lastCol * (SHEIGHT - 1) + 3 * lastCol, (lastRow + 1) * (SHEIGHT - 1) + 3 * lastRow, square, 0); // not to leave the last cell hoovered
		}
		Nokia5110_DisplayBuffer();
		displayStatus();
		Timer2(20);
	}
	if (Sw2 && currentCell != -1)
	{
		Sw2 = 0;
		if (GameArray[currentCell] == ' ')
		{ 
			row = currentCell / 3;
			col = currentCell % 3;
			// If Current Player is X

			if (turn == 'X')
			{

				Nokia5110_PrintBMP(col * (SHEIGHT - 1) + 3 * col, (row + 1) * (SHEIGHT - 1) + 3 * row, X, 0); // draw x in the chosen cell
				GameArray[currentCell] = turn;																		// fill the matrix and take that place for X
				Clear_Led_2();
				Set_Led_3();
				if (checkWinner(turn))
				{ // if strike
					Display_Winner(turn);
					if (CheckPlayAgain())
					{ // want to play again?
						GameInitialization();
						DrawClearGameMatrix();
						return;
					}
					else
					{			   
						EndGame(); // goodBye
						while (1)
							; // an unbreakable infinite loop after clearing screen by EndGame()
					}
				}
				
				turn = 'O';
			}
			else
			{

				Nokia5110_PrintBMP(col * (SHEIGHT - 1) + 3 * col, (row + 1) * (SHEIGHT - 1) + 3 * row, O, 0);
				GameArray[currentCell] = turn;
				Clear_Led_3();
				Set_Led_2();
				if (checkWinner(turn))
				{
					Display_Winner(turn);
					if (CheckPlayAgain())
					{
						GameInitialization();
						DrawClearGameMatrix();
						return;
					}
					else
					{
						EndGame();
						while (1)
							;
					}
				}
				// if game has no winner yet switch to player X
				turn = 'X';
			}
			Nokia5110_DisplayBuffer();
			displayStatus();
			playedCell++;
			if (playedCell== 9) // Game finished with no winner
			{
				displayDraw();
				if (CheckPlayAgain())
				{
					GameInitialization();
					DrawClearGameMatrix();
					return;
				}
				else
				{
					EndGame();
					while (1)
						;
				}
			}
		}
		else // if the cell was already taken
		{
			Nokia5110_SetCursor(8, 3);
			Nokia5110_OutString("Not");
			Nokia5110_SetCursor(8, 4);
			Nokia5110_OutString("Your");
			Nokia5110_SetCursor(8, 5);
			Nokia5110_OutString("Spot");

			// Blink  LedS 3 times
			Blink_LEDS();

			displayStatus();

		}
		Timer2(20); // 2 seconds
	}
}


int checkWinner(char player)
{int Winning_cell_1 = 0, Winning_cell_2 = 0, Winning_cell_3 = 0, i = -1; // Initialize i with a default value that won't match any case

// Check for a winning combination, then set the value of 'i'
if (GameArray[0] == player && GameArray[1] == player && GameArray[2] == player) 
	 i = 0;
else if (GameArray[3] == player && GameArray[4] == player && GameArray[5] == player)
    i = 1;
else if (GameArray[6] == player && GameArray[7] == player && GameArray[8] == player)
    i = 2;
else if (GameArray[0] == player && GameArray[3] == player && GameArray[6] == player)
    i = 3;
else if (GameArray[1] == player && GameArray[4] == player && GameArray[7] == player)
    i = 4;
else if (GameArray[2] == player && GameArray[5] == player && GameArray[8] == player)
    i = 5;
else if (GameArray[0] == player && GameArray[4] == player && GameArray[8] == player)
    i = 6;
else if (GameArray[2] == player && GameArray[4] == player && GameArray[6] == player)
    i = 7;

//assign the winning cells
switch(i) {
    case 0:
        Winning_cell_1 = 0;
        Winning_cell_2 = 1;
        Winning_cell_3 = 2;
        break;
    case 1:
        Winning_cell_1 = 3;
        Winning_cell_2 = 4;
        Winning_cell_3 = 5;
        break;
    case 2:
        Winning_cell_1 = 6;
        Winning_cell_2 = 7;
        Winning_cell_3 = 8;
        break;
    case 3:
        Winning_cell_1 = 0;
        Winning_cell_2 = 3;
        Winning_cell_3 = 6;
        break;
    case 4:
        Winning_cell_1 = 1;
        Winning_cell_2 = 4;
        Winning_cell_3 = 7;
        break;
    case 5:
        Winning_cell_1 = 2;
        Winning_cell_2 = 5;
        Winning_cell_3 = 8;
        break;
    case 6:
        Winning_cell_1 = 0;
        Winning_cell_2 = 4;
        Winning_cell_3 = 8;
        break;
    case 7:
        Winning_cell_1 = 2;
        Winning_cell_2 = 4;
        Winning_cell_3 = 6;
        break;
    default:
        //no winner is found
        break;
}

// If a winning combination is found, display the result
if (Winning_cell_1 || Winning_cell_2 || Winning_cell_3) {
    if (player == 'X') {
        // Handle 'X' player winning
        Nokia5110_PrintBMP((Winning_cell_1 % 3) * (SHEIGHT - 1) + 3 * (Winning_cell_1 % 3), (Winning_cell_1 / 3 + 1) * (SHEIGHT - 1) + 3 * (Winning_cell_1 / 3), selectedX, 0);
        Nokia5110_PrintBMP((Winning_cell_2 % 3) * (SHEIGHT - 1) + 3 * (Winning_cell_2 % 3), (Winning_cell_2 / 3 + 1) * (SHEIGHT - 1) + 3 * (Winning_cell_2 / 3), selectedX, 0);
        Nokia5110_PrintBMP((Winning_cell_3 % 3) * (SHEIGHT - 1) + 3 * (Winning_cell_3 % 3), (Winning_cell_3 / 3 + 1) * (SHEIGHT - 1) + 3 * (Winning_cell_3 / 3), selectedX, 0);
        Clear_Led_Pin();
        Blink_LEDS_2();
        Clear_Led_Pin();
        GPIO_PORTF_DATA_R = 0x04; // LED is blue (X won)
        Nokia5110_DisplayBuffer();
    } else {
        // Handle 'O' player winning
        Nokia5110_PrintBMP((Winning_cell_1 % 3) * (SHEIGHT - 1) + 3 * (Winning_cell_1 % 3), (Winning_cell_1 / 3 + 1) * (SHEIGHT - 1) + 3 * (Winning_cell_1 / 3), selectedO, 0);
        Nokia5110_PrintBMP((Winning_cell_2 % 3) * (SHEIGHT - 1) + 3 * (Winning_cell_2 % 3), (Winning_cell_2 / 3 + 1) * (SHEIGHT - 1) + 3 * (Winning_cell_2 / 3), selectedO, 0);
        Nokia5110_PrintBMP((Winning_cell_3 % 3) * (SHEIGHT - 1) + 3 * (Winning_cell_3 % 3), (Winning_cell_3 / 3 + 1) * (SHEIGHT - 1) + 3 * (Winning_cell_3 / 3), selectedO, 0);
        Clear_Led_Pin();
        Blink_LEDS_3();
        Clear_Led_Pin();
        GPIO_PORTF_DATA_R = 0x08; // LED is green  (O won)
        Nokia5110_DisplayBuffer();
    }
    // Draw the strike line with pixels (dots) on the 3 hovered winning cells
    switch(i) {
			  int j;
        case 0: 
            for ( j = 0; j < 48; j++)
                Nokia5110_SetPixel(j, 6);
            break;
        case 1: 
            for ( j = 0; j < 48; j++)
                Nokia5110_SetPixel(j, 23);
            break;
        case 2: 
            for ( j = 0; j < 48; j++)
                Nokia5110_SetPixel(j, 40);
            break;
        case 3: 
            for ( j = 0; j < 48; j++)
                Nokia5110_SetPixel(6, j);
            break;
        case 4: 
            for ( j = 0; j < 48; j++)
                Nokia5110_SetPixel(23, j);
            break;
        case 5: 
            for (j = 0; j < 48; j++)
                Nokia5110_SetPixel(40, j);
            break;
        case 6: 
            for ( j = 0; j < 48; j++)
                Nokia5110_SetPixel(j, j);
            break;
        case 7: 
            for ( j = 0; j < 48; j++)
                Nokia5110_SetPixel(j, 47 - j);
            break;
        default:
            break;
    }

    Nokia5110_DisplayBuffer();
    Timer2(5);
    GPIO_PORTF_DATA_R = 0x00; // LED is dark (end of round)
    Clear_Led_Pin();
    return 1; // There is a winner
}

return 0;// No winner

}

void Display_Winner(char player)
{
	if(player == 'X')
		Xwin++;
  else	
		Owin++; 
	Nokia5110_ClearBuffer();
	Nokia5110_SetCursor(2, 3);
	Nokia5110_OutChar(player);
	Nokia5110_SetCursor(4, 4);
	Nokia5110_OutString("wins");
	Nokia5110_DisplayBuffer();
	Timer2(50);
	Nokia5110_ClearBuffer();
	Nokia5110_SetCursor(2, 3);
	Nokia5110_OutString("X: ");
	Nokia5110_OutUDec(Xwin);
	Nokia5110_SetCursor(2, 4);
	Nokia5110_OutString("O: ");
	Nokia5110_OutUDec(Owin);
	Timer2(50);
	Nokia5110_Clear(); 
}

void displayDraw()
{
	GPIO_PORTF_DATA_R = 0x0E; 
	Blink_LEDS();
	Timer2(5);
	GPIO_PORTF_DATA_R = 0x00;
	Nokia5110_ClearBuffer();
	Nokia5110_DisplayBuffer();
	Nokia5110_SetCursor(2, 3);
	Nokia5110_OutString("No");
	Nokia5110_SetCursor(2, 4);
	Nokia5110_OutString("Winner");
	Timer2(70);
	Nokia5110_Clear();
}

void displayStatus()
{
	Nokia5110_SetCursor(9, 2);
	Nokia5110_OutChar(turn);
	Nokia5110_SetCursor(8, 3);
	Nokia5110_OutString("Turn");
}

int CheckPlayAgain()
{
	Nokia5110_ClearBuffer();
	Nokia5110_SetCursor(1, 1);
	Nokia5110_OutString("Play Again?");
	Nokia5110_SetCursor(2, 3);
	Nokia5110_OutString("SW1 - SW2");
	Nokia5110_SetCursor(2, 4);
	Nokia5110_OutString("Yes - No");
	while(1){
		Timer2(10);
		if (Sw1 == 1)
		{
			Sw1 = 0;
			return 1;
		}

		else if(Sw2==1)
		{
			Sw2 = 0;
			return 0;
		}
	}
}

void EndGame()
{
	Nokia5110_Clear();
	Nokia5110_SetCursor(2, 3);
	Nokia5110_OutString("Goodbye");
	Set_Led_Pin();
	Timer2(20);
	Clear_Led_Pin();
	Nokia5110_Clear(); // Clear for nokia lcd
}
