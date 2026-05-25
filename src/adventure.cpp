#include "../include/types.h"
#include "../include/adventure_manager.h"
#include "../include/team_manager.h"
#include "../include/adventurer_manager.h"
#include <ctime>

//add adventure log
void addAdventureLog(std::vector<AdventureLog>& logs,
                    std::vector<Team>& teams,
                    std::vector<Adventurer>& list,
                    const std::string& teamName,
                    const std::string& notes ){
    AdventureLog log;
    //自动分配最大的ID
    int id=0;
    for(auto& log:logs){
        if(id<log.id){
            id=log.id;
        }
    }
    id+=1;
    log.id=id;
    std::cout<<"successfully add a new log ,id is "<<id<<std::endl;
    //冒险时间为当前时间
    log.time=std::time(nullptr);
    //如果是以队伍为单位冒险的，也就是队伍名称非空
    if(!(teamName.empty())){
        log.teamName=teamName;
        //参加者的ID为队伍中所有成员的ID
        auto team=findTeam(teams,log.teamName);
        if(team == nullptr){
            std::cout<<"ERROR:when adding log, the team '"<<teamName<<"' does not exist!"<<std::endl;
            return;
        }
        for(auto id:team->memberIds){
            log.participantIDds.push_back(id);
        }
        team->lastActiveTime=log.time;
    }else{
        std::cout<<"ERROR:when adding log,the teamname is empty!"<<std::endl;
        return;
        //不接受以个人为单位冒险哈
    }
    log.notes=notes;
    logs.push_back(log);
    //更新队伍最近活动时间
    std::cout<<"Successfully add an adventure log ,log id is "<<id<<" ,paticipant team is "<<teamName<<std::endl;
}
void addCasualties(std::vector<AdventureLog>& logs,
                   int logID,
                   int adventurerID,
                   bool rescued,
                   const std::string location,
                   std::vector<Adventurer>& list
                   ){
    CasualtyInfo casualty;
    casualty.adventurerID=adventurerID;
    auto adventurer=findAdventurerById(list,adventurerID);
    if(!adventurer){
        std::cout<<"ERROR:when adding casualty,the adventurer id is empty!"<<std::endl;
        return;
    }
    casualty.name=adventurer->name;
    casualty.rescued=rescued;
    casualty.location=location;
    if(rescued){
        //已经被营救，带出迷宫
        std::cout<<"adventurer "<<adventurer->name<<" is rescued"<<std::endl;
    }else{
        std::cout<<"adventurer "<<adventurer->name<<"'s location is "<<location<<std::endl;
    }
    //在log中更新遇难者信息
    for(auto& log:logs){
        if(log.id==logID){
            log.casualties.push_back(casualty);
            std::cout<<"log id "<<logID<<" ,casualties info are updated."<<std::endl;
            return;
        }
    }
    std::cout<<"ERROR:when updating casualty info in log,log id "<<logID<<" is empty!"<<std::endl;

}
//把time_t转换成可读日期字符串
static std::string timeTostr(std::time_t t){
    if(t==0){
        return "unknown time.";
    }
    std::tm* tm_info=std::localtime(&t);
    char buf[32];//字符数组，用来存储时间
    std::strftime(buf,sizeof(buf),"%Y-%m-%d %H:%M",tm_info);
    return std::string(buf);
}
//打印单条冒险记录
void printLog(const AdventureLog& log,std::vector<Adventurer>& adventurers){
    std::cout<<"==========log "<<log.id<<" ==========="<<std::endl;
    std::cout<<"team name: "<<log.teamName<<std::endl;
    std::cout<<"paticipants name: "<<std::endl;
    std::cout<<"Time: "<<timeTostr(log.time)<<std::endl;

    for(auto id:log.participantIDds){
        auto adventurer = findAdventurerById(adventurers,id);
        if(adventurer){
            std::cout<<adventurer->name<<std::endl;
        } else {
            std::cout<<"<unknown>";
        }
    }
    if(log.casualties.empty()){
        std::cout<<"There is no casualty."<<std::endl;
    }else{
        for(auto casualty:log.casualties){
            std::cout<<"casualty name: "<<casualty.name;
            if(casualty.rescued){
                std::cout<<" is rescued."<<std::endl;
            }else{
                std::cout<<"is not rescued. Location: "<<casualty.location<<std::endl;
            }
        }
    }
    //打印备注
    std::cout<<"NOTES: "<<log.notes<<std::endl;
}
//打印所有冒险记录
void printAllLogs(std::vector<AdventureLog>& logs,std::vector<Adventurer>& adventurers){
    if(logs.empty()){
        std::cout<<"=====There is no log.====="<<std::endl;
    }
    for(auto& log:logs){
        printLog(log,adventurers);
    }
}
//打印某只队伍的所有记录
void printLogsByTeam(const std::vector<AdventureLog>& logs,
                     std::vector<Adventurer>& adventurers,
                     const std::string& teamName){
    int cnt=0;
    for(auto& log:logs){
        if(log.teamName==teamName){
            printLog(log,adventurers);
            cnt++;
        }
    }
    if(cnt==0){
        std::cout<<"There is no log belong to this team "<<teamName<<std::endl;
    }

}
//按照logID查找对应的记录
AdventureLog* findLogById(std::vector<AdventureLog>& logs,int logid){
    for(auto& log:logs){
        if(log.id==logid){
            return &log;
        }
    }
    return nullptr;
}