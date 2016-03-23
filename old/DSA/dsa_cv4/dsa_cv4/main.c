//
//  main.c
//  dsa_cv4
//
//  Created by Martin Elias on 10/13/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>

typedef struct _z{
    int c;
    struct _z *dalsi;
}z;

z *cisla;

void rremove(int pos){
    
    z *pom = cisla;
    z *pred = NULL;
    
    int i = 0;
    while (pom->dalsi != NULL) {
        
        if (i == pos) {
            
            if (pos == 0) {
                cisla = pom->dalsi;
                free(pom);
                return;
            }
            if (pom->dalsi == NULL) {
                pred->dalsi = NULL;
                free(pom);
                return;
            }
            else{
                pred->dalsi = pom->dalsi;
                free(pom);
                return;
            }
        }
        pred = pom;
        pom = pom->dalsi;
        i++;
    }
}

void get(int pos){
    
    z *pom = cisla;
    
    int i = 1;
    while (pom != NULL) {
        
        if (i == pos) {
            printf("%d\n", pom->c);
            return;
        }
        
        pom = pom->dalsi;
        i++;
    }
}

void insert(int pos, int c){
    
    z *pom = cisla;
    z *pred = NULL;
    z *novy = NULL;
    
    int i = 0;
    while (1) {
        
        if (i == pos) {
            
            novy = (z *)malloc(sizeof(z));
            novy->c = c;
            
            if (pos == 0) {
                novy->dalsi = pom;
                cisla = novy;
                break;
            }
            if (pom == NULL) {
                pred->dalsi = novy;
                novy->dalsi = pom;
                //->dalsi = NULL;
                break;
            }
            else{
                pred->dalsi = novy;
                novy->dalsi = pom;
                break;
            }
            
        }
        else{
            
        }
        pred = pom;
        pom = pom->dalsi;
        i++;
    }
}

#define is_num(ch) '0' >= '9'


int main (int argc, const char * argv[]) {
    
    char ch;
    
    insert(0,7);
    insert(0,-4);
    insert(2,2);
    insert(2,7);
    insert(0,10);
    get(5);
    insert(4,8);
    rremove(2);
    insert(5,-3);
    get(1);
    
    return 0;
}

