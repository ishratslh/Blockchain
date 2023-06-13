#include "partie2.h"

#ifndef PART3
#define PART3

typedef struct cellKey {
	Key * data ;
	struct cellKey * next ;
} CellKey ; //structure CellKey contenant une liste chainée de clés

typedef struct cellProtected{
	Protected* data;
	struct cellProtected *next;
} CellProtected;

typedef struct hashcell{
	Key* key;
	int val;
} HashCell ;

typedef struct hashtable{
	HashCell** tab;
	int size;
} HashTable ;

CellKey* create_cell_key(Key* key);

void ajout_en_tete(CellKey** cKey, Key* key);

CellKey* read_public_keys(char* nomfic);

void print_list_keys(CellKey* LCK);

void delete_cell_key(CellKey* c);

void delete_list_keys(CellKey* c);

CellProtected* create_cell_protected(Protected* pr);

void ajoute_en_tete_protected(CellProtected** cp, Protected*  p);

CellProtected* read_protected(char* file);

void print_list_protected(CellProtected* liste);

void delete_cell_protected(CellProtected* c);

void delete_list_protected(CellProtected* c);

void delete_no_valid(CellProtected** liste);

HashCell* create_hashcell(Key* key);

int hash_function(Key* key, int size);

int find_position(HashTable* t, Key* key);

HashTable* create_hashtable(CellKey* keys, int size);

void delete_hashtable(HashTable* t);

Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV);

#endif
