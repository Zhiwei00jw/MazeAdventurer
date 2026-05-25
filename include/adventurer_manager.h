#pragma once

#include "types.h"
#include <vector>
#include <string>

static std::string genderToStr(Gender g);
static std::string raceToStr(Race g);
static std::string professionToStr(Profession g);
//print one adventurer's info
void printAdventurer(const Adventurer& a);
//add adventurer
void addAdventurer(std::vector<Adventurer>& list,const std::string& name,Gender gender,Race race,Profession Profession);
//remove adventurer
bool removeAdventurer(std::vector<Team>& teams,std::vector<Adventurer>& list,int id);
//find adventurer
Adventurer* findAdventurerById(std::vector<Adventurer>& list,int id);
//print all adventurers
void printAllAdventurers(const std::vector<Adventurer>& list);
//filter by race and print
void filterByRace(const std::vector<Adventurer>& list,Race race);
//filter by profession and print
void filterByProfession(const std::vector<Adventurer>& list,Profession profession);
//filter by gender and print
void filterByGender(const std::vector<Adventurer>& list,Gender gender);