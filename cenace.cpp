#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>

#define INITIAL_POINT "2"
#define MAX_POINT 300
#define MIN_POINT 1
#define WIN_REWORD 3
#define DRAW_REWORD 1
#define PUNISHMENT 1

using namespace std;

int  player;
char player_X, player_O;
int  win;
char player_move, mark;
int  O_move_count;
char O_paths[5][50] = {'\0'}; //this will be a stack for storing computer's moves.
char gameboard[10]; //this is a c string. 10th one is for null char.
char auto_train;
int  auto_train_count, total_training_match;
int  exit_CENACE;
int  match, CENACEscore, humanScore;

void choose_player();
void inputmove();
void newboard();
void printboard();
int  checkwin();
int  space_in_board();
void updateLearning_data();
void menu();
void CENACE_intro();
void updateGraphing_data();
void scoreUpdate();

int main()
{
    system("title CENACE");
    system("COLOR 0B");
    system("cls");
    CENACE_intro();
    menu();
    if(exit_CENACE)
    {
        system("cls");
        Sleep(250);
        return 0;
    }
    system("mkdir Learning_data");
    system("mkdir Graphing_data");

start:
    auto_train_count = 0;
    O_move_count = 0;
    match = 1;
    CENACEscore = 0;
    humanScore = 0;

    while(true)
    {
        player = 1;
        newboard();
        choose_player();
        do
        {
            if(player % 2)
            {
                player = 1;
                mark = 'O';
            }
            else
            {
                player = 2;
                mark = 'X';
            }
            system("cls");
            printboard();
            inputmove();

            if(player_move == '1' && gameboard[0] == ' ')
            {
                gameboard[0] = mark;
            }
            else if(player_move == '2' && gameboard[1] == ' ')
            {
                gameboard[1] = mark;
            }
            else if(player_move == '3' && gameboard[2] == ' ')
            {
                gameboard[2] = mark;
            }
            else if(player_move == '4' && gameboard[3] == ' ')
            {
                gameboard[3] = mark;
            }
            else if(player_move == '5' && gameboard[4] == ' ')
            {
                gameboard[4] = mark;
            }
            else if(player_move == '6' && gameboard[5] == ' ')
            {
                gameboard[5] = mark;
            }
            else if(player_move == '7' && gameboard[6] == ' ')
            {
                gameboard[6] = mark;
            }
            else if(player_move == '8' && gameboard[7] == ' ')
            {
                gameboard[7] = mark;
            }
            else if(player_move == '9' && gameboard[8] == ' ')
            {
                gameboard[8] = mark;
            }
            else
            {
                cout << "\n\tINVALID MOVE!";
                Beep(800, 500);
                fflush(stdin);
                player --;
            }

            win = checkwin();
            if(win == 1)
            {
                scoreUpdate();
                system("cls");
                printboard();
                if(player == 1)
                {
                    cout << "\n\n\t>> Player-O won! <<";
                    if(auto_train != 'y' && auto_train != 'Y')
                    {
                        Beep(500,300);
                        Beep(500,300);
                        Beep(700,500);
                    }
                }
                else
                {
                    cout << "\n\n\t>> Player-X won! <<";
                    if(auto_train != 'y' && auto_train != 'Y')
                    {
                        Beep(700,300);
                        Beep(700,300);
                        Beep(500,500);
                    }
                }

                updateLearning_data();
                updateGraphing_data();
            }
            else if(win == -1)
            {
                scoreUpdate();
                system("cls");
                printboard();
                cout << "\n\n\t>> Draw! <<";
                if(auto_train != 'y' && auto_train != 'Y')
                {
                    Beep(400,250);
                    Beep(500,250);
                }

                updateLearning_data();
                updateGraphing_data();
            }

            player ++;

        }
        while(win == 0);

        //Printing all paths
        cout << "\nFile paths: ";
        for(int i = 0; i < O_move_count; i++)
        {
            cout << "\n   %s", O_paths[i];
        }

        //Asking user if he/she wants to play again
        if(auto_train != 'y' && auto_train != 'Y')
        {
            char ask;
            cout << "\n\n\tPlay again? (y/n) --> ";
            while(1)
            {
                ask = getch();
                cout << ask;
                if(ask == 'y' || ask == 'Y')
                {
                    O_move_count = 0;
                    break;
                }
                else if(ask == 'n' || ask == 'N')
                {
                    system("cls");
                    O_move_count = 0;
                    menu();
                    if(exit_CENACE)
                    {
                        system("cls");
                        Sleep(250);
                        return 0;
                    }
                    else goto start;
                }
                else if(ask == 8)
                {
                    cout << " \a";
                }
                else if(ask == 13)
                {
                    cout << "\t                      ";
                    cout << "\n\n\tPlay again? (y/n) --> ";
                }
                else
                {
                    cout << "\b \b\a";
                }
            }
        }

        if(auto_train == 'y' || auto_train == 'Y')
        {
            auto_train_count ++;
            O_move_count = 0;
            if(auto_train_count == total_training_match)
            {
                cout << "\n\n\tTraining complete.";
                cout << "\n\tPress any key to continue. ";
                getch();
                auto_train = 'n';
                system("cls");
                menu();
                if(exit_CENACE)
                {
                    system("cls");
                    Sleep(250);
                    return 0;
                }
                else goto start;
            }
        }
        match ++;
    }
}

