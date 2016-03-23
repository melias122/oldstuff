#include <stdio.h>
#include <stdlib.h>

typedef struct _bTree {
    
    int data;
    struct _bTree *vacsi, *mensi;
}bTree;

bTree *uzol = NULL;
int spolu = 0;

void insert(int hodnota){
    
    bTree * pom     = uzol;
    
    bTree * novy    = (bTree *) malloc(sizeof(bTree));
    
    novy->data      = hodnota;
    novy->mensi     = NULL;
    novy->vacsi     = NULL;
    
    // Inicializacia
    if (uzol == NULL) {
        uzol = novy;
    }
    else{
        while (pom != NULL) {
            
            // Ideme vpravo
            if (hodnota > pom->data) {
                
                if (pom->vacsi != NULL) {
                    pom = pom->vacsi;
                }
                else {
                    pom->vacsi = novy;
                    break;
                }
            }
            // Ideme vlavo
            else {
                
                if (pom->mensi != NULL) {
                    pom = pom->mensi;
                } 
                else {
                    pom->mensi = novy;
                    break;
                }
            }
        }
    }
}

void search(int hodnota){
    
    bTree * pom = uzol;
    
    while (1) {
        
        if(hodnota == pom->data){
            spolu++;
            break;
        }
        if (hodnota > pom->data) {
            ++spolu;
            
            if (pom->vacsi != NULL) {
                pom = pom->vacsi;
                
            }
            else {
                break;
            }
        }
        // Ideme vlavo
        else {
            
            ++spolu;
            
            if (pom->mensi != NULL) {
                pom = pom->mensi;
            } 
            else {
                break;
            }
        } 
    }
}

int main(){
    
    char c;
    int i,a;
    
    while((c = getc(stdin)) != EOF){
        
        if(c == 'i'){
            for(i=0;i<6;i++)
                c = getc(stdin);
            
            scanf("%d",&a);
            insert(a);
        }
        if(c == 's'){
            for(i=0;i<6;i++)
                c = getc(stdin);
            
            scanf("%d",&a);
            search(a);
        }
    }
    
    printf("%d\n", spolu);
    
    
    return 0;
}