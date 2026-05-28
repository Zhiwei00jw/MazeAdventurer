#include "../include/adventurer_system.h"
#include "../include/json.hpp"
#include <iostream>
#include <fstream>
#include <ctime>
//====== 构造与析构函数===========//
AdventurerSystem::AdventurerSystem(const std::string& filename): dataFile(filename){
    loadData();
}
AdventurerSystem::~AdventurerSystem(){
    saveData();
}

//============内部辅助函数==============//
Adventurer*     AdventurerSystem::findAdventurerById(int id){
    for(auto&a : lists)
        if(a.id == id) return &a;
    return nullptr;
}
Team*   AdventurerSystem::findTeamByName(const std::string& name){
    for(auto& t : teams)
        if(t.name == name) return &t;
    return nullptr;
}
AdventureLog*   AdventurerSystem::findLogById(int id){
    for(auto& l :logs)
        if(l.id == id) return &l;
    return nullptr;
}
//========打印辅助函数===========//
static std::string genderToStr(Gender g){
    switch(g){
        case Gender::Male: return "Male";
        case Gender::Female: return "Female";
        default: return "Other";
    }
}
static std::string raceToStr(Race g){
    switch(g){
        case Race::Human: return "Human";
        case Race::Elf: return "Elf";
        case Race::Dwarf: return "Dwarf";
        case Race::Orc : return "Orc";
        default: return "Other";
    }
}
static std::string professionToStr(Profession g){
    switch(g){
        case Profession::Warrior: return "Warrior";
        case Profession::Mage: return "Mage";
        case Profession::Rogue: return "Rogue";
        case Profession::Ranger : return "Ranger";
        case Profession::Cleric : return "Cleric";
        default: return "Other";
    }
}
void AdventurerSystem::printAdventurer(const Adventurer& a) const{
    std::cout<<"===================="<<"\n";
    std::cout<<"name: "<<a.name<<std::endl;
    std::cout<<"gender: "<<genderToStr(a.gender)<<std::endl;
    std::cout<<"race: "<<raceToStr(a.race)<<std::endl;
    std::cout<<"profession: "<<professionToStr(a.profession)<<std::endl;
    std::cout<<"ID: "<<a.id<<std::endl;
    std::cout<<"TeamName: "<<a.teamName<<std::endl;
    std::cout<<"===================="<<std::endl;
}
//print single team with all members
void AdventurerSystem::printTeam(const Team& team)const{
    int member=1;
    //print team name
    std::cout<<"Team Name: "<<team.name<<std::endl;
    std::cout<<"Team ranking: "<<team.rating<<std::endl;
    std::cout<<"Last active time: "<<timeSince(team.lastActiveTime)<<std::endl;
    //print all members
    for(const auto& id:team.memberIds){
        std::cout<<"====member"<<member<<"===="<<std::endl;
        Adventurer* adventurer=findAdventurerById(id);
        if(adventurer){
            printAdventurer(*adventurer);
            member++;
        }else{
            std::cout<<"The team "<<team.name<<" is empty!"<<std::endl;
        }
       
    }
}
//=========信息展示辅助函数==========//
//将队伍评级转换成⭐表示的辅助函数
void printRankStar(int rank){
    switch (rank)
    {
    case 1:
        std::cout<<"team ranking: ⭐  Not bad! "<<std::endl;
        break;
    case 2:
        std::cout<<"team ranking: ⭐ ⭐ Very good!"<<std::endl;
        break;
    case 3:
        std::cout<<"team ranking: ⭐ ⭐ ⭐ Excellent!"<<std::endl;
        break;
    default:
        std::cout<<"This team is definitely empty!"<<std::endl;//空的队伍应该不予评级
        break;
    }
}
//统计队伍中各种职业数的函数
void AdventurerSystem::teamProStatics(Team& team){
    for(auto& id:team.memberIds){
        Adventurer* adventurer= findAdventurerById(id);
        if(adventurer){
            //此ID不为空，指针不为空指针
            switch (adventurer->profession)
            {
            case Profession::Warrior:
                team.profession_static[0]++;
                break;
            case Profession::Mage:
                team.profession_static[1]++;
                break;
            case Profession::Ranger:
                team.profession_static[2]++;
                break;
            case Profession::Rogue:
                team.profession_static[3]++;
                break;
            case Profession::Cleric:
                team.profession_static[4]++;
                break;
            default:
                break;
            }
        }else{
            //此ID为空，进行报错
            std::cout<<"ERROR:empty ID !Have you checked it before?"<<std::endl;
        }
    }
}
//时间管理函数,把时间戳转换成几分钟前，几小时前，几天前这种易读形式
static std::string timeSince(std::time_t t){
    if(t==0){
        return "never go to the maze.";
    }
    double seconds = std::difftime(std::time(nullptr),t);//现在时间距离上次出队的时间差
    if(seconds<3600){
        //小于一个小时
        int minitue=int(seconds/60);
        return "Last active time is " + std::to_string(minitue) + "minitues ago.";
    }else if(seconds<86400){
        //小于一天
        int hour=int(seconds/3600);
        return "Last active time is " + std::to_string(hour) + "hours ago.";
    }else{
        int day=int(seconds/86400);
        return "Last active time is " + std::to_string(day) + "days ago.";
    }

}
//========冒险者管理=============//
void AdventurerSystem::addAdventurer(const std::string& name,Gender g,Race r,Profession p){
    Adventurer a;
    //allocate id;find the largest id existed
    int maxId=0;
    for(const auto& existing:lists){
        if(existing.id>maxId) maxId=existing.id;
    }
    a.id = maxId + 1;
    a.name=name;
    a.gender=g;
    a.race=r;
    a.profession=p;
    a.teamName="";
    lists.push_back(a);
    //status hint
    std::cout<<"add an adventurer successfully: "<< a.name<<" ID = "<<a.id<<std::endl;
}
void AdventurerSystem::removeAdventurer(int id){
    for(auto it = lists.begin(); it != lists.end(); ++it){
        if(it->id == id){
            //级联删除，从队伍中也删除这个冒险者
            if(!it->teamName.empty()){
                Team* t = findTeamByName(it->teamName);
                if(t){
                    auto& members = t->memberIds;
                    members.erase(
                        std::remove(members.begin(),members.end(),id),
                        members.end()
                    );
                }
            }
            std::cout<<"已经删除冒险者： "<< it->name <<"\n";
            it=lists.erase(it);
            for(it; it!=lists.end(); ++it){
                it->id-=1;
                //修改被改过ID的冒险者在队伍里面的ID信息
                if(!it->teamName.empty()){
                    auto t=findTeamByName(it->teamName);
                    for(auto& memberid:t->memberIds){
                        if(memberid==it->id+1){
                            memberid-=1;
                        }
                    }
                }
            }
            return;
        }
    }
    std::cout<<"未找到 ID= "<< id <<"的冒险者"<< "\n";
}
void AdventurerSystem::printAllAdventurers() const{
    if(lists.empty()){
        std::cout<<"There's no adventurer log!"<<std::endl;
    }else{
        std::cout<<"=========all Adventures========="<<std::endl;
        for(const auto& a:lists){
            printAdventurer(a);
        }
    }
}
void AdventurerSystem::printAdventurersByRace(Race r) const{
    bool found=false;
    if(lists.empty()){
        std::cout<<"There's no adventurer log!"<<std::endl;
    }else{
        std::cout<<"=========all Adventures filter by race========="<<std::endl;
        for(const auto& a:lists){
            if(a.race==r) printAdventurer(a);
            found=true;
        }
        if(!found){
            std::cout<<"There's no such adventures!"<<std::endl;
        }
    } 
}
void AdventurerSystem::printAdventurersByProfession(Profession p) const{
    bool found=false;
    if(lists.empty()){
        std::cout<<"There's no adventurer log!"<<std::endl;
    }else{
        std::cout<<"=========all Adventures filter by profession========="<<std::endl;
        for(const auto& a:lists){
            if(a.profession==p) printAdventurer(a);
            found=true;
        }
        if(!found){
            std::cout<<"There's no such adventures!"<<std::endl;
        }
    } 
}