void choose_player()
{
    //for simplicity, players are fixed.
    player_O = '2'; //2 = Computer(CENACE)
    player_X = '1'; //1 = Human
}


// This function takes input from both players
void inputmove()
{
    //Input from human or auto input move while in auto train
    if(player == 2)
    {
        if(auto_train != 'y' && auto_train != 'Y')
        {
            cout << "\n\n\tEnter your move (1-9): ";
            player_move = getch();
        }

        //For auto train. No need to touch this one.
        // Not the most efficient way, but it works!
        else
        {
            char anti_mark;
            if(mark == 'X') anti_mark = 'O';
            else anti_mark = 'X';

            if     (gameboard[0] == mark && gameboard[1] == mark && gameboard[2] == ' ') player_move = '3';
            else if(gameboard[1] == mark && gameboard[2] == mark && gameboard[0] == ' ') player_move = '1';
            else if(gameboard[0] == mark && gameboard[2] == mark && gameboard[1] == ' ') player_move = '2';

            else if(gameboard[3] == mark && gameboard[4] == mark && gameboard[5] == ' ') player_move = '6';
            else if(gameboard[4] == mark && gameboard[5] == mark && gameboard[3] == ' ') player_move = '4';
            else if(gameboard[3] == mark && gameboard[5] == mark && gameboard[4] == ' ') player_move = '5';

            else if(gameboard[6] == mark && gameboard[7] == mark && gameboard[8] == ' ') player_move = '9';
            else if(gameboard[7] == mark && gameboard[8] == mark && gameboard[6] == ' ') player_move = '7';
            else if(gameboard[6] == mark && gameboard[8] == mark && gameboard[7] == ' ') player_move = '8';

            else if(gameboard[0] == mark && gameboard[3] == mark && gameboard[6] == ' ') player_move = '7';
            else if(gameboard[3] == mark && gameboard[7] == mark && gameboard[0] == ' ') player_move = '1';
            else if(gameboard[0] == mark && gameboard[6] == mark && gameboard[3] == ' ') player_move = '4';

            else if(gameboard[1] == mark && gameboard[4] == mark && gameboard[7] == ' ') player_move = '8';
            else if(gameboard[4] == mark && gameboard[7] == mark && gameboard[1] == ' ') player_move = '2';
            else if(gameboard[1] == mark && gameboard[7] == mark && gameboard[4] == ' ') player_move = '5';

            else if(gameboard[2] == mark && gameboard[5] == mark && gameboard[8] == ' ') player_move = '9';
            else if(gameboard[5] == mark && gameboard[8] == mark && gameboard[2] == ' ') player_move = '3';
            else if(gameboard[2] == mark && gameboard[8] == mark && gameboard[5] == ' ') player_move = '6';

            else if(gameboard[0] == mark && gameboard[4] == mark && gameboard[8] == ' ') player_move = '9';
            else if(gameboard[4] == mark && gameboard[8] == mark && gameboard[0] == ' ') player_move = '1';
            else if(gameboard[0] == mark && gameboard[8] == mark && gameboard[4] == ' ') player_move = '5';

            else if(gameboard[2] == mark && gameboard[4] == mark && gameboard[6] == ' ') player_move = '7';
            else if(gameboard[4] == mark && gameboard[6] == mark && gameboard[2] == ' ') player_move = '3';
            else if(gameboard[2] == mark && gameboard[6] == mark && gameboard[4] == ' ') player_move = '5';


            else if(gameboard[0] == anti_mark && gameboard[1] == anti_mark && gameboard[2] == ' ') player_move = '3';
            else if(gameboard[1] == anti_mark && gameboard[2] == anti_mark && gameboard[0] == ' ') player_move = '1';
            else if(gameboard[0] == anti_mark && gameboard[2] == anti_mark && gameboard[1] == ' ') player_move = '2';

            else if(gameboard[3] == anti_mark && gameboard[4] == anti_mark && gameboard[5] == ' ') player_move = '6';
            else if(gameboard[4] == anti_mark && gameboard[5] == anti_mark && gameboard[3] == ' ') player_move = '4';
            else if(gameboard[3] == anti_mark && gameboard[5] == anti_mark && gameboard[4] == ' ') player_move = '5';

            else if(gameboard[6] == anti_mark && gameboard[7] == anti_mark && gameboard[8] == ' ') player_move = '9';
            else if(gameboard[7] == anti_mark && gameboard[8] == anti_mark && gameboard[6] == ' ') player_move = '7';
            else if(gameboard[6] == anti_mark && gameboard[8] == anti_mark && gameboard[7] == ' ') player_move = '8';

            else if(gameboard[0] == anti_mark && gameboard[3] == anti_mark && gameboard[6] == ' ') player_move = '7';
            else if(gameboard[3] == anti_mark && gameboard[7] == anti_mark && gameboard[0] == ' ') player_move = '1';
            else if(gameboard[0] == anti_mark && gameboard[6] == anti_mark && gameboard[3] == ' ') player_move = '4';

            else if(gameboard[1] == anti_mark && gameboard[4] == anti_mark && gameboard[7] == ' ') player_move = '8';
            else if(gameboard[4] == anti_mark && gameboard[7] == anti_mark && gameboard[1] == ' ') player_move = '2';
            else if(gameboard[1] == anti_mark && gameboard[7] == anti_mark && gameboard[4] == ' ') player_move = '5';

            else if(gameboard[2] == anti_mark && gameboard[5] == anti_mark && gameboard[8] == ' ') player_move = '9';
            else if(gameboard[5] == anti_mark && gameboard[8] == anti_mark && gameboard[2] == ' ') player_move = '3';
            else if(gameboard[2] == anti_mark && gameboard[8] == anti_mark && gameboard[5] == ' ') player_move = '6';

            else if(gameboard[0] == anti_mark && gameboard[4] == anti_mark && gameboard[8] == ' ') player_move = '9';
            else if(gameboard[4] == anti_mark && gameboard[8] == anti_mark && gameboard[0] == ' ') player_move = '1';
            else if(gameboard[0] == anti_mark && gameboard[8] == anti_mark && gameboard[4] == ' ') player_move = '5';

            else if(gameboard[2] == anti_mark && gameboard[4] == anti_mark && gameboard[6] == ' ') player_move = '7';
            else if(gameboard[4] == anti_mark && gameboard[6] == anti_mark && gameboard[2] == ' ') player_move = '3';
            else if(gameboard[2] == anti_mark && gameboard[6] == anti_mark && gameboard[4] == ' ') player_move = '5';

            else
            {
                while(1)
                {
                    //Note that: 1. 'move' is a char.
                    //           2. adding 48 converts an int into a char.
                    player_move = (char)((rand() % 10) + 48 + 1);
                    if(gameboard[(int)(player_move - 48) - 1] == ' ')//Accepting computer's input only if there is a blank space
                    {
                        gameboard[9] = '\0'; //no use, just for safety
                        break;
                    }
                    else continue;
                }
            }
        }
    }

    //Input from computer
    else
    {
        // The code commented bellow is the original code from the C file. This is the main AI part.
        // All of our work is here.
        // Need to rewrite this part of code for CSV file and implement Stack, BST, Hash map here.
        // I am adding a temporary code to give random move for now

        while(true)
        {
            //Note that: 1. 'move' is a char.
            //           2. adding 48 converts an int into a char.
            player_move = (char)((rand() % 10) + 48 + 1);
            if(gameboard[(int)(player_move - 48) - 1] == ' ')//Accepting computer's input only if there is a blank space
            {
                gameboard[9] = '\0'; //no use, just for safety
                break;
            }
            else continue;
        }



//        FILE* file;
//        int  i, j = 0, k;
//        char current_board[10], cmd[50] = "md Learning_data\\";
//
//        char path[50] = "Learning_data\\";
//        char temp[2] = "i";
//        char point[4]; //This is a string to store the point read from file
//        int  point_list[space_in_board()];
//        int  point_pose[space_in_board()];
//        int  sum_of_available_move_point = 0;
//
//
//
//        //Creating folder inside learning data starts here
//
//        for(i = 0; i <= 8; i ++)
//        {
//            if(gameboard[i] == ' ')
//            {
//                current_board[i] = '-';
//            }
//            else
//            {
//                current_board[i] = gameboard[i];
//            }
//        }
//        current_board[9] = '\0';//end of string
//
//        strcat(cmd, current_board);
//        system(cmd);
//        //Creating folder inside learning data ends here
//
//        //Creating the .txt files inside "Learning_data\<FOLDER NAME>\...." starts here
//        for(i = 0; i < space_in_board(); i ++)
//        {
//            point_list[i] = 0;
//            point_pose[i] = 0;
//        }
//
//        for(i = 0; i < 9; i ++)
//        {
//            if(gameboard[i] == ' ')
//            {
//                strcat(path, current_board);
//                strcat(path, "\\");
//                temp[0] = (char)(i + 1 + 48);
//                strcat(path, temp);
//                strcat(path, ".txt");
//                file = fopen(path, "r");
//                    if(file == NULL)//If file doesn't exist then creat file
//                    {
//                        file = fopen(path, "w");
//                        fprintf(file, INITIAL_POINT);
//                        fclose(file);
//
//                        file = fopen(path, "r");
//                        fgets(point, 4, file);
//                        sum_of_available_move_point += atoi(point);
//                        point_list[j] = atoi(point);
//                        fclose(file);
//                    }
//                    else
//                    {
//                        fgets(point, 4, file);
//                        sum_of_available_move_point += atoi(point);
//                        point_list[j] = atoi(point);
//                        fclose(file);
//                    }
//                    point_pose[j] = i;
//                    j ++;
//                if(i < 8) strcpy(path, "Learning_data\\");
//            }
//        }
//        //Creating the .txt files inside "Learning_data\<FOLDER NAME>\...." ends here
//
//        //Taking input from learning data
//        int move_priority[sum_of_available_move_point];
//
//        for(i = 0, j = 0; i < space_in_board(); i ++)
//        {
//            for(k = 0; k < point_list[i]; k ++)
//            {
//                move_priority[j] = point_pose[i] + 1;
//                j ++;
//            }
//        }
//
//        cout << "\n\n\tEnter your move (1-9): ";
//        srand(time(NULL));
//
//        while(1)
//        {
//            //Note that: 1. 'move' is a char.
//            //           2. adding 48 converts an int into a char.
//            int move_pose = (rand() % sum_of_available_move_point);
//            player_move = (char)(move_priority[move_pose] + 48);
//            if(gameboard[(int)(player_move - 48) - 1] == ' ')//Accepting computer's input only if there is a blank space
//            {
//                gameboard[9] = '\0';
//                break;
//            }
//            else continue;
//        }
//        cout << player_move;
//
//        //final current-path for given move
//        char name_of_txt_file [2] = {player_move, '\0'};
//        strcpy(path, "Learning_data\\");
//        strcat(path, current_board);
//        strcat(path, "\\");
//        strcat(path, name_of_txt_file);
//        strcat(path, ".txt");
//
//        strcpy(O_paths[O_move_count], path);
//        O_move_count ++;
    }
}

