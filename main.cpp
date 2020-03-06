// writen by Jacky Fan Tsz Kin @ 6 Mar 2020
//	Github @redfrogss https://github.com/redfrogsss

#include <iostream> 
#include <vector>
#include <ctime>

using namespace std;

class Snake {
private:
	vector<vector<int>> snakePosition;	//{{x,y} , {x,y}}, snakePosition[0] = 1st digit position of snake, snakePosition[0][0] = x, snakePosition[0][1] = y
	vector<int> foodPosition;	//foodPosition[0] = x, foodPosition[1] = y
	bool isLost = false;
	int score = 0;
public:
	void initializePosition() {
		snakePosition.push_back({ 6,5 });
		snakePosition.push_back({ 5,5 });
		foodPosition.resize(2);
		generateFood();
	}

	void initializeScore() {
		
	}

	void go(string direction) {	//"up", "down", "left", "right"
		int target_x, target_y;	//target direct's position
		bool isValidDirection = true;

		target_x = snakePosition[0][0];
		target_y = snakePosition[0][1];

		if (direction == "up") {
			target_y--;
		}
		else if (direction == "down") {
			target_y++;
		}
		else if (direction == "left") {
			target_x--;
		}
		else if (direction == "right") {
			target_x++;
		}
		else {
			isValidDirection = false;
		}

		if (isValidDirection == true) {
			if (isValidMovement(target_x, target_y)) {
				moveSnake(target_x, target_y);
			}
		}
	}

	void moveSnake(int x, int y) {
		int i = snakePosition.size() - 1;

		while (i > 0) {
			snakePosition[i] = snakePosition[i-1];
			i--;
		}
		snakePosition[0] = { x, y };
	}

	void generateFood() {
		int x = 0, y = 0;	//x (1 ~ 48), y (1 ~ 18)

		srand(time(NULL));	//seed

		do {
			//generate food position
			x = rand() % 48 + 1;
			y = rand() % 28 + 1;
		} while (isValidFoodPosition(x, y) == false);

		//assign food
		foodPosition[0] = x;
		foodPosition[1] = y;
	}

	void eat() {
		int x = snakePosition[snakePosition.size() - 1][0];
		int y = snakePosition[snakePosition.size() - 1][1];
		snakePosition.push_back({x,y});

		score++;

		generateFood();
	}

	void askNextStep() {
		string nextStep;

		cout << "Please enter your next step:" << endl;
		cout << "(up/down/left/right)" << endl;
		cin >> nextStep;

		go(nextStep);
	}

	bool isValidFoodPosition(int x, int y) {
		//check if the food is in the snake's position
		for (int n = 0; n < snakePosition.size(); n++) {	// n(th) digit of snake
			if (snakePosition[n][0] == x && snakePosition[n][1] == y) {
				return false;
			}
		}

		//check range of x and y
		if (x > 0 && x < 49) {
			if (y > 0 && y < 19)
				return true;
			else
				return false;
		}
		else
			return false;
	}

	bool isValidMovement(int x, int y) {	//false = player failed, true = player can continue
		//if snake touch itself, return false
		if (x == snakePosition[1][0] && y == snakePosition[1][1]) {
			return false;
		}
		for (int i = 2; i < snakePosition.size(); i++) {
			if (x == snakePosition[i][0] && y == snakePosition[i][1]) {
				isLost = true;
				return false;
			}
		}

		//if snake touch walls, return true and set isLost to true
		//frame:	x == 0 || x == 49
		//			y == 0 || y == 19
		if (x <= 0 || x >= 49)
			isLost = true;

		if (y <= 0 || y >= 19)
			isLost = true;

		//if snake touch food, pass to eat() and return true
		if (x == foodPosition[0] && y == foodPosition[1])
			eat();

		return true;
	}

	vector<vector<int>> &getSnakePosition() {
		return snakePosition;
	}

	vector<int> getFoodPosition() {
		return foodPosition;
	}

	bool getIsLost() {
		return isLost;
	}

	int getScore() {
		return score;
	}
};

class Display {	//control how the game displayed
private:
	int width = 50, height = 20;	//the frame size of the game
	vector<vector<int>> map;	//store all digits in 50 * 20 frame, following ASCII code

public:
	void initializeMap() {	//initialize map to all 0 line-by-line
		map.clear();

		vector<int> line;
		line.resize(20,0);	//set a line to hv 20 zeros

		for (int i = 0; i < width; i++) {	//height, 0 ~ 19
			map.push_back(line);
		}
	}

	void setFramePixel() {
		//frame:	x == 0 || x == 49
		//			y == 0 || y == 19
		for (int y = 0; y < height; y++) {	//height, 0 ~ 19
			for (int x = 0; x < width; x++) {	//width, 0 ~ 49
				if (x == 0 || x == 49) {
					map[x][y] = 35; //set pixel to "#"
				}
				if (y == 0 || y == 19) {
					map[x][y] = 35;
				}
			}
		}
	}

	void setSnakePixel(vector<vector<int>> &snakePosition) {
		for (int x = 0; x < snakePosition.size(); x++) {
			if (x == 0) {
				map[snakePosition[x][0]][snakePosition[x][1]] = 43;	//head of the snake
			}
			else {
				map[snakePosition[x][0]][snakePosition[x][1]] = 79;
			}
		}
	}

	void setFoodPixel(vector<int> foodPosition) {
		int x = foodPosition[0], y = foodPosition[1];
		map[x][y] = 88;
	}

	void display() {
		char pixel;

		system("cls");	//clean screen

		for (int y = 0; y < height; y++) {	//height, 0 ~ 19
			for (int x = 0; x < width; x++) {	//width, 0 ~ 49
				pixel = map[x][y];
				cout << pixel;
			}
			cout << endl;
		}
	}

	void displayScore(int score) {
		cout << "Score:\t" << score << endl;
	}
};

void snake() {	//start the game
	Display screen;
	Snake player;

	player.initializePosition();

	while (player.getIsLost() == false) {
		screen.initializeMap();
		screen.setFramePixel();
		screen.setSnakePixel(player.getSnakePosition());
		screen.setFoodPixel(player.getFoodPosition());
		screen.display();
		screen.displayScore(player.getScore());

		player.askNextStep();
	}

	cout << "You lost! Your total score is " << player.getScore() << "."<< endl;
	system("pause");
}

int selectAction() {
	int action;

	//show menu
	system("cls");
	cout << "Welcome to THE SNAKE GAME." << endl;
	cout << "Please select action by entering the number." << endl;
	cout << "\t1) start the game" << endl;
	cout << "\t2) exit" << endl;

	//user input action name for taking specific action
	cin >> action;
	return action;
}

bool performAction(int action) {	//true to back to menu, false to exit the program
	system("cls");
	switch (action) {
	case 1:	
		//start the game
		snake();
		break;
	case 2:
		//exit
		return false;
		break;
	default:
		cout << "Please try again." << endl;
	}
	return true;
}

int main()
{
	while (performAction(selectAction()) == true);
	return 0;
}