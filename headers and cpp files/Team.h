#pragma once
#include "Player.h"
#include <fstream>

const int maxSizeTeam = 20;

class Team
{
public:

	Team();
	Team(const char* name,
		const char* coach,
		const char* stadium,
		double budget,
		int matchesForSeason,
		int wins,
		int draws,
		int loses,
		int points,
		int allGoals,
		int allConcededGoals);
	Team(const Team& other);
	Team& operator=(const Team& other);
	virtual ~Team();

	const char* getNameTeam() const;
	const char* getNameCoach() const;
	const char* getNameStadium() const;
	double getBudget() const;
	int getMatchesForSeason() const;
	int getWins() const;
	int getDraws() const;
	int getLoses() const;
	int getPoints() const;
	int getAllGoals() const;
	int getAllConcededGoals() const;


	void setNameTeam(const char* name);
	void setNameCoach(const char* coach);
	void setNameStadium(const char* stadium);
	void setBudget(double budget);
	void setMatchesForSeason(int matchesForSeason);
	void setWins(int wins);
	void setDraws(int draws);
	void setLoses(int loses);
	void setPoints(int points);
	void setAllGoals(int allGoals);
	void setAllConcededGoals(int allConcededGoals);

	void addPlayer(const Player& player);
	void removePlayer(const char* namePlayer);

	virtual bool canBuyPlayer(const Player& player) const = 0;
	virtual bool isValidTeam() const = 0;
	virtual Team* clone() const = 0;

	void updateStatistics(int goalsScored, int goalsConceded);
	void startNewSeason();
	virtual void printTeamInfo() const;

	Player* getPlayerByName(const char* namePlayer);

	int getCurrentSize() const;
	const Player& getPlayer(int index) const;

	bool canSwapPlayersWith(const Team& other, const char* playerFromThisTeam, const char* playerFromOtherTeam) const;
	void swapPlayersWith(Team& other, const char* playerFromThisTeam, const char* playerFromOtherTeam);

	virtual const char* getType() const = 0;

	void regulatePlayersSalaries();

	void saveToFile(std::ofstream& out) const;
	void loadFromFile(std::ifstream& in);
protected:
	char* nameTeam;
	char* nameCoach;
	char* nameStadium;
	Player players[maxSizeTeam];
	double budget;

	int matchesForSeason;
	int wins;
	int draws;
	int loses;
	int points;

	int allGoals;
	int allConcededGoals;

	int currentSize;

	void copyFromTeam(const Team& other);
	void freeTeam();

	int countByPosition(Position position) const;
	bool hasPlayerWithName(const char* namePlayer) const;
	bool hasPlayerWithNumber(int number) const;

	int findPlayerIndexByName(const char* namePlayer) const;
	bool hasPlayerWithNameExceptIndex(const char* namePlayer, int skippedIndex) const;
	bool hasPlayerWithNumberExceptIndex(int number, int skippedIndex) const;
};