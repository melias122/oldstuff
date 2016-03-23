#include <iostream>

using namespace std;

void print(char board[8][8]){
    for (size_t i = 0; i < 8; i++)
    {
        for (size_t j = 0; j < 8; j++)
        {
            cout << board[i][j];
        }
        cout << endl;
    }
}

void horse(char board[8][8], int x, int y, int counter){
    if (x > 7 || x < 0 || y > 7 || y < 0){
        return;
    }

    if (board[x][y] != '.'){
        return;
    } else {
        board[x][y] = 'A' + counter++;
    }
    print(board);
    cout << endl;


    horse(board, x + 2, y + 1, counter);
    horse(board, x + 2, y - 1, counter);
    horse(board, x - 2, y + 1, counter);
    horse(board, x - 2, y - 1, counter);

    horse(board, x + 1, y + 2, counter);
    horse(board, x + 1, y - 2, counter);
    horse(board, x - 1, y + 2, counter);
    horse(board, x - 1, y - 2, counter);
}

void sswap(char *a, char *b){
    char c = *a;
    *a = *b;
    *b = c;
}

void perm(char s[], int i, int n){
    if(i == n){
        cout << s << endl;
    }
    for(int j = i; j < n; j++){
        sswap(&s[i], &s[j]);
        perm(s, i+1, n);
        sswap(&s[i], &s[j]);
    }
}

int main(){
    char s[] = {'a', 'b', 'c', 'd'};
    perm(s, 0, 4);

    //    int n = 8;

    //    char board[8][8];
    //    for(auto &line : board){
    //	for(auto &field: line){
    //	    field = '.';
    //	}
    //    }

    //    horse(board, 0, 0, 0);

    return 0;
}
