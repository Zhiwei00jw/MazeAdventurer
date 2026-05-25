#include "../include/adventurer_manager.h"
#include <vector>
#include <string>
#include "../include/team_manager.h"

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
//print one adventurer's info
void printAdventurer(const Adventurer& a){
    std::cout<<"name: "<<a.name<<std::endl;
    std::cout<<"gender: "<<genderToStr(a.gender)<<std::endl;
    std::cout<<"race: "<<raceToStr(a.race)<<std::endl;
    std::cout<<"profession: "<<professionToStr(a.profession)<<std::endl;
    std::cout<<"ID: "<<a.id<<std::endl;
    std::cout<<"TeamName: "<<a.teamName<<std::endl;
    std::cout<<"===================="<<std::endl;
}
//add adventurer
void addAdventurer(std::vector<Adventurer>& list,const std::string& name,Gender gender,Race race,Profession profession){
    Adventurer a;
    //allocate id;find the largest id existed
    int maxId=0;
    for(const auto& existing:list){
        if(existing.id>maxId) maxId=existing.id;
    }
    a.id = maxId + 1;
    a.name=name;
    a.gender=gender;
    a.race=race;
    a.profession=profession;
    a.teamName="";
    list.push_back(a);
    //status hint
    std::cout<<"add an adventurer successfully: "<< a.name<<" ID = "<<a.id<<std::endl;
}
//remove adventurer
bool removeAdventurer(std::vector<Team>& teams,std::vector<Adventurer>& list,int id){
    for( auto it= list.begin();it!=list.end();++it){
        if(it->id==id) {
            std::cout<<"Adventure "<<it->name<<" has been deleted!"<<std::endl;
            if(!(it->teamName.empty())){
                //要从队伍里面清空这个冒险者的信息
                auto team=findTeam(teams,it->teamName);
                removeTeamMember(teams,list,team->name,id);
            }
            it=list.erase(it);
            //modify the id of adventures who are behind this adventurer
            for( it ;it!=list.end();++it){
                it->id-=1;
                //还要修改队伍里面储存的冒险者的ID信息
                if(!(it->teamName.empty())){
                    //修改这些被改过ID的冒险者在队伍里面的信息
                    auto team=findTeam(teams,it->teamName);
                    for(auto& memberid:team->memberIds){
                        if(memberid==it->id+1){
                            memberid-=1;
                        }
                    }
                }
            }
            return true;
        }
    }
    std::cout<<"Not found!"<<std::endl;
    return false;
}
//find adventurer
Adventurer* findAdventurerById(std::vector<Adventurer>& list,int id){
    for( auto& it: list){
        if(it.id==id) {
            return &it;
        }
    }
    return nullptr;
}
//print all adventurers
void printAllAdventurers(const std::vector<Adventurer>& list){
    if(list.empty()){
        std::cout<<"There's no adventurer log!"<<std::endl;
    }else{
        std::cout<<"=========all Adventures========="<<std::endl;
        for(const auto& a:list){
            printAdventurer(a);
        }
    }
}
//filter by race and print
void filterByRace(const std::vector<Adventurer>& list,Race race){
    bool found=false;
    if(list.empty()){
        std::cout<<"There's no adventurer log!"<<std::endl;
    }else{
        std::cout<<"=========all Adventures filter by race========="<<std::endl;
        for(const auto& a:list){
            if(a.race==race) printAdventurer(a);
            found=true;
        }
        if(!found){
            std::cout<<"There's no such adventures!"<<std::endl;
        }
    } 
}
//filter by profession and print
void filterByProfession(const std::vector<Adventurer>& list,Profession profession){
    bool found=false;
    if(list.empty()){
        std::cout<<"There's no adventurer log!"<<std::endl;
    }else{
        std::cout<<"=========all Adventures filter by profession========="<<std::endl;
        for(const auto& a:list){
            if(a.profession==profession) printAdventurer(a);
            found=true;
        }
        if(!found){
            std::cout<<"There's no such adventures!"<<std::endl;
        }
    } 
}   
void filterByGender(const std::vector<Adventurer>& list,Gender gender){
    bool found=false;
    if(list.empty()){
        std::cout<<"There's no adventurer log!"<<std::endl;
    }else{
        std::cout<<"=========all Adventures filter by gender========="<<std::endl;
        for(const auto& a:list){
            if(a.gender==gender){
                printAdventurer(a);
                found=true;
            }
        }
        if(!found){
            std::cout<<"There's no such adventurer!"<<std::endl;
        }
    }  
}