void newboard()
{
    int i;
    gameboard[9] = '\0';
    for(i = 0; i < 9; i ++)
    {
        gameboard[i] = ' ';
    }
}

void printboard()
{
    //printf("\n\t\t 1|2|3");
    //printf("\n\t\t 4|5|6");
    //printf("\n\t\t 7|8|9\n");
    if(auto_train == 'y' || auto_train == 'Y')
    {
        printf("\n\t      Match:%4d", match);
        printf("\n      ___________________________");
        printf("\n     |              /            |");
        printf("\n     | CENACE:%4d vs HUMAN:%4d |", CENACEscore, humanScore);
        printf("\n     |_____________/_____________|");
    }

    else
    {
        printf("\n\t      Match:%4d", match);
        printf("\n      ___________________________");
        printf("\n     |              /            |");
        printf("\n     | CENACE:%4d vs HUMAN:%4d |", CENACEscore, humanScore);
        printf("\n     |_____________/_____________|");
    }

    if(player == 1)
    {
        printf("\n\n\t  Player-O, your turn\n");
    }
    else
    {
        printf("\n\n\t  Player-X, your turn\n");
    }

    printf("\n\t       |       |       ");
    printf("\n\t   %c   |   %c   |   %c   ", gameboard[0], gameboard[1], gameboard[2]);
    printf("\n\t_______|_______|_______");
    printf("\n\t       |       |       ");
    printf("\n\t   %c   |   %c   |   %c   ", gameboard[3], gameboard[4], gameboard[5]);
    printf("\n\t_______|_______|_______");
    printf("\n\t       |       |       ");
    printf("\n\t   %c   |   %c   |   %c   ", gameboard[6], gameboard[7], gameboard[8]);
    printf("\n\t       |       |       \n");
}

