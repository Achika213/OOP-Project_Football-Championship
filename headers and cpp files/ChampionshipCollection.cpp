#include "ChampionshipCollection.h"
#include <iostream>
#include <stdexcept>
#include <fstream>

ChampionshipCollection::ChampionshipCollection()
{
    capacity = 4;
    count = 0;
    championships = new Championship[capacity];
}

ChampionshipCollection::ChampionshipCollection(const ChampionshipCollection& other)
{
    copyFrom(other);
}

ChampionshipCollection& ChampionshipCollection::operator=(const ChampionshipCollection& other)
{
    if (this != &other)
    {
        free();
        copyFrom(other);
    }

    return *this;
}

ChampionshipCollection::~ChampionshipCollection()
{
    free();
}

void ChampionshipCollection::free()
{
    delete[] championships;
    championships = nullptr;
    count = 0;
    capacity = 0;
}

void ChampionshipCollection::copyFrom(const ChampionshipCollection& other)
{
    count = other.count;
    capacity = other.capacity;
    championships = new Championship[capacity];

    for (int i = 0; i < count; i++)
    {
        championships[i] = other.championships[i];
    }
}

void ChampionshipCollection::resize()
{
    capacity *= 2;

    Championship* newChampionships = new Championship[capacity];

    for (int i = 0; i < count; i++)
    {
        newChampionships[i] = championships[i];
    }

    delete[] championships;
    championships = newChampionships;
}

int ChampionshipCollection::findChampionshipIndexByYear(int year) const
{
    for (int i = 0; i < count; i++)
    {
        if (championships[i].getYear() == year)
            return i;
    }

    return -1;
}

bool ChampionshipCollection::hasChampionshipWithYear(int year) const
{
    return findChampionshipIndexByYear(year) != -1;
}

void ChampionshipCollection::addChampionship(const Championship& championship)
{
    if (hasChampionshipWithYear(championship.getYear()))
        throw std::logic_error("Championship with this year already exists");

    if (count >= capacity)
        resize();

    championships[count] = championship;
    count++;
}

int ChampionshipCollection::getNextYear() const
{
    if (count == 0)
        return 2026;

    int maxYear = championships[0].getYear();

    for (int i = 1; i < count; i++)
    {
        if (championships[i].getYear() > maxYear)
            maxYear = championships[i].getYear();
    }

    return maxYear + 1;
}

Championship& ChampionshipCollection::startNewSeason()
{
    int newYear = getNextYear();

    Championship newChampionship(newYear);
    addChampionship(newChampionship);

    return championships[count - 1];
}

Championship* ChampionshipCollection::getChampionshipByYear(int year)
{
    int index = findChampionshipIndexByYear(year);

    if (index == -1)
        return nullptr;

    return &championships[index];
}

const Championship* ChampionshipCollection::getChampionshipByYear(int year) const
{
    int index = findChampionshipIndexByYear(year);

    if (index == -1)
        return nullptr;

    return &championships[index];
}

void ChampionshipCollection::printAllChampionships() const
{
    if (count == 0)
    {
        std::cout << "No championships in the collection.\n";
        return;
    }

    std::cout << "Championships:\n";

    for (int i = 0; i < count; i++)
    {
        std::cout << i + 1 << ". Year: " << championships[i].getYear();

        if (championships[i].isFinished())
            std::cout << " | Finished";
        else
            std::cout << " | Not finished";

        std::cout << '\n';
    }
}

int ChampionshipCollection::getCount() const
{
    return count;
}

void ChampionshipCollection::saveToFile(const char* fileName) const
{
    if (!fileName)
        throw std::invalid_argument("Invalid file name");

    std::ofstream out(fileName);

    if (!out)
        throw std::logic_error("Cannot open file for writing");

    out << count << '\n';

    for (int i = 0; i < count; i++)
    {
        championships[i].saveToFile(out);
    }

    out.close();
}

void ChampionshipCollection::loadFromFile(const char* fileName)
{
    if (!fileName)
        throw std::invalid_argument("Invalid file name");

    std::ifstream in(fileName);

    if (!in)
        throw std::logic_error("Cannot open file for reading");

    free();

    in >> count;
    in.ignore();

    capacity = count;

    if (capacity < 4)
        capacity = 4;

    championships = new Championship[capacity];

    for (int i = 0; i < count; i++)
    {
        championships[i].loadFromFile(in);
    }

    in.close();
}