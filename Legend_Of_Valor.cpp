#include<iostream>
#include<string>
#include<fstream>
#include<cstdlib>
using namespace std;

const int Dimension = 14;

string loadgame(string filename) {
	string name;
	ifstream infile(filename); 
	if (!infile) {
		cout << "File cannot be open" << endl;
		return name;
	}
	char c;
	infile >> c;
	while (infile && c != ',') {
		name += c;
		infile >> c;
	}
	return name;

}
class Player;
class Enemy {
public:
	string name;
	int Health;
	int attack;
	Enemy() {
		name = "";
		Health = 0;
		attack = 0;
	}
	Enemy(string name, int Health, int attack) {
		this->name = name;
		this->Health = Health;
		this->attack = attack;
	}
	virtual string getname() const = 0;
	virtual int getHealth() const = 0;
	virtual int getattack() const = 0;
	virtual void attackPlayer(Player* P) = 0;
	virtual ~Enemy() {}
};

class Player {
public:
	string filename;
	static int count;
	string name;
	int health, mana, experience, attack, defense, level, x, y;
	Player() :name(""), health(0), mana(0), experience(0), attack(0), defense(0), level(0), filename(), x(13), y(0) {
		count++;
	}
	Player(string n, string namefile) :name(n), health(100), mana(100), experience(0), attack(30), defense(30), level(0), filename(namefile), x(13), y(0) {
		count++;
	}
	virtual void getPlayerInfo(string n, string namefile) = 0;
	void setX(int x) {
		this->x = x;
	}
	void setY(int y) {
		this->y = y;
	}
	int getX()const {
		return x;
	}
	int getY() const {
		return y;
	}
	string getname()const {
		return name;
	}
	int getHealth() const {
		return health;
	}
	int getMana() const {
		return mana;
	}
	int getExperience()const {
		return experience;
	}
	int getAttack() const {
		return attack;
	}
	int getDefense()const {
		return defense;
	}
	int getLevel() const {
		return level;
	}
	int getcount() const {
		return count;
	}
	void getattacked(Enemy* E) {
		health = health - E->attack;
	}
	virtual void attackEnemy(Enemy* E) = 0;
	virtual void displayStats() = 0;
	virtual void ShowPlayerInventory() = 0;
	virtual void HealPlayer() = 0;
	virtual void UseSpecialSkill(Enemy* E) = 0;
	virtual void DefensiveSkil() = 0;
	virtual void LevelUp() = 0;
	virtual void GainExperience(int ex) = 0;
	virtual void UseShop() = 0;
	virtual void fightenemy(Player* P, Enemy* E) = 0;
	void save() {
		ofstream outfile(filename, ios::trunc);
		outfile << name << "," << health << "," << mana << "," << experience << "," << attack << "," << defense << "," << level << "," << x << "," << y << endl;
		outfile.close();
	}
	void quit() {
		int n;
		cout << "1.Save and Quit." << endl;
		cout << "2.Quit" << endl;
		cin >> n;
		if (n == 1) {
			save();
			exit(0);
		}
		else if (n == 2) {
			exit(0);
		}
		else {
			cout << "Invalid input. Returning to game." << endl; 
		}
	}
	void loadthegame() {
		ifstream infile(filename); 
		if (!infile) {
			cout << "Save file cannot be opened." << endl;
			return;
		}
		char c;
		do {
			infile >> c;
		} while (infile && c != ','); 
		infile >> health;
		infile >> c;
		infile >> mana;
		infile >> c;
		infile >> experience;
		infile >> c;
		infile >> attack;
		infile >> c;
		infile >> defense;
		infile >> c;
		infile >> level;
		infile >> c;
		infile >> x;
		infile >> c;
		infile >> y;
		infile.close();
	}
	friend class Board;

	virtual ~Player() {} 
};

int Player::count = 0;

class Board {
protected:
	string** board;
public:
	Board() {
		board = new string * [Dimension];
		for (int i = 0; i < Dimension; i++) {
			board[i] = new string[Dimension];
		}
		for (int i = 0; i < Dimension; i++) {
			for (int j = 0; j < Dimension; j++) {
				board[i][j] = ".";
			}
		}
	}
	void displayBoard(Player* P)const {
		board[P->getX()][P->getY()] = "P";
		cout << "Current status of Game is:-" << endl;
		cout << "---------------------------------------------------------" << endl;
		for (int i = 0; i < Dimension; i++) {
			cout << "| ";
			for (int j = 0; j < Dimension; j++) {
				cout << board[i][j] << " | ";
			}
			cout << endl;

			cout << "---------------------------------------------------------" << endl;
		}
	}
	bool CheckHound(int x, int y) {
		if (board[x][y] == "H") {
			return true;
		}
		else {
			return false;
		}
	}
	bool CheckDragon(int x, int y) {
		if (board[x][y] == "D") {
			return true;
		}
		else {
			return false;
		}
	}
	bool CheckWitch(int x, int y) {
		if (board[x][y] == "W") {
			return true;
		}
		else {
			return false;
		}
	}
	bool CheckBoss(int x, int y) {
		if (board[x][y] == "B") {
			return true;
		}
		else {
			return false;
		}
	}

