#include <stdio.h>
#define N 4

// 1 - pawn
// 2 - knight
// 3 - bishop
// 4 - rook
// 5 - queen
// 6 - king

// 0 - clean
// 7 - not clean

typedef struct{
    int info[16][16];
}Flag;

int dx[16] = {0, 1, 0, -1, 1, 1, -1, -1, -2, -2, -1, 1, 2, 2, 1, -1};
int dy[16] = {1, 0, -1, 0, -1, 1, 1, -1, 1, -1, -2, -2, 1, -1, 2, 2};
char trans[16][4] = {"R", "D", "L", "U", "LD", "RD", "RU", "LU", "UUR", "UUL", "ULL", "DLL", "DDR", "DDL", "DRR", "URR"};
int ez[8][2] = {{}, {}, {}, {4, 8}, {0, 4}, {0, 8}, {0, 8}};

// [0] for type, [1] for number of steps.
int min = 40, tmp[256][2], ans[256][2];

int in(int x, int y){
    return x >= 0 && y >= 0 && x < N && y < N;
}

void init(Flag *flag){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            flag->info[i][j] = 0;
        }
    }
}

void dfs(int board[][16], int x, int y, int rem, int steps, Flag flag){
    if(rem + steps - 1 >= min) return;
    if(rem == 1){
        min = steps;
        for(int i = 0; i < min; i++){
            ans[i][0] = tmp[i][0];
            ans[i][1] = tmp[i][1];
        }
        return;
    }

    int nx, ny, toc = 0, wall, race = board[x][y];
    Flag new;
    switch(race){
        case 1:
            if(in(x - 1, y) && board[x - 1][y] == 0){
                tmp[steps][0] = 3;
                tmp[steps][1] = 1;

                board[x][y] = 0;
                board[x - 1][y] = 1;
                dfs(board, x - 1, y, rem, steps + 1, flag);
                board[x][y] = 1;
                board[x - 1][y] = 0;
            }
            if(in(x - 1, y + 1) && board[x - 1][y + 1] > 0 && board[x - 1][y + 1] < 7){
                tmp[steps][0] = 6;
                tmp[steps][1] = 1;

                board[x][y] = 0;
                dfs(board, x - 1, y + 1, rem - 1, steps + 1, flag);
                board[x][y] = 1;
            }
            if(in(x - 1, y - 1) && board[x - 1][y - 1] > 0 && board[x - 1][y - 1] < 7){
                tmp[steps][0] = 7;
                tmp[steps][1] = 1;

                board[x][y] = 0;
                dfs(board, x - 1, y - 1, rem - 1, steps + 1, flag);
                board[x][y] = 1;
            }
            return;

        case 2:
            for(int i = 8; i < 16; i++){
                nx = x + dx[i];
                ny = y + dy[i];
                if(in(nx, ny) && board[nx][ny] < 7 && !flag.info[nx][ny]){
                    tmp[steps][0] = i;
                    tmp[steps][1] = 1;

                    if(board[nx][ny] > 0) init(&new);
                    else board[nx][ny] = 2, toc = flag.info[x][y] = 1;

                    board[x][y] = 0;
                    dfs(board, nx, ny, (toc)? rem:rem - 1, steps + 1, (toc)? flag:new);
                    board[x][y] = 2;

                    if(toc) toc = board[nx][ny] = flag.info[x][y] = 0;
                }
            }
            return;

        default:
            for(int i = ez[race][0]; i < ez[race][1]; i++){
                wall = 0;
                for(int j = 1; j <= (race < 6)? N:1; j++){
                    nx = x + dx[i]*j;
                    ny = y + dy[i]*j;
                    if(in(nx, ny) && board[nx][ny] < 7 && !flag.info[nx][ny]){
                        tmp[steps][0] = i;
                        tmp[steps][1] = j;

                        if(board[nx][ny] > 0){
                            init(&new);
                            wall = 1;
                        }
                        else board[nx][ny] = race, toc = flag.info[x][y] = 1;

                        board[x][y] = 0;
                        dfs(board, nx, ny, (toc)? rem:rem - 1, steps + ((race == 6)? j:1), (toc)? flag:new);
                        board[x][y] = race;

                        if(toc) toc = board[nx][ny] = flag.info[x][y] = 0;

                        if(!wall) continue;
                    }
                    break;
                }
            }
            return;
    }
}

int count(int board[][16], int *rem){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(board[i][j] > 0 && board[i][j] < 7){
                (*rem)++;
            }
        }
    }
}

int main(){
    int x, y, rem = 0;

    int board[16][16] = {{2, 7, 4, 0, 0, 0, 0, 0},
                         {0, 7, 3, 1, 0, 0, 0, 0},
                         {2, 6, 7, 7, 0, 0, 0, 0},
                         {4, 1, 1, 2, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0, 0, 0, 0}};

    Flag flag;
    init(&flag);

    count(board, &rem);
    printf("Enter initial x, y: ");
    scanf("%d%d", &x, &y);

    dfs(board, x, y, rem, 0, flag);

    printf("Minimum step = %d\n", min);
    for(int i = 0; i < min; i++){
        printf("%3s %3d steps\n", trans[ans[i][0]], ans[i][1]);
    }
}