#include <iostream>
#include "../include/types.h"
#include "../include/adventurer_manager.h"
#include "../include/team_manager.h"
#include "../include/adventure_manager.h"
#include "../include/storage.h"
#include "../include/menu.h"

const std::string DATA_FILE="../data/data.json";

int main(){
    std::cout<<"starting successfully!"<<std::endl;
    std::vector<Adventurer> adventurers;
    std::vector<Team> teams;
    std::vector<AdventureLog> logs;
    
    loadData(DATA_FILE,adventurers,teams,logs);
    runMainMenu(adventurers,teams,logs);
    saveData(DATA_FILE,adventurers,teams,logs);
    std::cout<<"GOOD BYE!"<<std::endl;
    return 0;
}