	bool MovePlayer(Player& P, int x, int y) {
		if (x < 0 || x >= Dimension || y < 0 || y >= Dimension) {
			cout << endl << "Those coordinates are off the map." << endl << endl;
			return false;
		}
		else if (x > P.getX() + 1 || x < P.getX() - 1 || y > P.getY() + 1 || y < P.getY() - 1) {
			cout << endl << "Such travel is not possible." << endl << endl;
			return false;
		}
		else if (x < 11 && P.level < 5) {
			cout << endl << "Level too low to move to Dragon Territory." << endl << endl;
			return false;
		}
		else if (x < 9 && P.level < 10) {
			cout << endl << "Level too low to move to Witches Territory." << endl << endl;
			return false;
		}
		else if (x < 5 && P.level < 15) {
			cout << endl << "Level too low to face Boss." << endl << endl;
			return false;
		}
		else {
			board[P.getX()][P.getY()] = ".";
			P.setX(x);
			P.setY(y);
			board[x][y] = "P";
			return true;
		}
	}
	void PlaceHounds() {
		const int NumHound = 7;
		int size = Dimension;
		for (int i = 0; i < NumHound; i++) {
			int Xaxis = 12 + (rand() % 3);
			int Yaxis = rand() % (size - 1) + 1;

			board[Xaxis - 1][Yaxis - 1] = "H";

		}
	}
	void PlaceDragon() {
		const int NumDragon = 10;
		int size = Dimension;
		for (int i = 0; i < NumDragon; i++) {
			int Xaxis = 9 + (rand() % 3);
			int Yaxis = rand() % (size - 1) + 1;

			board[Xaxis - 1][Yaxis - 1] = "D";
		}
	}
	void PlaceWitch() {
		const int NumWitch = 10;
		int size = Dimension;
		for (int i = 0; i < NumWitch; i++) {
			int Xaxis = 5 + (rand() % 4);
			int Yaxis = rand() % (size - 1) + 1;

			board[Xaxis - 1][Yaxis - 1] = "W"; 
		}
	}
	void PlaceBoss() {
		const int NumHound = 1;
		int size = Dimension;
		for (int i = 0; i < NumHound; i++) {
			int Xaxis = 1;
			int Yaxis = 7;

			board[Xaxis - 1][Yaxis - 1] = "B";

		}
	}
	~Board() {
		for (int i = 0; i < Dimension; i++) {
			delete[] board[i];
		}
		delete[] board;
	}
};

class Inventory {
	string* invent;
	int count;
public:
	Inventory() {
		invent = new string[10];
		count = 0;
	}
	void ShowInventory() {
		cout << endl << "1. :" << invent[0] << endl;
		cout << "2. :" << invent[1] << endl;
		cout << "3. :" << invent[2] << endl << endl;
	}
	void addItem(string n) {
		if (count > 9) {
			cout << "Inventory limit exceeded";
		}
		else {
			for (int i = 0; i < count; i++) {
				if (invent[i] == n) {
					return;
				}
			}
			invent[count] = n;
			count++;
		}
	}
	void RemoveObject(string n) {
		if (count <= 0) {
			cout << "Items cannot be less than 0.";
		}
		else {
			for (int i = 0; i < count; i++) {
				if (invent[i] == n) {
					for (int j = i; j < count - 1; j++) { 
						invent[j] = invent[j + 1];
					}
					count--;
					return;
				}
			}
		}
	}

	bool UseHealItem() {
		for (int i = 0; i < count; i++) {
			if (invent[i] == "Healing Portion") {
				for (int j = i; j < count - 1; j++) {
					invent[j] = invent[j + 1];
				}
				count--;
				cout << "Healing Portion is used successfully." << endl;
				return true;
			}
		}
		cout << "No healing portion is available for healing." << endl;
		return false;
	}
	~Inventory() {}
};

class WarriorShop {
public:

	string Menu(int& mana) {
		int choice;
		cout << endl << "+---------------   Warrior Item Shop   ------------------+" << endl;
		cout << "|                                                        |" << endl;
		cout << "| 1. Sword.              ( 20 Mana Points )              |" << endl;
		cout << "| 2. Shield              ( 25 Mana Points )              |" << endl;
		cout << "| 3. Healing Portion     ( 15 Mana Points )              |" << endl;
		cout << "| 4. Armor               ( 20 Mana Points )              |" << endl;
		cout << "|                                                        |" << endl;
		cout << "+--------------------------------------------------------+" << endl << endl;
		cout << "Enter your option:";
		cin >> choice;
		switch (choice) {
		case 1: {
			if (mana > 20) {
				mana -= 20;
				cout << endl << "Sword is successfully purchased and added to inventory." << endl;
				return "Sword";
			}
			else {
				cout << "Insufficient Mana." << endl;
			}
			break;
		}
		case 2: {
			if (mana > 25) {
				mana -= 25;
				cout << endl << "Shield is successfully purchased and added to inventory." << endl;
				return "Shield";
			}
			else {
				cout << "Insufficient Mana." << endl;
			}
			break;
		}
		case 3: {
			if (mana > 15) {
				mana -= 15;
				cout << endl << "Healing Portion is successfully purchased and added to inventory." << endl;
				return "Healing Portion";
			}
			else {
				cout << "Insufficient Mana." << endl;
			}
			break;
		}
		case 4: {
			if (mana > 20) {
				mana -= 20;
				cout << endl << "Armor is successfully purchased and added to inventory." << endl;
				return "Armor";
			}
			else {
				cout << "Insufficient Mana." << endl;
			}
			break;
		}
		default: {
			cout << "Invalid Choice." << endl;
		}
		}
		return "";
	}
	~WarriorShop() {}
};

