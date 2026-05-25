#include "../include/team_manager.h"
#include "../include/adventurer_manager.h"
#include <iostream>
#include <ctime>

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
void teamProStatics(int& warrior,int& mage,int& cleric,int& rogue,int& ranger,std::vector<Adventurer>& list,std::vector<int> teamIds){
    for(auto& id:teamIds){
        Adventurer* adventurer=findAdventurerById(list,id);
        if(adventurer){
            //此ID不为空，指针不为空指针
            switch (adventurer->profession)
            {
            case Profession::Warrior:
                warrior++;
                break;
            case Profession::Mage:
                mage++;
                break;
            case Profession::Ranger:
                ranger++;
                break;
            case Profession::Rogue:
                rogue++;
                break;
            case Profession::Cleric:
                cleric++;
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
//add team
bool addTeam(std::vector<Team>& team,const std::string& name){
    //check whether the team name is existed.
    for(const auto& t:team){
        if(t.name==name){
            std::cout<<"The team name is already existed!"<<std::endl;
            return false;
        }
    }
    Team t;
    t.name=name;
    t.lastActiveTime=0;
    t.rating=0;
    team.push_back(t);
    std::cout<<"Team "<<name<<" is created~~"<<std::endl;
    for(const auto& id:t.memberIds){
        std::cout<<"Member ID: "<<id<<std::endl;
    }
    return true;

}
//remove team
bool removeTeam(std::vector<Team>& team,std::vector<Adventurer>& list,const std::string& name){
    //check whether the team name is existed.
    for(auto it = team.begin();it != team.end();++it){
        if(it->name==name){
            std::cout<<"The team name is existed!"<<std::endl;
            for(auto id:it->memberIds){
                auto member=findAdventurerById(list,id);
                member->teamName="";
            }
            team.erase(it);
            std::cout<<"Remove successfully"<<std::endl;
            return true;
        }
    }   
    std::cout<<"ERROR:The team name is not existed!"<<std::endl;
    return false;
}
//add team member
bool addTeamMember(std::vector<Team>& team,std::vector<Adventurer>& list,const std::string& name,int id){
    //1.查找队伍是否存在
    for(auto& t:team){
        if(t.name==name){
            std::cout<<"find team "<<name<<std::endl;
            //判断队伍中是否已经存在此ID，防止重复添加同一个id
            for(auto& id_:t.memberIds){
                if(id_==id){
                    std::cout<<"ERROR:the adventurer is already existed in this team!"<<std::endl;
                    return false;
                }
            }
            t.memberIds.push_back(id);
            Adventurer* adventurer=findAdventurerById(list,id);
            //如果此ID的冒险者不存在，打印错误信息，返回false
            if(adventurer == nullptr)
            {
                std::cout<<"ERROR:adventurer is not existed!!";
                return false;
            }else{
                //判断此冒险者是否已有队伍，如果已有，则先在原来的队伍中删掉此ID信息，此冒险者自己所属的队伍会自动更新成最新的
                if(adventurer->teamName.empty()){
                    adventurer->teamName=name;
                    std::cout<<"Successfully add team member to "<<name<<std::endl;
                }else{
                    auto single_team=findTeam(team,adventurer->teamName);
                    removeTeamMember(team,list,single_team->name,id);
                    adventurer->teamName=name;
                }
                return true;
            }
            
        }
    }
    std::cout<<"team "<<name<<" is not existed!!"<<std::endl;
    return false;
}
//remove team member
bool removeTeamMember(std::vector<Team>& team,std::vector<Adventurer>& list,const std::string& name,int id)
{
    //1.查找队伍是否存在，存在，删除；不存在，打印错误信息并返回false
    for(auto it = team.begin();it != team.end();++it){
        if(it->name==name){
            for(auto id_iterator = it->memberIds.begin();id_iterator != it->memberIds.end();++id_iterator){
                if(*id_iterator==id){
                    it->memberIds.erase(id_iterator);//删除此ID
                    Adventurer* adventurer=findAdventurerById(list,id);
                    adventurer->teamName="";//清空此冒险者的队伍信息
                    std::cout<<"remove member from this team successfully!"<<std::endl;
                    return true;
                }
            }
            std::cout<<"ERROR:didn't find this member in the team!"<<std::endl;
            return false;

        }
    }
    std::cout<<"ERROR:didn't find this team!"<<std::endl;
    return false;
}
//team ranking
bool teamRanking(std::vector<Team>& team,std::vector<Adventurer>& list){
    //对所有队伍进行评级，先检查队伍是否是空的，如果是，不予评级
    int wa_con=0;
    int ma_con=0;
    int cle_con=0;
    int ran_con=0;
    int rog_con=0;
    for(auto& t:team){
        if(t.memberIds.empty()){
            std::cout<<"This team is empty!no rank."<<std::endl;
            return false;
        }else{
            teamProStatics(wa_con,ma_con,cle_con,rog_con,ran_con,list,t.memberIds);
            std::cout<<"TeamNmae: "<<t.name;
            if(wa_con && ma_con && cle_con){
                t.rating=3;
                printRankStar(3);
            }else if(wa_con+ma_con+cle_con+ran_con+rog_con == 1){
                printRankStar(1);
                t.rating=1;
            }else{
                printRankStar(2);
                t.rating=2;
            }
            wa_con=0;
            ma_con=0;
            cle_con=0;
            rog_con=0;
            ran_con=0;

        }
    }

    return true;
}
//print single team with all members
void printTeam(const Team& team,std::vector<Adventurer>& list){
    int member=1;
    //print team name
    std::cout<<"Team Name: "<<team.name<<std::endl;
    std::cout<<"Team ranking: "<<team.rating<<std::endl;
    std::cout<<"Last active time: "<<timeSince(team.lastActiveTime)<<std::endl;
    //print all members
    for(const auto& id:team.memberIds){
        std::cout<<"====member"<<member<<"===="<<std::endl;
        auto adventurer=findAdventurerById(list,id);
        if(adventurer){
            printAdventurer(*adventurer);
            member++;
        }else{
            std::cout<<"The team "<<team.name<<" is empty!"<<std::endl;
        }
       
    }
}
//print all team
void printAllTeams(std::vector<Team>& teams,std::vector<Adventurer>& list){
    for(const auto& team:teams){
        printTeam(team,list);
    }
}

//find team by team name
Team* findTeam(std::vector<Team>& team,std::string& name){
    for(auto& it:team){
        if(it.name == name){
            std::cout<<"Find team successfully!"<<std::endl;
            return &it;
        }
    }
    return nullptr;

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