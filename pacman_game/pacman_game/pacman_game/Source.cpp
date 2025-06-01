#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <string>
#include <vector>

using namespace std;
//global variables
int lives = 3;// number of lives 
char tmp_map[18][32];//temporary map to find path 
//game map
char map[18][32] =
{
	"+#############################+",
	"|                  #######    |",
	"|                             |",
	"|## ########### ##   #########|",
	"|   |                         |",
	"| | |### |  |           |     |",
	"| |      |  | |###  |   |  |  |",
	"| | #####|  | |      ## |     |",
	"| |           |###  |      |  |",
	"| |##### ###         ##       |",
	"|          ######  ####### ###|",
	"|                             |",
	"|# ### ####      ###   #######|",
	"|                             |",
	"|           #                 |",
	"|                             |",
	"|                             |",
	"+#############################+"
};
//function to display map of the game
void ShowMap()
{
	for (int i = 0; i < 18; i++)
	{
		printf("%s\n", map[i]);
	}
}

//function to set cursor position
void gotoxy(short x, short y)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);//get console
	COORD position = { x, y };//coordinate x and y

	SetConsoleCursorPosition(hStdout, position);//sets the cursor on x and y 
}
//create class for entity
class entity
{
public:
	//initialize x and y
	entity(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
	//move the entity horizontally
	void move_x(int p)
	{
		if (map[y][x + p] == ' ')//if destination is emplty
			x += p;
	}
	//move the entity virtically
	void move_y(int p)
	{
		if (map[y + p][x] == ' ')
			y += p;
	}
	//Function to move the entity by p units horizontally and q units vertically
	void move(int p, int q)
	{
		x += p;
		y += q;
	}
	//get the current x coordinate of the entity
	int get_x()
	{
		return x;
	}
	//get the current y coordinate of the entity
	int get_y()
	{
		return y;
	}
	//Function to draw the entity on the game map
	void draw(char p)
	{
		map[x][y] = p;
		gotoxy(x, y);
		printf("%c", p);
	}

	// variables to store entity's position
	int x;
	int y;
};

struct walk
{
	short walk_count;
	short x;
	short y;
	short back;
};

struct target
{
	short x;
	short y;
};

vector<target> walk_queue;//vector to store position of target

vector<walk> BFSArray;//vector to store search path info
//function to add position to search path
void AddArray(int x, int y, int wc, int back)
{
	//check if entity at x or y in temp map is empty
	if (tmp_map[y][x] == ' ' || tmp_map[y][x] == '.')
	{
		tmp_map[y][x] = '#';
		walk tmp;
		tmp.x = x;
		tmp.y = y;
		tmp.walk_count = wc;
		tmp.back = back;
		BFSArray.push_back(tmp);//add current position vector 
	}
}

void FindPath(int sx, int sy, int x, int y)
{
	memcpy(tmp_map, map, sizeof(map));
	BFSArray.clear();
	walk tmp;
	tmp.x = sx;
	tmp.y = sy;
	tmp.walk_count = 0;
	tmp.back = -1;
	BFSArray.push_back(tmp);

	int i = 0;
	//iterate while i is less than the size of BFSArray
	while (i < BFSArray.size())
	{
		// Check if the current position in 'BFSArray' matches x and y
		if (BFSArray[i].x == x && BFSArray[i].y == y)
		{
			walk_queue.clear();
			target tmp2;
			while (BFSArray[i].walk_count != 0)
			{
				tmp2.x = BFSArray[i].x;
				tmp2.y = BFSArray[i].y;
				walk_queue.push_back(tmp2);

				i = BFSArray[i].back;
			}

			break;
		}
		//add positions to search path
		AddArray(BFSArray[i].x + 1, BFSArray[i].y, BFSArray[i].walk_count + 1, i);
		AddArray(BFSArray[i].x - 1, BFSArray[i].y, BFSArray[i].walk_count + 1, i);
		AddArray(BFSArray[i].x, BFSArray[i].y + 1, BFSArray[i].walk_count + 1, i);
		AddArray(BFSArray[i].x, BFSArray[i].y - 1, BFSArray[i].walk_count + 1, i);

		i++;
	}

	BFSArray.clear();
}
//function for classic mode 
void classic()
{

	system("cls");
	//menu
	printf("Instruction:\n1. Arrow Keys to move your hero\n2. Eat the dots produced by the Eater to gain poins\n3. Don't get caught by the Eater\n\n");
	printf("H -> Hard\nN -> Normal\nE -> Easy\n\nInput : ");

	char diffi;// variable for difficulty level
	//get input for level
	cin >> diffi;
	int pts = 0;//variable for points
	//speed of game based on chosen difficulty level
	int speedmod = 3;
	if (diffi == 'N')//normal level
	{
		speedmod = 2;
	}
	else if (diffi == 'H')//hard level
	{
		speedmod = 1;
	}
	//FOR loop for number of lives
	for (int i = 3; i > 0; i--)
	{
		bool running = true;
		int x = 15; // hero x
		int y = 16; // hero y
		int old_x;
		int old_y;

		int ex = 1;
		int ey = 1;

		system("cls");
		ShowMap();//show game map
		//display the player at initial position
		gotoxy(x, y);
		cout << "H";

		int frame = 0;
		//path for the EATER
		FindPath(ex, ey, x, y);
		//loop for the game
		while (running)
		{
			gotoxy(x, y);
			cout << " ";

			old_x = x;
			old_y = y;

			if (GetAsyncKeyState(VK_UP))
			{
				if (map[y - 1][x] == '.') { y--; pts++; }
				else
					if (map[y - 1][x] == ' ') y--;
			}
			if (GetAsyncKeyState(VK_DOWN))
			{
				if (map[y + 1][x] == '.') { y++; pts++; }
				else
					if (map[y + 1][x] == ' ') y++;
			}
			if (GetAsyncKeyState(VK_LEFT))
			{
				if (map[y][x - 1] == '.') { x--; pts++; }
				else
					if (map[y][x - 1] == ' ') x--;
			}
			if (GetAsyncKeyState(VK_RIGHT))
			{
				if (map[y][x + 1] == '.') { x++; pts++; }
				else
					if (map[y][x + 1] == ' ') x++;
			}
			//update path of eater if player moves
			if (old_x != x || old_y != y)
			{
				FindPath(ex, ey, x, y);
			}

			gotoxy(x, y); cout << "H";
			//erase the dots eaten by player
			map[ey][ex] = '.';
			gotoxy(ex, ey); cout << ".";
			//move the eater based on path
			if (frame % speedmod == 0 && walk_queue.size() != 0)
			{
				ex = walk_queue.back().x;
				ey = walk_queue.back().y;
				walk_queue.pop_back();
			}

			gotoxy(ex, ey); cout << "E";
			//check if player is caught by eater
			if (ex == x && ey == y)
			{
				break;
			}
			//display score and lives remaining
			gotoxy(32, 18);
			gotoxy(32, 1);
			cout << "Score: " << pts;
			Sleep(100);
			frame++;

			cout << "\tlives remaining: " << i << endl;

		}
	}
	//game messege
	system("cls");
	cout << "***************************GAME OVER***************************" << endl;
	cout << "\t\t\tYour Highest score: " << pts << endl;
	system("pause");
}

//function for survival mode
void survival()
{
	bool running = true;
	int x = 15; // hero x
	int y = 16; // hero y
	int old_x;
	int old_y;

	int ex = 1;
	int ey = 1;


	int pts = 0;
	int speedmod = 3;
	system("cls");
	ShowMap();

	gotoxy(x, y); cout << "H";

	int frame = 0;

	FindPath(ex, ey, x, y);

	while (running)
	{
		gotoxy(x, y); cout << " ";

		old_x = x;
		old_y = y;

		if (GetAsyncKeyState(VK_UP))
		{
			if (map[y - 1][x] == '.') { y--; pts++; }
			else
				if (map[y - 1][x] == ' ') y--;
		}
		if (GetAsyncKeyState(VK_DOWN))
		{
			if (map[y + 1][x] == '.') { y++; pts++; }
			else
				if (map[y + 1][x] == ' ') y++;
		}
		if (GetAsyncKeyState(VK_LEFT))
		{
			if (map[y][x - 1] == '.') { x--; pts++; }
			else
				if (map[y][x - 1] == ' ') x--;
		}
		if (GetAsyncKeyState(VK_RIGHT))
		{
			if (map[y][x + 1] == '.') { x++; pts++; }
			else
				if (map[y][x + 1] == ' ') x++;
		}

		if (old_x != x || old_y != y)
		{
			FindPath(ex, ey, x, y);
		}

		gotoxy(x, y); cout << "H";

		map[ey][ex] = '.';
		gotoxy(ex, ey); cout << ".";

		if (frame % speedmod == 0 && walk_queue.size() != 0)
		{
			ex = walk_queue.back().x;
			ey = walk_queue.back().y;
			walk_queue.pop_back();
		}

		gotoxy(ex, ey); cout << "E";

		if (ex == x && ey == y)
		{
			break;
		}



		gotoxy(32, 18);
		gotoxy(32, 1); cout << pts;
		Sleep(100);
		frame++;

		if (pts > 50)
		{
			speedmod = 2;
		}if (pts > 100)
		{
			speedmod = 1;
		}

	}

	system("cls");
	cout << "***********************************GAME OVER***********************************" << endl;
	cout << "\t\t\tYour Highest score: " << pts << endl << endl;
	system("pause");
}

void menu()
{
	int ss;//user choice
	char cont;
	//display main menu
	do
	{
		system("cls");
		cout << "***********************************************" << endl;
		cout << "***********************************************" << endl;

		cout << "      ############		       " << endl;
		cout << "    #################             " << endl;
		cout << "   #################                " << endl;
		cout << "  ##############				    " << endl;
		cout << " #############		     PACMAN.... " << endl;
		cout << "  ##############		           " << endl;
		cout << "   #################			   " << endl;
		cout << "    #################	          " << endl;
		cout << "       ############			   " << endl;
		cout << "\n";
		cout << "Please select your mode:" << endl;
		cout << "1. Classic mode" << endl;
		cout << "2. Survival mode" << endl;
		cout << "3. How to play" << endl;
		cout << "***********************************************" << endl;
		cout << "***********************************************" << endl;

		cin >> ss;
		switch (ss)
		{
		case 1://classic mode
			do
			{
				classic();
				system("cls");
				cout << "Do you wish to play again?" << endl;
				cout << "Enter Y/N: ";
				cin >> cont;
			} while (cont == 'Y');
			cout << "To return to main menu, enter 0. To quit, enter 9. ";
			cout << "Your choice: ";
			cin >> ss;
			if (ss == 9)
			{
				system("cls");
				cout << "Exiting..." << endl;
				cout << "Bye-bye..." << endl;
			}
			break;
		case 2://survival mode
			do
			{
				survival();
				system("cls");
				cout << "Do you wish to play again?" << endl;
				cout << "Enter Y/N: ";
				cin >> cont;
			} while (cont == 'Y');
			cout << "To return to main menu, enter 0. To quit, enter 9. ";
			cout << "Your choice: ";
			cin >> ss;
			if (ss == 9)
			{
				system("cls");
				cout << "Exiting..." << endl;
				cout << "Bye-bye..." << endl;
			}
			break;
		case 3://instruction on how to play
			system("cls");
			cout << "--------------------------------------------------------------" << endl;
			cout << " HOW TO PLAY: " << endl;
			cout << "--------------------------------------------------------------" << endl << endl;
			cout << "Classic Mode:" << endl;
			cout << "1. Choose your map type." << endl;
			cout << "2. Choose your level of difficulty." << endl;
			cout << "3. Play the game by using the LEFT, RIGHT, UP, DOWN keys" << endl;
			cout << "   to move the Hero through the map. Eat the '.'s to score points." << endl;
			cout << "   Avoid getting eaten by the Eater." << endl << endl;
			cout << "--------------------------------------------------------------" << endl << endl;
			cout << "Survival Mode: " << endl;
			cout << "1. Choose your map type." << endl;
			cout << "2. Play the game by using the LEFT, RIGHT, UP, DOWN keys" << endl;
			cout << "   to move the Hero through the map. Eat the '.'s to score points." << endl;
			cout << "   Avoid getting eaten by the Eater." << endl;
			cout << "3. The Eater will increase in speed as you score more points." << endl << endl;
			cout << "--------------------------------------------------------------" << endl << endl;
			cout << "Enter 0 to return to menu." << endl;
			cin >> ss;
		case 4://exit the game
			system("cls");
			cout << "Exiting..." << endl;
			cout << "Bye-bye..." << endl;
		}
	} while (ss == 0);
}

int main()
{
	//color of background and text
	system("COLOR FC");
	system("COLOR 0E");

	menu();
	//pause and display press any key to continue messege
	system("pause");
	return 0;
}
