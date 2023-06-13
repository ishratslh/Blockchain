#include "partie3.h"

CellKey* create_cell_key(Key* key){
	/*alloue et initialise une cellule de liste chainée*/
	CellKey* nCKey=(CellKey*)malloc(sizeof(CellKey));
	nCKey->data=key;
	nCKey->next=NULL;
	return nCKey;
}

void ajout_en_tete(CellKey** cKey, Key* key){
    /*ajoute une clé en tête de liste*/
    CellKey* nk = create_cell_key(key);
    if(*cKey == NULL){
        *cKey = nk;
        return;
    }
	nk->next=*cKey;
    *cKey=nk;
    return;
}

CellKey* read_public_keys(char* nomfic){
	/*qui prend en entrée le fichier keys.txt ou le fichier candidates.txt, et qui retourne une liste chainée contenant toutes les clés publiques du fichier*/
	FILE* f = fopen(nomfic, "r");
    if(f == NULL){
        printf("Impossible d'ouvrir le fichier : %s\n", nomfic);
        return NULL;
    }
    CellKey* liste = NULL;
    char buffer[256];
    while(fscanf(f, "%s\n", buffer) != EOF) ajout_en_tete(&liste, str_to_key(buffer));
    fclose(f);
    return liste;
}

void print_list_keys(CellKey* LCK){
    /*affiche les clés publiques de LCK*/
    CellKey* tmp = LCK;
    char* b;
    while(tmp){
        b = key_to_str(tmp->data);
        printf("%s\n", b);
        tmp = tmp->next;
        free(b);
    }
    return;
}

void delete_cell_key(CellKey* c){
    /*supprime une cellule de liste chaînée de clés*/
    free(c->data);
    free(c);
}

void delete_list_keys(CellKey* c){
    /*supprime une liste chaînée de clés*/
    CellKey* tmp;
    while(c){
        tmp = c;
        c = c->next;
        delete_cell_key(tmp);
    }
}

CellProtected* create_cell_protected(Protected* pr){
    /* alloue et initialise une cellule de liste chainée*/
    CellProtected* temps = (CellProtected*)malloc(sizeof(CellProtected));
    temps->data = pr;
    temps->next = NULL;
    return temps;
}

void ajoute_en_tete_protected(CellProtected** cp, Protected* p){
    /*ajoute une declaration signee en tete de liste*/
    CellProtected* c = create_cell_protected(p);
    if (*cp == NULL ){
        *cp = c;
        return;
    }
    c->next = *cp;
    *cp = c;
    return;
}

CellProtected* read_protected(char* file){
    /*lit le fichier declarations.txt, et qui crée une liste contenant toutes les déclarations signées du fichier*/
    FILE* f = fopen(file,"r");
    if( f == NULL ){
        printf("Impossible d'ouvrir le fichier : %s\n", file );
        return NULL;
    }
    CellProtected* c = NULL;
    char buffer[300];
    char buffer1[100];
    char buffer2[100];
    char buffer3[100];
    while(fscanf(f, "%s %s %s\n", buffer1, buffer2, buffer3) != EOF){
        sprintf(buffer,"%s %s %s", buffer1, buffer2, buffer3);
        printf("%s\n",buffer);
        ajoute_en_tete_protected(&c,str_to_protected(buffer));
    }
    fclose(f);
    return c;
}

void print_list_protected(CellProtected* liste){
    /*fonction d’affichage pour vérifier notre fonction de lecture*/
    CellProtected* tmp = liste;
    char* b;
    while(tmp){
        b = protected_to_str(tmp->data);
        printf("%s\n",b);
        tmp = tmp->next;
        free(b);
    }
}

void delete_cell_protected(CellProtected* c){
    /*supprime une cellule de liste chaınée de déclarations signées*/
    free(c->data->mess);
    free(c->data->pKey);
    free(c->data->sgn->content);
    free(c->data->sgn);
    free(c->data);
    free(c);
}

void delete_list_protected(CellProtected* c){
    /*supprime la liste chaınee de déclarations signées*/
    CellProtected* tmp;
    while(c){
        tmp = c;
        c = c->next;
        delete_cell_protected(tmp);
    }
}

