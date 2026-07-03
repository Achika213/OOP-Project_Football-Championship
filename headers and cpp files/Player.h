#pragma once
#include <fstream>

enum class Position
{
	UNKNOWN,
	GOALKEEPER,
	DEFENDER,
	MIDFIELDER,
	WINGER,
	FORWARD
};

class Player
{
public:
	Player();
	Player(const char* namePlayer,
		int number,
		Position position,
		double salary,
		double transferSum,
		int matches,
		int goals);
	Player(const Player& other);
	Player& operator=(const Player& other);
	~Player();

	const char* getName() const;
	int getNumber() const;
	Position getPosition() const;
	double getSalary() const;
	double getTransferSum() const;
	int getMatches() const;
	int getGoals() const;

	void setName(const char* namePlayer);
	void setNumber(int number);
	void setPosition(Position position);
	void setSalary(double salary);
	void setTransferSum(double transferSum);
	void setMatches(int matches);
	void setGoals(int goals);

	void print() const;
	void salaryRegulation(int teamMatches);

	void saveToFile(std::ofstream& out) const;
	void loadFromFile(std::ifstream& in);
protected:
	char* namePlayer;
	int number;
	Position position;
	double salary;
	double transferSum;
	int matches;
	int goals;

	void copyFromPlayer(const Player& other);
	void freePlayer();
};

