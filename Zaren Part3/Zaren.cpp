/*
Author Tyler Thaxton
Zaren OS
*/
#include <ctime>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <queue>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <utility>
#include <vector>

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
    void setTimeRemaining(int time)
    {
        timeRemaining = time;
    }
    int getTimeRemaining()
    {
        return timeRemaining;
    }
    void setArrivalTime(int time)
    {
        arrivalTime = time;
    }
    int getArrivalTime()
    {
        return arrivalTime;
    }
    void setCPUPercentage(int percent)
    {
        cpuPercentage = percent;
    }
    int getCPUPercentage()
    {
        return cpuPercentage;
    }
private:
    string processName;
    int processClass; //1-application 2-system type
    int priority; //-127 to 128
    int state;//1-running 2-ready 3-block 4-suspended ready 5-suspended blocked
    int memory;
    int timeRemaining;
    int arrivalTime;
    int cpuPercentage;
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
    PCB* SetupPCB(string name, int priority, int processClass, int memory, int timeRemaining, int arrivalTime, int cpuPercentage)
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
        if (cpuPercentage > 100)
        {
            cout << "CPU Percentage can not be more than 100" << endl;
            return NULL;
        }
        PCB *pcb = AllocatePCB();
        pcb->setProcessName(name);
        pcb->setPriority(priority);
        pcb->setProcessClass(processClass);
        pcb->setState(2);
        pcb->setMemory(memory);
        pcb->setTimeRemaining(timeRemaining);
        pcb->setArrivalTime(arrivalTime);
        pcb->setCPUPercentage(cpuPercentage);
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
        if (state == 2)
        {
            ready.push(pcb);
            return;
        }
        else if(state == 3)
        {
            blocked.push(pcb);
            return;
        }
        else if(state == 4)
        {
            suspendedReady.push(pcb);
            return;
        }
        else if(state == 5)
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
    void showQueue(int queues)
    {
        PCB *temp;
        queue<PCB*> readyCopy = ready;
        if (queues == 1 || queues == 5)
        {
            cout << endl << "Ready Queue" << endl;
            while (!readyCopy.empty())
            {
                temp = readyCopy.front();
                readyCopy.pop();
                showPCB(temp);

            }

        }
        if (queues == 2 || queues == 5)
        {
            cout << endl <<"Blocked Queue" << endl;
            queue<PCB*> blockedCopy = blocked;
            while (!blockedCopy.empty())
            {
                temp = blockedCopy.front();
                blockedCopy.pop();
                showPCB(temp);

            }

        }
        if(queues == 3 || queues == 5)
        {
            cout << endl << "Suspended Ready Queue" << endl;
            queue<PCB*> suspendedReadyCopy = suspendedReady;
            while (!suspendedReadyCopy.empty())
            {
                temp = suspendedReadyCopy.front();
                suspendedReadyCopy.pop();
                showPCB(temp);

            }

        }
        if (queues == 4 || queues == 5)
        {
            cout << endl << "Suspended Blocked Queue" << endl;
            queue<PCB*> suspendedBlockedCopy = suspendedBlocked;
            while (!suspendedBlockedCopy.empty())
            {
                temp = suspendedBlockedCopy.front();
                suspendedBlockedCopy.pop();
                showPCB(temp);
            }

        }
        return;
    }
    void showPCB(PCB *pcb)
    {
            cout << "Name: " << pcb->getProcessName() << " ";
            int processClass = pcb->getProcessClass();
            if (processClass ==  1)
            {
                cout << "Class: Application " << " ";
            }
            else if (processClass == 2)
            {
                cout << "Class: System " << " ";
            }
            cout << "Priority: " << pcb->getPriority() << " ";
            int state = pcb->getState();
            if(state == 1)
            {
                cout << "State: Running" << " ";
            }
            else if (state == 2)
            {
                cout << "State: Ready" << " ";
            }
            else if (state == 3)
            {
                cout << "State: Blocked" << " ";
            }
            else if (state == 4)
            {
                cout << "State: Suspended Ready" << " ";
            }
            else if (state == 5)
            {
                cout << "State: Suspended Blocked" << " ";
            }
            cout << "Memory: " << pcb->getMemory() << " " << endl;
            cout << "      " << "Time Remaining: " << pcb->getTimeRemaining() << " ";
            cout << "Arrival Time: " << pcb->getArrivalTime() << " ";
            cout << "Percent of CPU: " << pcb->getCPUPercentage() << " " << endl;
    }
    void readFile()
    {
        string file;
        cout << "Input File Name: ";
        cin >> file;
        cout << endl;
        ifstream infile(file.c_str());
        if (infile.is_open())
        {
            string processName;
            char processClass;
            int processClassINT;
            int priority;
            int memory;
            int timeRemaining;
            int arrivalTime;
            int cpuPercentage;
            while (infile >> processName >> processClass >> priority >> memory >> timeRemaining >> arrivalTime >> cpuPercentage)
            {
                if (processClass == 'A')
                {
                    processClassINT = 1;
                }
                else if (processClass == 'S')
                {
                    processClassINT = 2;
                }
                PCB *pcb =SetupPCB(processName, priority, processClassINT, memory, timeRemaining, arrivalTime, cpuPercentage);
                if (pcb != NULL)
                {
                    schedule.push_back(pcb);
                }
            }
            infile.close();
        }
        else
        {
            cout << "File could not be opened" << endl;
        }
    }
    void SJF()
    {
        cout << string(100, '\n');
        cout << "Shortest Job First" << endl;
        readFile();
        if (!schedule.empty())
        {
           while (!schedule.empty())
           {
               PCB *pcb = schedule[0];
               int index = 0;
               for (int i = 0; i < schedule.size(); i++)
               {
                   PCB *temp = schedule[i];
                   if (temp->getTimeRemaining() < pcb->getTimeRemaining())
                   {
                       pcb = temp;
                       index = i;
                   }
               }
               ready.push(pcb);
               schedule.erase(schedule.begin()+index);
           }
            showQueue(1);
            int time = 0;
            int counter = 0;
            int turnaround = 0;
            cout << endl;
            while (!ready.empty())
            {
                PCB *pcb = ready.front();
                time = time + pcb->getTimeRemaining();
                turnaround = turnaround + time;
                counter++;
                cout << pcb->getProcessName() << endl;
                ready.pop();
                delete pcb;
            }
            cout << endl << "Time to completion: " << time << endl;
            cout << "Average Turnaround Time : " << (turnaround/counter) << endl;
            cout << "Press Enter" << endl;
            cin.get();
            cin.clear();
            cin.ignore(10000, '\n');

        }
        else
        {
            cout << "Nothing to schedule" << endl;
            cout << "Press Enter" << endl;
            cin.get();
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
    void FIFO()
    {
        cout << string(100, '\n');
        cout << "First In First Out" << endl;
        readFile();
        if (!schedule.empty())
        {
            for (int i = 0; i < schedule.size(); i++)
            {
                ready.push(schedule[i]);
            }
            schedule.clear();
            showQueue(1);
            int time = 0;
            int counter = 0;
            int turnaround = 0;
            cout << endl;
            while (!ready.empty())
            {
                PCB *pcb = ready.front();
                if (time < pcb->getArrivalTime())
                {
                    time = pcb->getArrivalTime();
                }
                time = time + pcb->getTimeRemaining();
                turnaround = turnaround + time - pcb->getArrivalTime();
                counter++;
                cout << pcb->getProcessName() << endl;
                ready.pop();
                delete pcb;
            }
            cout << endl << "Time to completion: " << time << endl;
            cout << "Average Turnaround Time : " << (turnaround/counter) << endl;
            cout << "Press Enter" << endl;
            cin.get();
            cin.clear();
            cin.ignore(10000, '\n');
        }
        else
        {
            cout << "Nothing to schedule" << endl;
            cout << "Press Enter" << endl;
            cin.get();
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
    void STCF()
    {
        cout << string(100, '\n');
        cout << "Shortest Time to Completion First" << endl;
        readFile();
        if (!schedule.empty())
        {
            ofstream outFile;
            outFile.open ("STCF.txt");
            PCB *pcb = schedule[0];
            outFile << pcb->getProcessName() << " arrived" << endl;
            outFile << pcb->getProcessName() << " running" << endl;
            schedule.erase(schedule.begin());
            int time = pcb->getArrivalTime();
            int turnaround = 0;
            int counter = 1;
            while (!schedule.empty())
            {
                PCB *temp = schedule[0];
                schedule.erase(schedule.begin());
                pcb->setTimeRemaining(pcb->getTimeRemaining() - (temp->getArrivalTime() - time));
                time = temp->getArrivalTime();
                counter ++;
                while (pcb != NULL && pcb->getTimeRemaining() <= 0)
                {
                    outFile << pcb->getProcessName() << " completed" << endl;
                    turnaround = turnaround + (time + pcb->getTimeRemaining() - pcb->getArrivalTime());
                    PCB *hold = NULL;
                    if (!ready.empty())
                    {
                        hold = ready.front();
                        ready.pop();
                        outFile << hold->getProcessName() << " running" << endl;
                        hold->setTimeRemaining(hold->getTimeRemaining() + pcb->getTimeRemaining());
                    }
                    delete pcb;
                    pcb = hold;
                }
                if (pcb == NULL)
                {
                    pcb = temp;
                    outFile << temp->getProcessName() << " arrived" << endl;
                    outFile << temp->getProcessName() << " running" << endl;
                }
                else
                {
                    int placed = 0;
                    outFile << temp->getProcessName() << " arrived" << endl;
                    if (pcb->getTimeRemaining() > temp->getTimeRemaining())
                    {
                        swap(pcb,temp);
                        outFile << pcb->getProcessName() << " running" << endl;
                    }
                    if (ready.empty())
                    {
                        ready.push(temp);
                        outFile << temp->getProcessName() << " moved to ready queue" << endl;
                    }
                    else
                    {
                        int queueSize = ready.size();
                        for (int i = 0; i < queueSize; i++)
                        {
                            PCB *hold = ready.front();
                            ready.pop();
                            if (temp->getTimeRemaining() < hold->getTimeRemaining() && placed == 0)
                            {
                                ready.push(temp);
                                outFile << temp->getProcessName() << " moved to ready queue" << endl;
                                placed = 1;
                            }
                            ready.push(hold);
                        }
                        if (placed == 0)
                        {
                            ready.push(temp);
                            outFile << temp->getProcessName() << " moved to ready queue" << endl;
                        }
                    }
                }
            }
            time = time + pcb->getTimeRemaining();
            turnaround = turnaround + time - pcb->getArrivalTime();
            outFile << pcb->getProcessName() << " completed" << endl;
            delete pcb;
            while (!ready.empty())
            {
                PCB *pcb = ready.front();
                time = time + pcb->getTimeRemaining();
                turnaround = turnaround + time - pcb->getArrivalTime();
                outFile << pcb->getProcessName() << " completed" << endl;
                ready.pop();
                delete pcb;
            }
            outFile << endl << "Time to completion: " << time << endl;
            outFile << "Average Turnaround Time : " << (turnaround/counter) << endl;
            outFile.close();
            cout << "STCF Complete" << endl;
            cout << "Press Enter" << endl;
            cin.get();
            cin.clear();
            cin.ignore(10000, '\n');
        }
        else
        {
            cout << "Nothing to schedule" << endl;
            cout << "Press Enter" << endl;
            cin.get();
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
    void FPPS()
    {
        cout << string(100, '\n');
        cout << "Fixed Priority Pre-Emptive Scheduling" << endl;
        readFile();
        if (!schedule.empty())
        {
            ofstream outFile;
            outFile.open ("FPPS.txt");
            PCB *pcb = schedule[0];
            outFile << pcb->getProcessName() << " arrived" << endl;
            outFile << pcb->getProcessName() << " running" << endl;
            schedule.erase(schedule.begin());
            int time = pcb->getArrivalTime();
            int turnaround = 0;
            int counter = 1;
            while (!schedule.empty())
            {
                PCB *temp = schedule[0];
                schedule.erase(schedule.begin());
                pcb->setTimeRemaining(pcb->getTimeRemaining() - (temp->getArrivalTime() - time));
                time = temp->getArrivalTime();
                counter ++;
                while (pcb != NULL && pcb->getTimeRemaining() <= 0)
                {
                    outFile << pcb->getProcessName() << " completed" << endl;
                    turnaround = turnaround + (time + pcb->getTimeRemaining() - pcb->getArrivalTime());
                    PCB *hold = NULL;
                    if (!ready.empty())
                    {
                        hold = ready.front();
                        ready.pop();
                        outFile << hold->getProcessName() << " running" << endl;
                        hold->setTimeRemaining(hold->getTimeRemaining() + pcb->getTimeRemaining());
                    }
                    delete pcb;
                    pcb = hold;
                }
                if (pcb == NULL)
                {
                    pcb = temp;
                    outFile << temp->getProcessName() << " arrived" << endl;
                    outFile << temp->getProcessName() << " running" << endl;
                }
                else
                {
                    int placed = 0;
                    outFile << temp->getProcessName() << " arrived" << endl;
                    if (pcb->getPriority() < temp->getPriority())
                    {
                        swap(pcb,temp);
                        outFile << pcb->getProcessName() << " running" << endl;
                    }
                    if (ready.empty())
                    {
                        ready.push(temp);
                        outFile << temp->getProcessName() << " moved to ready queue" << endl;
                    }
                    else
                    {
                        int queueSize = ready.size();
                        for (int i = 0; i < queueSize; i++)
                        {
                            PCB *hold = ready.front();
                            ready.pop();
                            if (temp->getPriority() > hold->getPriority() && placed == 0)
                            {
                                ready.push(temp);
                                outFile << temp->getProcessName() << " moved to ready queue" << endl;
                                placed = 1;
                            }
                            ready.push(hold);
                        }
                        if (placed == 0)
                        {
                            ready.push(temp);
                            outFile << temp->getProcessName() << " moved to ready queue" << endl;
                        }
                    }
                }
            }
            time = time + pcb->getTimeRemaining();
            turnaround = turnaround + time - pcb->getArrivalTime();
            outFile << pcb->getProcessName() << " completed" << endl;
            delete pcb;
            while (!ready.empty())
            {
                PCB *pcb = ready.front();
                time = time + pcb->getTimeRemaining();
                turnaround = turnaround + time - pcb->getArrivalTime();
                outFile << pcb->getProcessName() << " completed" << endl;
                ready.pop();
                delete pcb;
            }
            outFile << endl << "Time to completion: " << time << endl;
            outFile << "Average Turnaround Time : " << (turnaround/counter) << endl;
            outFile.close();
            cout << "FPPS Complete" << endl;
            cout << "Press Enter" << endl;
            cin.get();
            cin.clear();
            cin.ignore(10000, '\n');
        }
        else
        {
            cout << "Nothing to schedule" << endl;
            cout << "Press Enter" << endl;
            cin.get();
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
    void RR()
    {
        cout << string(100, '\n');
        cout << "Round Robin" << endl;
        readFile();
        if (!schedule.empty())
        {
            int quantum;
            cout << "Enter Time Quantum: ";
            cin >> quantum;
            cout << endl;
            while (quantum <= 0)
            {
                cout << "Time quantum must be an integer greater than 0" << endl;
                cout << "Enter Time Quantum: ";
                cin.clear();
                cin.ignore(10000, '\n');
                cin >> quantum;
                cout << endl;
            }
            ofstream outFile;
            outFile.open ("RR.txt");
            PCB *pcb = schedule[0];
            schedule.erase(schedule.begin());
            outFile << pcb->getProcessName() << " arrived" << endl;
            outFile << pcb->getProcessName() << " running" << endl;
            int time = pcb->getArrivalTime();
            int turnaround = 0;
            int counter = 1;
            while(!schedule.empty() || !ready.empty())
            {
                pcb->setTimeRemaining(pcb->getTimeRemaining() - quantum);
                if (pcb->getTimeRemaining() <= 0)
                {
                    time = time + quantum + pcb->getTimeRemaining();
                    outFile << pcb->getProcessName() << " completed" << endl;
                    turnaround = turnaround + (time + pcb->getTimeRemaining() - pcb->getArrivalTime());
                    delete pcb;
                    pcb = NULL;
                }
                else
                {
                    time = time + quantum;
                }
                if (!schedule.empty())
                {
                   PCB *temp = schedule[0];
                   while (!schedule.empty() && temp->getArrivalTime() <= time)
                   {
                       ready.push(temp);
                       schedule.erase(schedule.begin());
                       counter++;
                       outFile << temp->getProcessName() << " arrived" << endl;
                       outFile << temp->getProcessName() << " moved to ready queue" << endl;
                       if (!schedule.empty())
                       {
                           temp = schedule[0];
                       }
                   }
                }
                if (!ready.empty())
                {
                    if (pcb != NULL)
                    {
                        ready.push(pcb);
                        outFile << pcb->getProcessName() << " moved to ready queue" << endl;
                    }
                    pcb = ready.front();
                    ready.pop();
                    outFile << pcb->getProcessName() << " running" << endl;
                }
                else
                {
                    if (pcb == NULL)
                    {
                        if (!schedule.empty())
                        {
                            pcb = schedule[0];
                            schedule.erase(schedule.begin());
                            time = pcb->getArrivalTime();
                            outFile << pcb->getProcessName() << " arrived" << endl;
                            counter++;
                            outFile << pcb->getProcessName() << " running" << endl;
                        }
                    }
                }
            }
            outFile << pcb->getProcessName() << " completed" << endl;
            time = time + pcb->getTimeRemaining();
            turnaround = turnaround + time - pcb->getArrivalTime();
            outFile << endl << "Time to completion: " << time << endl;
            outFile << "Average Turnaround Time : " << (turnaround/counter) << endl;
            outFile.close();
            cout << "RR Complete" << endl;
            cout << "Press Enter" << endl;
            cin.get();
            cin.clear();
            cin.ignore(10000, '\n');
        }
        else
        {
            cout << "Nothing to schedule" << endl;
            cout << "Press Enter" << endl;
            cin.get();
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
    void MLFQ()
    {
        cout << string(100, '\n');
        cout << "Multilevel Feedback Queue" << endl;
        readFile();
        if (!schedule.empty())
        {
            int numberQueues;
            cout << "Enter Number of Queues: ";
            cin >> numberQueues;
            cout << endl;
            while (numberQueues <= 0)
            {
                cout << "Number of queues must be an integer greater than 0" << endl;
                cout << "Enter Number of Queues: ";
                cin.clear();
                cin.ignore(10000, '\n');
                cin >> numberQueues;
                cout << endl;
            }
            int timeAllotment[numberQueues];
            for (int i = 0; i < numberQueues; i++)
            {
                cout << "Enter Time Allotment for queue " << i+1 << " : ";
                cin >> timeAllotment[i];
                cout << endl;
                while (timeAllotment[i] <= 0)
                {
                    cout << "Time Allotment must be an integer greater than 0" << endl;
                    cout << "Enter Time Allotment for queue " << i+1 << " : ";
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cin >> timeAllotment[i];
                    cout << endl;
                }
            }
            int resetCycles;
            cout << "Enter Number of Cycles Until Queue Reset: ";
            cin >> resetCycles;
            cout << endl;
            while (resetCycles <= 0)
            {
                cout << "Number of cycles must be an integer greater than 0" << endl;
                cout << "Enter Number of Cycles Until Queue Reset: ";
                cin.clear();
                cin.ignore(10000, '\n');
                cin >> resetCycles;
                cout << endl;
            }
            ofstream outFile;
            outFile.open ("MLFQ.txt");

            PCB *pcb = schedule[0];
            schedule.erase(schedule.begin());
            pcb->setPriority(0);
            outFile << pcb->getProcessName() << " arrived" << endl;
            outFile << pcb->getProcessName() << " running" << endl;
            int time = pcb->getArrivalTime();
            int turnaround = 0;
            int counter = 1;
            int currentQueue = 0;
            int cycles = 0;
            while(!schedule.empty() || !ready.empty())
            {
                pcb->setTimeRemaining(pcb->getTimeRemaining() - timeAllotment[currentQueue]);
                if (pcb->getTimeRemaining() <= 0)
                {
                    time = time + timeAllotment[currentQueue] + pcb->getTimeRemaining();
                    outFile << pcb->getProcessName() << " completed" << endl;
                    turnaround = turnaround + (time + pcb->getTimeRemaining() - pcb->getArrivalTime());
                    delete pcb;
                    pcb = NULL;
                }
                else
                {
                    time = time + timeAllotment[currentQueue];
                    if((currentQueue + 1) <= (numberQueues - 1))
                    {
                       pcb->setPriority(currentQueue + 1);
                    }
                }

                if (!schedule.empty())
                {
                   PCB *temp = schedule[0];
                   while (!schedule.empty() && temp->getArrivalTime() <= time)
                   {
                       ready.push(temp);
                       schedule.erase(schedule.begin());
                       counter++;
                       outFile << temp->getProcessName() << " arrived" << endl;
                       outFile << temp->getProcessName() << " moved to ready queue 1" << endl; //Queue 1 == priority 0
                       temp->setPriority(0);
                       if (!schedule.empty())
                       {
                           temp = schedule[0];
                       }
                   }
                }
                cycles++;
                if (cycles == resetCycles)
                {
                    if (!ready.empty())
                    {
                        for (int i = 0; i < ready.size();i++)
                        {
                            PCB *temp = ready.front();
                            ready.pop();
                            temp->setPriority(0);
                            ready.push(temp);
                            outFile << temp->getProcessName() << " moved to ready queue 1" << endl;
                        }
                    }

                    if (pcb != NULL)
                    {
                        pcb->setPriority(0);
                    }
                }
                if (!ready.empty())
                {
                    if (pcb != NULL)
                    {
                        ready.push(pcb);
                        outFile << pcb->getProcessName() << " moved to ready queue " << pcb->getPriority() + 1 <<  endl;
                    }
                    currentQueue = (-1);
                    int found = 0;
                    while(found == 0)
                    {
                        currentQueue++;
                        for (int i = 0; i < ready.size();i++)
                        {
                            PCB *temp = ready.front();
                            ready.pop();
                            if (temp->getPriority() == currentQueue && found == 0)
                            {
                                pcb = temp;
                                found = 1;
                            }
                            else
                            {
                                ready.push(temp);
                            }
                        }
                    }
                    outFile << pcb->getProcessName() << " running" << endl;
                }
                else
                {
                    if (pcb == NULL)
                    {
                        if (!schedule.empty())
                        {
                            pcb = schedule[0];
                            schedule.erase(schedule.begin());
                            time = pcb->getArrivalTime();
                            outFile << pcb->getProcessName() << " arrived" << endl;
                            counter++;
                            outFile << pcb->getProcessName() << " running" << endl;
                            currentQueue = 0;
                        }
                    }
                }
            }
            outFile << pcb->getProcessName() << " completed" << endl;
            time = time + pcb->getTimeRemaining();
            turnaround = turnaround + time - pcb->getArrivalTime();
            outFile << endl << "Time to completion: " << time << endl;
            outFile << "Average Turnaround Time : " << (turnaround/counter) << endl;
            outFile.close();
            cout << "MLFQ Complete" << endl;
            cout << "Press Enter" << endl;
            cin.get();
            cin.clear();
            cin.ignore(10000, '\n');
        }
        else
        {
            cout << "Nothing to schedule" << endl;
            cout << "Press Enter" << endl;
            cin.get();
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
    void LS()
    {
        cout << string(100, '\n');
        cout << "Lottery Scheduler" << endl;
        readFile();
        if (!schedule.empty())
        {
            int numberTickets;
            cout << "Enter Number of Tickets: ";
            cin >> numberTickets;
            cout << endl;
            while (numberTickets < 100)
            {
                cout << "Number of tickets must be an integer greater than 99" << endl;
                cout << "Enter Number of Tickets: ";
                cin.clear();
                cin.ignore(10000, '\n');
                cin >> numberTickets;
                cout << endl;
            }
            ofstream outFile;
            outFile.open ("LS.txt");
            srand (time(NULL));
            PCB *pcb = schedule[0];
            schedule.erase(schedule.begin());
            int cpuCurrentPercentage = pcb->getCPUPercentage();
            outFile << pcb->getProcessName() << " arrived" << endl;
            outFile << pcb->getProcessName() << " running" << endl;
            int time = pcb->getArrivalTime();
            int turnaround = 0;
            int counter = 1;
            int issuedTickets = 0;
            vector<int> tickets;
            vector<PCB*> ticketOwner;
            ticketOwner.push_back(pcb);
            int issuingTickets = ((numberTickets * pcb->getCPUPercentage()) / 100);
            tickets.push_back(issuingTickets);
            issuedTickets = issuedTickets + issuingTickets;
            while(!schedule.empty() || ticketOwner.size())
            {
                pcb->setTimeRemaining(pcb->getTimeRemaining() - 100);
                if (pcb->getTimeRemaining() <= 0)
                {
                    time = time + 100 + pcb->getTimeRemaining();
                    outFile << pcb->getProcessName() << " completed" << endl;
                    turnaround = turnaround + (time + pcb->getTimeRemaining() - pcb->getArrivalTime());
                    for (int i = 0; i < ticketOwner.size(); i++)
                    {
                        if (ticketOwner[i] == pcb)
                        {
                            issuedTickets = issuedTickets - tickets[i];
                            tickets.erase(tickets.begin()+ i);
                            ticketOwner.erase(ticketOwner.begin() + i);
                            cpuCurrentPercentage = cpuCurrentPercentage - pcb->getCPUPercentage();
                        }
                    }
                    delete pcb;
                    pcb = NULL;
                }
                else
                {
                    time = time + 100;
                }

                if (!schedule.empty())
                {
                   PCB *temp = schedule[0];
                   while (!schedule.empty() && temp->getArrivalTime() <= time && ((cpuCurrentPercentage + temp->getCPUPercentage()) < 100))
                   {
                       schedule.erase(schedule.begin());
                       counter++;
                       outFile << temp->getProcessName() << " arrived" << endl;
                       outFile << temp->getProcessName() << " moved to ready queue" << endl;
                       ticketOwner.push_back(temp);
                       int issuingTickets = ((numberTickets * temp->getCPUPercentage()) / 100);
                       tickets.push_back(issuingTickets);
                       issuedTickets = issuedTickets + issuingTickets;
                       if (!schedule.empty())
                       {
                           temp = schedule[0];
                       }
                   }
                }
                if (!ticketOwner.empty())
                {
                    if (pcb != NULL)
                    {
                        outFile << pcb->getProcessName() << " moved to ready queue" << endl;
                    }
                    int draw = rand() % issuedTickets + 1;
                    int found = 0;
                    for (int i = 0; i < tickets.size(); i++)
                    {
                        draw = draw - tickets[i];
                        if (draw <= 0 && found == 0)
                        {
                            pcb = ticketOwner[i];
                            found = 1;
                        }
                    }
                    outFile << pcb->getProcessName() << " running" << endl;
                }
                else
                {
                    if (pcb == NULL)
                    {
                        if (!schedule.empty())
                        {
                            pcb = schedule[0];
                            schedule.erase(schedule.begin());
                            time = pcb->getArrivalTime();
                            outFile << pcb->getProcessName() << " arrived" << endl;
                            counter++;
                            outFile << pcb->getProcessName() << " running" << endl;
                            ticketOwner.push_back(pcb);
                            int issuingTickets = ((numberTickets * pcb->getCPUPercentage()) / 100);
                            tickets.push_back(issuingTickets);
                            issuedTickets = issuedTickets + issuingTickets;
                        }
                    }
                }
            }
            outFile << endl << "Time to completion: " << time << endl;
            outFile << "Average Turnaround Time : " << (turnaround/counter) << endl;
            outFile.close();
            cout << "LS Complete" << endl;
            cout << "Press Enter" << endl;
            cin.get();
            cin.clear();
            cin.ignore(10000, '\n');
        }
        else
        {
            cout << "Nothing to schedule" << endl;
            cout << "Press Enter" << endl;
            cin.get();
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
private:
    queue<PCB*> ready;
    queue<PCB*> blocked;
    queue<PCB*> suspendedReady;
    queue<PCB*> suspendedBlocked;
    vector<PCB*> schedule;

};



class Zaren
{
public:
    void start()
    {
        test = 0;
        cout << string(100, '\n'); //http://www.cplusplus.com/forum/beginner/3304/
        cout << "Welcome to Zaren" << endl;
        cout << "Press enter to get started";
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
    void OSTest2() //PCB Tests
    {
        PCB *a = pcbControl.SetupPCB("a",12,1,0,0,0,0);
        PCB *b = pcbControl.SetupPCB("b",-12,2,0,0,0,0);
        PCB *c = pcbControl.SetupPCB("c",32,1,0,0,0,0);
        PCB *d = pcbControl.SetupPCB("d",34,1,0,0,0,0);
        PCB *e = pcbControl.SetupPCB("e",12,2,0,0,0,0);
        PCB *f = pcbControl.SetupPCB("f",16,1,0,0,0,0);
        PCB *g = pcbControl.SetupPCB("g",12,1,0,0,0,0);

        b->setState(3);
        c->setState(4);
        d->setState(5);
        e->setState(3);
        g->setState(5);

        pcbControl.InsertPCB(a);
        pcbControl.InsertPCB(b);
        pcbControl.InsertPCB(c);
        pcbControl.InsertPCB(d);
        pcbControl.InsertPCB(e);
        pcbControl.InsertPCB(f);

        pcbControl.showQueue(5);

        pcbControl.RemovePCB(a);
        pcbControl.RemovePCB(b);
        pcbControl.RemovePCB(c);
        pcbControl.RemovePCB(d);
        pcbControl.RemovePCB(e);
        pcbControl.RemovePCB(f);

        pcbControl.FreePCB(a);
        pcbControl.FreePCB(b);
        pcbControl.FreePCB(c);
        pcbControl.FreePCB(d);
        pcbControl.FreePCB(e);
        pcbControl.FreePCB(f);

        pcbControl.showQueue(5);
    }
    void OSTest3()
    {
        PCB *a = pcbControl.SetupPCB("a",12,1,0,0,0,0);
        PCB *b = pcbControl.SetupPCB("b",-12,2,0,0,0,0);
        PCB *c = pcbControl.SetupPCB("c",32,1,0,0,0,0);
        PCB *d = pcbControl.SetupPCB("d",34,1,0,0,0,0);
        PCB *e = pcbControl.SetupPCB("e",12,2,0,0,0,0);
        PCB *f = pcbControl.SetupPCB("f",16,1,0,0,0,0);
        PCB *g = pcbControl.SetupPCB("g",12,1,0,0,0,0);

        pcbControl.InsertPCB(a);
        pcbControl.InsertPCB(b);
        pcbControl.InsertPCB(c);
        pcbControl.InsertPCB(d);
        pcbControl.InsertPCB(e);
        pcbControl.InsertPCB(f);

        suspend();
        resume();
        setPriority();
        showPCB();
        showALL();
        showReady();
        showBlocked();
    }
    void OSTest4()//Schedule tests
    {
        //pcbControl.SJF();
        //pcbControl.FIFO();
        //pcbControl.STCF();
        //pcbControl.FPPS();
        //pcbControl.RR();
        //pcbControl.MLFQ();
        pcbControl.LS();
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
        cout << "4 - PCB Controls" << endl;
        cout << "5 - Exit" << endl;
        cout << "6 - Help" << endl;
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
            PCBUserControls();
        }
        else if (command == 5)
        {
            exit();
        }
        else if (command == 6)
        {
            help();
        }
        else
        {
            cout << "Not valid command" << endl;
            cout << "Press enter" << endl;
            cin.get();
            menu();
        }

    }
    void version ()
    {
        cout << string(100, '\n');
        cout << "Zaren Version 1.2" << endl;
        cout << "Press enter to return to menu";
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

        cout << "Press enter to return to menu";
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
        cout << "Press enter to return to menu";
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
    void PCBUserControls()
    {
        cout << string(100, '\n');
        int command = 0;
        cout << "Command List" << endl;
        cout << "1  - Suspend" << endl;
        cout << "2  - Resume" << endl;
        cout << "3  - Set Priority" << endl;
        cout << "4  - Show PCB" << endl;
        cout << "5  - Show All" << endl;
        cout << "6  - Show Ready" << endl;
        cout << "7  - Show Blocked" << endl;
        cout << "8  - SJF" << endl;
        cout << "9  - FIFO" << endl;
        cout << "10 - STCF" << endl;
        cout << "11 - FPPS" << endl;
        cout << "12 - RR" << endl;
        cout << "13 - MLFQ" << endl;
        cout << "14 - LS" << endl;
        cout << "15 - Return to Menu" << endl;
        cout << "Enter the number of desired command: ";
        cin.clear();
        cin >> command;
        cin.clear();
        cin.ignore(10000, '\n');

        if (command == 1)
        {
            suspend();
        }
        else if (command == 2)
        {
            resume();
        }
        else if (command == 3)
        {
            setPriority();
        }
        else if (command == 4)
        {
            showPCB();
        }
        else if (command == 5)
        {
            showALL();
        }
        else if (command == 6)
        {
            showReady();
        }
        else if (command == 7)
        {
            showBlocked();
        }
        else if (command == 8)
        {
            pcbControl.SJF();
        }
        else if (command == 9)
        {
            pcbControl.FIFO();
        }
        else if (command == 10)
        {
            pcbControl.STCF();
        }
        else if (command == 11)
        {
            pcbControl.FPPS();
        }
        else if (command == 12)
        {
            pcbControl.RR();
        }
        else if (command == 13)
        {
            pcbControl.MLFQ();
        }
        else if (command == 14)
        {
            pcbControl.LS();
        }
        else if (command == 15)
        {
            menu();
        }
        else
        {
            cout << "Not valid command" << endl;
            cout << "Press enter" << endl;
            cin.get();
        }
        PCBUserControls();
    }
    void suspend()
    {
        cout << string(100, '\n');
        string pcbName;
        cout << "Suspend" << endl;
        cout << "Enter process name: ";
        cin.clear();
        cin >> pcbName;
        cin.clear();
        cin.ignore(10000, '\n');
        cout << endl;
        PCB *pcb = pcbControl.FindPCB(pcbName);
        if (pcb != NULL)
        {
            int state = pcb->getState();
            if(state == 2)
            {
                pcb->setState(4);
            }
            else if (state == 3)
            {
                pcb->setState(5);
            }
            if (state == 2 || state == 3)
            {
                pcbControl.RemovePCB(pcb);
                pcbControl.InsertPCB(pcb);
                cout << "PCB suspended" << endl;
            }
            else if (state == 4 || state == 5)
            {
                cout << "PCB is already suspended" << endl;
            }
        }
        else
        {
            cout << "PCB not found" << endl;
        }
        cout << "Press Enter" << endl;
        cin.get();
        return;
    }
    void resume()
    {
        cout << string(100, '\n');
        string pcbName;
        cout << "Resume" << endl;
        cout << "Enter process name: ";
        cin.clear();
        cin >> pcbName;
        cin.clear();
        cin.ignore(10000, '\n');
        cout << endl;
        PCB *pcb = pcbControl.FindPCB(pcbName);
        if (pcb != NULL)
        {
            int state = pcb->getState();
            if(state == 4)
            {
                pcb->setState(2);
            }
            else if (state == 5)
            {
                pcb->setState(3);
            }
            if (state == 4 || state == 5)
            {
                pcbControl.RemovePCB(pcb);
                pcbControl.InsertPCB(pcb);
                cout << "PCB resumed" << endl;
            }
            else if (state == 2 || state == 3)
            {
                cout << "PCB is not suspended" << endl;
            }
        }
        else
        {
            cout << "PCB not found" << endl;
        }
        cout << "Press Enter" << endl;
        cin.get();
        return;
    }
    void setPriority()
    {
        cout << string(100, '\n');
        string pcbName;
        int pcbPriority;
        cout << "Set Priority" << endl;
        cout << "Enter process name: ";
        cin.clear();
        cin >> pcbName;
        cin.clear();
        cin.ignore(10000, '\n');
        cout << endl << "Enter new priority, integer between -127 and 128: ";
        cin.clear();
        cin >> pcbPriority;
        cin.clear();
        cin.ignore(10000, '\n');
        cout << endl;

        PCB *pcb = pcbControl.FindPCB(pcbName);
        if (pcb != NULL)
        {
            if (pcbPriority < -127 || pcbPriority > 128)
            {
                cout << "Priority needs to be an integer between -127 and 128" <<endl;
            }
            else
            {
                pcb->setPriority(pcbPriority);
                cout << "New priority set" << endl;
            }
        }
        else
        {
            cout << "PCB not found" << endl;
        }
        cout << "Press Enter" << endl;
        cin.get();
        return;

    }
    void showPCB()
    {
        cout << string(100, '\n');
        string pcbName;
        cout << "Show PCB" << endl;
        cout << "Enter process name: ";
        cin.clear();
        cin >> pcbName;
        cin.clear();
        cin.ignore(10000, '\n');
        cout << endl;
        PCB *pcb = pcbControl.FindPCB(pcbName);
        if (pcb != NULL)
        {
            cout << "PCB" << endl;
            cout << "Name: " << pcb->getProcessName()<< endl;
            int processClass = pcb->getProcessClass();
            if (processClass ==  1)
            {
                cout << "Class: Application" << endl;
            }
            else if (processClass == 2)
            {
                cout << "Class: System" << endl;
            }
            cout << "Priority: " << pcb->getPriority() <<endl;
            int state = pcb->getState();
            if(state == 1)
            {
                cout << "State: Running" << endl;
            }
            else if (state == 2)
            {
                cout << "State: Ready" << endl;
            }
            else if (state == 3)
            {
                cout << "State: Blocked" << endl;
            }
            else if (state == 4)
            {
                cout << "State: Suspended Ready" << endl;
            }
            else if (state == 5)
            {
                cout << "State: Suspended Blocked" << endl;
            }
            cout << "Memory: " << pcb->getMemory() << endl;
        }
        else
        {
            cout << "PCB not found" << endl;
        }
        cout << "Press Enter" << endl;
        cin.get();
        return;
    }
    void showALL()
    {
        cout << string(100, '\n');
        pcbControl.showQueue(5);
        cout << "Press Enter" << endl;
        cin.get();
        return;
    }
    void showReady()
    {
        cout << string(100, '\n');
        pcbControl.showQueue(1);
        cout << "Press Enter" << endl;
        cin.get();
        return;
    }
    void showBlocked()
    {
        cout << string(100, '\n');
        pcbControl.showQueue(2);
        cout << "Press Enter" << endl;
        cin.get();
        return;
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
            cout << "Press enter to Exit";
            cin.get();
            return;
        }
        else if(command == "Y" || command == "y")
        {
            return;
        }
        else if (command == "N" || command == "n")
        {
            cout << "Press enter to return to menu";
            cin.get();
            menu();
        }
        else
        {
            cout << "Not valid command" << endl;
            cout << "Press enter" << endl;
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
        cout << "Press enter to continue to PCB Controls";
        cin.get();
        cout << string(100, '\n');
        cout << "Help" << endl;
        cout << "PCB - Process Control Block" << endl;
        cout << "Suspend - Take name from user. It then suspends the PCB with given name." << endl;
        cout << "Resume - Take name from user. It then resumes the PCB with given name." << endl;
        cout << "Set Priority - Takes name and priority from user. It then changes the PCB with given name to given priority." << endl;
        cout << "Show PCB - Takes name from user. It then displays the information of PCB with given name." << endl;
        cout << "Show All - Shows all PCB from all queues." << endl;
        cout << "Show Ready - Shows all PCB from ready queue." << endl;
        cout << "Show Blocked - Shows all PCB from blocked queue." << endl;
        cout << "Press enter to return to menu.";
        cin.get();
    }
    int test;
    PCBControl pcbControl;
};

int main()
{
    Zaren OS;
    //OS.OSTest(); //Test Functions
    //OS.OSTest2(); //Automatic PCB Test
    //OS.OSTest3(); //User PCB Controls Test
    OS.OSTest4(); //Schedulers Test
    //OS.start();
}