class Warrior :public Player, public Inventory, public WarriorShop {
public:
	Warrior() :Player() {}
	Warrior(string n, string namefile) :Player(n, namefile) {}
	void getPlayerInfo(string n, string namefile) {
		this->filename = namefile;
		this->name = n;
		this->health = 100;
		this->mana = 100;
		this->experience = 0;
		this->attack = 30;
		this->defense = 30;
		this->level = 0;
		this->x = Dimension - 1;
		this->y = 0;
	}
	void displayStats()override {
		cout << endl << "Stats of " << getname() << ":-" << endl;
		cout << "Level of " << getname() << " is: " << getLevel() << endl;
		cout << "Health of " << getname() << " is: " << getHealth() << endl;
		cout << "Attack of " << getname() << " is: " << getAttack() << endl;
		cout << "Defense of " << getname() << " is: " << getDefense() << endl;
		cout << "Experience of " << getname() << " is " << getExperience() << endl;
		cout << "Mana of " << getname() << " is: " << getMana() << endl;
	}
	void attackEnemy(Enemy* E)override {
		E->Health -= attack;
	}
	void ShowPlayerInventory()override {
		cout << "Inventory of " << getname() << ":-" << endl;
		ShowInventory();
	}
	void addPlayerObject(string n) {
		addItem(n);
	}
	void RemovePlayerObject(string n) {
		RemoveObject(n);
	}
	void HealPlayer() override {
		if (UseHealItem()) {
			health += 30;
			if (health > 100) {
				health = 100;
			}
			cout << getname() << "'s health restored to " << health << "." << endl;
		}
	}
	void UseSpecialSkill(Enemy* E) override {
		if (mana > 50) {
			cout << "-------   Rasengan (Super Punch)   -------" << endl;;
			E->Health -= 50;
			mana -= 50;
		}
		else {
			cout << "Mana too low for using special ability." << endl;
		}
	}
	void DefensiveSkil() override {
		if (mana > 50) {
			cout << "Defensive skill activated." << endl;
			mana -= 50;
		}
		else {
			cout << "Not enough mana to activate Defensive Skill." << endl;
		}
	}
	void LevelUp()override {
		level++;
	}
	void GainExperience(int ex)override {
		experience += ex;
	}
	void UseShop()override {
		string choice = Menu(mana);
		if (!choice.empty()) { 
			addItem(choice);
		}
	}
	bool checkwin(Enemy* E) {
		if (E->getHealth() <= 0) {
			cout << "You have defeated the " << E->getname() << endl;
			return true;
		}
		return false;
	}
	bool checklose(Enemy* E) {
		if (health <= 0) {
			cout << "You have been defeated by " << E->getname() << endl;
			return true;
		}
		return false;
	}
	void fightenemy(Player* P, Enemy* E) {
		cout << endl << "A " << E->getname() << " has appeared." << endl << endl;

		while (health > 0 && E->getHealth() > 0) {
			cout << "Your turn : " << endl;
			int choice;
			bool validChoice = false;

			while (!validChoice) {
				cout << "Enter your choice :" << endl;
				cout << "1.For Attacking the " << E->getname() << endl;
				cout << "2.For Healing." << endl;
				cout << "3.For Using special skill." << endl;
				cout << "4.For Using Defensive skill." << endl;
				cin >> choice;

				if (choice >= 1 && choice <= 4) {
					validChoice = true;
					switch (choice) {
					case 1:
						P->attackEnemy(E);
						cout << "You dealt " << attack << " damage!" << endl;
						break;
					case 2:
						P->HealPlayer();
						break;
					case 3:
						P->UseSpecialSkill(E);
						break;
					case 4:
						P->DefensiveSkil();
						break;
					}
				}
				else {
					cout << "Invalid input. Enter again." << endl;
				}
			}

			if (checkwin(E)) {
				break;
			}

			E->attackPlayer(P);
			cout << "Enemy dealt " << E->getattack() << " damage!" << endl;

			if (checklose(E)) {
				break;
			}

			cout << endl;
			P->displayStats();
			cout << endl;
		}

		if (E->getHealth() <= 0) {
			if (E->getname() == "Boss") {
				cout << endl << "*****************************************************" << endl;
				cout << "  Congratulations, " << P->getname() << "! You have defeated" << endl;
				cout << "  the Boss and completed Legends of Valor. YOU WIN!" << endl;
				cout << "*****************************************************" << endl;
				exit(0);
			}
			P->LevelUp();
			P->GainExperience(25);
		}
		else {
			cout << endl << "GAME OVER. " << P->getname() << " has fallen in battle." << endl;
			exit(0);
		}
		return;
	}
	~Warrior() {}
};

