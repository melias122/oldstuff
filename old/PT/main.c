//
//  main.c
//  dsa_pr3
//
//  Created by Martin Elias on 12/7/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

int obchody[10];
int mat[100000][100000];

int main (int argc, const char * argv[]) {

    int n;          // pocet krizovatiek
    int m;          // pocet ciest medzi krizovatkami
    int x;          // cesta z x
    int y;          // do y
    int dlzka;      // s dlzkou
    int s;          // pocet obchodov
    int i, j;
    
    scanf("%d %d", &n, &m);
    // incializacia prvkov matice na -1
    for (i = 0; i < n; i ++) {
        for (j = 0; j < m; j++) {
            mat[i][j] = 0;
        }
    }
    
    // Naplenie matice
    for (i = 0; i < m; i++) {
        scanf("%d%d%d", &x, &y, &dlzka);
        fprintf(stderr, "Debug: x = %d, y = %d, dlzka = %d\n",x, y, dlzka);
    }
    
    scanf("%d", &s);
    for (i = 0; i < s; i++) {
        scanf("%d", &dlzka);
        obchody[i] = dlzka;
        fprintf(stderr, "Debug: obchody[%d] = %d\n",i , dlzka);
    }
    
    
    printf("koniec\n");
    return 0;
}

