#include <cstdio>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <map>
#include <conio.h>
using namespace std;

const int ShipTypes = 4;
const int ShipSizeAmount [4] = {0, 1, 0, 0};

map <char, bool> IsDirection = {
    {'W', 1},
    {'S', 1},
    {'A', 1},
    {'D', 1}
};
map <char, pair<int,int>> DirectionVector = {
    {'W', {1, 0}},
    {'S', {-1, 0}},
    {'D', {0, 1}},
    {'A', {0, -1}}

};
map <char, char> ShipBow = {
    {'W', 'V'},
    {'S', 'A'},
    {'D', '>'},
    {'A', '<'}

};
map <char, char> ShipSterm = {
    {'W', 'M'},
    {'S', 'U'},
    {'D', 'C'},
    {'A', 'D'}

};
const int ACode = 64;
const char ShipCorpse = 'H';
const char EmptyTile = '~';
const char DeathTile = 'X';
const char MissTile = 'O';

char sea [10][12][12];
char shots [10][12][12];
bool death = 0;


int read_player_amount(void)
{
    char input [256];
    cin >> input;
    if((atoi(input) < 2) || (atoi(input) > 5))
    {
        cout<<"Ошибка ввода. В игре могут принимать участие от 2 до 5 игроков. Попробуйте снова\n";
        return read_player_amount();
    }
    else
    return atoi(input);

}
pair <int, int> read_coordinate(string comment)
{
    pair <int, int> ret_val;
    char input1[256];
    char input2;

    cout<<comment;
    cin>>input2>>input1;
    input2 = toupper(input2);
    if((atoi(input1) < 1) || (atoi(input1) > 10) || (input2  < 'A') || (input2 > 'J' ))
        return read_coordinate("Некорректный ввод. Попробуйте снова:\n");
    else
    {
        ret_val.first = atoi(input1);
        ret_val.second = (int)input2 - ACode;
        return ret_val;
    }

}

char read_direction(string comment)
{
    char input;

    cout<<comment;
    cin>>input;
    input = toupper(input);
    if (IsDirection[input] != 1)
        {

            return read_direction("Некорректный ввод. Попробуйте снова:\n");
        }
         else
        {
            return input;
        }
}

void show_status(int attacker, int target)
{
    cout<<"+-ABCDEFJHIJ T ABCDEFGHIJ->\n";
    for(int i=1;i<=10;i++)
    {
        cout<<i%10<<" ";
        for(int j=1;j<=10;j++)
        {
            cout<<sea[attacker][i][j];
        }
        cout<<" | ";
        for(int j=1;j<=10;j++)
        {
            cout<<shots[target][i][j];
        }
        cout<<"\n";
    }
}

void dfs_check_if_dead(int x, int y, int target, char block)
{
    cout<<"dfs:"<<x<<" "<<y<<" "<<block<<endl;
    if(sea[target][x][y] != DeathTile)
    {
        death = 0;
    }
    if(sea[target][x-1][y] != EmptyTile && block != 'W')
        dfs_check_if_dead(x-1, y, target, 'S');
    if(sea[target][x+1][y] != EmptyTile && block != 'S')
        dfs_check_if_dead(x+1, y, target, 'W');
    if(sea[target][x][y-1] != EmptyTile && block != 'A')
        dfs_check_if_dead(x, y-1, target, 'D');
    if(sea[target][x][y+1] != EmptyTile && block != 'D')
        dfs_check_if_dead(x, y+1, target, 'A');
}
void make_shot(int attacker, int target)
{
    pair <int, int> coordinate;
    show_status(attacker, target);
    coordinate = read_coordinate("Введите координаты точки, куда нужно выстрелить\n");
    
    if((sea[target][coordinate.first][coordinate.second] == EmptyTile) 
    || (sea[target][coordinate.first][coordinate.second] == DeathTile))
    {
        cout<<"Вы промахнулись!\n";
        shots[target][coordinate.first][coordinate.second] = MissTile;
    }
    else
    {
        shots[target][coordinate.first][coordinate.second] = DeathTile;
        sea[target][coordinate.first][coordinate.second] = DeathTile;
        death = 1;
        dfs_check_if_dead(coordinate.first, coordinate.second, target, '0');
        if(death)
        {
            cout<<"Вражеский корабль потоплен, так держать!\n";
        }
        else
        {
            cout<<"Вражеский корабль подбит, продолжайте в том же духе!\n";
        }
        
        make_shot(attacker, target);
    }

}

int ship_build(int p)
{
    int ret_val = 0;
    pair<int, int> input_c;
    char input_d;
    for(int i = 1; i <= ShipTypes; i++)
        for(int j = 0; j < ShipSizeAmount[i-1]; j++)
            {
                ret_val++;
                cout<<"Сейчас ваш кораблик размера "<<i+1<<". Введите ";
                input_c = read_coordinate("координаты начала корабля:\n");
                input_d = read_direction("А теперь направление, куда смотрит кораблик: \n");

                sea[p][input_c.first][input_c.second] = ShipSterm[input_d];
                for(int h = 1; h <i; h++)
                {
                    sea[p][input_c.first + DirectionVector[input_d].first*h]
                          [input_c.second + DirectionVector[input_d].second*h] 
                          = ShipCorpse;
                }
                sea[p][input_c.first + DirectionVector[input_d].first*i]
                      [input_c.second + DirectionVector[input_d].second*i] 
                      = ShipBow[input_d];

                system("cls");
                cout<<"Установка прошла успешно! Переходим к следующему кораблю.\n";
            }
    return ret_val;
}

void finishing_game(int winner)
{
    cout<<"Победил игрок "<<winner<<"! Поздравляем!\n";
}

int main()
{
    setlocale(LC_ALL, "Russian");

    cout<<"Привет! \nПожалуйста, введите число участников\n";
    const int PlayerAmount = read_player_amount();
    
    int ShipAmount [10];
    for(int i=0;i<12;i++)
        for(int j=0;j<12;j++)
            for(int h=0;h<PlayerAmount;h++)
                {sea[h][i][j] = EmptyTile;
                shots[h][i][j] = '.';}

    for(int i=0;i<PlayerAmount;i++)
    {
     system("cls");
     cout<<"\nСейчас будет ходить игрок №"<<i+1;
     cout<<". Нажмите любую клавишу для продолжения...\n";
     getch();
     system("cls");
     ShipAmount[i] = ship_build(i);
    }

    for(int i = 0; i<PlayerAmount; i++)
    {
        cout<<"\n";
        for(int j=1;j<11;j++)
        {cout<<"\n";
        for(int h=1;h<11;h++)
        cout<<sea[i][j][h];}
    }
    
    int attacker = 0;
    int target = 1;
    cout<<"\nВсе кораблики установлены! Начинаем игру.\n (Нажмите любую кнопку)";
    getch();

    while(2+2 == 4)
    {
        target = (attacker + 1) % PlayerAmount;
        while(ShipAmount[target] == 0)
        {
            target = (target + 1) % PlayerAmount;
            if(attacker == target)
            {
                finishing_game(attacker);
                exit(0);
            }
        }
        //system("cls");
        cout<<"Игрок "<<attacker + 1
            <<" атакует игрока "<<target + 1<<"!\n Нажмите любую клавишу для продолжения...\n";
         getch();
        make_shot(attacker, target);
        cout<<"Ваш ход окончен.\n (нажмите любую клавишу)\n";
        attacker = (attacker + 1) % PlayerAmount;
        while(ShipAmount[attacker] == 0)
        attacker = (attacker + 1) % PlayerAmount;
    }




 

 
    return 0;
}