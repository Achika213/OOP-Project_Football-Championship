#pragma once
#include "Team.h"
#include "Match.h"
#include <fstream>

class Championship
{
public:
	Championship();
	Championship(int year);
	Championship(const Championship& other);
	Championship& operator=(const Championship& other);
	~Championship();
	
	void addTeam(const Team& team);
	bool isValidTeamCount() const;

	void generateSchedule();

	void playRound();
	void playAllRounds();

	void printStandings() const;

	const Team* getChampion() const;
	const Team* getRunnerUp() const;
	const Team* getThirdPlace() const;

	void printTopScorers() const;

	void save(const char* fileName) const;
	void load(const char* fileName);

	int getYear() const;
	bool isFinished() const;

	Team* getTeamByName(const char* teamName);
	const Team* getTeamByName(const char* teamName) const;
	void printTeams() const;
	void printFinalResults() const;

	void saveToFile(std::ofstream& out) const;
	void loadFromFile(std::ifstream& in);
private:
	int year;

	Team** teams;
	int teamCount;
	int capacity;

	Match* matches;
	int matchesCount;
	
	int currentRound;
	bool finished;

	void copyFromChampionship(const Championship& other);
	void freeChampionship();
};