class MageShop {
public:
	string Menu(int& mana) {
		int choice;
		cout << endl << "+--------------   Mage Item Shop   ------------------+" << endl;
		cout << "|                                                       |" << endl;
		cout << "| 1. Dragon Breath.              ( 20 Mana Points )     |" << endl;
		cout << "| 2. Susano (Body	Defense)      ( 25 Mana Points )     |" << endl;
		cout << "| 3. Healing Portion            ( 15 Mana Points )     |" << endl;
		cout << "| 4. Mana Cloak                  ( 20 Mana Points )     |" << endl;
		cout << "|                                                       |" << endl;
		cout << "+-------------------------------------------------------+" << endl << endl;
		cout << "Enter your option:";
		cin >> choice;
		switch (choice) {
		case 1: {
			if (mana > 20) {
				mana -= 20;
				cout << endl << "Dragon Breath is successfully purchased and added to inventory." << endl;
				return "Dragon Breath";
			}
			else {
				cout << "Insufficient Mana." << endl;
			}
			break;
		}
		case 2: {
			if (mana > 25) {
				mana -= 25;
				cout << endl << "Susano is successfully purchased and added to inventory." << endl;
				return "Susano";
			}
			else {
				cout << "Insufficient Mana." << endl;
			}
			break;
		}
		case 3: {
			if (mana > 15) {
				mana -= 15;
				cout << endl << "Healing Portion is successfully purchased and added to inventory." << endl;
				return "Healing Portion";
			}
			else {
				cout << "Insufficient Mana." << endl;
			}
			break;
		}
		case 4: {
			if (mana > 20) {
				mana -= 20;
				cout << endl << "Mana Cloak is successfully purchased and added to inventory." << endl;
				return "Mana Cloak";
			}
			else {
				cout << "Insufficient Mana." << endl;
			}
			break;
		}
		default: {
			cout << "Invalid Choice." << endl;
		}
		}
		return "";
	}
	~MageShop() {}
};

class Mage :public Player, public Inventory, public MageShop {
public:
	Mage() :Player() {}
	Mage(string n, string namefile) :Player(n, namefile) {}
	void getPlayerInfo(string n, string namefile) {
		this->filename = namefile;
		this->name = n;
		this->health = 100;
		this->mana = 100;
		this->experience = 0;
		this->attack = 30;
		this->defense = 30;
		this->level = 0;
		this->x = Dimension - 1;
		this->y = 0;
	}
	void displayStats() override {
		cout << endl << "Stats of " << getname() << ":-" << endl;
		cout << "Level of " << getname() << " is: " << getLevel() << endl;
		cout << "Health of " << getname() << " is: " << getHealth() << endl;
		cout << "Attack of " << getname() << " is: " << getAttack() << endl;
		cout << "Defense of " << getname() << " is: " << getDefense() << endl;
		cout << "Experience of " << getname() << " is " << getExperience() << endl;
		cout << "Mana of " << getname() << " is: " << getMana() << endl;
	}
	void attackEnemy(Enemy* E) override {
		E->Health -= attack;
	}
	void ShowPlayerInventory() override {
		cout << "Inventory of " << getname() << ":-" << endl;
		ShowInventory();
	}
	void addPlayerObject(string n) {
		addItem(n);
	}
	void RemovePlayerObject(string n) {
		RemoveObject(n);
	}
	void HealPlayer() override {
		if (UseHealItem()) {
			health += 30;
			if (health > 100) {
				health = 100;
			}
			cout << getname() << "'s health restored to " << health << "." << endl;
		}
	}
	void UseSpecialSkill(Enemy* E) override {
		if (mana > 50) {
			cout << "-------   Rasengan (Super Punch)   -------" << endl;
			E->Health -= 50;
			health += 40;
			mana -= 50;
		}
		else {
			cout << "Mana too low for using special ability." << endl;
		}
	}
	void DefensiveSkil() override {
		if (mana > 50) {
			cout << "Defensive skill activated." << endl;
			mana -= 50;
		}
		else {
			cout << "Not enough mana to activate Defensive Skill." << endl;
		}
	}
	void LevelUp() override {
		level++;
	}
	void GainExperience(int ex) override {
		experience += ex;
	}
	void UseShop() override {
		string choice = Menu(mana);
		if (!choice.empty()) {
			addItem(choice);
		}
	}
	bool checkwin(Enemy* E) {
		if (E->getHealth() <= 0) {
			cout << "You have defeated the " << E->getname() << endl;
			return true;
		}
		return false;
	}
	bool checklose(Enemy* E) {
		if (health <= 0) {
			cout << "You have been defeated by " << E->getname() << endl;
			return true;
		}
		return false;
	}
	void fightenemy(Player* P, Enemy* E) override {
		cout << endl << "A " << E->getname() << " has appeared." << endl << endl;

		while (health > 0 && E->getHealth() > 0) {
			cout << "Your turn : " << endl;
			int choice;
			bool validChoice = false;

			while (!validChoice) {
				cout << "Enter your choice :" << endl;
				cout << "1.For Attacking the " << E->getname() << endl;
				cout << "2.For Healing." << endl;
				cout << "3.For Using special skill." << endl;
				cout << "4.For Using Defensive skill." << endl;
				cin >> choice;

				if (choice >= 1 && choice <= 4) {
					validChoice = true;
					switch (choice) {
					case 1:
						attackEnemy(E);
						cout << "You dealt " << attack << " damage!" << endl;
						break;
					case 2:
						HealPlayer();
						break;
					case 3:
						UseSpecialSkill(E);
						break;
					case 4:
						DefensiveSkil();
						break;
					}
				}
				else {
					cout << "Invalid input. Enter again." << endl;
				}
			}

			if (checkwin(E)) {
				break;
			}

			E->attackPlayer(P);
			cout << "Enemy dealt " << E->getattack() << " damage!" << endl;

			if (checklose(E)) {
				break;
			}

			cout << endl;
			P->displayStats();
			cout << endl;
		}

		if (E->getHealth() <= 0) {
			if (E->getname() == "Boss") {
				cout << endl << "*****************************************************" << endl;
				cout << "  Congratulations, " << P->getname() << "! You have defeated" << endl;
				cout << "  the Boss and completed Legends of Valor. YOU WIN!" << endl;
				cout << "*****************************************************" << endl;
				exit(0);
			}
			P->GainExperience(25);
			P->LevelUp();
			cout << "Updated Stats are:-" << endl;
			P->displayStats();
			cout << endl;
		}
		else {
			cout << endl << "GAME OVER. " << P->getname() << " has fallen in battle." << endl;
			exit(0);
		}
		return;
	}
	~Mage() {}
};

