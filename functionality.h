/* PROGRAMMING FUNDAMENTAL'S PROJECT FOR FALL 2022 BS(CS)
 * You need to define the required function in the part instructed here below.
 * Avoid making any unnecessary changes, particularly the ones you don't understand.
 * The different pieces should be defined using an array - uncomment the following code once you are done creating the array.
 * TIP: Understand thoroughly before getting started with coding.
 * */

 //---Piece Starts to Fall When Game Starts---//
 /*

 fallingPiece

 This function increments the vertical value falling shape.
 If the shape hits other blocks vertically next shape starts falling.
 Bomb falss every 30 seconds.
 Every next shape is randomly chosen and random colour is given.
 Bomb falls in random column while all other shapes fall from left.
 Bomb destroys 4 blocks if colour does not match and destroys all the grid is colour is matched.
 */
void fallingPiece(float& timer, float& delay, int colors[], bool& bomb, float& bomb_time, bool& hardDrop, int nextPiece[][2], int shadowPiece[][2])
{

	//count variables count the shape that that falls after the bomb is intitalised in 3rd next piece.
	static int count = 0;
	//afterBomb counts the shape that falls after bomb has been deployed so bomb effects can be employed.
	static int afterBomb = 5;
	//Bomb sets true as the bomb is initialised in the 3rd next piece.
	static bool Bomb;

	//if timer is greater than delay increment in y-axis.
	if (timer > delay)
	{
		//point_2 is given the values of point_1 as if anamoloy becomes false point_1 original values can be used.
		//point_1 y values are incremented.
		for (int i = 0; i < 4; i++)
		{
			point_2[i][0] = point_1[i][0];
			point_2[i][1] = point_1[i][1];
			//How much units downward
			point_1[i][1] += 1;
		}

		//if blocks hits blocks already on the grid vertically, next piece is to fall.
		if (!anamoly())
		{
			//shape stores the random number that the shapes are to be made.
			int shape;

			//Once the bomb comes in the 3rd next piece count starts counting the blolcks as the third one is to bo bomb.
			if (Bomb)
			{
				count++;
			}
			//if bomb time is less than 30s other blocks are randomly selected.
			if (bomb_time < 30)
			{
				shape = rand() % 7;
			}
			//if bomb time is greater than 30s the 3rd next piece is going to be bomb.
			else
			{
				shape = 7;
				//bomb time is initialized with zero.
				bomb_time = 0;
				//Bomb is set true.
				Bomb = true;
			}

			//if count is equal to three then the falling piece is bomb.
			if (count == 3)
			{
				//bomb is set true.
				bomb = true;
				//Bomb is set false as now it becomes true when net Bomb comes.
				Bomb = false;
				//count is initialized with zero.
				count = 0;
				//afterBomb is initialised with zero.
				afterBomb = 0;
			}
			//if count is not true bomb is set to false.
			else
			{
				bomb = false;
			}

			//after bomb has fallen afterBomb starts counting next pieces.
			if (!bomb)
			{
				afterBomb++;
			}

			//bomb has fallen if afterBomb is equal to 1.
			//if bomb has fallen and it is on the last row nothing happens and bomb disappers.
			if ((afterBomb == 1) && (point_2[0][1] == (M - 1)))
			{
				gameGrid[point_2[0][1]][point_1[0][0]] = 0;
			}

			//but if bomb has fallen and bomb colour matches with the block bomb has fallen on all the grid is destroyed.
			else if ((afterBomb == 1) && (gameGrid[point_2[0][1] + 1][point_1[0][0]] == colors[0]))
			{
				for (int i = 0; i < M; i++)
				{
					for (int j = 0; j < N; j++)
					{
						gameGrid[i][j] = 0;
					}
				}
			}

			//but if bomb has fallen and bomb colour does not match with the block bomb has fallen on bomb destroys two right and one block below the bomb.
			else if (afterBomb == 1)
			{
				gameGrid[point_2[0][1]][point_1[0][0]] = 0;
				gameGrid[point_2[0][1] + 1][point_1[0][0]] = 0;
				gameGrid[point_2[0][1]][point_1[0][0] + 1] = 0;
				gameGrid[point_2[0][1] + 1][point_1[0][0] + 1] = 0;
			}

			//but if fallen block is not bomb it gets stored in the gameGrid array.
			else
			{
				for (int i = 0; i < 4; i++)
				{
					gameGrid[point_2[i][1]][point_1[i][0]] = colors[0];
				}
			}

			//colours are shifted as the blocks are now to be shifted and the nextPiece to come is given random colour.
			colors[0] = colors[1];
			colors[1] = colors[2];
			colors[2] = colors[3];
			colors[3] = 1 + rand() % 7;

			//all the blocks are shifted and the next block is given a random shape.
			for (int i = 0; i < 4; i++)
			{
				point_1[i][0] = nextPiece[i][0];
				point_1[i][1] = nextPiece[i][1];
				nextPiece[i][0] = nextPiece[i + 4][0];
				nextPiece[i][1] = nextPiece[i + 4][1];
				nextPiece[i + 4][0] = nextPiece[i + 8][0];
				nextPiece[i + 4][1] = nextPiece[i + 8][1];
				nextPiece[i + 8][0] = BLOCKS[shape][i] % 2;
				nextPiece[i + 8][1] = BLOCKS[shape][i] / 2;
			}

			//if the falling block is bomb give it a random column to fall.
			if (bomb)
			{
				//randomly selected column.
				int col = rand() % (N - 1);
				for (int i = 0; i < 4; i++)
				{
					point_1[i][0] += col;
				}
			}

			//hardDrop is set as false so the next piece can fall with normal delay.
			hardDrop = false;
		}
		//timer is set as zero after every increment in y-axis.
		timer = 0;
	}
}


