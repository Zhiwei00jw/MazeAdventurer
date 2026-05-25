#pragma once

#include "types.h"
#include <vector>
#include <string>

//save all data to file
bool saveData(const std::string& filename,
            const std::vector<Adventurer>& adventurers,
            const std::vector<Team>& teams,
            const std::vector<AdventureLog>& logs);
//load data from file
bool loadData(const std::string& filename,
             std::vector<Adventurer>& adventurers,
             std::vector<Team>& teams,
             std::vector<AdventureLog>& logs);