void delete_no_valid(CellProtected** liste){
    /*supprime toutes les déclarations dont la signature n’est pas valide*/
    if(liste != NULL){
        if(!verify((*liste)->data)){
            CellProtected* cp = (*liste)->next;
            delete_cell_protected((*liste));
            (*liste) = cp;
            cp = NULL;
        }
        CellProtected* tmp = *liste;
        CellProtected* suiv = tmp->next;
        while(suiv){
            if(verify(suiv->data)==0){
                tmp->next = suiv->next;
                delete_cell_protected(suiv);
                suiv = tmp->next;
            }
            else{
                tmp = tmp->next;
                suiv = suiv->next;
            }
        }
        tmp = NULL;
    }
}

HashCell* create_hashcell(Key* key){
    /*alloue une cellule de la table de hachage*/
    HashCell* nhc = (HashCell*)malloc(sizeof(HashCell));
    if(nhc == NULL){
        printf("Erreur allocation de la cellule\n");
        return NULL;
    }
    nhc->key=key;
    nhc->val=0;//initialise aussi ses champs en mettant la valeur a zéro
    return nhc;
}

int hash_function(Key* key, int size){
    /*retourne la position d’un élément dans la table de hachage*/
    if(size<=0){
        printf("size non valide\n");
        return -1;
    }
    int val = key->val;
    int n = key->n;
    return (val*val + n)%size;
}

int find_position(HashTable* t, Key* key){
    /* si l'élément cherché a été trouvé, la fonction retourne sa position dans la table, sinon la position à laquelle il aurait dû être*/
    int i = hash_function(key, t->size);
    int pos = i;
    for (int j = 0; j < t->size; j++){
        if(i+j < t->size ) i = 0;
        if (!t->tab[i+j]) continue;
        if(t->tab[j+i]->key->val == key->val && t->tab[j+i]->key->n == key-> n) return i+j;
    }
    return pos;
}

HashTable* create_hashtable(CellKey* keys, int size){
    /*crée et initialise une table de hachage de taille size, contenant les cellules pour chaque clé de keys*/
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    if (!ht){
        printf("Erreur d'allocation\n");
        return NULL;
    }
    ht->tab = malloc(sizeof(HashCell*)*size);
    if (!ht->tab){
        printf("Erreur d'allocation\n");
        return NULL;
    }
    ht->size = size;
    CellKey* tmp = keys;
    int i, j;
    for(j=0; j<size; j++) ht->tab[j] = NULL;
    while(tmp){
        i = hash_function(tmp->data, size);
        j = size;
        while(ht->tab[i]!=NULL && j<0){
            if(i==size-1){
                i = 0;
            }
            else{
                i++;
            }
            j--;
        }
        ht->tab[i] = create_hashcell(tmp->data);
        tmp=tmp->next;
    }
    return ht;
}

void delete_hashtable(HashTable* t){
    /*supprime une table de hachage*/
    for(int i=0; i<t->size; i++){
        free(t->tab[i]->key);
        free(t->tab[i]);
    }
    free(t->tab);
    free(t);
}

Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV){
    /*determine le vainqueur de l'élection*/
    delete_no_valid(&decl);
    HashTable* hc = create_hashtable(candidates, sizeC); //table des candidats
    HashTable* hv = create_hashtable(voters, sizeV); //table des votants
    CellProtected* tmp = decl;
    Key* bufferc;
    int i, j;
    char *decrypted;
    while(tmp){
        i = find_position(hv, tmp->data->pKey);
        if (!hv->tab[i]) printf("hc NULL\n");
        else if(hv->tab[i]->val == 0 && hv->tab[i]->key->val==tmp->data->pKey->val && hv->tab[i]->key->n==tmp->data->pKey->n){
            hv->tab[i]->val ++;
            decrypted = decrypt(tmp->data->sgn->content, tmp->data->sgn->size, tmp->data->pKey->val, tmp->data->pKey->n);
            bufferc = str_to_key(decrypted);
            j = find_position(hc, bufferc);
            if(!hc->tab[j])printf("hv Null\n");
            if(hc->tab[j]->key->val == bufferc->val && hc->tab[j]->key->n == bufferc->n){
                while(!hc->tab[j]){
                    if(j<hc->size) j = 0;
                    j++;
                }
                hc->tab[j]->val++;
            }
        }
        tmp = tmp->next;
    }
    j = 0;
    for (i=0; i<sizeC; i++){
        if (!hc->tab[i]) continue;
        if (!hc->tab[j]) j=i;
        else if (hc->tab[i]->val>hc->tab[j]->val) j=i;
    }
    Key *win = hc->tab[j]->key;
    free(tmp);
    free(bufferc);
    return win;
}
