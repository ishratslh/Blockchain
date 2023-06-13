#include "partie4.h"

void block_to_script(Block* b, char* nomfic){
    /*permet d’écrire dans un fichier un bloc*/
    FILE *f = fopen(nomfic,"w");
	if (f == NULL){
        printf("Impossible d'ouvrir le fichier %s \n", nomfic);
        return;
    }
    char *key = key_to_str(b->author);
    fprintf(f,"%s\n",key);
    free(key);
    fprintf(f,"%s\n",b->hash);
    fprintf(f,"%s\n",b->previous_hash);
    fprintf(f,"%d\n",b->nonce);
    CellProtected* decl = b->votes;
    char* bufferd;
    while (decl){
        bufferd = protected_to_str(decl->data);
        fprintf(f,"%s\n",bufferd);
        free(bufferd);
        decl=decl->next;
    }
    fprintf(f,"END\n");
    fclose(f);
}

Block* script_to_block(char* nomFic){
    /*permet de lire un bloc depuis un fichier*/
    Block* nBlock=(Block*)malloc(sizeof(Block));
    char buffer[256];

	FILE *f = fopen(nomFic,"r");
	if ( f == NULL ){
        printf("Impossible d'ouvrir le fichier %s \n", nomFic);
        return NULL ;
    }

    fgets(buffer,256,f);
    nBlock->author=str_to_key(buffer);

    fgets(buffer,256,f);
    nBlock->hash = (unsigned char*)strdup(buffer);

    fgets(buffer,256,f);
    nBlock->previous_hash = (unsigned char*)strdup(buffer);

    fgets(buffer,256,f);
    nBlock->nonce = atoi(buffer);

    fgets(buffer,256,f);
    Protected* pr;
    while(buffer[0] != 'E'){
        pr = str_to_protected(buffer);
        ajoute_en_tete_protected(&nBlock->votes,pr);
        fgets(buffer,256,f);
    }

    fclose(f);
    return nBlock;
}

char* block_to_str(Block* block){
    /*génère une chaîne de caractères représentant un bloc*/
    char* key = key_to_str(block->author);
    int length = strlen(key);
    CellProtected* decl = block->votes;
    while(decl){
        char* pr = protected_to_str(decl->data);
        length += strlen(pr)+1;
        free(pr);
        decl = decl->next;
    }
    char num[100];
    sprintf(num,"%d",block->nonce);
    length += strlen(num)+1;
    char* str = (char*)malloc((length+strlen((char*)block->previous_hash)+10)*sizeof(char));
    strcpy(str,key);
    free(key);
    strcat(str," ");
    strcat(str,(char*)block->previous_hash);
    strcat(str," ");
    decl = block->votes;
    while(decl){
        char* pr = protected_to_str(decl->data);
        strcat(str,pr);
        strcat(str," ");
        free(pr);
        decl = decl->next;
    }
    strcat(str,num);
    return str;
}

unsigned char* valhach(char* mess){
    /*retourne la valeur hachée de mess obtenue par l'algorithme SHA256*/
    unsigned char* val = SHA256((const unsigned char*)mess,strlen((char*)mess),0);
    char* res = (char*)malloc((SHA256_DIGEST_LENGTH*2+2)*sizeof(char));
    if (res == NULL) {
        printf("Erreur d allocation");
    }
    res[0] = '\0';
    char buffer[10];
    for(int i=0; i<SHA256_DIGEST_LENGTH; i++){
        sprintf(buffer,"%x02",val[i]);
        strcat(res,buffer);
    }
    return (unsigned char*)res;
}

void compute_proof_of_work(Block *B, int d){
    /*rend un bloc valide*/
    B->nonce=0;

    if (B->hash == NULL) {
        char *str = block_to_str(B);
        B->hash = valhach(str);
        free(str);
    }

    unsigned char buffer[256];
    unsigned char nonce[128];
    int cpt = 0, i = 0;

    sprintf(nonce, "%d", B->nonce);
    strcpy(buffer, B->hash);
    strcat(buffer, nonce);
    unsigned char *valh = valhach(buffer);


    // Compte le nombre de zeros successif de la valeur hachee
    while (cpt < d){
        // On incremente le compteur si on trouve un zero
        if (valh[i] == '0') {
            i++;
            cpt++;
        }

      // Sinon on reinitialise le compteur et on incremente B->nonce
      else {
            B->nonce++;
            i = 0;
            cpt = 0;
            free(valh);
            sprintf(nonce, "%d", B->nonce);
            strcpy(buffer, B->hash);
            strcat(buffer, nonce);
            valh = valhach(buffer);
        } 
    }
    //printf("test : %d %s\n", B->nonce, valh);
    free(valh);
}

int verify_block(Block* b, int d){
    /*vérifie si un bloc est valide*/
    unsigned char* str = valhach(block_to_str(b));
    for(int i= 0;i<d;i++){
        if(str[i]!='0') return 0; 
    }
    free(str);
    return 1;
}

void delete_block(Block* b){
    /*supprime un bloc*/
    free(b->hash);
    free(b->previous_hash);
    delete_list_protected(b->votes);
    free(b);
}
