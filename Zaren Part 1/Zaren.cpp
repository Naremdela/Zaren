/*
Author Tyler Thaxton
Zaren OS
*/
#include <iostream>
#include <ctime>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

class Zaren
{
public:
    void start()
    {
        test = 0;
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
        cout << "Zaren Version 1.0" << endl;
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
