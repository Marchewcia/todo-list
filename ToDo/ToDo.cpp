//4. Usuwanie zadań
//5. Zapisywanie listy do pliku i jej wczytywanie
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <iomanip>
#include <string>

//stores all variables used temporarly in code (content changes depends on what piece of code is executed at the moment)
namespace tmp {
    int index;
    int answer;
    std::string desc;
    bool yesnt;
}

//stores all user preferences
namespace pref {
    int idWidth = 5;
    int descWidth = 45;
    bool confirm = true;
    bool autoSave = true;
    std::string saveDirectory;
    std::string configFile = "C:/";
}

//holds all 
class Task {
    private:
        std::string desc;
        bool done;
        int id;
    public:
        Task(int id, std::string desc) {
            this->done = false;
            this->id = id;
            this->desc = desc;
        }
        void writeData() {
            std::string progress;
            if (this->done) {
                progress = "Completed";
            }
            else {
                progress = "Incompleted";
            }
            std::cout << std::left << "|" << std::setw(pref::idWidth) << this->id;
            std::cout << std::left << std::setw(pref::descWidth) << "|" + this->desc;
            std::cout << std::left << "|" << progress << std::endl;
        }
        void changeDesc(std::string desc) {
            this->desc = desc;
        }
        void changeProgress(bool newStatus) {
            this->done = newStatus;
        }
        bool isDone() {
            return this->done;
        }
        void fixId() {
            this->id--;
        }
};

void program(bool);
void settingConfig();
void configWrite();

bool isIndexValid();
void newElement();
void editElement();
void markDoneUndone();
void deleteItem();

int nextId = 0;
std::vector<Task> tasklist;

int main(){
    configWrite();
    std::cout << "Task manager ver. 1.0 \n Select option: \n 1. New file \n 2. Open file \n 3. Open settings config \n 4. Exit program \n";
    std::cin >> tmp::answer;
    switch (tmp::answer) {
        case 1:
            program(0);
            tmp::index = 1;
            break;
        case 2:
            tmp::index = 1;
            break;
        case 3:
            settingConfig();
            tmp::index = 1;
            break;
        default:
            tmp::index = 0;
            break;
    }
    if (tmp::index == 1) {
        main();
    }
}

void configRead() {

}

void configWrite() {
    std::fstream cfile;
    cfile.open("ToDoConfig.txt", std::fstream::in | std::fstream::out | std::fstream::app);
    cfile << pref::idWidth << "\n" << pref::descWidth << "\n" << pref::confirm << "\n" << pref::autoSave << "\n" << pref::saveDirectory;
    cfile.close();
}

void program(bool loadFile){
    //loading tasklist from file if selected
    if (loadFile) {

    }

    //printing list of tasks in current tasklist
    std::cout << "List of tasks:\n";
    std::cout << std::left << "|" << std::setw(pref::idWidth) << "Id";
    std::cout << std::left << std::setw(pref::descWidth) << "|Description";
    std::cout << std::left << "|Status\n";
    for (int i = 0; i < nextId; i++) {
        tasklist[i].writeData();
    }

    //calculating and displaying progress bar of how much tasks are completed off total
    std::cout << "\nProgress:\n[";
    float percDone = 0;
    for (int i = 0; i < nextId; i++) {
        if (tasklist[i].isDone()) {
            percDone++;
        }
    }
    if (percDone != 0) {
        percDone = (percDone / nextId) * 100;
    }
    for (int i = 100; i > 0; i -= 2) {
        if (percDone > 0) {
            percDone -= 2;
            std::cout << "#";
        }
        else {
            std::cout << std::setw(1) << "";
        }
    }
    std::cout << "]";

    //option menu
    std::cout << "\nSelect option: \n 1. Add a task \n 2. Edit a task \n 3. Mark/unmark task as complete \n 4. Delete task \n 5. Exit\n";
    std::cin >> tmp::answer;
    switch (tmp::answer) {
        case 1:
            newElement();
            break;
        case 2:
            editElement();
            break;
        case 3:
            markDoneUndone();
            break;
        case 4:
            deleteItem();
            break;
        case 5:
            break;
        default:
            return;
    }
    program(false);
}

