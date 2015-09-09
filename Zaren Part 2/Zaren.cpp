/*
Author Tyler Thaxton
Zaren OS
*/
#include <iostream>
#include <ctime>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <queue>

using namespace std;

class PCB
{
public:
    PCB()
    {
        processName = "";
        processClass = 0;
        priority = 0;
        state = 0;
        memory = 0;
    }
    void setProcessName (string name)
    {
        processName = name;
    }
    string getProcessName()
    {
        return processName;
    }
    void setProcessClass(int value)
    {
        processClass = value;
    }
    int getProcessClass()
    {
        return processClass;
    }
    void setPriority(int value)
    {
        priority = value;
    }
    int getPriority()
    {
        return priority;
    }
    void setState(int value)
    {
        state = value;
    }
    int getState()
    {
        return state;
    }
    void setMemory(int value)
    {
        memory = value;
    }
    int getMemory()
    {
        return memory;
    }
private:
    string processName;
    int processClass; //1-application 2-system type
    int priority; //-127 to 128
    int state;//1-running 2-ready 3-block 4-suspended ready 5-suspended blocked
    int memory;
};

class PCBControl
{
public:
    PCBControl()
    {
    }
    PCB* AllocatePCB()
    {
        PCB *pcb = new PCB();
        return pcb;
    }
    void FreePCB(PCB *pcb)
    {
        delete pcb;
    }
    PCB* SetupPCB(string name, int priority, int processClass)
    {
        if (FindPCB(name) != NULL)
        {
            cout << "Process Name already in use" << endl;
            return NULL;
        }
        if (priority > 128 || priority < -127)
        {
            cout << "Priority must be between -127 and 128" << endl;
            return NULL;
        }
        if (processClass < 1 || processClass > 2)
        {
            cout << "Process Class must either be 1-Application or 2-System" << endl;
            return NULL;
        }
        PCB *pcb = AllocatePCB();
        pcb->setProcessName(name);
        pcb->setPriority(priority);
        pcb->setProcessClass(processClass);
        pcb->setState(1);
        return pcb;


    }
    PCB* FindPCB(string processName)
    {
        PCB *temp;
        queue<PCB*> readyCopy = ready;
        while (!readyCopy.empty())
        {
            temp = readyCopy.front();
            readyCopy.pop();

            if (temp->getProcessName() == processName)
            {
                return temp;
            }
        }
        queue<PCB*> blockedCopy = blocked;
        while (!blockedCopy.empty())
        {
            temp = blockedCopy.front();
            blockedCopy.pop();
            if (temp->getProcessName() == processName)
            {
                return temp;
            }
        }
        queue<PCB*> suspendedReadyCopy = suspendedReady;
        while (!suspendedReadyCopy.empty())
        {
            temp = suspendedReadyCopy.front();
            suspendedReadyCopy.pop();
            if (temp->getProcessName() == processName)
            {
                return temp;
            }
        }
        queue<PCB*> suspendedBlockedCopy = suspendedBlocked;
        while (!suspendedBlockedCopy.empty())
        {
            temp = suspendedBlockedCopy.front();
            suspendedBlockedCopy.pop();
            if (temp->getProcessName() == processName)
            {
                return temp;
            }
        }
        return NULL;
    }
    void InsertPCB (PCB *pcb)
    {
        int state = pcb->getState();
        if (state == 1)
        {
            ready.push(pcb);
            return;
        }
        else if(state == 2)
        {
            blocked.push(pcb);
            return;
        }
        else if(state == 3)
        {
            suspendedReady.push(pcb);
            return;
        }
        else if(state == 4)
        {
            suspendedBlocked.push(pcb);
            return;
        }
        else
        {
            cout << "PCB not in a proper state" << endl;
            return;
        }
    }
    void RemovePCB(PCB *pcb)
    {
        int found = 0;
        PCB *temp;
        queue<PCB*> readyTemp;
        queue<PCB*> blockedTemp;
        queue<PCB*> suspendedReadyTemp;
        queue<PCB*> suspendedBlockedTemp;


        queue<PCB*> readyCopy = ready;
        while (!readyCopy.empty())
        {
            temp = readyCopy.front();
            readyCopy.pop();

            if(pcb == temp)
            {
                found = 1;
            }
            else if (pcb != temp)
            {
                readyTemp.push(temp);
            }

        }
        if (found == 1)
        {
            ready = readyTemp;
            return;
        }
        else if (found == 0)
        {
            queue<PCB*> blockedCopy = blocked;
            while (!blockedCopy.empty())
            {
                temp = blockedCopy.front();
                blockedCopy.pop();

                if(pcb == temp)
                {
                    found = 1;
                }
                else if (pcb != temp)
                {
                    blockedTemp.push(temp);
                }
            }
        }
        if (found == 1)
        {
            blocked = blockedTemp;
            return;
        }
        else if (found == 0)
        {
            queue<PCB*> suspendedReadyCopy = suspendedReady;
            while (!suspendedReadyCopy.empty())
            {
                temp = suspendedReadyCopy.front();
                suspendedReadyCopy.pop();
                if(pcb == temp)
                {
                    found = 1;
                }
                else if (pcb != temp)
                {
                    suspendedReadyTemp.push(temp);
                }
            }
        }
        if (found == 1)
        {
            suspendedReady = suspendedReadyTemp;
            return;
        }
        else if (found == 0)
        {
            queue<PCB*> suspendedBlockedCopy = suspendedBlocked;
            while (!suspendedBlockedCopy.empty())
            {
                temp = suspendedBlockedCopy.front();
                suspendedBlockedCopy.pop();
                if(pcb == temp)
                {
                    found = 1;
                }
                else if (pcb != temp)
                {
                    suspendedBlockedTemp.push(temp);
                }
            }
        }
        if (found == 1)
        {
            suspendedBlocked = suspendedBlockedTemp;
            return;
        }
        else if (found == 0)
        {
            cout << "PCB not in queue" << endl;
        }
    }
private:
    queue<PCB*> ready;
    queue<PCB*> blocked;
    queue<PCB*> suspendedReady;
    queue<PCB*> suspendedBlocked;

};

