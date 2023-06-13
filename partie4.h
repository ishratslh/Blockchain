#include "partie3.h"
#include <openssl/sha.h>

typedef struct block {
    Key* author ; //cle publique du createur
    CellProtected* votes ; //list declarations de vote
    unsigned char* hash ; //valeur hachee du bloc
    unsigned char* previous_hash ; //valeur hachee du precedent
    int nonce ; //preuve de travail
} Block ;

//Ex7 – Structure d’un bloc et persistance
void block_to_script(Block* b, char* nomfic);

Block* script_to_block(char* nomFic);

char* block_to_str(Block* block); 

unsigned char* valhach(char* mess);

void compute_proof_of_work(Block *B, int d);

int verify_block(Block*, int d);

void delete_block(Block* b);
