//---------------------------------------------------------------------------
//Program: Skeleton for Task 1c � group assignment
//Author: Casey-Jo Kenny, Josh Long, Kai Henry
//Last updated: 05/04/2019
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//----- include libraries
//---------------------------------------------------------------------------

//include standard libraries
#include <iostream>	
#include <iomanip> 
#include <conio.h> 
#include <cassert> 
#include <string>
#include <sstream>
#include <vector>
#include <fstream>

using namespace std;

//include our own libraries
#include "RandomUtils.h"    //for seed, random
#include "ConsoleUtils.h"	//for clrscr, gotoxy, etc.
#include "TimeUtils.h"		//for getSystemTime, timeToString, etc.

//---------------------------------------------------------------------------
//----- define constants
//---------------------------------------------------------------------------

//defining the size of the grid

const int  SIZEX(20);    	//horizontal dimension
const int  SIZEY(15);		  //vertical dimension

const int  TWOSIZEX(25);    	//horizontal dimension (For Two Player Grid)
const int  TWOSIZEY(20);		  //vertical dimension (For Two Player Grid)


//defining symbols used for display of the grid and content

const char HEAD('0');   	//head
const char TAIL('o');     //tail
const char PLAYERTWOHEAD('O');   	//head
const char PLAYERTWOTAIL('c');     //tail
const char MOUSE('@');		//mouse
const char TUNNEL(' ');   //tunnel
const char WALL('#');    	//border
const char PILL('+');     //pill
const char MONGOOSE('M');     //mongoose

//defining the command letters to move the spot on the maze

const int  UP(72);			  //up arrow
const int  DOWN(80); 		  //down arrow
const int  RIGHT(77);		  //right arrow
const int  LEFT(75);		  //left arrow

const char  PTUP('W');			  //up arrow
const char  PTDOWN('S'); 		  //down arrow
const char  PTRIGHT('D');		  //right arrow
const char  PTLEFT('A');		  //left arrow

//defining the other command letters

const char QUIT('Q');		  //to end the game
const char CHEAT('C');     //to toggle cheat mode

const int miceNeeded = 10; //amount of mice needed for the player to win (game to end)


//---------------------------------------------------------------------------
//----- Structures
//---------------------------------------------------------------------------

struct Item
{
  int x, y;
  char symbol;
 
};

struct GameItems //Structure to group all of the Items used in the game together
{
  vector<Item> snake = { { 0, 0, HEAD },{0,0,TAIL}, {0,0,TAIL}, {0,0,TAIL} };//Creates the snake
  vector<Item> snake2 = { { 0, 0, PLAYERTWOHEAD },{0,0,PLAYERTWOTAIL}, {0,0,PLAYERTWOTAIL}, {0,0,PLAYERTWOTAIL} };//Creates the snake for Player 2
  Item mouse = { 0 ,0, MOUSE }; //Creates the Mouse
  Item pill = { 0, 0, PILL }; //Creates the Pill
  Item mongoose = { 0, 0, MONGOOSE }; //Creates the Mongoose

} gameItems;

struct ScoreData //Structure to group all of the Items used in the game together
{
  int MaxChar = 20; //Sets the max characters for the name as 20
  string name; //Stores the players name
  int score = 0; //Creates the variable to store the score for the player
  int highScore = 0; //Creates a variable to store the highScore for the player

} scoreData;

struct MouseData //Structure to group all of the vairables used for the mouse in the game together
{
  int miceCollected = 0; //Creates a variable to store how many mice the player has collected
  int mouseCount = 0; //Creates a variable to store the amount of mice has been collected to be used to show when the pill should spawn

} mouseData;

struct CheatModeData //Structure to group all of thevairables used for the cheat mode in the game together
{
  bool cheatModeOn = false; //Creates a bool to store if the player has turned cheating on
  bool noScore = false; //Creates a bool to store if the player has ever turned cheating on (used to save score or not)

} cheatModeData;

struct PillData //Structure to group all of the vairables used for the pill in the game together
{
  bool pillActive = false; //Creates a bool to store if a pill is active on the map or not
  int pillMoveCount = 0; //Creates a variable to keep track of how many moves have passed since the pill has spawned (so it can despawn)

} pillData;

struct EndGameData //Structure to group all of the vairables used to detech the end of the game together
{
  bool died = false; //Creates a bool to see if the snake has died
  bool wonGame = false; //Creates a bool to store if the player has won the game

} endGameData;


struct InvincibilityData
{
  int invincibleMoves = 20;
  bool Invincible = false;

} invincibilityData;

struct TwoPlayerData
{
  int Players;
  bool twoPlayerGame = false;
  int playerOneScore;
  int playerTwoScore;
  bool playerOneDied = false;
  bool playerTwoDied = false;

} twoPlayerData;

struct LevelData
{
	int level;
	bool unlock1 = false;
	bool unlock2 = false;
	bool allUnlocked = false;
} levelData;

//---------------------------------------------------------------------------
//----- run game
//---------------------------------------------------------------------------

int main()
{
  //function declarations (need to go through and make sure parameters are passed properly & or no &
  void GetAmountOfPlayers(TwoPlayerData& twoPlayerData);
  void TwoPlayerOption(TwoPlayerData& twoPlayerData);
  void getPlayerName(ScoreData& scoreData);
  void initialiseGame(char grid[][SIZEX], char maze[][SIZEX], GameItems& gameItems, PillData& pillData, MouseData& mouseData, bool& mongooseSpawn, LevelData& levelData); //Initialises the game
  void renderGame(const char g[][SIZEX], const string& mess, ScoreData& scoreData, MouseData& mouseData, InvincibilityData& invincibilityData);
  void updateGame(char g[][SIZEX], const char m[][SIZEX], GameItems& gameItems, const int kc, string& mess, EndGameData& endGameData, CheatModeData& cheatModeData, MouseData& mouseData, PillData& pillData, bool& mongooseSpawn, InvincibilityData& invinciblityData);
  void updateScore(ScoreData& scoreData);
  void getDateAndTime();
  void cheatMode(CheatModeData& cheatModeData, string& message, GameItems& gameItems);
  void powerUpPill(MouseData& mouseData, PillData& pillData);
  void endProgram(ScoreData& scoreData, CheatModeData& cheatModeData, EndGameData& endGameData);
  void pillCounter(PillData& pillData);
  void checkHighScore(ScoreData& scoreData);
  void MongooseSpawn(MouseData& mouseData, bool& mongooseSpawn);
  void MongooseMove(GameItems& gameItems, const char grid[][SIZEX]);

  void setLevel(LevelData& levelData);

  bool wantsToQuit(const int key);
  bool isArrowKey(const int k);
  bool wantsToCheat(const int key);


  int  getKeyPress();


  
  //local variable declarations 

  char grid[SIZEY][SIZEX];			//grid for display
  char maze[SIZEY][SIZEX];			//structure of the maze

  string message("LET'S START...");	//current message to player
  int key; //current key selected by player
  bool mongooseSpawn = false; //Creates a bool to store if the mongoose can spawn

//action...

  seed();								//seed the random number generator
  SetConsoleTitle("FoP 2018-19 - Task 1c - Game Skeleton");

  GetAmountOfPlayers(twoPlayerData);

  if (twoPlayerData.twoPlayerGame == true)
  {
    TwoPlayerOption(twoPlayerData);
   
  }
  else
  {

    getPlayerName(scoreData);			//Gets the players name
    checkHighScore(scoreData); //Checks for a Highscore
	  setLevel(levelData);

    initialiseGame(grid, maze, gameItems, pillData, mouseData, mongooseSpawn, levelData);	//initialise grid (incl. walls and spot)

    do
    {
      renderGame(grid, message, scoreData, mouseData, invincibilityData); //display game info, modified grid and messages

      key = toupper(getKeyPress()); //read in  selected key: arrow or letter command
      powerUpPill(mouseData, pillData); //Spawns a PowerUp pill when nesseccary
      MongooseSpawn(mouseData, mongooseSpawn);


      if (isArrowKey(key)) //If the player wants to move the snake
      {
        updateScore(scoreData); //Calls the update Score function to increase the score whenever the snake moves
        updateGame(grid, maze, gameItems, key, message, endGameData, cheatModeData, mouseData, pillData, mongooseSpawn, invincibilityData); //Updates the game
        pillCounter(pillData); //Updates the pillcounter
        MongooseMove(gameItems, grid); //Moves the mongoose

      }
      else if (wantsToQuit(key)) //If the player wants to quit by pressing Q
      {
        message = "PLAYER WANTS TO QUIT"; //Set this message
      }
      else if (wantsToCheat(key)) //If the player toggles cheat
      {
        cheatMode(cheatModeData, message, gameItems); //Call the cheat function
      }
      else //If anything else 
      {
        message = "INVALID KEY!";  //Set this message
      }

    } while (!wantsToQuit(key) && !endGameData.died && !endGameData.wonGame); //while user does not want to quit, is still alive and hasn't yet won the game

    renderGame(grid, message, scoreData, mouseData, invincibilityData); //display game info, modified grid and messages
    endProgram(scoreData, cheatModeData, endGameData); //display final message
  }
}


