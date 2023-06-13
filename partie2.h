#include "partie1.h"
#include <string.h>

#ifndef PART2
#define PART2

typedef struct key {
    long val;
	long n;
} Key ; //structure Key contenant deux long representant une clé (publique ou secrète)

typedef struct signature {
    long size;
    long *content; //ou content[longueur]?
} Signature ; //structure Signature contenant un tableau de long dont on connait la longueur

typedef struct protected {
	Key* pKey;
	char* mess;
	Signature* sgn;
} Protected ; //structure Protected contenant la clé publique l'électeur, son message, et la signature associée


void init_key(Key* key, long val, long n);

void init_pair_keys(Key* pKey, Key* sKey, long low_size, long up_size);

char* key_to_str(Key* key);

Key* str_to_key(char* str);

Signature* init_signature(long* content, int size);

Signature* sign(char* mess, Key* sKey);

char* signature_to_str(Signature* sgn);

Signature* str_to_signature(char* str);

Signature* init_signature(long* content, int size);

Signature* sign(char* mess, Key* sKey);

Protected* init_protected(Key* pKey, char* mess, Signature* sgn) ;

int verify(Protected* pr);

char* protected_to_str(Protected* pr);

Protected* str_to_protected (char* mess);

void generate_random_data(int nv, int nc);

#endif