class ArcherShop {
public:
	string Menu(int& mana) { 
		int choice;
		cout << endl << "+--------------   Archer Shop   ------------------+" << endl;
		cout << "|                                                       |" << endl;
		cout << "| 1. Poison Arrow.               ( 20 Mana Points )     |" << endl;
		cout << "| 2. Golden Bow                  ( 10 Mana Points )     |" << endl;
		cout << "| 3. Healing Portion             ( 15 Mana Points )     |" << endl;
		cout << "| 4. Archer Suit                 ( 25 Mana Points )     |" << endl;
		cout << "|                                                       |" << endl;
		cout << "+-------------------------------------------------------+" << endl << endl;
		cout << "Enter your option:";
		cin >> choice;
		switch (choice) {
		case 1: {
			if (mana > 20) {
				mana -= 20;
				cout << endl << "Poison Arrow is successfully purchased and added to inventory." << endl;
				return "Poison Arrow";
			}
			else {
				cout << "Insufficient Mana." << endl;
			}
			break;
		}
		case 2: {
			if (mana > 10) {
				mana -= 10;
				cout << endl << "Golden Bow is successfully purchased and added to inventory." << endl;
				return "Golden Bow";
			}
			else {
				cout << "Insufficient Mana." << endl;
			}
			break;
		}
		case 3: {
			if (mana > 15) {
				mana -= 15;
				cout << endl << "Healing Medkit is successfully purchased and added to inventory." << endl;
				return "Healing Medkit";
			}
			else {
				cout << "Insufficient Mana." << endl;
			}
			break;
		}
		case 4: {
			if (mana > 25) {
				mana -= 25;
				cout << endl << "Archer Suit is successfully purchased and added to inventory." << endl;
				return "Archer Suit";
			}
			else {
				cout << "Insufficient Mana." << endl;
			}
			break;
		}
		default: {
			cout << "Invalid Choice." << endl;
		}
		}
		return "";
	}
	~ArcherShop() {}
};

class Archer :public Player, public Inventory, public ArcherShop {
public:
	Archer() :Player() {}
	Archer(string n, string namefile) :Player(n, namefile) {}
	void getPlayerInfo(string n, string namefile) {
		this->filename = namefile;
		this->name = n;
		this->health = 100;
		this->mana = 100;
		this->experience = 0;
		this->attack = 30;
		this->defense = 30;
		this->level = 0;
		this->x = Dimension - 1;
		this->y = 0;
	}
	void displayStats() override {
		cout << endl << "Stats of " << getname() << ":-" << endl;
		cout << "Level of " << getname() << " is: " << getLevel() << endl;
		cout << "Health of " << getname() << " is: " << getHealth() << endl;
		cout << "Attack of " << getname() << " is: " << getAttack() << endl;
		cout << "Defense of " << getname() << " is: " << getDefense() << endl;
		cout << "Experience of " << getname() << " is " << getExperience() << endl;
		cout << "Mana of " << getname() << " is: " << getMana() << endl;
	}
	void attackEnemy(Enemy* E) override {
		E->Health -= attack;
	}
	void ShowPlayerInventory() override {
		cout << "Inventory of " << getname() << ":-" << endl;
		ShowInventory();
	}
	void addPlayerObject(string n) {
		addItem(n);
	}
	void RemovePlayerObject(string n) {
		RemoveObject(n);
	}
	void HealPlayer() override {
		if (UseHealItem()) {
			health += 30;
			if (health > 100) {
				health = 100;
			}
			cout << getname() << "'s health restored to " << health << "." << endl;
		}
	}
	void UseSpecialSkill(Enemy* E) override {
		if (mana > 50) {
			cout << "-------   Rasengan (Super Punch)   -------" << endl;
			E->Health -= 50;
			mana -= 50;
		}
		else {
			cout << "Mana too low for using special ability." << endl;
		}
	}
	void DefensiveSkil() override {
		if (mana > 50) {
			cout << "Defensive skill activated." << endl;
			mana -= 50;
		}
		else {
			cout << "Not enough mana to activate Defensive Skill." << endl;
		}
	}
	void LevelUp() override {
		level++;
	}
	void GainExperience(int ex) override {
		experience += ex;
	}
	void UseShop() override {
		string choice = Menu(mana);
		if (!choice.empty()) {
			addItem(choice);
		}
	}
	bool checkwin(Enemy* E) {
		if (E->getHealth() <= 0) {
			cout << "You have defeated the " << E->getname() << endl;
			return true;
		}
		return false;
	}
	bool checklose(Enemy* E) {
		if (health <= 0) {
			cout << "You have been defeated by " << E->getname() << endl;
			return true;
		}
		return false;
	}
	void fightenemy(Player* P, Enemy* E) override {
		cout << endl << "A " << E->getname() << " has appeared." << endl << endl;

		while (health > 0 && E->getHealth() > 0) {
			cout << "Your Turn : " << endl;
			int choice;
			bool validChoice = false;

			while (!validChoice) {
				cout << "Enter your choice :" << endl;
				cout << "1.For Attacking the " << E->getname() << endl;
				cout << "2.For Healing." << endl;
				cout << "3.For Using special skill." << endl;
				cout << "4.For Using Defensive skill." << endl;
				cin >> choice;

				if (choice >= 1 && choice <= 4) {
					validChoice = true;
					switch (choice) {
					case 1:
						attackEnemy(E);
						cout << "You dealt " << attack << " damage!" << endl;
						break;
					case 2:
						HealPlayer();
						break;
					case 3:
						UseSpecialSkill(E);
						break;
					case 4:
						DefensiveSkil();
						break;
					}
				}
				else {
					cout << "Invalid input. Enter again." << endl;
				}
			}

			if (checkwin(E)) {
				break;
			}

			E->attackPlayer(P);
			cout << "Enemy dealt " << E->getattack() << " damage!" << endl;

			if (checklose(E)) {
				break;
			}

			cout << endl;
			P->displayStats();
			cout << endl;
		}

		if (E->getHealth() <= 0) {
			if (E->getname() == "Boss") {
				cout << endl << "*****************************************************" << endl;
				cout << "  Congratulations, " << P->getname() << "! You have defeated" << endl;
				cout << "  the Boss and completed Legends of Valor. YOU WIN!" << endl;
				cout << "*****************************************************" << endl;
				exit(0);
			}
			P->GainExperience(25);
			P->LevelUp();
			cout << endl << "Updated stats are:-" << endl;
			P->displayStats();
			cout << endl;
		}
		else {
			cout << endl << "GAME OVER. " << P->getname() << " has fallen in battle." << endl;
			exit(0);
		}
		return;
	}
	~Archer() {}
};