int checkwin() //1 = win, -1 = draw
{
    if     (gameboard[0] == 'X' && gameboard[1] == 'X' && gameboard[2] == 'X') return 1;
    else if(gameboard[3] == 'X' && gameboard[4] == 'X' && gameboard[5] == 'X') return 1;
    else if(gameboard[6] == 'X' && gameboard[7] == 'X' && gameboard[8] == 'X') return 1;
    else if(gameboard[0] == 'X' && gameboard[3] == 'X' && gameboard[6] == 'X') return 1;
    else if(gameboard[1] == 'X' && gameboard[4] == 'X' && gameboard[7] == 'X') return 1;
    else if(gameboard[2] == 'X' && gameboard[5] == 'X' && gameboard[8] == 'X') return 1;
    else if(gameboard[0] == 'X' && gameboard[4] == 'X' && gameboard[8] == 'X') return 1;
    else if(gameboard[2] == 'X' && gameboard[4] == 'X' && gameboard[6] == 'X') return 1;

    else if(gameboard[0] == 'O' && gameboard[1] == 'O' && gameboard[2] == 'O') return 1;
    else if(gameboard[3] == 'O' && gameboard[4] == 'O' && gameboard[5] == 'O') return 1;
    else if(gameboard[6] == 'O' && gameboard[7] == 'O' && gameboard[8] == 'O') return 1;
    else if(gameboard[0] == 'O' && gameboard[3] == 'O' && gameboard[6] == 'O') return 1;
    else if(gameboard[1] == 'O' && gameboard[4] == 'O' && gameboard[7] == 'O') return 1;
    else if(gameboard[2] == 'O' && gameboard[5] == 'O' && gameboard[8] == 'O') return 1;
    else if(gameboard[0] == 'O' && gameboard[4] == 'O' && gameboard[8] == 'O') return 1;
    else if(gameboard[2] == 'O' && gameboard[4] == 'O' && gameboard[6] == 'O') return 1;

    else if(gameboard[0] != ' ' && gameboard[1] != ' ' && gameboard[2] != ' ' && gameboard[3] != ' ' && gameboard[4] != ' ' && gameboard[5] != ' ' && gameboard[6] != ' ' && gameboard[7] != ' ' && gameboard[8] != ' ')
        return -1;

    else return 0;
}