//---------------------------------------------------------------------------
//----- initialise game state
//---------------------------------------------------------------------------

void GetAmountOfPlayers(TwoPlayerData& twoPlayerData)
{
  selectTextColour(clWhite); //Sets the colour of the text to white

  do
  {

    cout << "\n\n\n\n\n\n\n\n\n\n           HOW MANY PLAYERS WOULD YOU LIKE? (1 OR 2): "; //Output asking the player how many players that they would like (Spaced to be in the center of the screen)
    twoPlayerData.Players = getch();
    system("CLS"); //Clears the screen


  } while (twoPlayerData.Players != '1' && twoPlayerData.Players != '2'); //If the character doesn't input 1 or 2 then it keeps looping until they choose

  if (twoPlayerData.Players == '1')
  {
    twoPlayerData.twoPlayerGame = false;
  }
  else
  {
    twoPlayerData.twoPlayerGame = true;
  }
}


void getPlayerName(ScoreData& scoreData) //Creates a function to get the players name
{
  selectTextColour(clWhite); //Sets the colour of the text to white

  do
  {

    cout << "\n\n\n\n\n\n\n\n\n\n           ENTER PLAYER NAME (Maximum 20 Characters): "; //Output asking the player to enter their name (Spaced to be in the center of the screen)
    cin >> scoreData.name; //Takes in the name entered and stores it in the name variable

    system("CLS"); //Clears the screen


  } while (scoreData.name.length() > scoreData.MaxChar); //If the name is above 20 characters it loops
}

void initialiseGame(char grid[][SIZEX], char maze[][SIZEX], GameItems& gameItems, PillData& pillData, MouseData& mouseData, bool& mongooseSpawn, LevelData& levelData) //Initialises the game
{
  void setInitialMazeStructure(char maze[][SIZEX], LevelData& levelData); //References function
  void setSnakeInitialCoordinates(const char maze[][SIZEX], GameItems& gameItems); //References function
  void setPillCoordinates(const char maze[][SIZEX], GameItems& gameItems); //References function
  void setMouseCoordinates(const char maze[][SIZEX], GameItems& gameItems); //References function
  void updateGrid(char grid[][SIZEX], const char maze[][SIZEX], GameItems& gameItems, PillData& pillData, MouseData& mouseData, bool& mongooseSpawn); //References function

  setInitialMazeStructure(maze, levelData); //Initialise maze
  setSnakeInitialCoordinates(maze, gameItems); //Set the snakes coordinates
  setPillCoordinates(maze, gameItems); //Sets the pills coordinates
  updateGrid(grid, maze, gameItems, pillData, mouseData, mongooseSpawn); //Prepare grid
  setMouseCoordinates(maze, gameItems); //Sets the mouses coordinates
  updateGrid(grid, maze, gameItems, pillData, mouseData, mongooseSpawn); //Updates the grid
}
void setLevel(LevelData& levelData)
{
	int chosenLevel;
	cout << "\n\n\n\n\n\n\n           Enter Level Number (1-Easy, 2-Medium, 3-Hard): ";
	cin >> chosenLevel;
	while (chosenLevel != 1 && chosenLevel != 2 && chosenLevel != 3)
	{
		cout << "\n\n\n\n\n\n\n           Please Enter a VALID Level Number (1-Easy, 2-Medium, 3-Hard): ";
		cin >> chosenLevel;
	}
	if (chosenLevel == 1)
	{
		levelData.level = 1;
	}

	if (chosenLevel == 2 /*&& levelData.unlock1 == true*/)
	{
		levelData.level = 2;
	}
	//else if (chosenLevel == 2 && levelData.unlock1 == false)
	//{
	//	cout << "\n\n\n\n\n\n\n        You have not yet unlocked this level. Now loading level 1. \n";
	//	system("Pause");
	//	levelData.level = 1;
	//}
	if (chosenLevel == 3 /*&& levelData.unlock2 == false*/)
	{
		levelData.level = 3;
	}
	//else if (chosenLevel == 3 /*&& levelData.unlock2 == false*/)
	//{
	//	cout << "\n\n\n\n\n\n\n          You have not yet unlocked this level. Now loading level 2. \n";
	//	system("Pause");
	//	levelData.level = 2;
	//}

  system("CLS"); //Clears the screen
}

void setSnakeInitialCoordinates(const char maze[][SIZEX], GameItems& gameItems) //Sets the initial spawn of the snake
{
  for (size_t i(0); i < gameItems.snake.size(); ++i) //Makes sure that all the sections of the snake spawn in at the same point
  {
    gameItems.snake.at(i).y = 6; //Sets the snake at y position 6 (middle)
    gameItems.snake.at(i).x = 9; //Sets the snake at x position 9 (middle)
  }
}

void setMouseCoordinates(const char maze[][SIZEX], GameItems& gameItems) //Sets the inital spawn of the mouse
{
  while (maze[gameItems.mouse.y][gameItems.mouse.x] != TUNNEL) //Makes sure that the mouse can't spawn inside a wall and if it does to respawn it
  {
    gameItems.mouse.x = random(SIZEX - 2); //Sets to a random x spot in the grid (-2 because they are the outside walls)
    gameItems.mouse.y = random(SIZEY - 2); //Sets to a random y spot in the grid (-2 because they are the outside walls)
  }

}

