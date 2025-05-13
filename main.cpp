#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <fstream>
#include"strongled.h"
using namespace std;

// [Classes remain unchanged — omitted here for brevity. Assume all classes are same as in user-provided code.]

int main()
{
    srand(time(0));
    Kingdom player1, player2;
    int currentPlayer = 1;
    int choice;

    while (true)
    {
        Kingdom& kingdom = (currentPlayer == 1) ? player1 : player2;

        if (kingdom.isGameOver() || kingdom.isWinner()) break;

        cout << "\n===== Player " << currentPlayer << "'s Turn =====\n";
        cout << "1. View Kingdom Status\n";
        cout << "2. Train Army\n";
        cout << "3. Take Loan\n";
        cout << "4. Trade Resources\n";
        cout << "5. Build Barrack\n";
        cout << "6. Build Blacksmith\n";
        cout << "7. Conquer Council\n";
        cout << "8. Recruit Troops\n";
        cout << "9. Progress Day\n";
        cout << "10. Save Game\n";
        cout << "11. Load Game\n";
        cout << "12. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1: kingdom.displayStatus(); break;
        case 2:
            int train;
            cout << "Enter troops to train: ";
            cin >> train;
            kingdom.recruitTroops(train);
            break;
        case 3:
            int loan;
            cout << "Enter loan amount: ";
            cin >> loan;
            kingdom.takeLoan(loan);
            break;
        case 4:
            int resourceChoice, amount;
            cout << "1. Food\n2. Wood\n3. Stone\n4. Iron\nEnter resource to trade: ";
            cin >> resourceChoice;
            cout << "Enter amount: ";
            cin >> amount;
            switch (resourceChoice)
            {
            case 1: kingdom.res->modifyResource(kingdom.res->food, -amount); break;
            case 2: kingdom.res->modifyResource(kingdom.res->wood, -amount); break;
            case 3: kingdom.res->modifyResource(kingdom.res->stone, -amount); break;
            case 4: kingdom.res->modifyResource(kingdom.res->iron, -amount); break;
            default: cout << "Invalid resource.\n";
            }
            break;
        case 5: kingdom.army->buildBarracks(kingdom.res->gold); break;
        case 6: kingdom.army->buildBlacksmith(kingdom.res->gold); break;
        case 7: kingdom.conquerCouncil(); break;
        case 8:
            int troops;
            cout << "Enter number of troops: ";
            cin >> troops;
            kingdom.recruitTroops(troops);
            break;
        case 9: kingdom.progressDay(); break;
        case 10:
            if (currentPlayer == 1)
                kingdom.saveGame("savegame1.txt");
            else
                kingdom.saveGame("savegame2.txt");
            break;
        case 11:
            if (currentPlayer == 1)
                kingdom.loadGame("savegame1.txt");
            else
                kingdom.loadGame("savegame2.txt");
            break;
        case 12: return 0;
        default: cout << "Invalid choice.\n";
        }

        // Switch player
        currentPlayer = (currentPlayer == 1) ? 2 : 1;
    }

    cout << "\nGame Over!\n";
    if (player1.isWinner())
        cout << "Player 1 wins by conquering councils!\n";
    else if (player2.isWinner())
        cout << "Player 2 wins by conquering councils!\n";
    else if (player1.isGameOver())
        cout << "Player 1 lost the game.\n";
    else if (player2.isGameOver())
        cout << "Player 2 lost the game.\n";

    return 0;
}
