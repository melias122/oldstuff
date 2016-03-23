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
    int i;
    
    scanf("%d %d", &n, &m);
    for (i = 0; i < m; i++) {
        scanf("%d%d%d", &x, &y, &dlzka);
        fprintf(stderr, "Debug: x = %d, y = %d, dlzka = %d",x, y, dlzka);
    }
    
    scanf("%d", &s);
    for (i = 0; i < s; i++) {
        scanf("%d", &s);
        obchody[i] = s;
        fprintf(stderr, "Debug: obchody[%d] = %d",i , s);
    }
    
    
    printf("Hello, World!\n");
    return 0;
}