int space_in_board()
{
    int i, spc = 0;
    for(i = 0; i < 9; i ++)
    {
        if(gameboard[i] == ' ') spc++;
    }
    return spc;
}

void updateLearning_data()
{

    //The commented code below is the original one from the C version.
    //Rewrite this function and use CSV file for storing and updating learning data


//    FILE* file;
//    int i;
//    char point[4];
//    int intpoint;
//
//    for(i = 0; i < O_move_count; i ++)
//    {
//        file = fopen(O_paths[i], "r");
//        fgets(point, 4, file);
//        intpoint = atoi(point);
//        fclose(file);
//
//        if(win == 1 && player == 1) intpoint += WIN_REWORD; //player 1 = CENACE
//        else if(win == -1) intpoint += DRAW_REWORD;
//        else intpoint -= PUNISHMENT;
//
//        if(intpoint >= MAX_POINT) intpoint = MAX_POINT;
//        else if(intpoint <= MIN_POINT) intpoint = MIN_POINT;
//
//        sprintf(point, "%d", intpoint);
//
//        file = fopen(O_paths[i], "w");
//        fprintf(file, point);
//        fclose(file);
//    }

}

void menu()
{
menu:
    exit_CENACE = 0;

    cout << "\n\n\t1. Auto Train CENACE";
    cout << "\n\t2. Play";
    cout << "\n\t3. Rules";
    cout << "\n\t4. About";
    cout <<"\n\t5. Exit";
    cout << "\n\n\t--> ";

    char choice = getch();
    cout << choice;

    switch (choice)
    {
    case '1':
        system("cls");

        cout << "\n\tTraining may take some time.";
        cout << "\n\tDo not exit the program during training.";
        cout << "\n\tContinue? (y/n) --> ";

        auto_train = getch();
        cout << "%c", auto_train;

        if(auto_train == 'y' || auto_train == 'Y')
        {
            cout << "\n\tMatches to be played --> ";
            cin >> total_training_match;
            fflush(stdin);
        }
        else
        {
            system("cls");
            goto menu;
        }
        return;
        break;

    case '2':
        return;
        break;

    case '3':
        system("cls");
        cout << "\n\tRULES:";
        cout << "\n\t1. CENACE will always go first.";
        cout << "\n\t2. Move positions are from 1 to 9 on the game board.\n";
        cout << "\n\t\t       |       |       ";
        cout << "\n\t\t   1   |   2   |   3   ";
        cout << "\n\t\t_______|_______|_______";
        cout << "\n\t\t       |       |       ";
        cout << "\n\t\t   4   |   5   |   6   ";
        cout << "\n\t\t_______|_______|_______";
        cout << "\n\t\t       |       |       ";
        cout << "\n\t\t   7   |   8   |   9   ";
        cout << "\n\t\t       |       |       \n";
        cout << "\n\t3. +3 for win, +1 for draw, -1 for lose.";

        getch();
        system("cls");
        goto menu;
        break;

    case '4':
        system("cls");
//        printf("\n     _______________________________________________________");
//        printf("\n    |                                                       |");
//        printf("\n    | Computer Educable Noughts and Crosses Engine (CENACE) |");
//        printf("\n    |_______________________________________________________|\n");
//
//        printf("\n\tAuthor: Aaban Saad Preyanta");
//        printf("\n\tNSU ID: 2233389642");
//        printf("\n\tBachelor of Computer Science and Engineering");
//        printf("\n\tDepartment of Electrical and Computer Engineering");
//        printf("\n\tNorth South University, Dhaka, Bangladesh\n");
//
//        printf("\n\tThe name CENACE originated from the 1961 MENACE reinforcement learning model by Donald Michie. ");
//        printf("\n\tI have programmed CENACE as a computer simulation of Michie's MENACE. ");
//        printf("\n\tInstead of matchboxes, CENACE uses folders and text files to keep track of its moves.\n");
//        printf("\n  ____________________________________________________________________________________________________\n\n");
//
//        printf("\n\tMENACE: The Matchbox Educable Noughts and Crosses Engine was a ");
//        printf("\n\tmechanical computer made from 304 matchboxes designed and built by ");
//        printf("\n\tartificial intelligence researcher Donald Michie in 1961. ");
//        printf("\n\tIt was designed to play human opponents in games of noughts and crosses (tic-tac-toe) ");
//        printf("\n\tby returning a move for any given state of play ");
//        printf("\n\tand to refine its strategy through reinforcement learning. (Wikipedia)");


        getch();
        system("cls");
        goto menu;
        break;

    case '5':
        exit_CENACE = 1;
        return;
        break;

    default:
        getch();
        system("cls");
        goto menu;
        break;

    }
}

