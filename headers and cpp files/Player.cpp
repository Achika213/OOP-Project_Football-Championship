#define _CRT_SECURE_NO_WARNINGS
#include "Player.h"
#include <iostream>
#include <cstring>	
#include <stdexcept>
#include <fstream>

int positionToInt(Position position)
{
	switch (position)
	{
	case Position::GOALKEEPER:
		return 1;
	case Position::DEFENDER:
		return 2;
	case Position::MIDFIELDER:
		return 3;
	case Position::WINGER:
		return 4;
	case Position::FORWARD:
		return 5;
	default:
		return 0;
	}
}

Position intToPosition(int value)
{
	switch (value)
	{
	case 1:
		return Position::GOALKEEPER;
	case 2:
		return Position::DEFENDER;
	case 3:
		return Position::MIDFIELDER;
	case 4:
		return Position::WINGER;
	case 5:
		return Position::FORWARD;
	default:
		throw std::invalid_argument("Invalid position value in file");
	}
}

void Player::saveToFile(std::ofstream& out) const
{
	if (!out)
		throw std::logic_error("Invalid output file");

	out << namePlayer << '\n';
	out << number << '\n';
	out << positionToInt(position) << '\n';
	out << salary << '\n';
	out << transferSum << '\n';
	out << matches << '\n';
	out << goals << '\n';
}

void Player::loadFromFile(std::ifstream& in)
{
	if (!in)
		throw std::logic_error("Invalid input file");

	char buffer[128];
	int positionValue;

	in.getline(buffer, 128);
	setName(buffer);

	in >> number;
	in >> positionValue;
	position = intToPosition(positionValue);

	in >> salary;
	in >> transferSum;
	in >> matches;
	in >> goals;

	in.ignore();
}

Player::Player()
{
	namePlayer = new char[1];
	namePlayer[0] = '\0';

	number = 0;
	position = Position::UNKNOWN;
	salary = 0.0;
	transferSum = 0.0;
	matches = 0;
	goals = 0;
}

Player::Player(const char* namePlayer,
	int number,
	Position position,
	double salary,
	double transferSum,
	int matches,
	int goals)
{
	this->namePlayer = nullptr;

	try
	{
		setName(namePlayer);
		setNumber(number);
		setPosition(position);
		setSalary(salary);
		setTransferSum(transferSum);
		setMatches(matches);
		setGoals(goals);
	}
	catch (...)
	{
		freePlayer();
		throw;
	}
}

Player::Player(const Player& other)
{
	namePlayer = nullptr;
	copyFromPlayer(other);
}

Player& Player::operator=(const Player& other)
{
	if (this != &other)
	{
		copyFromPlayer(other);
	}
	return *this;
}

Player::~Player()
{
	freePlayer();
}

const char* Player::getName() const
{
	return namePlayer;
}

int Player::getNumber() const
{
	return number;
}

Position Player::getPosition() const
{
	return position;
}

double Player::getSalary() const
{
	return salary;
}

double Player::getTransferSum() const
{
	return transferSum;
}

int Player::getMatches() const
{
	return matches;
}

int Player::getGoals() const
{
	return goals;
}

void Player::setName(const char* namePlayer)
{
	if (!namePlayer)
		throw std::invalid_argument("Invalid name");

	char* newName = new char[strlen(namePlayer) + 1];
	strcpy(newName, namePlayer);

	delete[] this->namePlayer;
	this->namePlayer = newName;
}

void Player::setNumber(int number)
{
	if (number <= 0)
		throw std::invalid_argument("Invalid number");
	this->number = number;
}

void Player::setPosition(Position position)
{
	if (position == Position::UNKNOWN)
		throw std::invalid_argument("Invalid position");

	this->position = position;
}

void Player::setSalary(double salary)
{
	if (salary < 0)
		throw std::invalid_argument("Invalid salary");
	this->salary = salary;
}

void Player::setTransferSum(double transferSum)
{
	if (transferSum < 0)
		throw std::invalid_argument("Invalid transfer sum");
	this->transferSum = transferSum;
}

void Player::setMatches(int matches)
{
	if (matches < 0)
		throw std::invalid_argument("Invalid matches");
	this->matches = matches;
}

void Player::setGoals(int goals)
{
	if (goals < 0)
		throw std::invalid_argument("Invalid goals");
	this->goals = goals;
}

void Player::print() const
{
	std::cout << "Player info: " << std::endl;
	std::cout << "Name: " << namePlayer << std::endl;
	std::cout << "Number: " << number << std::endl;

	std::cout << "Position: ";
	switch (position)
	{
	case Position::FORWARD:
		std::cout << "Forward" << std::endl;
		break;
	case Position::WINGER:
		std::cout << "Winger" << std::endl;
		break;
	case Position::MIDFIELDER:
		std::cout << "Midfielder" << std::endl;
		break;
	case Position::DEFENDER:
		std::cout << "Defender" << std::endl;
		break;
	case Position::GOALKEEPER:
		std::cout << "Goalkeeper" << std::endl;
		break;
	default:
		std::cout << "Unknown" << std::endl;
		break;
	}

	std::cout << "Salary: " << salary << std::endl;
	std::cout << "Transfer Sum: " << transferSum << std::endl;
	std::cout << "Matches: " << matches << std::endl;
	std::cout << "Goals: " << goals << std::endl;
}


void Player::salaryRegulation(int teamMatches)
{
	if (teamMatches < 0)
		throw std::invalid_argument("Invalid team matches");

	if (matches > teamMatches / 2.0)
		salary *= 1.20;

	if (matches < 7)
		salary *= 0.90;

	if (goals > 5)
		salary *= 1.10;

	if (goals > 10)
		salary *= 1.20;

	if (goals < 5 && position == Position::FORWARD)
		salary *= 0.85;
}

void Player::copyFromPlayer(const Player& other)
{
	char* newName = new char[strlen(other.namePlayer) + 1];
	strcpy(newName, other.namePlayer);

	delete[] this->namePlayer;
	this->namePlayer = newName;

	this->number = other.number;
	this->position = other.position;
	this->salary = other.salary;
	this->transferSum = other.transferSum;
	this->matches = other.matches;
	this->goals = other.goals;
}

void Player::freePlayer()
{
	delete[] namePlayer;
	namePlayer = nullptr;
}