void setPillCoordinates(const char maze[][SIZEX], GameItems& gameItems) //Sets the inital spawn of the pill
{
  gameItems.pill.x = random(SIZEX - 2); //Sets to a random x spot in the grid (-2 because they are the outside walls)
  gameItems.pill.y = random(SIZEY - 2); //Sets to a random y spot in the grid (-2 because they are the outside walls)

  while (maze[gameItems.pill.y][gameItems.pill.x] != TUNNEL) //Makes sure that the pill can't spawn inside a wall and if it does to respawn it
  {
    gameItems.pill.x = random(SIZEX - 2); //Sets to a random x spot in the grid (-2 because they are the outside walls)
    gameItems.pill.y = random(SIZEY - 2); //Sets to a random y spot in the grid (-2 because they are the outside walls)
  }
}
void setInitialMazeStructure(char maze[][SIZEX], LevelData& levelData) //Creates the initial maze structure
{
	// //initialise maze configuration
	if (levelData.level == 1)
	{
		//	//fstream level1;

		//	//level1.open("Level1.maz", ios::in);

		//	//if (level1.fail())
		//	//{
		//	//	"ERROR: Level File Failed to Load";
		//	//}
		//	//else
		//	//{
		//		//level1 >> miceNeeded;
		//		//while (level1)
		//		//{
		//			string input;

		char initialMaze[SIZEY][SIZEX]
			= { { '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', '#', ' ', ' ', ' ', '#', ' ', ' ', ' ', '#', '#', '#', '#', '#', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', '#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', '#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', '#', '#', '#', '#', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', '#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', '#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', '#', ' ', ' ', ' ', '#', ' ', ' ', ' ', '#', '#', '#', '#', '#', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#' } };
		for (int row(0); row < SIZEY; ++row)
		{
			for (int col(0); col < SIZEX; ++col)
			{
				switch (initialMaze[row][col])
				{
					//not a direct copy, in case the symbols used are changed
				case '#': maze[row][col] = WALL; break;
				case ' ': maze[row][col] = TUNNEL; break;
				}
			}
		}

		//			/*for (int i = 0; i < SIZEY; ++i)
		//			{
		//				for (int c = 0; c < SIZEX; ++c)
		//				{
		//					initialMaze[i][c] = level1.get();
		//				}
		//			}

		//			for (int row(0); row < SIZEY; ++row)
		//			{
		//				for (int col(0); col <SIZEX; ++col)
		//					switch (initialMaze[row][col])
		//					{
		//					case '#': maze[row][col] = WALL; break;
		//					case ' ': maze[row][col] = TUNNEL; break;
		//					case '@': maze[row][col] = MOUSE; break;
		//					}
		//			}*/
		//				

		//		//}
		//	/}

	}

	if (levelData.level == 2) {
		//	fstream level2;
		char initialMaze[SIZEY][SIZEX]
			= { { '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', '#', '#', '#', '#', ' ', ' ', ' ', ' ', '#', '#', '#', '#', '#', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', '#', '#', '#', '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#', '#', '#', '#', ' ', ' ', ' ', '#' },
		{ '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#' } };
		for (int row(0); row < SIZEY; ++row)
		{
			for (int col(0); col < SIZEX; ++col)
			{
				switch (initialMaze[row][col])
				{
				case '#': maze[row][col] = WALL; break;
				case ' ': maze[row][col] = TUNNEL; break;
				}
			}
		}
		//	level2.open("Level2.maz.txt", ios::in);

		//	if (level2.fail())
		//	{
		//		"ERROR: Level File Failed to Load";
		//	}

		//	else
		//	{
		//		level2 >> miceNeeded;
		//	}


	}
	if (levelData.level == 3) {

		//	fstream level2;
		char initialMaze[SIZEY][SIZEX]
			= { { '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#', '#', '#', '#', ' ', ' ', '#', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', '#', '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#' },
		{ '#', ' ', ' ', '#', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#', '#', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#', ' ', ' ', '#' },
		{ '#', ' ', ' ', '#', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', '#', '#', ' ', '#', '#', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', '#', '#', ' ', ' ', ' ', '#' },
		{ '#', ' ', '#', '#', ' ', '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#' } };
		for (int row(0); row < SIZEY; ++row)
		{
			for (int col(0); col < SIZEX; ++col)
			{
				switch (initialMaze[row][col])
				{
				case '#': maze[row][col] = WALL; break;
				case ' ': maze[row][col] = TUNNEL; break;
				}
			}
		}
		//	fstream level3;

		//	level3.open("Level3.maz.txt", ios::in);

		//	if (level3.fail())
		//	{
		//		"ERROR: Level File Failed to Load";
		//	}

		//	else
		//	{
		//		level3 >> miceNeeded;

	}
}



//---------------------------------------------------------------------------
//----- Update Game
//---------------------------------------------------------------------------

void updateGame(char g[][SIZEX], const char m[][SIZEX], GameItems& gameItems, const int kc, string& mess, EndGameData& endGameData, CheatModeData& cheatModeData, MouseData& mouseData, PillData& pillData, bool& mongooseSpawn, InvincibilityData& invincibilityData)
{
  
  void updateGameData(const char g[][SIZEX], GameItems& gameItems, const int kc, string& m, MouseData& mouseData, EndGameData& endGameData, CheatModeData& cheatModeData,PillData& pillData, bool& mongooseSpawn, InvincibilityData& invincibilityData);
  void updateGrid(char grid[][SIZEX], const char maze[][SIZEX], GameItems& gameItems, PillData& pillData, MouseData& mouseData, bool& mongooseSpawn);

  updateGameData(g, gameItems, kc, mess, mouseData, endGameData, cheatModeData, pillData, mongooseSpawn, invincibilityData); //Updates all of the GameData such as moving the snake
  updateGrid(g, m, gameItems, pillData, mouseData, mongooseSpawn);	//update the grid display
}

void updateGameData(const char g[][SIZEX], GameItems& gameItems, const int key, string& mess, MouseData& mouseData, EndGameData& endGameData, CheatModeData& cheatModeData, PillData& pillData, bool& mongooseSpawn, InvincibilityData& invincibilityData) //Updates all of the GameData such as moving the snak
{
  bool isArrowKey(const int k); //Checks if the key being pressed is an arrow key

  assert(isArrowKey(key)); //Checks if the key being pressed is an arrow key

  void setKeyDirection(int k, int& dx, int& dy); //Sets the direction that the snake will move depending on which key is pressed
  void respawnMouse(const char g[][SIZEX], GameItems& gameItems); //Respawns the mouse 
  void moveSnake(GameItems& gameItems, int dx, int dy); //Moves the snake
  void increaseSnakeSize(GameItems& gameItems); //Increases the snake size
  void checkGameOver(MouseData& mouseData, EndGameData& endGameData, string& message); //Check if the game has ended (if player wins)
  void toggleInvincibility(InvincibilityData& invincibilityData); //Toggles the invincibility state of the snake


  //reset message to blank
  mess = "";

  //calculate direction of movement for given key
  int dx(0), dy(0);
  setKeyDirection(key, dx, dy);

  //check new target position in grid and update game data (incl. spot coordinates) if move is possible
  switch (g[gameItems.snake.at(0).y + dy][gameItems.snake.at(0).x + dx]) //...depending on what's on the target position in grid...
  {
  case TUNNEL:		//There is space for the snake to move into

    if (invincibilityData.Invincible)
    {
      invincibilityData.invincibleMoves++;
    }

    if (invincibilityData.Invincible && invincibilityData.invincibleMoves >= 20)
    {
      invincibilityData.Invincible = false;
      mess = "Invincibility Mode Deactivated!";
    }

    moveSnake(gameItems, dx, dy); //Move the snake into that cell

    break;

  case MOUSE: //If the snake passes over a mouse

    mess = "You've caught a mouse!"; //Displays this message

    moveSnake(gameItems, dx, dy); //Moves the snake over the mouse
    respawnMouse(g, gameItems); //Respawns the mouse at another cell

    ++mouseData.miceCollected; //Increases the miceCollected int (used for keeping track of how many mice the player has collected)
    ++mouseData.mouseCount; //Increases the miceCount int (used for the powerup pill)

    if (!cheatModeData.cheatModeOn) // Checks if cheat mode is not active
    {
      increaseSnakeSize(gameItems); //Increases the snake size by 2 more tail cells
    }

    checkGameOver(mouseData, endGameData, mess);
    break;

  case PILL: //If the snake passes over a pill

    moveSnake(gameItems, dx, dy); //Moves the snake over the pill
    gameItems.snake.resize(4); //Resize the snake to only be 4 cells long
    pillData.pillActive = false; //Set the pillActive to false (so it vanishes)
    toggleInvincibility(invincibilityData);

    mess = "Invincibility Mode Activated!";
    break;

  case MONGOOSE:

    moveSnake(gameItems, dx, dy); //Moves the snake over the mouse
    endGameData.died = true;
    mess = "Killed by a Mongoose!";
    mongooseSpawn = false;

    break;

  case WALL: //If the snake hits a wall

    if (invincibilityData.Invincible)
    {
      if (gameItems.snake.at(0).x >= SIZEY && key == RIGHT) // go through right wall
      {
        moveSnake(gameItems, dx, dy);
        gameItems.snake.at(0).x = 1; // emerge left
      }
      else if (gameItems.snake.at(0).x == 1 && key == LEFT) // go through left wall
      {
        moveSnake(gameItems, dx, dy);
        gameItems.snake.at(0).x = SIZEY + 3; // emerge right

      }
      else if (gameItems.snake.at(0).y == 1 && key == UP) // can't go through top 
      {
        moveSnake(gameItems, dx, dy);
        gameItems.snake.at(0).y = 13;
      }
      else if (gameItems.snake.at(0).y == SIZEY - 2 && key == DOWN) // go through lower wall
      {
        moveSnake(gameItems, dx, dy);
        gameItems.snake.at(0).y = 1; // emerge top
      }

    }

    if (!invincibilityData.Invincible)
    {
      mess = "You Lose!"; //Displays this message
      endGameData.died = true; //Sets the bool died to true
    }
 
    break;

  case TAIL: //If the snake hits its own tail

    mess = "You Lose!"; //Displau this message
    endGameData.died = true; //Sets the bool died to true
  }
}

//---------------------------------------------------------------------------
//----- Snake and Mouse Code (used above)
//---------------------------------------------------------------------------

void increaseSnakeSize(GameItems& gameItems) //Increases the size of the snake
{
  gameItems.snake.push_back({ gameItems.snake.at(gameItems.snake.size() - 1).x,gameItems.snake.at(gameItems.snake.size() - 1).y,TAIL }); //Increse snake size by 2
  gameItems.snake.push_back({ gameItems.snake.at(gameItems.snake.size() - 1).x,gameItems.snake.at(gameItems.snake.size() - 1).y,TAIL }); //^^
}
void moveSnake(GameItems& gameItems, int dx, int dy) //Moves the snake in the grid
{
  for (size_t i(gameItems.snake.size() - 1); i >= 1; --i) //For each part of the snake (head and tail)
  {
    gameItems.snake.at(i).y = gameItems.snake.at(i - 1).y; //Place the new part at the position of the last part
    gameItems.snake.at(i).x = gameItems.snake.at(i - 1).x; //Place the new part at the position of the last part
  }

  gameItems.snake.at(0).y += dy;	//go in that Y direction
  gameItems.snake.at(0).x += dx;	//go in that X direction
}
void respawnMouse(const char g[][SIZEX], GameItems& gameItems) //Respawns the mouse
{ 
  do
  {
    gameItems.mouse.y = random(SIZEY - 2); //Sets to a random y spot in the grid (-2 because they are the outside walls)
    gameItems.mouse.x = random(SIZEX - 2); //Sets to a random x spot in the grid (-2 because they are the outside walls)

  } while (g[gameItems.mouse.y][gameItems.mouse.x] != TUNNEL); //Makes sure that the mouse can't spawn inside a wall and if it does to respawn it
}




//---------------------------------------------------------------------------
//----- Place everything
//---------------------------------------------------------------------------

void updateGrid(char grid[][SIZEX], const char maze[][SIZEX], GameItems& gameItems, PillData& pillData, MouseData& mouseData, bool& mongooseSpawn) //update grid configuration after each move
{
  void placeMaze(char g[][SIZEX], const char b[][SIZEX]); //Places the maze
  void placeItem(char g[][SIZEX], const Item& item); //Places items inside the maze

  placeMaze(grid, maze);	//Reset the empty maze configuration into grid

  for (int i(gameItems.snake.size() - 1); i >= 0; --i)		 //For each section of the snake
  {
    placeItem(grid, gameItems.snake.at(i));	//Set snake in the grid
  }
  placeItem(grid, gameItems.mouse);	//Set mouse in the grid

  if (pillData.pillActive) //If the bool pillActive is true
  {
    placeItem(grid, gameItems.pill); //Set the pill in the grid
  }
  if (mongooseSpawn == true) //If the bool pillActive is true
  {
    placeItem(grid, gameItems.mongoose); //Set the pill in the grid
  }
}

void placeMaze(char grid[][SIZEX], const char maze[][SIZEX]) //reset the empty/fixed maze configuration into grid
{
  for (int row(0); row < SIZEY; ++row)
    for (int col(0); col < SIZEX; ++col)
      grid[row][col] = maze[row][col];
}

void placeItem(char g[][SIZEX], const Item& item) //Function to place an item inside the grid, used for snake, mouse and pill
{
  g[item.y][item.x] = item.symbol; //Gets the position and spawns the symbol there
}


//---------------------------------------------------------------------------
//----- process key
//---------------------------------------------------------------------------

void setKeyDirection(const int key, int& dx, int& dy)
{ //calculate direction indicated by key
  bool isArrowKey(const int k);
  assert(isArrowKey(key));
  switch (key)//...depending on the selected key...
  {
  case LEFT:  //when LEFT arrow pressed...
    dx = -1;	//decrease the X coordinate
    dy = 0;
    break;
  case RIGHT: //when RIGHT arrow pressed...
    dx = +1;	//increase the X coordinate
    dy = 0;
    break;
  case UP:  	//when UP arrow pressed...
    dx = 0;
    dy = -1;  //decrease the y coordinate
    break;
  case DOWN:  //when DOWN arrow pressed...
    dx = 0;
    dy = +1;  //increase the y coordinate
    break;

  }
}

int getKeyPress()
{ //get key or command selected by user
  //KEEP THIS FUNCTION AS GIVEN
  int keyPressed;
  keyPressed = _getch();			//read in the selected arrow key or command letter
  while (keyPressed == 224) 		//ignore symbol following cursor key
    keyPressed = _getch();
  return keyPressed;
}

bool isArrowKey(const int key)
{	//check if the key pressed is an arrow key (also accept 'K', 'M', 'H' and 'P')

  return (key == LEFT) || (key == RIGHT) || (key == UP) || (key == DOWN);
}
bool wantsToQuit(const int key)
{	//check if the user wants to quit (when key is 'Q' or 'q')

  return key == (QUIT);
}
bool wantsToCheat(const int key)
{
  return key == (CHEAT);
}

//---------------------------------------------------------------------------
//----- display info on screen
//---------------------------------------------------------------------------

string tostring(int x)
{	//convert an integer to a string
  std::ostringstream os;
  os << x;
  return os.str();
}
string tostring(char x)
{	//convert a char to a string
  std::ostringstream os;
  os << x;
  return os.str();
}
void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message)
{	//display a string using specified colour at a given position 
  gotoxy(x, y);
  selectBackColour(backColour);
  selectTextColour(textColour);
  cout << message + string(40 - message.length(), ' ');
}
void renderGame(const char g[][SIZEX], const string& mess, ScoreData& scoreData, MouseData& mouseData, InvincibilityData& invincibilityData)
{ //display game title, messages, maze, spot and other items on screen
  string tostring(char x);
  string tostring(int x);

  void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);
  void paintGrid(const char g[][SIZEX], InvincibilityData& invincibilityData);


 
  
  //display game title
  showMessage(clBlack, clDarkGreen, 0, 0, "___GAME___");

  showMessage(clWhite, clDarkGreen, 40, 1, "          " + getDate() + "   " + getTime() + "  ");
  showMessage(clWhite, clDarkGreen, 40, 2, "                                       ");
  showMessage(clWhite, clDarkGreen, 40, 3, "        FoP Task 1c - April 2019       ");
  showMessage(clWhite, clDarkGreen, 40, 4, "                                       ");

  showMessage(clWhite, clDarkGreen, 40, 5,  " ------------ CS4G - Group 2 ----------");
  showMessage(clWhite, clDarkGreen, 40, 6, "                                       ");
  showMessage(clWhite, clDarkGreen, 40, 7, " Casey-Jo Kenny                b8016012");
  showMessage(clWhite, clDarkGreen, 40, 8, " Josh Long                     b8014932");
  showMessage(clWhite, clDarkGreen, 40, 9, " Kai Henry                     b8021762");
  //display menu options available

  showMessage(clWhite, clDarkGreen, 40, 10, "                                       ");
  showMessage(clWhite, clDarkGreen, 40, 11, " --------------- Controls -------------");
  showMessage(clWhite, clDarkGreen, 40, 12, "                                       ");
  showMessage(clWhite, clDarkGreen, 40, 13, " TO MOVE  - USE KEYBOARD ARROWS        ");
  showMessage(clWhite, clDarkGreen, 40, 14, " TO CHEAT - ENTER 'C'                  ");
  showMessage(clWhite, clDarkGreen, 40, 15, " TO QUIT  - ENTER 'Q'                  ");
  showMessage(clWhite, clDarkGreen, 40, 16, "                                       ");
  showMessage(clWhite, clDarkGreen, 40, 17, " ------------- Player Info ------------");
  showMessage(clWhite, clDarkGreen, 40, 18, "                                       ");
  showMessage(clWhite, clDarkGreen, 40, 19, " PLAYERS NAME     : " + scoreData.name);
  showMessage(clWhite, clDarkGreen, 40, 20, " BEST SCORE SO FAR: " + tostring(scoreData.highScore));
  showMessage(clWhite, clDarkGreen, 40, 21, " CURRENT SCORE    : " + tostring(scoreData.score));
  showMessage(clWhite, clDarkGreen, 40, 22, " MICE COLLECTED   : " + tostring(mouseData.miceCollected) + "/" + tostring(miceNeeded));
  showMessage(clWhite, clDarkGreen, 40, 23, "                                       ");


  //print auxiliary messages if any
  showMessage(clWhite, clDarkGreen, 40, 24, mess);	//display current message



  //display grid contents
  paintGrid(g, invincibilityData);
}


void paintGrid(const char g[][SIZEX], InvincibilityData& invincibilityData)
{ //display grid content on screen
  selectBackColour(clBlack);
  selectTextColour(clWhite);
  gotoxy(0, 2);

  for (int row(0); row < SIZEY; ++row)
  {
    for (int col(0); col < SIZEX; ++col)
    {


      if (invincibilityData.Invincible)
      {
        if (g[row][col] == HEAD) //sets the head of the snake to Magenta
          selectTextColour(clRed);

        if (g[row][col] == TAIL) //Sets the body of the snake to green
          selectTextColour(clBlue);
      }
      else
      {
        if (g[row][col] == HEAD) //sets the head of the snake to Magenta
          selectTextColour(clMagenta);

        if (g[row][col] == TAIL) //Sets the body of the snake to green
          selectTextColour(clGreen);
      }

      if (g[row][col] == MOUSE)
        selectTextColour(clYellow);
      if (g[row][col] == PILL)
        selectTextColour(clRed);
      if (g[row][col] == MONGOOSE)
        selectTextColour(clCyan);

      cout << g[row][col];	//output cell content
      selectTextColour(clWhite);


    }
    cout << endl;
  }
}

//---------------------------------------------------------------------------
//----- "Behind the Scenes" Functions
//---------------------------------------------------------------------------


void pillCounter(PillData& pillData) //Function to control the pill counter
{
  if (pillData.pillActive) //If the pill is on the grid
  {
    pillData.pillMoveCount++; //Add one to the counter
  }
}

void powerUpPill(MouseData& mouseData, PillData& pillData) //A function to control the powerup pill
{
  if (mouseData.mouseCount == 2) //If the mouse counter is equal to 2
  {
    pillData.pillActive = true; //Set the pill to active so the pill spawns
    mouseData.mouseCount = 0; //Reset the mouse counter
  }
  if (pillData.pillMoveCount == 10) //If the pill move counter becomes equal to 10
  {
    pillData.pillActive = false; //Sets the pillActive to false to despawn the pill
    pillData.pillMoveCount = 0; //Reset the move counter
  }
}

void updateScore(ScoreData& scoreData) //Function to control the score update
{
  scoreData.score++; //Adds one to the score variable
}

void endProgram(ScoreData& scoreData, CheatModeData& cheatModeData, EndGameData& endGameData) //Function for ending the program
{
  void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message); //Function that shows messages on screen
  void saveHighScore(ScoreData& scoreData, CheatModeData& cheatModeData, EndGameData& endGameData); //Function to save the high score

  saveHighScore(scoreData, cheatModeData, endGameData); //Checks if the score should be saved and saves accordingly
  showMessage(clBlack, clWhite, 0, 29, "Thanks for Playing!"); //Displays this message
  system("pause");	//Holds output screen until a keyboard key is hit
}


void cheatMode(CheatModeData& cheatModeData, string& message, GameItems& gameItems) //Function for controlling the cheatMode
{
  if (!cheatModeData.cheatModeOn) //If cheatmode isn't on and the key gets toggled
  {
    message = "Cheat Mode On"; //Display this message
    cout << "\a" << "\a" << "\a"; //Output the alarm 3 times
    cheatModeData.cheatModeOn = true; //Set cheatmode to true (so that the program knows its active)
    cheatModeData.noScore = true; //Sets noScore to true so that the score can't be recorded
    gameItems.snake.resize(4); //Resizes the snake back to normal
  }
  else //If cheatmode is on and the key gets toggled
  {
    message = "Cheat Mode Off"; //Display this message
    cheatModeData.cheatModeOn = false; //Turn the cheatmode bool to false
  }
}

void checkGameOver(MouseData& mouseData, EndGameData& endGameData, string& message) //Function to check if the game has ended
{
  if (mouseData.miceCollected == miceNeeded) //If the amount of mice the player has collected is the same as the amount needed
  {
    message = "You Win!"; //Display this message
    endGameData.wonGame = true; //Set the wonGame bool to true so the program knows the player has won
  }
}

void checkHighScore(ScoreData& scoreData) //Function to check if their is a high score found when the game loads
{
  fstream fileRead;	//Defines the file variable
  fileRead.open(scoreData.name + ".txt", ios::in);  //Opens the file that has the users name associated to it 
  if (fileRead.fail())	//If the file open fails then it will set the high score to a preset 500
  {
    scoreData.highScore = 500;
  }
  else
  {
    fileRead >> scoreData.highScore;	//If the file open succeeds then it will set the users high score to the high score variable that will be displayed in the right hand side menu
  }
}
void saveHighScore(ScoreData& scoreData, CheatModeData& cheatModeData, EndGameData& endGameData) //Function to save the users high score after a win
{
  fstream fileManip; //Defines the file manipulation variable
  int oldScore;	//Used to see if the old score is better or worse than the new score
  fileManip.open(scoreData.name + ".txt", ios::in);	//Opens the users file

  if ((endGameData.wonGame) && (!cheatModeData.noScore))	//If the user has collected the correct amount of mice and has not used cheatmode then the if statement will execute
  {
    if (fileManip.fail())	//If the previous file open fails it means that the user is new and will create them a text file and record their score
    {
      fileManip.setf(ios::fixed);
      fileManip.open(scoreData.name + ".txt", ios::out);
      fileManip << scoreData.score;
    }
    else
    {
      fileManip >> oldScore; //If a file already exists then it will put the current contents of the file into oldScore

      if (scoreData.score < oldScore) //If the users newest score is better than their old score then it will record the new one in their text file
      {
        fstream fileManip(scoreData.name + ".txt", ios::out);
        fileManip.setf(ios::fixed);
        fileManip << scoreData.score;

      }
      else
      {
        fstream fileManip(scoreData.name + ".txt", ios::out);	// if the users old score is better then it will keep it that way
        fileManip.setf(ios::fixed);
        fileManip << oldScore;
      }
    }
  }


}

void MongooseSpawn(MouseData& mouseData, bool& mongooseSpawn)
{
  if (mouseData.miceCollected == 3) //If the number of mice that has been collected is 3
  {
    mongooseSpawn = true; //Spawn the mongoose
  }

}
void MongooseMove(GameItems& gameItems, const char grid[][SIZEX]) //A function to move the mongoose every time the player moves
{
  int move = random(9); //Chooses a random direction for the mongoose to move in

  switch (move)
  {
  case 1: //Mongoose moves up

    if (grid[gameItems.mongoose.y - 1][gameItems.mongoose.x] == TUNNEL) //Checks that the place the mongoose will move is a tunnel
    {
      gameItems.mongoose.y -= 1;
      gameItems.mongoose.x += 0;
    }
    break;

  case 2://Mongoose moves down

    if (grid[gameItems.mongoose.y + 1][gameItems.mongoose.x] == TUNNEL) //Checks that the place the mongoose will move is a tunnel
    {
      gameItems.mongoose.y += 1;
      gameItems.mongoose.x += 0;
    }
    break;

  case 3: //Mongoose moves left

    if (grid[gameItems.mongoose.y][gameItems.mongoose.x - 1] == TUNNEL) //Checks that the place the mongoose will move is a tunnel
    {
      gameItems.mongoose.y += 0;
      gameItems.mongoose.x -= 1;
    }
    break;

  case 4: //Mongoose moves right

    if (grid[gameItems.mongoose.y][gameItems.mongoose.x + 1] == TUNNEL) //Checks that the place the mongoose will move is a tunnel
    {
      gameItems.mongoose.y += 0;
      gameItems.mongoose.x += 1;
    }
    break;

  case 5: //Mongoose stays still

    gameItems.mongoose.y += 0;
    gameItems.mongoose.x += 0;

    break;

  case 6: //Mongoose moves diagonal up left

    if (grid[gameItems.mongoose.y - 1][gameItems.mongoose.x - 1] == TUNNEL) //Checks that the place the mongoose will move is a tunnel
    {
      gameItems.mongoose.y -= 1;
      gameItems.mongoose.x -= 1;
    }
    break;

  case 7: //Mongoose moves diagonal up right

    if (grid[gameItems.mongoose.y - 1][gameItems.mongoose.x + 1] == TUNNEL) //Checks that the place the mongoose will move is a tunnel
    {
      gameItems.mongoose.y -= 1;
      gameItems.mongoose.x += 1;
    }
    break;

  case 8: //Mongoose moves diagonal down left

    if (grid[gameItems.mongoose.y + 1][gameItems.mongoose.x - 1] == TUNNEL) //Checks that the place the mongoose will move is a tunnel
    {
      gameItems.mongoose.y += 1;
      gameItems.mongoose.x -= 1;
    }
    break;

  case 9: //Mongoose moves diagonal down right

    if (grid[gameItems.mongoose.y + 1][gameItems.mongoose.x + 1] == TUNNEL) //Checks that the place the mongoose will move is a tunnel
    {
      gameItems.mongoose.y += 1;
      gameItems.mongoose.x += 1;
    }
    break;
  }
}





void toggleInvincibility(InvincibilityData& invincibilityData)
{
  invincibilityData.Invincible = true;
  invincibilityData.invincibleMoves = 0;
  
}

//---------------------------------------------------------------------------
//----- Two Player Option
//---------------------------------------------------------------------------

void TwoPlayerOption(TwoPlayerData& twoPlayerData) //Runs as the "main" for the Two Player Mode
{
  void initialiseGameTwoPlayer(char grid[][TWOSIZEX], char maze[][TWOSIZEX], GameItems& gameItems, PillData& pillData, MouseData& mouseData);
  void renderGameTwoPlayer(const char g[][TWOSIZEX], const string& mess, ScoreData& scoreData, MouseData& mouseData);
  void updateGameTwoPlayer(char g[][TWOSIZEX], const char m[][TWOSIZEX], GameItems& gameItems, const int kc, string& mess, EndGameData& endGameData, CheatModeData& cheatModeData, MouseData& mouseData, PillData& pillData);
  void getDateAndTime();
  void powerUpPillTwoPlayer(MouseData& mouseData, PillData& pillData);
  void endProgramTwoPlayer(ScoreData& scoreData, CheatModeData& cheatModeData, EndGameData& endGameData);
  void pillCounterTwoPlayer(PillData& pillData);

  bool wantsToQuit(const int key);
  bool isArrowKey(const int k);
  bool isWASDKey(const int k);

  int  getKeyPress();

  //local variable declarations 

  char grid[TWOSIZEY][TWOSIZEX];			//grid for display
  char maze[TWOSIZEY][TWOSIZEX];			//structure of the maze

  string message("LET'S START...");	//current message to player
  int key; //current key selected by player
  

//action...

  seed();								//seed the random number generator
  SetConsoleTitle("FoP 2018-19 - Task 1c - Game Skeleton");


  initialiseGameTwoPlayer(grid, maze, gameItems, pillData, mouseData);	//initialise grid (incl. walls and spot)

  do
  {
    renderGameTwoPlayer(grid, message, scoreData, mouseData); //display game info, modified grid and messages
    key = toupper(getKeyPress()); //read in  selected key: arrow or letter command
    powerUpPillTwoPlayer(mouseData, pillData); //Spawns a PowerUp pill when nesseccary
    

    if (isArrowKey(key) || isWASDKey(key)) //Checks if either players wants to move their snake
    {
      updateGameTwoPlayer(grid, maze, gameItems, key, message, endGameData, cheatModeData, mouseData, pillData); //Updates the game
      pillCounterTwoPlayer(pillData); //Updates the pillcounter
    }
    else if (wantsToQuit(key)) //If the player wants to quit by pressing Q
    {
      message = "PLAYER WANTS TO QUIT"; //Set this message
    }
    else //If anything else 
    {
      message = "INVALID KEY!";  //Set this message
    }

  } while (!wantsToQuit(key) && (!twoPlayerData.playerOneDied && !twoPlayerData.playerTwoDied)); //while user does not want to quit, and both snakes are alive

  renderGameTwoPlayer(grid, message, scoreData, mouseData); //display game info, modified grid and messages
  endProgram(scoreData, cheatModeData, endGameData); //display final message

}

//---------------------------------------------------------------------------
//----- initialise game state
//---------------------------------------------------------------------------


void initialiseGameTwoPlayer(char grid[][TWOSIZEX], char maze[][TWOSIZEX], GameItems& gameItems, PillData& pillData, MouseData& mouseData) //Initialises the game
{
  void setInitialMazeStructureTwoPlayer(char maze[][TWOSIZEX]); //References function
  void setPlayerOneSnakeCoordinates(const char maze[][TWOSIZEX], GameItems& gameItems); //References function
  void setPlayerTwoSnakeCoordinates(const char maze[][TWOSIZEX], GameItems& gameItems); //References function
  void setPillCoordinatesTwoPlayer(const char maze[][TWOSIZEX], GameItems& gameItems); //References function
  void setMouseCoordinatesTwoPlayer(const char maze[][TWOSIZEX], GameItems& gameItems); //References function
  void updateGridTwoPlayer(char grid[][TWOSIZEX], const char maze[][TWOSIZEX], GameItems& gameItems, PillData& pillData, MouseData& mouseData); //References function

  setInitialMazeStructureTwoPlayer(maze); //Initialise maze
  setPlayerOneSnakeCoordinates(maze, gameItems); //Set the player one snakes coordinates
  setPlayerTwoSnakeCoordinates(maze, gameItems); //Set the player two snakes coordinates
  setPillCoordinatesTwoPlayer(maze, gameItems); //Sets the pills coordinates
  updateGridTwoPlayer(grid, maze, gameItems, pillData, mouseData); //Prepare grid
  setMouseCoordinatesTwoPlayer(maze, gameItems); //Sets the mouses coordinates
  updateGridTwoPlayer(grid, maze, gameItems, pillData, mouseData); //Updates the grid
}

void setPlayerOneSnakeCoordinates(const char maze[][TWOSIZEX], GameItems& gameItems) //Sets the initial spawn of the first snake
{
  for (size_t i(0); i < gameItems.snake.size(); ++i) //Makes sure that all the sections of the snake spawn in at the same point
  {
    gameItems.snake.at(i).y = 8; //Sets the first snake at y position 6 (middle)
    gameItems.snake.at(i).x = 11; //Sets the first snake at x position 9 (middle)
  }
}

void setPlayerTwoSnakeCoordinates(const char maze[][TWOSIZEX], GameItems& gameItems) //Sets the initial spawn of the second snake
{
  for (size_t i(0); i < gameItems.snake2.size(); ++i) //Makes sure that all the sections of the snake spawn in at the same point
  {
    gameItems.snake2.at(i).y = 6; //Sets the second snake at y position 6 (middle)
    gameItems.snake2.at(i).x = 9; //Sets the second snake at x position 9 (middle)
  }
}


void setMouseCoordinatesTwoPlayer(const char maze[][TWOSIZEX], GameItems& gameItems) //Sets the inital spawn of the mouse
{
  while (maze[gameItems.mouse.y][gameItems.mouse.x] != TUNNEL) //Makes sure that the mouse can't spawn inside a wall and if it does to respawn it
  {
    gameItems.mouse.x = random(TWOSIZEX - 2); //Sets to a random x spot in the grid (-2 because they are the outside walls)
    gameItems.mouse.y = random(TWOSIZEY - 2); //Sets to a random y spot in the grid (-2 because they are the outside walls)
  }

}

void setPillCoordinatesTwoPlayer(const char maze[][TWOSIZEX], GameItems& gameItems) //Sets the inital spawn of the pill
{
  gameItems.pill.x = random(TWOSIZEX - 2); //Sets to a random x spot in the grid (-2 because they are the outside walls)
  gameItems.pill.y = random(TWOSIZEY - 2); //Sets to a random y spot in the grid (-2 because they are the outside walls)

  while (maze[gameItems.pill.y][gameItems.pill.x] != TUNNEL) //Makes sure that the pill can't spawn inside a wall and if it does to respawn it
  {
    gameItems.pill.x = random(TWOSIZEX - 2); //Sets to a random x spot in the grid (-2 because they are the outside walls)
    gameItems.pill.y = random(TWOSIZEY - 2); //Sets to a random y spot in the grid (-2 because they are the outside walls)
  }
}

void setInitialMazeStructureTwoPlayer(char maze[][TWOSIZEX]) //Creates the initial maze structure
{
  //initialise maze configuration
  char initialMazePlayerTwo[TWOSIZEY][TWOSIZEX] 	//local array to store the maze structure
    = { { '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#','#', '#', '#', '#', '#' },
    { '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
    { '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
    { '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
    { '#', ' ', ' ', ' ', '#', '#', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#', '#', '#', ' ', ' ', ' ', '#' },
    { '#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', '#' },
    { '#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', '#' },
    { '#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', '#' },
    { '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
    { '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
    { '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
    { '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
    { '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
    { '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
    { '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
    { '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#', '#', '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
    { '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
    { '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
    { '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
    { '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#' } };

  //with '#' for wall, ' ' for tunnel, etc. 

  //copy into maze structure with appropriate symbols
  for (int row(0); row < TWOSIZEY; ++row)
  {
    for (int col(0); col < TWOSIZEX; ++col)
    {
      switch (initialMazePlayerTwo[row][col])
      {
        //not a direct copy, in case the symbols used are changed
      case '#': maze[row][col] = WALL; break;
      case ' ': maze[row][col] = TUNNEL; break;
      }
    }
  }
}

//---------------------------------------------------------------------------
//----- Update Game
//---------------------------------------------------------------------------

void updateGameTwoPlayer(char g[][TWOSIZEX], const char m[][TWOSIZEX], GameItems& gameItems, const int kc, string& mess, EndGameData& endGameData, CheatModeData& cheatModeData, MouseData& mouseData, PillData& pillData)
{

  void updateGameDataTwoPlayer(const char g[][TWOSIZEX], GameItems& gameItems, const int kc, string& m, MouseData& mouseData, EndGameData& endGameData, CheatModeData& cheatModeData, PillData& pillData);
  void updateGridTwoPlayer(char grid[][TWOSIZEX], const char maze[][TWOSIZEX], GameItems& gameItems, PillData& pillData, MouseData& mouseData);

  updateGameDataTwoPlayer(g, gameItems, kc, mess, mouseData, endGameData, cheatModeData, pillData); //Updates all of the GameData such as moving the snake
  updateGridTwoPlayer(g, m, gameItems, pillData, mouseData);	//update the grid display
}

void updateGameDataTwoPlayer(const char g[][TWOSIZEX], GameItems& gameItems, const int key, string& mess, MouseData& mouseData, EndGameData& endGameData, CheatModeData& cheatModeData, PillData& pillData) //Updates all of the GameData such as moving the snak
{
  bool isArrowKey(const int k); //Checks if the key being pressed is an arrow key
  bool isWASDKey(const int k); //Checks if the key being pressed is an arrow key

  void setPlayerOneKeyDirection(const int key, int& dx, int& dy); //Sets the direction that the player one snake will move depending on which key is pressed
  void setPlayerTwoKeyDirection(const int key, int& dx, int& dy); //Sets the direction that the  player two snake will move depending on which key is pressed
  void respawnMouseTwoPlayer(const char g[][TWOSIZEX], GameItems& gameItems); //Respawns the mouse 
  void movePlayerOneSnake(GameItems& gameItems, int dx, int dy); //Moves the snake
  void movePlayerTwoSnake(GameItems& gameItems, int dx, int dy); //Moves the snake
  void increasePlayerOneSnakeSize(GameItems& gameItems); //Increases the size of the snake
  void increasePlayerTwoSnakeSize(GameItems& gameItems); //Increases the size of the snake
  //void checkGameOver(MouseData& mouseData, EndGameData& endGameData, string& message); //Check if the game has ended (if player wins)

  //reset message to blank
  mess = "";

  //calculate direction of movement for given key
  int dx(0), dy(0);

  if (isArrowKey(key)) //Check if player one has tried to move
  {
    setPlayerOneKeyDirection(key, dx, dy); //Sets the direct

    //check new target position in grid and update game data (incl. spot coordinates) if move is possible
    switch (g[gameItems.snake.at(0).y + dy][gameItems.snake.at(0).x + dx]) //...depending on what's on the target position in grid...
    {
    case TUNNEL:		//There is space for the snake to move into

      movePlayerOneSnake(gameItems, dx, dy); //Move the snake into that cell

      break;

    case MOUSE: //If the snake passes over a mouse

      mess = "Player one caught a mouse!"; //Displays this message

      movePlayerOneSnake(gameItems, dx, dy); //Moves the snake over the mouse
      increasePlayerOneSnakeSize(gameItems);
      respawnMouseTwoPlayer(g, gameItems); //Respawns the mouse at another cell

      twoPlayerData.playerOneScore++; //Increases the player one score

      break;

    case PILL: //If the snake passes over a pill

      movePlayerOneSnake(gameItems, dx, dy); //Moves the snake over the pill
      gameItems.snake.resize(4); //Resize the snake to only be 4 cells long
      pillData.pillActive = false; //Set the pillActive to false (so it vanishes)

      break;


    case WALL: //If the snake hits a wall

      mess = "Player One has died! You Win Player One!"; //Displays this message
      twoPlayerData.playerOneDied = true; //Sets the bool died to true

      break;

    case TAIL: //If the snake hits its own tail

      mess = "Player One has died! You Win Player Two!"; //Displau this message
      twoPlayerData.playerOneDied = true; //Sets the bool died to true

      break;

    case PLAYERTWOTAIL: //If the snake hits player twos tail

      mess = "Player One has died! You Win Player Two!"; //Displau this message
      twoPlayerData.playerOneDied = true; //Sets the bool died to true

      break;

    case PLAYERTWOHEAD: //If the snake hits its own tail

      mess = "Both Snakes have Died! No one Wins!"; //Displau this message
      twoPlayerData.playerOneDied = true; //Sets the bool died to true
      twoPlayerData.playerTwoDied = true; //Sets the bool died to true
    }
  }
  
  if (isWASDKey(key)) //Check if player two has tried to move
  {
    setPlayerTwoKeyDirection(key, dx, dy); //Sets the direction

    //check new target position in grid and update game data (incl. spot coordinates) if move is possible
    switch (g[gameItems.snake2.at(0).y + dy][gameItems.snake2.at(0).x + dx]) //...depending on what's on the target position in grid...
    {
    case TUNNEL:		//There is space for the snake to move into

      movePlayerTwoSnake(gameItems, dx, dy); //Move the snake into that cell

      break;

    case MOUSE: //If the snake passes over a mouse

      mess = "Player Two caught a mouse!"; //Displays this message

      movePlayerTwoSnake(gameItems, dx, dy); //Moves the snake over the mouse
      increasePlayerTwoSnakeSize(gameItems);
      respawnMouseTwoPlayer(g, gameItems); //Respawns the mouse at another cell

      twoPlayerData.playerTwoScore++; //Increases the player two score

      break;

    case PILL: //If the snake passes over a pill

      movePlayerTwoSnake(gameItems, dx, dy); //Moves the snake over the pill
      gameItems.snake2.resize(4); //Resize the snake to only be 4 cells long
      pillData.pillActive = false; //Set the pillActive to false (so it vanishes)

      break;


    case WALL: //If the snake hits a wall

      mess = "Player Two has died! You Win Player One!"; //Displays this message
      twoPlayerData.playerTwoDied = true; //Sets the bool died to true

      break;

    case TAIL: //If the snake hits its own tail

      mess = "Player Two has died! You Win Player One!"; //Displau this message
      twoPlayerData.playerTwoDied = true; //Sets the bool died to true

      break;

    case HEAD: //If the snake hits its own tail

      mess = "Both Snakes have Died! No one Wins!"; //Displau this message
      twoPlayerData.playerOneDied = true; //Sets the bool died to true
      twoPlayerData.playerTwoDied = true; //Sets the bool died to true
    }
  }
}
  

//---------------------------------------------------------------------------
//----- Snake and Mouse Code (used above)
//---------------------------------------------------------------------------

void increasePlayerOneSnakeSize(GameItems& gameItems) //Increases the size of the snake
{
  gameItems.snake.push_back({ gameItems.snake.at(gameItems.snake.size() - 1).x,gameItems.snake.at(gameItems.snake.size() - 1).y,TAIL }); //Increse snake size by 2
  gameItems.snake.push_back({ gameItems.snake.at(gameItems.snake.size() - 1).x,gameItems.snake.at(gameItems.snake.size() - 1).y,TAIL }); //^^
}
void increasePlayerTwoSnakeSize(GameItems& gameItems) //Increases the size of the snake
{
  gameItems.snake2.push_back({ gameItems.snake2.at(gameItems.snake2.size() - 1).x,gameItems.snake2.at(gameItems.snake2.size() - 1).y,PLAYERTWOTAIL }); //Increse snake size by 2
  gameItems.snake2.push_back({ gameItems.snake2.at(gameItems.snake2.size() - 1).x,gameItems.snake2.at(gameItems.snake2.size() - 1).y,PLAYERTWOTAIL }); //^^
}
void movePlayerOneSnake(GameItems& gameItems, int dx, int dy) //Moves the snake in the grid
{
  for (size_t i(gameItems.snake.size() - 1); i >= 1; --i) //For each part of the snake (head and tail)
  {
    gameItems.snake.at(i).y = gameItems.snake.at(i - 1).y; //Place the new part at the position of the last part
    gameItems.snake.at(i).x = gameItems.snake.at(i - 1).x; //Place the new part at the position of the last part
  }

  gameItems.snake.at(0).y += dy;	//go in that Y direction
  gameItems.snake.at(0).x += dx;	//go in that X direction
}
void movePlayerTwoSnake(GameItems& gameItems, int dx, int dy) //Moves the snake in the grid
{
  for (size_t i(gameItems.snake2.size() - 1); i >= 1; --i) //For each part of the snake (head and tail)
  {
    gameItems.snake2.at(i).y = gameItems.snake2.at(i - 1).y; //Place the new part at the position of the last part
    gameItems.snake2.at(i).x = gameItems.snake2.at(i - 1).x; //Place the new part at the position of the last part
  }

  gameItems.snake2.at(0).y += dy;	//go in that Y direction
  gameItems.snake2.at(0).x += dx;	//go in that X direction
}
void respawnMouseTwoPlayer(const char g[][TWOSIZEX], GameItems& gameItems) //Respawns the mouse
{
  do
  {
    gameItems.mouse.y = random(TWOSIZEY - 2); //Sets to a random y spot in the grid (-2 because they are the outside walls)
    gameItems.mouse.x = random(TWOSIZEX - 2); //Sets to a random x spot in the grid (-2 because they are the outside walls)

  } while (g[gameItems.mouse.y][gameItems.mouse.x] != TUNNEL); //Makes sure that the mouse can't spawn inside a wall and if it does to respawn it
}




//---------------------------------------------------------------------------
//----- Place everything
//---------------------------------------------------------------------------

void updateGridTwoPlayer(char grid[][TWOSIZEX], const char maze[][TWOSIZEX], GameItems& gameItems, PillData& pillData, MouseData& mouseData) //update grid configuration after each move
{
  void placeMazeTwoPlayer(char g[][TWOSIZEX], const char m[][TWOSIZEX]); //Places the maze
  void placeItemTwoPlayer(char g[][TWOSIZEX], const Item& item); //Places items inside the maze

  placeMazeTwoPlayer(grid, maze);	//Reset the empty maze configuration into grid

  for (int i(gameItems.snake.size() - 1); i >= 0; --i)		 //For each section of the snake
  {
    placeItemTwoPlayer(grid, gameItems.snake.at(i));	//Set snake in the grid
  }
  for (int i(gameItems.snake2.size() - 1); i >= 0; --i)		 //For each section of the snake
  {
    placeItemTwoPlayer(grid, gameItems.snake2.at(i));	//Set snake in the grid
  }
  placeItemTwoPlayer(grid, gameItems.mouse);	//Set mouse in the grid

  if (pillData.pillActive) //If the bool pillActive is true
  {
    placeItemTwoPlayer(grid, gameItems.pill); //Set the pill in the grid
  }
}

void placeMazeTwoPlayer(char grid[][TWOSIZEX], const char maze[][TWOSIZEX]) //reset the empty/fixed maze configuration into grid
{
  for (int row(0); row < TWOSIZEY; ++row)
    for (int col(0); col < TWOSIZEX; ++col)
      grid[row][col] = maze[row][col];
}

void placeItemTwoPlayer(char g[][TWOSIZEX], const Item& item) //Function to place an item inside the grid, used for snake, mouse and pill
{
  g[item.y][item.x] = item.symbol; //Gets the position and spawns the symbol there
}


//---------------------------------------------------------------------------
//----- process key
//---------------------------------------------------------------------------

void setPlayerOneKeyDirection(const int key, int& dx, int& dy)
{ //calculate direction indicated by key
  bool isArrowKey(const int k);
  switch (key)//...depending on the selected key...
  {
  case LEFT:  //when LEFT arrow pressed...
    dx = -1;	//decrease the X coordinate
    dy = 0;
    break;
  case RIGHT: //when RIGHT arrow pressed...
    dx = +1;	//increase the X coordinate
    dy = 0;
    break;
  case UP:  	//when UP arrow pressed...
    dx = 0;
    dy = -1;  //decrease the y coordinate
    break;
  case DOWN:  //when DOWN arrow pressed...
    dx = 0;
    dy = +1;  //increase the y coordinate
    break;

  }
}
void setPlayerTwoKeyDirection(const int key, int& dx, int& dy)
{ //calculate direction indicated by key
  bool isWASDKey(const int k);

  switch (key)//...depending on the selected key...
  {
  case PTLEFT:  //when A pressed...
    dx = -1;	//decrease the X coordinate
    dy = 0;
    break;
  case PTRIGHT: //when D arrow pressed...
    dx = +1;	//increase the X coordinate
    dy = 0;
    break;
  case PTUP:  	//when W pressed...
    dx = 0;
    dy = -1;  //decrease the y coordinate
    break;
  case PTDOWN:  //when S arrow pressed...
    dx = 0;
    dy = +1;  //increase the y coordinate
    break;

  }
}
int getKeyPressTwoPlayer()
{ //get key or command selected by user
  //KEEP THIS FUNCTION AS GIVEN
  int keyPressed;
  keyPressed = _getch();			//read in the selected arrow key or command letter
  while (keyPressed == 224) 		//ignore symbol following cursor key
    keyPressed = _getch();
  return keyPressed;
}


bool isWASDKey(const int key)
{	//check if the key pressed is WASD

  return (key == PTLEFT) || (key == PTRIGHT) || (key == PTUP) || (key == PTDOWN);
}



//---------------------------------------------------------------------------
//----- display info on screen
//---------------------------------------------------------------------------

void renderGameTwoPlayer(const char g[][TWOSIZEX], const string& mess, ScoreData& scoreData, MouseData& mouseData)
{ //display game title, messages, maze, spot and other items on screen
  string tostring(char x);
  string tostring(int x);

  void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);
  void paintGridTwoPlayer(const char g[][TWOSIZEX]);


  //display game title
  showMessage(clBlack, clDarkGreen, 0, 0, "___GAME___");

  showMessage(clWhite, clDarkGreen, 40, 1, "          " + getDate() + "   " + getTime() + "  ");
  showMessage(clWhite, clDarkGreen, 40, 2, "                                       ");
  showMessage(clWhite, clDarkGreen, 40, 3, "        FoP Task 1c - April 2019       ");
  showMessage(clWhite, clDarkGreen, 40, 4, "                                       ");

  showMessage(clWhite, clDarkGreen, 40, 5, " ------------ CS4G - Group 2 ----------");
  showMessage(clWhite, clDarkGreen, 40, 6, "                                       ");
  showMessage(clWhite, clDarkGreen, 40, 7, " Casey-Jo Kenny                b8016012");
  showMessage(clWhite, clDarkGreen, 40, 8, " Josh Long                     b8014932");
  showMessage(clWhite, clDarkGreen, 40, 9, " Kai Henry                     b8021762");
  //display menu options available

  showMessage(clWhite, clDarkGreen, 40, 10, "                                       ");
  showMessage(clWhite, clDarkGreen, 40, 11, " ------------- Player One -------------");
  showMessage(clWhite, clDarkGreen, 40, 12, "                                       ");
  showMessage(clWhite, clDarkGreen, 40, 13, " COLOURS          : MAGENTA AND GREEN  ");
  showMessage(clWhite, clDarkGreen, 40, 14, " TO MOVE          : USE KEYBOARD ARROWS");
  showMessage(clWhite, clDarkGreen, 40, 15, " CURRENT SCORE    : " + tostring(twoPlayerData.playerOneScore));
  showMessage(clWhite, clDarkGreen, 40, 16, "                                       ");
  showMessage(clWhite, clDarkGreen, 40, 17, " ------------- Player Two -------------");
  showMessage(clWhite, clDarkGreen, 40, 18, "                                       ");
  showMessage(clWhite, clDarkGreen, 40, 19, " COLOURS          : DARK BLUE AND CYAN ");
  showMessage(clWhite, clDarkGreen, 40, 20, " TO MOVE          : USE WASD ARROWS");
  showMessage(clWhite, clDarkGreen, 40, 21, " CURRENT SCORE    : " + tostring(twoPlayerData.playerTwoScore));
  showMessage(clWhite, clDarkGreen, 40, 22, "                                       ");

  //print auxiliary messages if any
  showMessage(clWhite, clDarkGreen, 40, 23, mess);	//display current message



  //display grid contents
  paintGridTwoPlayer(g);
}


void paintGridTwoPlayer(const char g[][TWOSIZEX])
{ //display grid content on screen
  selectBackColour(clBlack);
  selectTextColour(clWhite);
  gotoxy(0, 2);

  for (int row(0); row < TWOSIZEY; ++row)
  {
    for (int col(0); col < TWOSIZEX; ++col)
    {
      {
        if (g[row][col] == HEAD) //sets the head of the snake to Magenta
          selectTextColour(clMagenta);
        else if (g[row][col] == PLAYERTWOHEAD)
        {
          selectTextColour(clDarkBlue);
        }
        if (g[row][col] == TAIL) //Sets the body of the snake to green
          selectTextColour(clGreen);
        else if (g[row][col] == PLAYERTWOTAIL)
        {
          selectTextColour(clCyan);
        }
      }
      if (g[row][col] == MOUSE)
        selectTextColour(clYellow);
      if (g[row][col] == PILL)
        selectTextColour(clRed);

      cout << g[row][col];	//output cell content
      selectTextColour(clWhite);


    }
    cout << endl;
  }
}

//---------------------------------------------------------------------------
//----- "Behind the Scenes" Functions
//---------------------------------------------------------------------------


void pillCounterTwoPlayer(PillData& pillData) //Function to control the pill counter
{
  if (pillData.pillActive) //If the pill is on the grid
  {
    pillData.pillMoveCount++; //Add one to the counter
  }
}

void powerUpPillTwoPlayer(MouseData& mouseData, PillData& pillData) //A function to control the powerup pill
{
  if (mouseData.mouseCount == 2) //If the mouse counter is equal to 2
  {
    pillData.pillActive = true; //Set the pill to active so the pill spawns
    mouseData.mouseCount = 0; //Reset the mouse counter
  }
  if (pillData.pillMoveCount == 10) //If the pill move counter becomes equal to 10
  {
    pillData.pillActive = false; //Sets the pillActive to false to despawn the pill
    pillData.pillMoveCount = 0; //Reset the move counter
  }
}
