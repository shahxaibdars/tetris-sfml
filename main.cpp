#include <SFML/Graphics.hpp>
#include <time.h>
#include "utils.h"
#include "pieces.h"
#include "functionality.h"


using namespace sf;
int main()
{
	srand(time(0));
	RenderWindow window(VideoMode(720, 480), title);
	//Declaring objects for the images to be load.
	Texture obj1, obj2, obj3, obj5, obj6;

	// loading the images from img folder to be displayed.
	obj1.loadFromFile("img/tiles.png");
	obj2.loadFromFile("img/background.png");
	obj3.loadFromFile("img/shadow.png");
	obj5.loadFromFile("img/paused.png");
	obj6.loadFromFile("img/gameover.png");

	sf::Font font;
	font.loadFromFile("arial.ttf");

	//Creating sprite objects from the loaded images.
	Sprite sprite(obj1), background(obj2), shadow(obj3), paused(obj5), GameOver(obj6);

	//declaring and initializing the variables for the program.
	//delta_x for the x-axis movement.
	//current_score for storing the score.
	//current_level for the level user is on. It updates after every five minutes.
	//lines_cleared for storing the number of lines cleared.
	int delta_x = 0, current_score = 0, current_level = 1, lines_cleared = 0;

	//timer for storing time till it is equal to delay.
	//total_time for storing total time.
	//bomb_time for storing time till it is equal to 30s. It is the after after which bomb falls.
	float timer = 0, delay = 0.5, total_time = 0, bomb_time = 0;

	//rotate becomes true if up arrow pressed.
	//bomb becomes true if the cutting falling piece is bomb.
	//pause becomes true if P pressed.
	//hardDrop becomes if space pressed.
	bool rotate = false, bomb = false, pause = false, hardDrop = false;

	//colors array to store colours array for the falling piece and next pieces.
	int colors[4] = {};
	//nextPiece array to hold next three pieces.
	int nextPiece[12][2];
	//shadowPiece array to hold the shadow.
	int shadowPiece[4][2];

	//n1,n2,n3,n4 for selecting the randam shape of the first four pieces.
	int n1 = rand() % 7;
	int n2 = rand() % 7;
	int n3 = rand() % 7;
	int n4 = rand() % 7;


	//Assigning random shapes to the first four pieces.
	for (int i = 0; i < 4; i++)
	{
		colors[i] = 1 + rand() % 7;
		point_1[i][0] = BLOCKS[n4][i] % 2;
		point_1[i][1] = BLOCKS[n4][i] / 2;
		nextPiece[i][0] = BLOCKS[n1][i] % 2;
		nextPiece[i][1] = BLOCKS[n1][i] / 2;
		nextPiece[i + 4][0] = BLOCKS[n2][i] % 2;
		nextPiece[i + 4][1] = BLOCKS[n2][i] / 2;
		nextPiece[i + 8][0] = BLOCKS[n3][i] % 2;
		nextPiece[i + 8][1] = BLOCKS[n3][i] / 2;
	}

	//Creating object of Clock.
	Clock clock;

	//Game starts as the window opens.
	while (window.isOpen())
	{
		// getting time in time variable using builtin functions.
		float time = clock.getElapsedTime().asSeconds();
		//restarting the clock after every iteration.
		clock.restart();

		//timer, total_time and bomb_time values are incremented with time value if the game is not paused and it is not over.
		if ((!pause) && (!gameOver()))
		{
			timer += time;
			total_time += time;
			bomb_time += time;
		}

		//---Event Listening Part---//
		Event e;
		//Event is occurring - until the game is in running state
		while (window.pollEvent(e))
		{
			//If cross/close is clicked/pressed
			if (e.type == Event::Closed)
				//Opened window disposes
				window.close();

			//If any other key (not cross) is pressed
			if (e.type == Event::KeyPressed)
			{

				//if game is not paused and it is not over.
				if (!pause && !gameOver())
				{
					//Check if the other key pressed is UP key
					if (e.key.code == Keyboard::Up)
						//Rotation gets on
						rotate = true;
					//Check if the other key pressed is LEFT key
					else if (e.key.code == Keyboard::Left)
						//Change in X-Axis - Negative
						delta_x = -1;
					//Check if the other key pressed is RIGHT key
					else if (e.key.code == Keyboard::Right)
						//Change in X-Axis - Positive
						delta_x = 1;
					//Check if the other key pressed is SPACE key
					else if (e.key.code == Keyboard::Space)
						//hardDrop gets on
						hardDrop = true;
				}

				//Check if the key pressed is P
				if (e.key.code == Keyboard::P)
					//pause value is inverted.
					pause = !pause;
			}
		}
		//Just another way to detect key presses without event listener
		if (Keyboard::isKeyPressed(Keyboard::Down))
			//If DOWN key is pressed, delay in falling piece decreases from 0 to 0.05
			delay = 0.05;

	   //if game is not paused and it is not over then all the functions will work.
		if ((!pause) && (!gameOver()))
		{
			//Example: fallingPiece() function is called here
			fallingPiece(timer, delay, colors, bomb, bomb_time, hardDrop, nextPiece, shadowPiece);
			//Shadow is displayed through this function.
			showShadow(shadowPiece);
			//Lines the filled up rows are cleared.
			clearLines(current_score, lines_cleared);
			//falling Piece is bomb it can not rotate or move.
			if (!bomb)
			{
				//to rotate the falling block.
				Rotate(rotate);
				//to move right or left.
				move(delta_x);

			}
		}

		///*** YOUR CALLING STATEMENTS END HERE ***///
		//////////////////////////////////////////////

		//Resetting the values for delta_x,rotate,delay.
		delta_x = 0;
		rotate = false;
		//Resetting the values for delay.
		if (!hardDrop)
			delay = 0.5;
		else
			delay = 0;

		//Creating new_time string variable to hold time passed value.
		int minutes = total_time / 60;
		int seconds = total_time - (minutes * 60);
		std::string new_time = std::to_string(minutes) + " : " + std::to_string(seconds);

		///////////////////Text Outputs //////////////////

		// Declaring Text to be displayed
		sf::Text TotalTime, score, lines, level;

		// Setting common elements like font, fontsize, color, style once.
		TotalTime.setFont(font);
		TotalTime.setFillColor(sf::Color::Red);
		TotalTime.setStyle(sf::Text::Bold);
		TotalTime.setCharacterSize(20);

		//Passing the common elements to other text variables.
		score = lines = level = TotalTime;

		//Declaring text to be displayed different for every element.
		TotalTime.setString(new_time);
		score.setString(std::to_string(current_score));
		lines.setString(std::to_string(lines_cleared));
		level.setString(std::to_string(current_level));

		//Setting their positions.
		level.setPosition(560.f, 330.f);
		TotalTime.setPosition(630.f, 330.f);
		score.setPosition(560.f, 230.f);
		lines.setPosition(650.f, 230.f);

		///////////////Text Output End Here///////////////////

		//everything on the window is cleared.
		window.clear(Color::Black);

		//if game is not over and it is not paused these display will be seen on screen.
		if ((!gameOver()) && (!pause))
		{
			window.draw(background);
			/*sf::Music music;
			if (!music.openFromFile("sound.ogg"))
				return -1; // error
			music.play();
			*/

			//This loop displays the static blocks that are stored in the gameGrid array.
			for (int i = 0; i < M; i++)
			{
				for (int j = 0; j < N; j++)
				{
					if (gameGrid[i][j] == 0)
						continue;
					sprite.setTextureRect(IntRect(gameGrid[i][j] * 18, 0, 18, 18));
					sprite.setPosition(j * 18, i * 18);
					sprite.move(28, 31); //offset
					window.draw(sprite);
				}
			}


			//This loop displays the falling piece stored in point_1 array.
			for (int i = 0; i < 4; i++)
			{
				sprite.setTextureRect(IntRect(colors[0] * 18, 0, 18, 18));
				sprite.setPosition((point_1[i][0]) * 18, point_1[i][1] * 18);
				sprite.move(28, 31);
				window.draw(sprite);
			}

			//Setting opacity for the shadow block.
			int opacity = 100;
			//bomb is made up of four blocks that the same place so its opacity must be reduced with ratio of 4.
			if (bomb)
				opacity /= 4;

			//This loop displays the shadow of every block.
			for (int i = 0; i < 4; i++)
			{
				shadow.setTextureRect(IntRect(colors[0] * 18, 0, 18, 18));
				shadow.setPosition((shadowPiece[i][0]) * 18, shadowPiece[i][1] * 18);
				shadow.setColor(sf::Color(250, 250, 250, opacity));
				shadow.move(28, 31);
				window.draw(shadow);
			}

			//This loop displays the first next piece on the right side of screen.
			for (int i = 0; i < 4; i++)
			{
				sprite.setTextureRect(IntRect(colors[1] * 18, 0, 18, 18));
				sprite.setPosition(nextPiece[i][0] * 18, nextPiece[i][1] * 18);
				sprite.move(400, 140);
				window.draw(sprite);
			}

			//This loop displays the second next piece on the right side of screen.
			for (int i = 0; i < 4; i++)
			{
				sprite.setTextureRect(IntRect(colors[2] * 18, 0, 18, 18));
				sprite.setPosition(nextPiece[i + 4][0] * 18, nextPiece[i + 4][1] * 18);
				sprite.move(400, 220);
				window.draw(sprite);
			}

			//This loop displays the third next piece on the right side of screen.
			for (int i = 0; i < 4; i++)
			{
				sprite.setTextureRect(IntRect(colors[3] * 18, 0, 18, 18));
				sprite.setPosition(nextPiece[i + 8][0] * 18, nextPiece[i + 8][1] * 18);
				sprite.move(400, 300);
				window.draw(sprite);
			}


			//---The Final on Which Everything is Drawn Over is Loaded---//
			window.draw(score);
			window.draw(TotalTime);
			window.draw(level);
			window.draw(lines);
		}

		//If the game is not over but it is paused then show the paused menu.
		else if ((pause) && (!gameOver()))
		{
			window.draw(paused);
		}

		//If game is over.
		else if (gameOver())
		{
			//Display gameover window.
			window.draw(GameOver);

			//Updating Positions and size for the text to be displayed.

			level.setCharacterSize(50);
			score.setCharacterSize(50);
			lines.setCharacterSize(50);
			TotalTime.setCharacterSize(50);
			level.setPosition(480.f, 360.f);
			TotalTime.setPosition(480.f, 160.f);
			score.setPosition(145.f, 360.f);
			lines.setPosition(160.f, 160.f);

			//---The Final on Which Everything is Drawn Over is Loaded---//
			window.draw(score);
			window.draw(TotalTime);
			window.draw(level);
			window.draw(lines);
		}
		//---The Window that now Contains the Frame is Displayed---//
		window.display();
	}
	return 0;
}