class KnightShop {
public:
	string Menu(int& mana) { 
		int choice;
		cout << endl << "+--------------   Knight Shop   ------------------+" << endl;
		cout << "|                                                       |" << endl;
		cout << "| 1. Dagger.                     ( 20 Mana Points )     |" << endl;
		cout << "| 2. CrossBow                    ( 10 Mana Points )     |" << endl;
		cout << "| 3. Healing Portion             ( 15 Mana Points )     |" << endl;
		cout << "| 4. Metal Suit                  ( 25 Mana Points )     |" << endl;
		cout << "|                                                       |" << endl;
		cout << "+-------------------------------------------------------+" << endl << endl;
		cout << "Enter your option:";
		cin >> choice;
		switch (choice) {
		case 1: {
			if (mana > 20) {
				mana -= 20;
				cout << endl << "Dagger is successfully purchased and added to inventory." << endl;
				return "Dagger";
			}
			else {
				cout << "Insufficient Mana." << endl;
			}
			break;
		}
		case 2: {
			if (mana > 10) {
				mana -= 10;
				cout << endl << "Cross Bow is successfully purchased and added to inventory." << endl;
				return "Cross Bow";
			}
			else {
				cout << "Insufficient Mana." << endl;
			}
			break;
		}
		case 3: {
			if (mana > 15) {
				mana -= 15;
				cout << endl << "Healing Portion is successfully purchased and added to inventory." << endl;
				return "Healing Portion";
			}
			else {
				cout << "Insufficient Mana." << endl;
			}
			break;
		}
		case 4: {
			if (mana > 25) {
				mana -= 25;
				cout << endl << "Metal Suit is successfully purchased and added to inventory." << endl;
				return "Metal Suit";
			}
			else {
				cout << "Insufficient Mana." << endl;
			}
			break;
		}
		default: {
			cout << "Invalid input." << endl;
		}
		}
		return "";
	}
	~KnightShop() {}
};