class Zaren
{
public:
    void start()
    {
        test = 0;
        PCBControl pcbControl;
        cout << string(100, '\n'); //http://www.cplusplus.com/forum/beginner/3304/
        cout << "Welcome to Zaren" << endl;
        cout << "Press any key to get started";
        cin.get();
        menu();
    }
    void OSTest()
    {
        test = 1;
        version();
        date();
        directory();
        help();
        exit();

    }
private:
    void menu ()
    {
        cout << string(100, '\n');
        int command = 0;
        cout << "Command List" << endl;
        cout << "1 - Version" << endl;
        cout << "2 - Date" << endl;
        cout << "3 - Directory" << endl;
        cout << "4 - Exit" << endl;
        cout << "5 - Help" << endl;
        cout << "Enter the number of desired command: ";
        cin.clear();
        cin >> command;
        cin.clear();
        cin.ignore(10000, '\n');

        if(test == 1)
        {
            return;
        }
        else if (command == 1)
        {
            version();
        }
        else if (command == 2)
        {
            date();
        }
        else if (command == 3)
        {
            directory();
        }
        else if (command == 4)
        {
            exit();
        }
        else if (command == 5)
        {
            help();
        }
        else
        {
            cout << "Not valid command" << endl;
            cout << "Press any key" << endl;
            cin.get();
            menu();
        }

    }
    void version ()
    {
        cout << string(100, '\n');
        cout << "Zaren Version 1.1" << endl;
        cout << "Press any key to return to menu";
        cin.get();
        if(test == 0)
        {
            menu();
        }
        else
        {
            return;
        }
    }
    void date()
    {
        cout << string(100, '\n');

        time_t timeIn = time(NULL);
        char mbstr[100];
        if (strftime(mbstr, sizeof(mbstr), "%A %c", localtime(&timeIn)))
        {
            cout << mbstr << endl;
        }

        cout << "Press any key to return to menu";
        cin.get();

        if(test == 0)
        {
            menu();
        }
        else
        {
            return;
        }
    }
    void directory()
    {
        cout << string(100, '\n');
        DIR *dir; //http://cboard.cprogramming.com/c-programming/134292-find-all-files-current-directory.html
        struct dirent *dp;

            char cwd[1024];

        getcwd(cwd, sizeof(cwd));

        if((dir  = opendir(cwd)) == NULL) {
            perror("\nUnable to open directory.");
        }
            while ((dp=readdir(dir)) != NULL) {
                    printf("%s\n", dp->d_name);
        }
        closedir(dir);
        cout << "Press any key to return to menu";
        cin.get();
        if(test == 0)
        {
            menu();
        }
        else
        {
            return;
        }
    }
    void exit()
    {
        cout << string(100, '\n');
        string command;
        cout << "Are you sure you wish to exit?" << endl;
        cout << "Y/N: ";
        cin.clear();
        cin >> command;
        cin.clear();
        cin.ignore(10000, '\n');

        if (test == 1)
        {
            cout << "End of Test" << endl;
            cout << "Press any key to Exit";
            cin.get();
            return;
        }
        else if(command == "Y" || command == "y")
        {
            return;
        }
        else if (command == "N" || command == "n")
        {
            cout << "Press any key to return to menu";
            cin.get();
            menu();
        }
        else
        {
            cout << "Not valid command" << endl;
            cout << "Press any key" << endl;
            cin.get();
            exit();
        }
    }
    void help()
    {
        cout << string(100, '\n');
        cout << "Help" << endl;
        cout << "Version - Displays current Operating System version" << endl;
        cout << "Date - Displays current date and time." << endl;
        cout << "Directory - Displays all files in current directory." << endl;
        cout << "Exit - Exits the Operating System. User inputs Y to exit or N to return to menu." << endl;
        cout << "Press any key to return to menu";
        cin.get();
        if(test == 0)
        {
            menu();
        }
        else
        {
            return;
        }
    }
    int test;
};

int main()
{
    Zaren OS;
    //OS.OSTest(); //Test Function
    OS.start();
}
