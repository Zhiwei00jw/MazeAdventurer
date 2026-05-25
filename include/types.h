#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <ctime>

//1.构建数据结构
enum class Gender {Male,Female,Other};
enum class Race {
    Human, //人类
    Elf,   //精灵
    Dwarf,  //矮人
    Orc,    //兽人
    Other   //其他
};
enum class Profession {
    Warrior, //战士
    Mage,    //法师
    Rogue,   //盗贼
    Cleric,  //牧师
    Ranger    //游侠
};

struct Adventurer {
    int id;      //冒险者ID
    std::string name;
    Gender gender;
    Race race;
    Profession profession;
    std::string teamName; //所属队伍
};
struct Team {
    std::string name; //队伍名称
    std::vector<int> memberIds; //队伍成员的ID
    std::time_t lastActiveTime; //最近一次冒险时间
    int rating;//队伍综合评级
};
struct CasualtyInfo{
    int adventurerID;//ID
    bool rescued;//是否带出迷宫
    std::string name;
    std::string location;//若未带出，遗留位置描述
};
struct AdventureLog{
    int id;
    std::time_t time;//冒险时间
    std::string teamName;
    std::vector<int> participantIDds;
    std::vector<CasualtyInfo> casualties;
    std::string notes;//备注
};