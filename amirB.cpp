// Amir Bakhtar
// Main menu 
// CMPS3350 Project

#include <iostream>
#include <cstdlib>
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////

int option; 

int main()
{
    cout << "Main Menu" << endl;
    cout << "Enter a number to begin" << endl;
    cout << "1 - Start" << endl;
    cout << "2 - Settings" << endl;
    cout << "3 - Quit" << endl;

    cin >> option;
    
    if (option == 1)
    {
        system ("clear");
        cout << "Redirect to start of game" << endl;
        return 0;
    }
    if (option == 2)
    {
        system ("clear");
        cout << "Redirect to Settings menu" << endl;
        return 0;
    }
    if (option == 3)
    {
        return 0;
    }
    else
    {
        cout << "Please input a valid number" << endl;
        return 0;
    }
    return 0;
}
