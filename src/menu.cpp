#include "../include/menu.h"
#include "../include/adventurer_manager.h"
#include "../include/team_manager.h"
#include "../include/adventure_manager.h"
#include <iostream>
#include <string>
#include <limits>

//安全输入函数，在用户输入数字的时候，限制用户只能输入数字
static int readInt(const std::string& prompt){
    int val;
    while(true){
        std::cout<<prompt;
        if(std::cin>>val){
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            return val;//这都是什么意思啊
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout<<"invalid input,please input number"<<std::endl;
    }
}
static std::string readLine(const std::string& prompt){
    std::string val;
    std::cout<<prompt;
    std::getline(std::cin,val);
    return val;
}
//枚举enum类型选择辅助
static Gender selectGender(){
    std::cout<<"Gender: 1.Female 2.Male 3.Others->";
    int c=readInt("");
    if(c==1)return Gender::Female;
    if(c==2)return Gender::Male;
    return Gender::Other;
}
static Race selectRace() {
    std::cout << "种族：1.人类  2.精灵  3.矮人  4.兽人  5.其他 -> ";
    switch (readInt("")) {
        case 1: return Race::Human;
        case 2: return Race::Elf;
        case 3: return Race::Dwarf;
        case 4: return Race::Orc;
        default: return Race::Other;
    }
}
static Profession selectProfession() {
    std::cout << "职业：1.战士  2.法师  3.盗贼  4.牧师  5.游侠 -> ";
    switch (readInt("")) {
        case 1: return Profession::Warrior;
        case 2: return Profession::Mage;
        case 3: return Profession::Rogue;
        case 4: return Profession::Cleric;
        default: return Profession::Ranger;
    }
}
//=======子菜单：冒险者管理==========
static void menuAdventurer(std::vector<Adventurer>& adventurers,std::vector<Team>& teams){
    while(true){
        std::cout << "\n===== 冒险者管理 =====" << std::endl;
        std::cout << "1. 添加冒险者" << std::endl;
        std::cout << "2. 删除冒险者" << std::endl;
        std::cout << "3. 查看所有冒险者" << std::endl;
        std::cout << "4. 按种族筛选" << std::endl;
        std::cout << "5. 按职业筛选" << std::endl;
        std::cout << "0. 返回" << std::endl;
        int choice =readInt("请选择: ");
        if(choice==0) break;
        if(choice==1){
            std::string name=readLine("name: ");
            Gender g=selectGender();
            Race r=selectRace();
            Profession p=selectProfession();
            addAdventurer(adventurers,name,g,r,p);
        }else if(choice==2){
            printAllAdventurers(adventurers);
            int id = readInt("Input the ID you want to delete.");
            removeAdventurer(teams,adventurers,id);
        }else if(choice==3){
            printAllAdventurers(adventurers);
        }else if(choice==4){
            Race   r=selectRace();
            filterByRace(adventurers,r);
        }else if(choice==5){
            Profession p=selectProfession();
            filterByProfession(adventurers,p);
        }
    }
}
//======子菜单：队伍管理=========
static void menuTeam(std::vector<Team>& teams,
                     std::vector<Adventurer>& adventurers){
    while(true){
        std::cout << "\n===== 队伍管理 =====" << std::endl;
        std::cout << "1. 创建队伍" << std::endl;
        std::cout << "2. 删除队伍" << std::endl;
        std::cout << "3. 查看所有队伍" << std::endl;
        std::cout << "4. 添加成员" << std::endl;
        std::cout << "5. 移除成员" << std::endl;
        std::cout << "6. 设置评级" << std::endl;
        std::cout << "0. 返回" << std::endl;
        int choice=readInt("please choose: ");
        if(choice==0) break;
        if(choice==1){
            std::string teamName=readLine("Please input team name. ");
            addTeam(teams,teamName);
        }else if(choice==2){
            printAllTeams(teams,adventurers);
            std::string teamName=readLine("Please input the team name that you want to delete.");
            removeTeam(teams,adventurers,teamName);
        }else if(choice==4){
            printAllTeams(teams,adventurers);
            std::string teamName=readLine("Please input the team name.");
            printAllAdventurers(adventurers);
            int id=readInt("input the adventurer's id.");
            addTeamMember(teams,adventurers,teamName,id);
        }else if(choice==3){
            printAllTeams(teams,adventurers);
        }else if (choice==5){
            printAllTeams(teams,adventurers);
            std::string teamName=readLine("Input the team name that you want to remove member from.");
            int id=readInt("Input the member's ID.");
            removeTeamMember(teams,adventurers,teamName,id);
        }else if(choice==6){
            teamRanking(teams,adventurers);
        }
    }
}
// ===== 子菜单：冒险记录 =====
static void menuAdventure(std::vector<AdventureLog>& logs,
                           std::vector<Team>& teams,
                           std::vector<Adventurer>& adventurers) {
    while(true){
        std::cout<<"\n=======冒险记录========"<<std::endl;
        std::cout << "1. 登记冒险" << std::endl;
        std::cout << "2. 添加伤亡信息" << std::endl;
        std::cout << "3. 查看所有记录" << std::endl;
        std::cout << "4. 按队伍查看记录" << std::endl;
        std::cout << "0. 返回" << std::endl;
        int choice=readInt("please choose: ");
        if(choice==0) break;
        if(choice==1){
            printAllTeams(teams,adventurers);
            std::string teamName=readLine("Please input team name.");
            std::string notes=readLine("Please input notes.");
            addAdventureLog(logs,teams,adventurers,teamName,notes);
        }else if (choice==2){
            printAllLogs(logs,adventurers);
            int logID=readInt("Please input logID.");
            printAllTeams(teams,adventurers);
            int adID=readInt("Please input casualty's ID.");
            int rescued=readInt("是否带出迷宫？1.是  0.否 .");
            std::string location="";
            if(!rescued) location=readLine("遗留位置: ");
            addCasualties(logs,logID,adID,rescued==1,location,adventurers);
        }else if(choice==3){
            printAllLogs(logs,adventurers);
        }else if(choice==4){
            printAllLogs(logs,adventurers);
            std::string teamName=readLine("Input team name.");
            printLogsByTeam(logs,adventurers,teamName);
        }
    }
}
void runMainMenu(std::vector<Adventurer>& adventurers,
                 std::vector<Team>& teams,
                 std::vector<AdventureLog>& logs){
    while(true){
        std::cout << "\n=============================";
        std::cout << "\n  迷宫冒险者管理系统";
        std::cout << "\n=============================";
        std::cout << "\n1. 冒险者管理";
        std::cout << "\n2. 队伍管理";
        std::cout << "\n3. 冒险记录";
        std::cout << "\n0. 保存并退出\n" << std::endl;
        int choice=readInt("Please choose.");
        if(choice==0) break;
        if(choice==1){
            menuAdventurer(adventurers,teams);
        }else if(choice==2){
            menuTeam(teams,adventurers);
        }else if(choice==3){
            menuAdventure(logs,teams,adventurers);
        }else std::cout<<"无效选项"<<std::endl;
    }
}