#pragma once

#include "types.h"
#include "json.hpp"
#include <vector>
#include <string>

class AdventurerSystem{
public:
    //constructor 构造函数,传入存档文件名，自动加载数据
    explicit AdventurerSystem(const std::string& filename);

    //析构函数
    ~AdventurerSystem();

    //冒险者管理函数
    void addAdventurer(const std::string& name,Gender g,Race r,Profession p);
    void removeAdventurer(int id);
    void printAllAdventurers() const;//跟在函数后面的这个const表示此函数不会修改成员数据
    void printAdventurersByRace(Race r) const;
    void printAdventurersByProfession(Profession p) const;

    //队伍管理函数
    void addTeam(const std::string& name);
    void removeTeam(const std::string& name);
    void addMemberToTeam(const std::string& teamName,int adventurerId);
    void removeMemberFromTeam(const std::string& teamName,int adventurerId);
    void setTeamRating(const std::string& teamName );
    void printAllTeams() const;

    //冒险记录管理
    int logAdventure(const std::string& teamName,const std::string& notes);
    void addCasualty(int logId,int adventurerId,bool rescued,const std::string& location);
    void printAllLogs() const;
    void printLogsByTeam(const std::string& teamName) const;

private:
    std::vector<Adventurer>     lists;
    std::vector<Team>           teams;
    std::vector<AdventureLog>   logs;
    std::string                 dataFile;

    //内部查找辅助函数
    Adventurer*     findAdventurerById(int id);
    Team*           findTeamByName(const std::string& name);
    AdventureLog*   findLogById(int id);

    //打印辅助函数
    void printAdventurer(const Adventurer& a) const;
    void printTeam(const Team& t) const;
    void printLog(const AdventureLog& l) const;

    //其他辅助函数
    void teamProStatics(Team& team);//队伍职业统计函数
 
    //存档函数
    void loadData();
    void saveData();
    
}