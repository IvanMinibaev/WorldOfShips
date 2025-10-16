#include <cstdio>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <map>
#include <conio.h>
using namespace std;

const int ShipTypes = 4;
const int ShipSizeAmount [4] = {1, 1, 0, 0};

map <char, bool> IsDirection = {
    {'W', 1},
    {'S', 1},
    {'A', 1},
    {'D', 1}
};
map <char, pair<int,int>> DirectionVector = {
    {'W', {-1, 0}},
    {'S', {1, 0}},
    {'D', {0, 1}},
    {'A', {0, -1}}

};
map <char, char> ShipBow = {
    {'W', 'A'},
    {'S', 'V'},
    {'D', '>'},
    {'A', '<'}

};
map <char, char> ShipSterm = {
    {'W', 'U'},
    {'S', 'П'},
    {'D', 'C'},
    {'A', 'D'}

};
const int ACode = 64;
const char ShipCorpse = 'H';
const char BorderTile = 'B';
const char EmptyTile = '~';
const char BoardTile = '.';
const char DeathTile = 'X';
const char MissTile = 'O';

int ShipAmount [10] = {0};
char sea [10][12][12];
char shots [10][12][12];
bool death = 0;


int read_player_amount(void)
{
    char input [256];
    cin.getline(input, 256);
    if((atoi(input) < 2) || (atoi(input) > 5) || (to_string(atoi(input)) != input))
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
    input2 = (char)toupper(input2);
    if((to_string(atoi(input1)) != input1) || (atoi(input1) < 1) || (atoi(input1) > 10) 
    || (input2  < 'A') || (input2 > 'J' ))
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
    input = (char)toupper(input);
    if (IsDirection[input] != 1)
        {
            return read_direction("Некорректный ввод. Попробуйте снова:\n");
        }
         else
        {
            return input;
        }
}

void show_death(int dead_buddy)
{
    cout<<"Ход игрока "<<dead_buddy+1<<". Нажмите любую кнопку для продолжения...";
    getch();
    printf("\033c");
    cout<<"К сожалению, все Ваши кораблики были потоплены. Вы проиграли :(\n"
        <<"Все дальнейшие ходы будут автоматически пропущены\n"
        <<"Но не расстраивайтесь, в следующий раз повезет именно Вам. До новых встреч!\n"
        <<"(Нажмите любую кнопку)";
    getch();
    printf("\033c");
}

void show_status(int attacker, int target)
{
    cout<<"<+-ABCDEFJHIJ T ABCDEFGHIJ-+>\n";
    for(int i = 1; i <= 10; i++)
    {
        cout<<" "<<i % 10<<" ";
        for(int j = 1; j <= 10; j++)
        {
            cout<<sea[attacker][i][j];
        }
        cout<<" | ";
        for(int j = 1; j <= 10; j++)
        {
            cout<<shots[target][i][j];
        }
        cout<<" "<<i % 10<<"\n";
    }
    cout<<" \\____________I____________/\n";
}

void dfs_check_if_dead(int x, int y, int target, char block)
{
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

void dfs_show_oreol(int x, int y, int target, char block)
{
  if(shots[target][x-1][y] == DeathTile && block != 'W')
  dfs_show_oreol(x-1, y, target, 'S');
  if(shots[target][x+1][y] == DeathTile && block != 'S')
  dfs_show_oreol(x+1, y, target, 'W');
  if(shots[target][x][y-1] == DeathTile && block != 'A')
  dfs_show_oreol(x, y-1, target, 'D');
  if(shots[target][x][y+1] == DeathTile && block != 'D')
  dfs_show_oreol(x, y+1, target, 'A');

  for(int i = -1; i < 2; i++)
    for(int j = -1; j < 2; j++)
    {
        if(shots[target][x+i][y+j] == BoardTile)
        shots[target][x+i][y+j] = MissTile;
    }
}

void make_shot(int attacker, int target)
{
    pair <int, int> coordinate;
    show_status(attacker, target);
    coordinate = read_coordinate("Введите координаты точки, куда нужно выстрелить\n");
    printf("\033c");
    if((sea[target][coordinate.first][coordinate.second] == EmptyTile) 
    || (sea[target][coordinate.first][coordinate.second] == DeathTile))
    {
        cout<<"Мимо!\n";
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
            ShipAmount[target]--;
            dfs_show_oreol(coordinate.first, coordinate.second, target, '0');
        }
        else
        {
            cout<<"Вражеский корабль ранен, продолжайте в том же духе!\n";
        }
        
        if(ShipAmount[target] != 0)
            make_shot(attacker, target);
        else
            cout<<"Вы уничтожили все кораблики врага.\n";

    }

}

