#pragma once
#include "types.h"
#include <vector>
#include <string>
#include "../include/adventurer_manager.h"
#include "../include/team_manager.h"

//add adventure log
void addAdventureLog(std::vector<AdventureLog>& logs,
    std::vector<Team>& teams,
    std::vector<Adventurer>& list,
    const std::string& teamName,
    const std::string& notes );
void addCasualties(std::vector<AdventureLog>& logs,
                   int logID,
                   int adventurerID,
                   bool rescued,
                   const std::string location,
                   std::vector<Adventurer>& list
                   );
//打印所有冒险记录
void printAllLogs(std::vector<AdventureLog>& logs,std::vector<Adventurer>& adventurers);
//打印某只队伍的所有记录
void printLogsByTeam(const std::vector<AdventureLog>& logs,
                     std::vector<Adventurer>& adventurers,
                     const std::string& teamName);
//按照logID查找对应的记录
AdventureLog* findLogById(std::vector<AdventureLog>& logs,int logid);