//========队伍管理=========//
void AdventurerSystem::addTeam(const std::string& name){
    //check whether the team name is existed.
    for(const auto& t:teams){
        if(t.name==name){
            std::cout<<"The team name is already existed!"<<std::endl;
        }
        return;
    }
    Team t;
    t.name=name;
    t.lastActiveTime=0;
    t.rating=0;
    teams.push_back(t);
    std::cout<<"Team "<<name<<" is created."<<std::endl;
    for(const auto& id:t.memberIds){
        std::cout<<"Member ID: "<<id<<std::endl;
    }
}
void AdventurerSystem::removeTeam(const std::string& name){
    //check whether the team name is existed.
    for(auto it = teams.begin();it != teams.end();++it){
        if(it->name==name){
            for(auto id:it->memberIds){
                auto member=findAdventurerById(id);
                member->teamName="";
            }
            teams.erase(it);
            std::cout<<"Remove team " <<name<<" successfully"<<std::endl;
        }
    }   
    std::cout<<"ERROR:The team name is not existed!"<<std::endl;
}
void AdventurerSystem::addMemberToTeam(const std::string& teamName,int adventurerId){
    //1.查找队伍是否存在
    for(auto& t:teams){
        if(t.name==teamName){
            std::cout<<"find team "<<teamName<<std::endl;
            //判断队伍中是否已经存在此ID，防止重复添加同一个id
            for(auto& id_:t.memberIds){
                if(id_==adventurerId){
                    std::cout<<"ERROR:the adventurer is already existed in this team!"<<std::endl;
                    return;
                }
            }
            t.memberIds.push_back(adventurerId);
            Adventurer* adventurer=findAdventurerById(adventurerId);
            //如果此ID的冒险者不存在，打印错误信息，返回false
            if(adventurer == nullptr)
            {
                std::cout<<"ERROR:adventurer is not existed!!";
                return;
            }else{
                //判断此冒险者是否已有队伍，如果已有，则先在原来的队伍中删掉此ID信息，此冒险者自己所属的队伍会自动更新成最新的
                if(adventurer->teamName.empty()){
                    adventurer->teamName=teamName;
                    std::cout<<"Successfully add team member to "<<teamName<<std::endl;
                }else{
                    removeMemberFromTeam(adventurer->teamName,adventurerId);
                    adventurer->teamName=teamName;
                }
                return;
            }
            
        }
    }
    std::cout<<"team: "<<teamName<<" is not existed!!"<<std::endl;
    return;   
}
void AdventurerSystem::removeMemberFromTeam(const std::string& teamName,int adventurerId){
    //1.查找队伍是否存在，存在，删除；不存在，打印错误信息并返回false
    for(auto it = teams.begin();it != teams.end();++it){
        if(it->name==teamName){
            for(auto id_iterator = it->memberIds.begin();id_iterator != it->memberIds.end();++id_iterator){
                if(*id_iterator==adventurerId){
                    it->memberIds.erase(id_iterator);//删除此ID
                    Adventurer* adventurer=findAdventurerById(adventurerId);
                    adventurer->teamName="";//清空此冒险者的队伍信息
                    std::cout<<"remove member from this team successfully!"<<std::endl;
                    return;
                }
            }
            std::cout<<"ERROR:didn't find this member(id: "<<adventurerId<<" ) in the team!"<<std::endl;
            return;

        }
    }
    std::cout<<"ERROR:didn't find this team: "<<teamName<<" !"<<std::endl;
    return; 
}
void AdventurerSystem::setTeamRating(const std::string& teamName ){
    Team* t = findTeamByName(teamName);
        if(t->memberIds.empty()){
            std::cout<<"This team is empty!no rank."<<std::endl;
            return;
        }else{
            teamProStatics(*t);
            std::cout<<"TeamNmae: "<<t->name;
            if(t->profession_static[0]&& t->profession_static[1] && t->profession_static[4]){
                t->rating=3;
                printRankStar(3);
            }else if( t->profession_static[0]+t->profession_static[1]+t->profession_static[2]
                +t->profession_static[3]+t->profession_static[4] == 1){
                printRankStar(1);
                t->rating=1;
            }else{
                printRankStar(2);
                t->rating=2;
            }

        }
    return;    
}
void AdventurerSystem::printAllTeams() const;