bool check_position(int player, int size, pair <int, int> coordinate, char direction)
{
    for(int h = 0; h < size; h++)
    {
        for(int i = -1; i < 2; i++)
            for(int j = -1; j < 2; j++)
            {
                if((sea[player][coordinate.first + DirectionVector[direction].first*h + i]
                               [coordinate.second + DirectionVector[direction].second*h + j] != EmptyTile) 
                || (shots[player][coordinate.first + DirectionVector[direction].first*h]
                                 [coordinate.second + DirectionVector[direction].second*h] == BorderTile))
                    return 1;
            }
    }
    return 0;
}

int ship_build(int player, int size)
{
    int ret_val = 0;
    pair <int, int> input_c;
    char input_d;

                ret_val++;
                cout<<"Сейчас ваш кораблик размера "<<size+1;
                input_c = read_coordinate(". Введите координаты начала корабля:\n");
                input_d = read_direction("А теперь направление, куда смотрит кораблик:\n");

                if (check_position(player, size, input_c, input_d))
                {
                    printf("\033c");
                    cout<<"Вы не можете поставить кораблик таким образом(\nВероятно, он стоит по соседству с другим корабликом, пересекает его или выходит за карту\nНажмите любую кнопку - и процесс его устанвки начнется заново.";
                    getch();
                    printf("\033c");
                    return ship_build(player, size);
                }
                sea[player][input_c.first][input_c.second] = ShipSterm[input_d];
                for(int h = 1; h < size; h++)
                {
                    sea[player][input_c.first + DirectionVector[input_d].first*h]
                               [input_c.second + DirectionVector[input_d].second*h] 
                               = ShipCorpse;
                }
                sea[player][input_c.first + DirectionVector[input_d].first*size]
                           [input_c.second + DirectionVector[input_d].second*size] 
                           = ShipBow[input_d];

                printf("\033c");
                cout<<"Установка прошла успешно! Переходим к следующему кораблю.\n";
    return ret_val;
}

void finishing_game(int winner)
{
    cout<<"Победил игрок "<<winner+1<<"! Ура Ура Ура!\nПриходите ещё :)\n Удачи!";
}

int main()
{
    setlocale(LC_ALL, "Russian");

    cout<<"Добро пожаловать в игру \"Морской бой\"! \nПожалуйста, введите число участников\n";
    const int PlayerAmount = read_player_amount();

    for(int i = 0;i < 12; i++)
        for(int j = 0;j < 12; j++)
            for(int h = 0; h < PlayerAmount; h++)
            {
                if((i != 0) && (i != 11) && (j != 0) && (j != 11))
                    shots[h][i][j] = BoardTile;
                else
                    shots[h][i][j] = BorderTile;
                sea[h][i][j] = EmptyTile;
            }

    for(int h = 0; h < PlayerAmount; h++)
    {
     printf("\033c");
     cout<<"Сейчас будет ходить игрок №"<<h+1;
     cout<<". Нажмите любую клавишу для продолжения...\n";
     getch();
     printf("\033c");
      for(int i = 1; i <= ShipTypes; i++)
            for(int j = 0; j < ShipSizeAmount[i-1]; j++)
                ShipAmount[h] += ship_build(h, i);
    }
    
    int attacker = 0;
    int target = 1;
    printf("\033c");
    cout<<"\nВсе кораблики установлены! Начинаем игру.\n (Нажмите любую кнопку)";
    getch();
   

    while(2+2 == 4)
    {
        target = (attacker + 1) % PlayerAmount;
        while(ShipAmount[target] < 1)
        {
            target = (target + 1) % PlayerAmount;
            if(attacker == target)
            {
                finishing_game(attacker);
                exit(0);
            }
        }
        printf("\033c");
        cout<<"Игрок "<<attacker + 1
            <<" атакует игрока "<<target + 1<<"! Нажмите любую клавишу для продолжения...\n";
         getch();
        make_shot(attacker, target);
        cout<<"Ваш ход окончен.\n (нажмите любую клавишу)\n";
        getch();
        attacker = (attacker + 1) % PlayerAmount;
        while(ShipAmount[attacker] < 1)
        {
            if(ShipAmount[attacker] == 0)
            show_death(attacker);
            attacker = (attacker + 1) % PlayerAmount;
        }
    }

    return 0;
}