/////////////////////////////////////////////
///*** START CODING YOUR FUNTIONS HERE ***///

/*

shadowFinished

This is actually anamoly function for the shadow piece.
This function check whether shadow has hit the floor or it has hit other blocks already on the grid.
its parameter is shadowPiece array.

*/

/////////////////Anamoloy function for shadow piece///////////////////
bool shadowFinished(int shadowPiece[][2])
{
	for (int i = 0; i < 4; i++)
	{
		//if shadowPiece touches the floor then return zero.
		if (shadowPiece[i][1] >= M)
			return 0;
		//if shadowPiece touches other blocks already on the grid then return zero.
		else if (gameGrid[shadowPiece[i][1]][shadowPiece[i][0]])
			return 0;
	}
	//If above conditions do not meet return 1.
	return 1;
}
/*

showShadow

First this function gives the value of point_1 to shadowPiece.
then till the shadow does not touch floor or other pieces it is incremented vertically.
As the shadow touches the floor or anyother block, it is decremented once and the function ends.

*/


///////////////////// showShadow /////////////////
void showShadow(int shadowPiece[][2])
{
	for (int i = 0; i < 4; i++)
	{
		//shadow Piece initialised with point_1 values.
		shadowPiece[i][0] = point_1[i][0];
		shadowPiece[i][1] = point_1[i][1];
	}

	//till shadowPiece do not touch other block or the floor increment its y-axis.
	while (shadowFinished(shadowPiece))
	{
		for (int i = 0; i < 4; i++)
		{
			shadowPiece[i][1] += 1;
		}
	}

	//decrement vertical values of shadowPiece else it will go into other pieces.
	for (int i = 0; i < 4; i++)
	{
		shadowPiece[i][1] -= 1;
	}
}


/*

move

this function moves the pices horizontally when right or left key is pressed.
then it increments point_0 x-axis with the value of delta_x.
if the block touches left wall or right wall, or it goes into other another block then reset it back to its original value.

*/

//////////Move////////////
void move(int& delta_x)
{

	//increment delta_x horizontally in point_1 values.
	if (delta_x)
	{
		for (int i = 0; i < 4; i++)
			point_1[i][0] += delta_x;

		//if block touches left, right walls or goes into other blolcks reset it to original values.
		if (!anamoly())
			for (int i = 0; i < 4; i++)
				point_1[i][0] -= delta_x;
	}
}

/*

clearLines

This function checks if any horizontal line is filled and then resets that row with the row above.
Increment the current_score according to the lines removed.

*/

//////////Clear Lines////////////

void clearLines(int& current_score, int& lines_cleared)
{

	//start checking row from the bottom.
	int checkRow = M - 1;
	//LinesRemoved set to zero.
	int linesRemoved = 0;

	//Start checking every row from the bottom.
	for (int row = (M - 1); row >= 0; row--)
	{
		//blocksPresent set to zero for every new row.
		int blocksPresent = 0;

		//start checking for blocks for every row from left.
		for (int col = 0; col < N; col++)
		{
			//increment blocksPresent if there is any block at that position.
			if (gameGrid[row][col])
			{
				blocksPresent++;
			}
			//if any row is filled with blocks then upper row is shifted downwards.
			gameGrid[checkRow][col] = gameGrid[row][col];
		}

		//if the number of blockspresent is less than no of columns then go to upper row.
		if (blocksPresent < N)
			checkRow--;
		//if number of blockspresent is equal to no of columns then increment no of lines to be removed.
		else
			linesRemoved++;
	}

	//increment current_score according to the lines removed.
	if (linesRemoved == 1)
		current_score += 10;
	else if (linesRemoved == 2)
		current_score += 30;
	else if (linesRemoved == 3)
		current_score += 60;
	else if (linesRemoved >= 4)
		current_score += 100;

	//Number of lines_cleared is incremented with linesRemoved.
	lines_cleared += linesRemoved;
}

/*

Rotate

This function rotates the falling block if up key is pressed.

*/

//////////Rotate////////////
void Rotate(bool& rotate)
{
	if (rotate)
	{
		for (int i = 0; i < 4; i++)
		{
			//Assigning points 2 the values of point 1.

			point_2[i][0] = point_1[i][0];
			point_2[i][1] = point_1[i][1];

			//Point of axis are [1][0] and [1][1]
			//saving the difference the of verticlal blocks from axis point in x-axis.
			//saving the difference the of horizontal blocks from axis point in y-axis.
			int x_axis = point_1[i][1] - point_1[1][1];
			int y_axis = point_1[i][0] - point_1[1][0];

			//The blocks from right of centre point goes downwards and from left upwards.
			//The blocks from upwards goes towards right and downwards toward left.
			point_1[i][0] = point_1[1][0] - x_axis;
			point_1[i][1] = point_1[1][1] + y_axis;
		}

		//if after rotating the blocks sets anamoly then reset the point_1 array.
		if (!anamoly())
		{
			for (int i = 0; i < 4; i++)
			{
				point_1[i][0] = point_2[i][0];
				point_1[i][1] = point_2[i][1];
			}
		}
	}
}


/*

gameOver

This function checks if the blocks in the grid has touched the roof.

*/

//////////Game Over////////////
bool gameOver()
{
	for (int column = 0; column < N; column++)
	{
		if (gameGrid[0][column] || gameGrid[1][column])
			return true;
	}
	return false;
}

///*** YOUR FUNCTIONS END HERE ***///
/////////////////////////////////////