class Knight :public Player, public Inventory, public KnightShop {
public:
	Knight() :Player() {}
	Knight(string n, string namefile) :Player(n, namefile) {}
	void getPlayerInfo(string n, string namefile) {
		this->filename = namefile;
		this->name = n;
		this->health = 100;
		this->mana = 100;
		this->experience = 0;
		this->attack = 30;
		this->defense = 30;
		this->level = 0;
		this->x = Dimension - 1;
		this->y = 0;
	}
	void displayStats() override {
		cout << endl << "Stats of " << getname() << ":-" << endl;
		cout << "Level of " << getname() << " is: " << getLevel() << endl;
		cout << "Health of " << getname() << " is: " << getHealth() << endl;
		cout << "Attack of " << getname() << " is: " << getAttack() << endl;
		cout << "Defense of " << getname() << " is: " << getDefense() << endl;
		cout << "Experience of " << getname() << " is " << getExperience() << endl;
		cout << "Mana of " << getname() << " is: " << getMana() << endl;
	}
	void attackEnemy(Enemy* E) override {
		E->Health -= attack;
	}
	void ShowPlayerInventory() override {
		cout << "Inventory of " << getname() << ":-" << endl;
		ShowInventory();
	}
	void addPlayerObject(string n) {
		addItem(n);
	}
	void RemovePlayerObject(string n) {
		RemoveObject(n);
	}
	void HealPlayer() override {
		if (UseHealItem()) { 
			health += 30;
			if (health > 100) {
				health = 100;
			}
			cout << getname() << "'s health restored to " << health << "." << endl;
		}
	}
	void UseSpecialSkill(Enemy* E) override {
		if (mana > 50) {
			cout << "-------   Chidori (Lightning Punch)   -------" << endl;
			E->Health -= 50;
			mana -= 50;
		}
		else {
			cout << "Mana too low for using special ability." << endl;
		}
	}
	void DefensiveSkil() override {
		if (mana > 50) {
			cout << "Defensive skill activated." << endl;
			mana -= 50;
		}
		else {
			cout << "Not enough mana to activate Defensive Skill." << endl;
		}
	}
	void LevelUp() override {
		level++;
	}
	void GainExperience(int ex) override {
		experience += ex;
	}
	void UseShop() override {
		string choice = Menu(mana);
		if (!choice.empty()) {
			addItem(choice);
		}
	}
	bool checkwin(Enemy* E) {
		if (E->getHealth() <= 0) {
			cout << "You have defeated the " << E->getname() << endl;
			return true;
		}
		return false;
	}
	bool checklose(Enemy* E) {
		if (health <= 0) {
			cout << "You have been defeated by " << E->getname() << endl;
			return true;
		}
		return false;
	}
	void fightenemy(Player* P, Enemy* E) override {
		cout << endl << "A " << E->getname() << " has appeared." << endl << endl;

		while (health > 0 && E->getHealth() > 0) {
			cout << "Your turn : " << endl;
			int choice;
			bool validChoice = false;

			while (!validChoice) {
				cout << "Enter your choice :" << endl;
				cout << "1.For Attacking the " << E->getname() << endl;
				cout << "2.For Healing." << endl;
				cout << "3.For Using special skill." << endl;
				cout << "4.For Using Defensive skill." << endl;
				cin >> choice;

				if (choice >= 1 && choice <= 4) {
					validChoice = true;
					switch (choice) {
					case 1:
						attackEnemy(E);
						cout << "You dealt " << attack << " damage!" << endl;
						break;
					case 2:
						HealPlayer();
						break;
					case 3:
						UseSpecialSkill(E);
						break;
					case 4:
						DefensiveSkil();
						break;
					}
				}
				else {
					cout << "Invalid input. Enter again." << endl;
				}
			}

			if (checkwin(E)) {
				break;
			}

			E->attackPlayer(P);
			cout << "Enemy dealt " << E->getattack() << " damage!" << endl;

			if (checklose(E)) {
				break;
			}

			cout << endl;
			P->displayStats();
			cout << endl;
		}

		if (E->getHealth() <= 0) {
			if (E->getname() == "Boss") {
				cout << endl << "*****************************************************" << endl;
				cout << "  Congratulations, " << P->getname() << "! You have defeated" << endl;
				cout << "  the Boss and completed Legends of Valor. YOU WIN!" << endl;
				cout << "*****************************************************" << endl;
				exit(0);
			}
			P->GainExperience(25);
			P->LevelUp();
			cout << endl << "Updated stats are:-" << endl;
			P->displayStats();
			cout << endl;
		}
		else {
			cout << endl << "GAME OVER. " << P->getname() << " has fallen in battle." << endl;
			exit(0);
		}
		return;
	}
	~Knight() {}
};

class Hounds : public Enemy {
public:
	Hounds() : Enemy("", 0, 0) {}
	Hounds(string name, int Health, int attack) : Enemy(name, Health, attack) {}
	void attackPlayer(Player* P) {
		P->health -= attack;
	}
	string getname() const override {
		return name;
	}
	int getHealth() const override {
		return Health;
	}
	int getattack() const override {
		return attack;
	}
	virtual ~Hounds() {}
};

class Dragon : public Enemy {
public:
	Dragon() : Enemy("", 0, 0) {}
	Dragon(string name, int Health, int attack) : Enemy(name, Health, attack) {}
	void attackPlayer(Player* P) {
		P->health -= attack;
	}
	string getname() const override {
		return name;
	}
	int getHealth() const override {
		return Health;
	}
	int getattack() const override {
		return attack;
	}
	virtual ~Dragon() {}
};

class Witch : public Enemy {
public:
	Witch() : Enemy("", 0, 0) {}
	Witch(string name, int Health, int attack) : Enemy(name, Health, attack) {}
	void attackPlayer(Player* P) {
		P->health -= attack;
	}
	string getname() const override {
		return name;
	}
	int getHealth() const override {
		return Health;
	}
	int getattack() const override {
		return attack;
	}
	virtual ~Witch() {}
};

class Boss : public Enemy {
private:
	int Health2;
public:
	Boss() : Enemy("", 0, 0) {
		this->Health2 = 0;
	}
	Boss(string name, int Health, int attack) : Enemy(name, Health, attack) {
		this->Health2 = Health;
	}
	string getname() const override {
		return name;
	}
	int getHealth() const override {
		return Health;
	}
	int getattack() const override {
		return attack;
	}
	void attackPlayer(Player* P) {
		P->health -= attack;
	}
	virtual ~Boss() {}
};

class LegendsOfValor {
public:
	Enemy* E[4];
	int initialHealth[4];  // Store initial health values

	LegendsOfValor() {
		E[0] = new Hounds("Hound", 20, 5);
		E[1] = new Dragon("Dragon", 30, 10);
		E[2] = new Witch("Witch", 40, 15);
		E[3] = new Boss("Boss", 50, 30);

		// Store initial health for resetting
		initialHealth[0] = 20;
		initialHealth[1] = 30;
		initialHealth[2] = 40;
		initialHealth[3] = 50;
	}

	// Function to reset enemy health
	void resetEnemyHealth(int index) {
		if (index >= 0 && index < 4) {
			E[index]->Health = initialHealth[index];
		}
	}

	Player* P;

