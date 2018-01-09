#include <iostream>
#include <conio.h>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <windows.h>
#include <string>
using namespace std;

class Save {
protected:
    int score;

public:
    void loadScore() {
        ifstream file_out_score;
        file_out_score.open("score.txt");
        file_out_score >> score;
        file_out_score.close();
    }
    void saveScore() {
        ofstream file_to_score;
        file_to_score.open("score.txt");
        file_to_score << score;
        file_to_score.close();
    }
};


class Board {
protected:
    char board[20][40];
    int head[100] = {10,10,10,11,10,12};
public:
        void startSnake() {
        for(int i = 0, j = 1 ; j<7 ; j+=2,i+=2) {
            board[head[i]][head[j]] = '#';
        }
    }

    void makeMap() {
        for (int i = 0; i<20; i++)
        {
            for (int j = 0; j<40; j++)
            {
                board[i][j] = ' ';
                board[i][0] = char(219);
                board[i][39] = char(219);
                board[0][j] = char(219);
                board[19][j] = char(219);
            }
        }

    }
    void printMap() {
        for (int s = 0; s<20;s++)
        {
            for (int l = 0; l<40;l++)
            {
                cout<<board[s][l]/*<<"\t"*/;
            }
            cout<<endl;
        }
    }

};


class Apple : public Board {
protected:
    int sour_apple, sweet_apple, bad_apple;
    int tail;
    int xsour, ysour;
    int xsweet, ysweet;
    int xbad, ybad;
    int points;

public:

    void sourGenerator() {
        bool a = false;
        do
        {
            xsour = (rand() % 18)+1;
            ysour = (rand() % 38)+1;
            if(sourCollector()==true)
                a=true;
            else a=false;
        } while(a==true);
        board[xsour][ysour] = '*';
    }

    void eatingSourApple() {
        if(head[0] == xsour && head[1] == ysour)
        {
            sourGenerator();
            sourGenerator();
            tail += 1;
            points += 1;
        }
    }

    bool sourCollector() {
        if(board[xsour][ysour]=='#') {
            return true;
        }
        else return false;
    }





    void sweetGenerator() {
        bool b = false;
        do
        {
            xsweet = (rand() % 18)+1;
            ysweet = (rand() % 38)+1;
            if(sweetCollector()==true)
                b=true;
            else b=false;
        } while(b==true);
        board[xsweet][ysweet] = '%';
    }

    void eatingSweetApple() {
        if(head[0] == xsweet && head[1] == ysweet)
        {
            sweetGenerator();
            sweetGenerator();
            tail += 2;
            points += 2;
        }
    }

    bool sweetCollector() {
        if(board[xsweet][ysweet]=='#') {
            return true;
        }
        else return false;
    }




    void badGenerator() {
        bool c = false;
        do
        {
            xbad = (rand() % 18)+1;
            ybad = (rand() % 38)+1;
            if(badCollector()==true)
                c=true;
            else c=false;
        } while(c==true);
        board[xbad][ybad] = '@';
    }

    void eatingBadApple() {
        if(head[0] == xbad && head[1] == ybad)
        {
            badGenerator();
            badGenerator();
            tail += 1;
            points -= 1;
        }
    }

    bool badCollector() {
        if(board[xbad][ybad]=='#') {
            return true;
        }
        else return false;
    }
};


class Snake : public Apple {
protected:
    //char board[20][40];
    //int head[100] = {10,10,10,11,10,12};
    //int tail;
    char direction = 'a';
    //bool run;
    //int xapple,yapple;
    //int points;
public:
    Snake()
    {
        tail = 3;
        direction = 'a';
        //run = true;
        points = 0;
    }




    void changeDirection() {

        board[head[tail*2-2]][head[tail*2-1]] = ' ';

        for(int i=tail*2-1;i>=3;i-=2) {
            head[i] = head[i-2];
            head[i-1] = head[i-3];
        }
        switch(direction)
        {
            case 'w': {
                head[0]-=1;
                break;
                }
            case 's': {
                head[0]+=1;
                break;
            }
            case 'a': {
                head[1]-=1;
                break;
            }
            case 'd': {
                head[1]+=1;
                break;
            }
            default: {
                changeDirection();
                break;
            }
        }

    }

};


class Runsnake : public Snake ,public Save {
protected:
    bool run;
public:
    Runsnake () {
        loadScore();
        run = true;
    }
        void runSnake() {
        makeMap();
        sweetGenerator();
        sourGenerator();
        badGenerator();
        while (run) {
            startSnake();
            if(kbhit()){
                direction = getch();

            }
            changeDirection();
            gotoxy(0,0);
            printMap();
            dontHitTheSnake();
            barriers();
            _sleep(150);
            eatingBadApple();
            eatingSourApple();
            eatingSweetApple();
            cout<<"\n \n \n \n \n "<<endl;
            cout<<"You have: " << points <<" points."<<endl;
            cout<<"Record to beat: "<< score <<endl;
            cout<<"Sign '@' will subtract your points by 1."<<endl;
            cout<<"Sign '*' will add 1 to your points."<<endl;
            cout<<"Sign '%' wil add 2 to your points."<<endl;
        }
        score = points;
        saveScore();
        system("cls");
        cout<<"\n \n \n \n \n";
        cout<< "You died"<<endl;
        cout<< "You earned: "<<points<<"."<<endl;

    }
    void gotoxy(int x, int y){
        COORD coord={x, y};
        SetConsoleCursorPosition (GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    void barriers() {
        if (head[0]== 0 || head[0] == 19 || head[1] ==0 || head[1] == 39)
            run = false;
    }

    void dontHitTheSnake() {
        for (int q=2; q < tail*2-1; q+=2) {
            if(head[0] == head[q] && head[1] == head[q+1])
                run = false;
        }
    }
};


int main()
{   srand(time(NULL));
    Runsnake snake;
    cout << "Wellcome!" << endl;
    cout<<  "This is snake game"<<endl;
    cout<<" \n \n \n "<<endl;
    getch();
    cout<<"\n \n "<<endl;
    cout<< "There are 3 types of apples in this game:"<<endl;
    cout<<" \n \n "<<endl;
    cout<<"These with sign '@' will subtract your points by 1."<<endl;
    cout<<"These with sign '*' will do nothing."<<endl;
    cout<<"These with sign '%' wil add 1 to your points."<<endl;
    cout<<"\n \n  \n"<<endl;
    cout<<"You navigate with w s a d(w = up, s = down, a = left, d = right)"<<endl;
    cout<<"click anything when you ready"<<endl;




    if(getch()) {
        system("cls");
        snake.runSnake();
    }

    cin.ignore();
    return 0;
}
