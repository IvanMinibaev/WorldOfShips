#include <cstdio>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <map>
#include <conio.h>
using namespace std;

const int ShipTypes = 4;
const int ShipSizeAmount [4] = {1, 1, 1, 1};
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
    {'W', 'v'},
    {'S', 'A'},
    {'D', '/'},
    {'A', '\\'}

};
map <char, char> ShipSterm = {
    {'W', 'M'},
    {'S', 'W'},
    {'D', '\\'},
    {'A', '/'}

};
const char ShipCorpse = '_';
char sea [10][10][10];

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

pair <pair<int, int>, char> read_input(void)
{
     pair <pair<int, int>, char> ret_val;
    char input1[256];
    char input2[256];
    char input3;

    cout<<"Введите, куда его поставить\n";
    cin>>input1>>input2>>input3;
    input3 = toupper(input3);
    if((((atoi(input1) < 1) || (atoi(input1) > 10)) || (IsDirection[input3] != 1)) || ((atoi(input2) < 1) || (atoi(input2) > 10)))
        {
            cout<<"Некорректный ввод. Попробуйте снова:\n";
            return read_input();
        }
         else
        {
            ret_val.first.first = atoi(input1) - 1;
            ret_val.first.second = atoi(input2) - 1;
            ret_val.second = input3;
            return ret_val;
        }
}

int ship_build(int p)
{
    int ret_val = 0;
    pair <pair<int, int>, char> input;
    for(int i = 1; i <= ShipTypes; i++)
        for(int j = 0; j < ShipSizeAmount[i-1]; j++)
            {
                ret_val++;
                cout<<"Сейчас ваш кораблик размера "<<i+1<<". ";
                input = read_input();

                sea[p][input.first.first][input.first.second] = ShipSterm[input.second];
                for(int h = 1; h <i; h++)
                {
                    sea[p][input.first.first + DirectionVector[input.second].first*h][input.first.second + DirectionVector[input.second].second*h] = ShipCorpse;
                }
               sea[p][input.first.first + DirectionVector[input.second].first*i][input.first.second + DirectionVector[input.second].second*i] = ShipBow[input.second];
            }
return ret_val;
}

int main()
{
    setlocale(LC_ALL, "Russian");

    cout<<"Привет! \nПожалуйста, введите число участников\n";
    const int PlayerAmount = read_player_amount();
    
    int ShipAmount [10];
    for(int i=0;i<10;i++)
        for(int j=0;j<10;j++)
            for(int h=0;h<PlayerAmount;h++)
                sea[h][i][j] = '~';

    for(int i=0;i<PlayerAmount;i++)
    {
     cout<<"\nСейчас будет ходить игрок №"<<i+1;
     cout<<". Нажмите любую клавишу для продолжения...\n";
     getch();
     ShipAmount[i] = ship_build(i);
    }

    for(int i = 0; i<PlayerAmount; i++)
    {
        cout<<"\n";
        for(int j=0;j<10;j++)
        {cout<<"\n";
        for(int h=0;h<10;h++)
        cout<<sea[i][j][h];}
    }



    
    return 0;
}