//menu used to display and change current user settings
void settingConfig() {
    std::cout << "Current settings:";
    std::cout << "\n 1. IdWidth (width of ID column): " << pref::idWidth;
    std::cout << "\n 2. DescWidth (width of description column): " << pref::descWidth;
    std::cout << "\n 3. Confirm(do you want to confirm every operation?, 1 - yes, 0 - no): " << pref::confirm;
    std::cout << "\n 4. AutoSave(do you want to auto save after every operation?, 1 - yes, 0 - no): " << pref::autoSave;
    std::cout << "\nChoose setting or hit 0 to return to main menu: ";
    int option;
    std::cin >> option;
    std::cout << std::endl;
    switch (option) {
        case 0:
            tmp::index = -1;
            break;
        case 1:
        case 2:
            std::cout << "Enter new width: ";
            std::cin >> tmp::answer;
            break;
        case 3:
            std::cout << "Change confirm value (1 - yes, 0 - no): ";
            std::cin >> tmp::yesnt;
            break;
        case 4:
            std::cout << "Change autosave value (1 - yes, 0 - no)";
            std::cin >> tmp::yesnt;
            break;       
    }
    if (tmp::index == -1) {
        return;
    }
    if (pref::confirm) {
        std::cout << "Do you want to change this setting?\n 1. Yes \n 2. No \n";
        std::cin >> tmp::index;
    }
    else {
        tmp::index = 1;
    }
    if (tmp::index == 1) {
        switch (option) {
            case 1:
                pref::idWidth = tmp::answer;
                break;
            case 2:
                pref::descWidth = tmp::answer;
                break;
            case 3:
                pref::confirm = tmp::yesnt;
                break;
            case 4:
                pref::autoSave = tmp::yesnt;
                break;
        }
    }
    settingConfig();
}

//function that checks if index of tasks is not out of bounds, so below 0 or above last element
bool isIndexValid() {
    std::cout << "Choose task\n";
    std::cin >> std::ws >> tmp::index;
    if (tmp::index >= 0 && tmp::index < nextId) {
        return true;
    }
    else {
        std::cout << "Invalid index\n";
        return false;
    }
}

//create new task, adds it to list
void newElement() {
    std::cout << "Insert task description \n";
    std::getline(std::cin >> std::ws, tmp::desc);
    tasklist.push_back(Task(nextId, tmp::desc));
    nextId++;
}

//change description of choosen task
void editElement() {
    if (isIndexValid()) {
        std::cout << "Insert new task description \n";
        std::cin >> tmp::desc;
        tasklist[tmp::index].changeDesc(tmp::desc);
    }
}

//change status of choosen task to from undone to done or backwards
void markDoneUndone() {
    if (isIndexValid()) {
        int status;
        int ans;
        if (pref::confirm) {
            if (tasklist[tmp::index].isDone()) {
                std::cout << "Do you want to mark task as undone?";
            }
            else {
                std::cout << "Do you want to mark task as done?";
            }
            std::cout << "\n 1. Yes \n 2. No \n";
            std::cin >> ans;
        }
        else {
            ans = 1;
        }
        status = tasklist[tmp::index].isDone();
        if (ans == 1) {
            if (status == 1) {
                tasklist[tmp::index].changeProgress(false);
            }
            else {
                tasklist[tmp::index].changeProgress(true);
            }
        }
    }
}

//deletes task from list and fix indexes to fill created gap
void deleteItem() {
    if (isIndexValid()) {
        if (pref::confirm) {
            std::cout << "Do you want to delete this task?\n 1. Yes \n 2. No \n";
            std::cin >> tmp::answer;
        }
        else {
            tmp::answer = 1;
        }
        if (tmp::answer == 1) {
            tasklist.erase(tasklist.begin() + tmp::index);
            while (tmp::index < nextId - 1) {
                tasklist[tmp::index].fixId();
                tmp::index++;
            }
            nextId--;
        }
    }
}