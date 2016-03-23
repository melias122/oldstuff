//
//  minister.h
//  dsa_z2
//
//  Created by Martin Elias on 11/2/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef minister_h
#define minister_h

typedef struct _node{
	unsigned int idStret;
	unsigned int casOD;
	unsigned int casDO;
	unsigned long priorita;
	struct _node * parent;
	struct _node * left;
	struct _node * right;
}treap;

// Prototypy
int getPriority(void);
void insert_treap(unsigned int casOD, unsigned int casDO, unsigned int idStret);
void buble_up(treap * node);
void insert_id(treap * node, unsigned int id);
void ftreeinterval(unsigned int casOD, unsigned int casDO, treap * node);
void findinterval(unsigned int casOD, unsigned int casDO);
void ftreepoint(unsigned int cas, treap * node);
void quit(void);
void findpoint(unsigned int cas);
int comparator(const void * a, const void * b);
void lrotate(treap * node);
void rrotate(treap * n);
int buble_down(treap * node);
int delete_by_id(unsigned short cislo_schodzky);

// Globalne premmene
treap           * root      = NULL;
unsigned long   * pole_id   = NULL;
unsigned int    * id        = NULL;
unsigned int      max       = 0;
unsigned int      pocet     = 0;


#endif
