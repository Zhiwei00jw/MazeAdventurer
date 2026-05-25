#include "../include/storage.h"
#include "../include/json.hpp"
#include <fstream>
#include <iostream>
using json = nlohmann::json;

//=======Adventurer 序列化===========
static json adventurerToJson(const Adventurer& a){
    return{
        {"id",      a.id},
        {"name",    a.name},
        {"gender",  (int)a.gender},
        {"race",    (int)a.race},
        {"profession",(int)a.profession},
        {"teamName", a.teamName}
    };
}
static Adventurer jsonToAdventurer(const json& j){
    Adventurer a;
    a.id=j["id"];
    a.name=j["name"];
    a.gender=(Gender)(int)j["gender"];
    a.race=(Race)(int)j["race"];
    a.profession=(Profession)(int)j["profession"];
    a.teamName=j["teamName"];
    return a;
}
//========Team 序列化=========//
static json teamToJson(const Team& t){
    return{
        {"name",      t.name},
        {"memberIds",    t.memberIds},
        {"lastActiveTime",  (long long)t.lastActiveTime},
        {"rating",    t.rating}
    };
}
static Team jsonToTeam(const json& j){
    Team a;
    a.name=j["name"];
    a.memberIds=j["memberIds"].get<std::vector<int>>();
    a.lastActiveTime=(std::time_t)(long long)j["lastActiveTime"];
    a.rating=j["rating"];
    return a;
}
// ===== AdventureLog 序列化 =====
static json casualtyToJson(const CasualtyInfo& c) {
    return {
        {"adventurerID", c.adventurerID},
        {"rescued",      c.rescued},
        {"location",     c.location}
    };
}

static CasualtyInfo jsonToCasualty(const json& j) {
    CasualtyInfo c;
    c.adventurerID = j["adventurerID"];
    c.rescued      = j["rescued"];
    c.location     = j["location"];
    return c;
}
static json logToJson(const AdventureLog& l){
    json casualties=json::array();
    for(const auto& c:l.casualties){
        casualties.push_back(casualtyToJson(c));
    }
    return{
        {"id",             l.id},
        {"time",           (long long)l.time},
        {"teamName",       l.teamName},
        {"participantIDs", l.participantIDds},
        {"casualties",     casualties},
        {"notes",          l.notes} 
    };

}
static AdventureLog jsonToLog(const json& j){
    AdventureLog l;
    l.id=j["id"];
    l.time=(std::time_t)(long long)j["time"];
    l.notes=j["notes"];
    l.teamName=j["teamName"];
    l.participantIDds=j["participantIDs"].get<std::vector<int>>();
    for(const auto& c:j["casualties"]){
        l.casualties.push_back(jsonToCasualty(c));
    }
    return l;
}
//save and load
//save all data to file
bool saveData(const std::string& filename,
            const std::vector<Adventurer>& adventurers,
            const std::vector<Team>& teams,
            const std::vector<AdventureLog>& logs){
    json root;
    root["adventurers"]=json::array();
    for(const auto& a:adventurers)
        root["adventurers"].push_back(adventurerToJson(a));
    root["teams"]=json::array();
    for(const auto& t:teams){
        root["teams"].push_back(teamToJson(t));
    }
    root["logs"] = json::array();
    for (const auto& l : logs)
        root["logs"].push_back(logToJson(l));
    std::ofstream file(filename);
    if(!file.is_open()){
        std::cout<<"Can not open file! file name: "<<filename<<std::endl;
        return false;
    }
    file<<root.dump(4);//表示用四个空格缩进，输出格式化的JSON
    std::cout<<"Data have been saved to "<<filename<<std::endl;
    return true;
}
//load data from file
bool loadData(const std::string& filename,
             std::vector<Adventurer>& adventurers,
             std::vector<Team>& teams,
             std::vector<AdventureLog>& logs){
    std::ifstream file(filename);
    if(!file.is_open()){
        //第一次运行文件不存在
        std::cout<<"Can not load the data,start from empty data"<<std::endl;
        return false;
    }
    //try-catch 如果JSON文件格式损坏，捕获异常而不是直接崩溃
    try{
        json root=json::parse(file);
        for(const auto& j:root["adventurers"]){
            adventurers.push_back(jsonToAdventurer(j));
        }
        for(const auto& j:root["teams"]){
            teams.push_back(jsonToTeam(j));
        }
        for(const auto & j:root["logs"]){
            logs.push_back(jsonToLog(j));
        }
    }catch(const json::exception& e){
        std::cout<<"the document is wrong."<<e.what()<<std::endl;
        return false;
    }
    std::cout<<"loading data successfully: "<<adventurers.size()<< "adventurers"
                                            <<teams.size()<<"teams"
                                            <<logs.size()<<"logs"<<std::endl;
    return true;


}