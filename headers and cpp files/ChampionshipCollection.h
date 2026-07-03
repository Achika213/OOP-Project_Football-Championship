#pragma once
#include "Championship.h"
#include <fstream>

class ChampionshipCollection
{
public:
    ChampionshipCollection();
    ChampionshipCollection(const ChampionshipCollection& other);
    ChampionshipCollection& operator=(const ChampionshipCollection& other);
    ~ChampionshipCollection();


    void addChampionship(const Championship& championship);
    Championship& startNewSeason();
    Championship* getChampionshipByYear(int year);
    const Championship* getChampionshipByYear(int year) const;
    bool hasChampionshipWithYear(int year) const;
    int getNextYear() const;
    void printAllChampionships() const;
    int getCount() const;

    void saveToFile(const char* fileName) const;
    void loadFromFile(const char* fileName);
private:
    Championship* championships;
    int count;
    int capacity;

    void free();
    void copyFrom(const ChampionshipCollection& other);
    void resize();

    int findChampionshipIndexByYear(int year) const;
};