void CENACE_intro()
{
    system("cls");
    printf("\n\t%c%c%c      %c%c%c   %c%c%c", 219, 219, 219, 219, 219, 219, 219, 219, 219);
    printf("\n\t%c%c%c      %c%c%c   %c%c%c", 219, 219, 219, 219, 219, 219, 219, 219, 219);
    printf("\n\t%c%c%c      %c%c%c", 219, 219, 219, 219, 219, 219);
    printf("\n\t%c%c%c      %c%c%c   %c%c%c", 219, 219, 219, 219, 219, 219, 219, 219, 219);
    printf("\n\t%c%c%c%c%c%c%c%c%c%c%c%c   %c%c%c", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
    printf("\n\t%c%c%c%c%c%c%c%c%c%c%c%c   %c%c%c", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
    printf("\n\t%c%c%c      %c%c%c   %c%c%c", 219, 219, 219, 219, 219, 219, 219, 219, 219);
    printf("\n\t%c%c%c      %c%c%c   %c%c%c", 219, 219, 219, 219, 219, 219, 219, 219, 219);
    printf("\n\t%c%c%c      %c%c%c   %c%c%c", 219, 219, 219, 219, 219, 219, 219, 219, 219);
    printf("\n\t%c%c%c      %c%c%c   %c%c%c  ", 219, 219, 219, 219, 219, 219, 219, 219, 219);
    Sleep(1500);
    system("cls");

    printf("\n\t%c%c%c", 219, 219, 219);
    printf("\n\t%c%c%c  %c%c", 219, 219, 219, 219, 219);
    printf("\n\t%c%c%c   %c", 219, 219, 219, 219);
    printf("\n\t%c%c%c      %c%c%c%c%c%c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
    printf("%c",219);
    printf("\n\t%c%c%c      %c%c%c%c%c%c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
    printf("%c",219);
    printf("\n\t%c%c%c      %c%c%c  %c%c%c  %c%c%c", 219, 219, 219, 219, 219, 219, 219, 219, 219);
    printf("\n\t%c%c%c      %c%c%c  %c%c%c  %c%c%c", 219, 219, 219, 219, 219, 219, 219, 219, 219);
    printf("\n\t%c%c%c      %c%c%c  %c%c%c  %c%c%c", 219, 219, 219, 219, 219, 219, 219, 219, 219);
    printf("\n\t%c%c%c      %c%c%c  %c%c%c  %c%c%c", 219, 219, 219, 219, 219, 219, 219, 219, 219);
    printf("\n\t%c%c%c      %c%c%c  %c%c%c  %c%c%c  ", 219, 219, 219, 219, 219, 219, 219, 219, 219);

    Sleep(1000);
    system("cls");

    printf("\n\t%c%c%c%c%c%c%c  %c%c%c%c%c%c%c  ", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
    printf("%c%c%c%c%c%c%c   %c%c", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);

    printf("\n\t%c%c       %c%c       ", 219, 219, 219, 219, 219);
    printf("%c%c   %c%c   %c%c", 219, 219, 219, 219, 219, 219);

    printf("\n\t%c%c       %c%c       ", 219, 219, 219, 219, 219);
    printf("%c%c   %c%c   %c%c", 219, 219, 219, 219, 219, 219);

    printf("\n\t%c%c       %c%c%c%c%c%c%c  ", 219, 219, 219, 219, 219, 219, 219, 219, 219);
    printf("%c%c   %c%c   %c%c", 219, 219, 219, 219, 219, 219);

    printf("\n\t%c%c       %c%c       ", 219, 219, 219, 219);
    printf("%c%c   %c%c   %c%c", 219, 219, 219, 219, 219, 219);

    printf("\n\t%c%c       %c%c       ", 219, 219, 219, 219);
    printf("%c%c   %c%c   %c%c", 219, 219, 219, 219, 219, 219);

    printf("\n\t%c%c%c%c%c%c%c  %c%c%c%c%c%c%c  ", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
    printf("%c%c   %c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219, 219, 219, 219);

    printf("\n");

    printf("\n\t%c%c%c%c%c%c%c%c%c%c   ", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
    printf("%c%c%c%c%c%c%c   %c%c%c%c%c%c%c  ", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);

    printf("\n\t%c%c      %c%c   ", 219, 219, 219, 219);
    printf("%c%c        %c%c       ", 219, 219, 219, 219);

    printf("\n\t%c%c      %c%c   ", 219, 219, 219, 219);
    printf("%c%c        %c%c       ", 219, 219, 219, 219);

    printf("\n\t%c%c%c%c%c%c%c%c%c%c   ", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
    printf("%c%c        %c%c%c%c%c%c%c  ", 219, 219, 219, 219, 219, 219, 219, 219, 219);

    printf("\n\t%c%c      %c%c   ", 219, 219, 219, 219);
    printf("%c%c        %c%c       ", 219, 219, 219, 219);

    printf("\n\t%c%c      %c%c   ", 219, 219, 219, 219);
    printf("%c%c        %c%c       ", 219, 219, 219, 219);

    printf("\n\t%c%c      %c%c   ", 219, 219, 219, 219, 219, 219);
    printf("%c%c%c%c%c%c%c   %c%c%c%c%c%c%c  ", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);


    Sleep(1000);
    printf("\n\n\tLoading CENACE %c     0%c", 196, 37);
    int i;
    for(i = 0; i < 100; i += 12)
    {
        Sleep(100);
        printf("\b\b\b\b\b\b\b\b\\    %2d%c", i, 37);
        Sleep(100);
        printf("\b\b\b\b\b\b\b\b/    %2d%c",i + 2, 37);
        Sleep(100);
        printf("\b\b\b\b\b\b\b\b%c    %2d%c", 196, i + 3, 37);
    }
    Sleep(500);
    system("cls");
    Sleep(1000);
}

void updateGraphing_data()
{
    FILE *file_lastScore, *file_scoreList;
    char path1[] = "Graphing_data\\Last_Score.txt";
    char path2[] = "Graphing_data\\Score_List.txt";
    char score[100];
    int  intscore;

    file_lastScore = fopen(path1, "r");
    if(file_lastScore == NULL)
    {
        file_lastScore = fopen(path1, "w");
        fprintf(file_lastScore, 0);
        fclose(file_lastScore);
    }
    fgets(score, 20, file_lastScore);
    intscore = atoi(score);
    fclose(file_lastScore);

    if(win == 1 && player == 1) intscore += WIN_REWORD;
    else if(win == -1) intscore += DRAW_REWORD;
    else intscore -= PUNISHMENT;

    sprintf(score, "%d", intscore);

    file_lastScore = fopen(path1, "w");
    fprintf(file_lastScore, score);
    fclose(file_lastScore);

    file_scoreList = fopen(path2, "a");
    fprintf(file_scoreList, strcat(score, "\n"));
    fclose(file_scoreList);
}

void scoreUpdate()
{
    if(win == 1 && player == 1)
    {
        CENACEscore += WIN_REWORD;
        humanScore -= PUNISHMENT;
    }
    else if(win == 1 && player == 2)
    {
        humanScore += WIN_REWORD;
        CENACEscore -= PUNISHMENT;
    }
    else if(win == -1)
    {
        CENACEscore += DRAW_REWORD;
        humanScore += DRAW_REWORD;
    }
}