	Board b;
	void PlaceHounds() {
		b.PlaceHounds();
	}
	void PlaceBoss() {
		b.PlaceBoss();
	}
	void PlaceDragon() {
		b.PlaceDragon();
	}
	void PlaceWitch() {
		b.PlaceWitch();
	}
	void displayBoard() {
		b.displayBoard(P);
	}
	void showPlayer() {
		P->displayStats();
	}
	void ShowPlayerInventory() {
		P->ShowPlayerInventory();
	}
	void displayStats() {
		P->displayStats();
	}
	void UseShop() {
		P->UseShop();
	}
	void Menu() {

		int choice;
		do {
			cout << "******************   MENU   *********************" << endl;
			cout << "|                                               |" << endl;
			cout << "| Enter the action you want to perform.         |" << endl;
			cout << "|                                               |" << endl;
			cout << "| 1. Travel                                     |" << endl;
			cout << "| 2. Explore Map                                |" << endl;
			cout << "| 3. Check Inventory                            |" << endl;
			cout << "| 4. Check Stats                                |" << endl;
			cout << "| 5. Shop                                       |" << endl;
			cout << "| 6. Save/Quit                                  |" << endl; 
			cout << "|                                               |" << endl;
			cout << "*************************************************" << endl << endl;
			cout << "Enter your choice:";
			cin >> choice;
			switch (choice) {
			case 1: {
				int x, y;
				cout << "Enter your coordinates:";
				cin >> x;
				cin >> y;

				if (x < 1 || x > Dimension || y < 1 || y > Dimension) {
					cout << endl << "Invalid coordinates entered." << endl << endl;
					break;
				}

				bool enemyHere = false;
				int enemyIndex = -1;
				if (b.CheckHound(x - 1, y - 1)) {
					enemyHere = true;
					enemyIndex = 0;
				}
				else if (b.CheckDragon(x - 1, y - 1)) {
					enemyHere = true;
					enemyIndex = 1;
				}
				else if (b.CheckWitch(x - 1, y - 1)) {
					enemyHere = true;
					enemyIndex = 2;
				}
				else if (b.CheckBoss(x - 1, y - 1)) {
					enemyHere = true;
					enemyIndex = 3;
				}

				if (b.MovePlayer(*P, x - 1, y - 1)) {
					if (enemyHere) {
						resetEnemyHealth(enemyIndex);
						P->fightenemy(P, E[enemyIndex]);
					}
				}

				displayBoard();

				break;
			}
			case 2: {
				displayBoard();
				break;
			}
			case 3: {
				ShowPlayerInventory();
				break;
			}
			case 4: {
				displayStats();
				break;
			}
			case 5: {
				UseShop();
				break;
			}
			case 6: {
				P->quit();
				break;
			}
			default: {
				cout << "Invalid input." << endl;
			}
			}
		} while (true);
	}


	~LegendsOfValor() {
		for (int i = 0; i < 4; i++) {
			delete E[i];
		}
		delete P;
	}

};

int main() {
	LegendsOfValor LV;
	int choice1, choice;
	srand(static_cast<unsigned int>(time(nullptr)));

	cout << "*************       Welcome to Legends Of Valor      ************" << endl;
	cout << "|                                                               |" << endl;
	cout << "| 1.New Game                                                    |" << endl;
	cout << "| 2.load previous Game                                          |" << endl;
	cout << "| 3.Quit Game                                                   |" << endl;
	cout << "|                                                               |" << endl;
	cout << "*****************************************************************" << endl << endl;
	cin >> choice1;
	if (choice1 == 1) {
		do {
			cout << "*****************************************************************" << endl;
			cout << "|                                                               |" << endl;
			cout << "| Select your Custom Character.                                 |" << endl;
			cout << "|                                                               |" << endl;
			cout << "| 1. Warrior                                                    |" << endl;
			cout << "| 2. Mage                                                       |" << endl;
			cout << "| 3. Archer                                                     |" << endl;
			cout << "| 4. Knight                                                     |" << endl;
			cout << "|                                                               |" << endl;
			cout << "*****************************************************************" << endl << endl;
			cout << "Enter your choice:";
			cin >> choice;
			switch (choice) {
			case 1: {
				LV.P = new Warrior("Warrior", "File.txt");
				break;
			}
			case 2: {
				LV.P = new Mage("Mage", "File.txt");
				break;
			}
			case 3: {
				LV.P = new Archer("Archer", "File.txt");
				break;
			}
			case 4: {
				LV.P = new Knight("Knight", "File.txt");
				break;
			}
			default: {
				cout << "Invalid Input." << endl << endl;;
			}
			}
		} while (choice < 1 || choice>4);
	}
	else if (choice1 == 2) {
		string str1 = loadgame("File.txt");
		if (str1 == "Archer") {
			LV.P = new Archer("Archer", "File.txt");
			LV.P->loadthegame();
		}
		else if (str1 == "Knight") {
			LV.P = new Knight("Knight", "File.txt");
			LV.P->loadthegame();
		}
		else if (str1 == "Mage") {
			LV.P = new Mage("Mage", "File.txt");
			LV.P->loadthegame();
		}
		else if (str1 == "Warrior") {
			LV.P = new Warrior("Warrior", "File.txt");
			LV.P->loadthegame();
		}
		else { 
			cout << "No valid save file found. Exiting." << endl;
			exit(0);
		}
	}
	else {
		cout << "Exiting the game." << endl;
		exit(0);
	}
	LV.showPlayer();
	LV.PlaceHounds();
	LV.PlaceBoss();
	LV.PlaceDragon();
	LV.PlaceWitch();
	LV.displayBoard();
	LV.Menu();

	system("pause");
	return 0;
}