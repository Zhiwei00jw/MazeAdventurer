#pragma once

#include "../include/types.h"
#include <vector>
#include <string>

void printRankStar(int rank);
void teamProStatics(int& warrior,int& mage,int& cleric,int& rogue,int& ranger,std::vector<Adventurer>& list,std::vector<int> teamIds);
//print single team with all members
void printTeam(const Team& team,std::vector<Adventurer>& list);
//add team
bool addTeam(std::vector<Team>& team,const std::string& name);
//remove team
bool removeTeam(std::vector<Team>& team,std::vector<Adventurer>& list,const std::string& name);
//add team member
bool addTeamMember(std::vector<Team>& team,std::vector<Adventurer>& list,const std::string& name,int id);
//remove team member
bool removeTeamMember(std::vector<Team>& team,std::vector<Adventurer>& list,const std::string& name,int id);
//team ranking
bool teamRanking(std::vector<Team>& team,std::vector<Adventurer>& list);
//print all team
void printAllTeams(std::vector<Team>& teams,std::vector<Adventurer>& list);//find team by team name
Team* findTeam(std::vector<Team>& team,std::string& name);
//计算自从队伍上次活跃时间以来的时间间隔
static std::string timeSince(std::time_t t);
