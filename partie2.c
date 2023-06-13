#include "partie2.h"

void init_key(Key* key, long val, long n){
    /*initialise une clé déjà allouée*/
    key->val=val;
    key->n=n;
}

void init_pair_keys(Key* pKey, Key* sKey, long low_size, long up_size){
    /*utilise le protocole RSA pour initialiser une clé publique et une clé secrète*/
    long n, s, u ;
    long p = random_prime_number(low_size, up_size, 5000);
    long q = random_prime_number(low_size, up_size, 5000);
    while (p == q) {
        q = random_prime_number(low_size, up_size, 5000);
    }

    generate_key_values(p, q, &n, &s, &u);

    if (u < 0){
		long t = (p - 1)*(q - 1);
	    u = u + t;
	}

    init_key(pKey, s, n);
    init_key(sKey, u, n);

    /*//Affichage des clés
    printf("pKey = (%lx,%lx) \n", s, n);
    printf("sKey = (%lx,%lx) \n", u, n);*/
}

char* key_to_str(Key* key){
    /*passe d’une variable de type Key à sa représentation sous forme de chaine de caractères*/
    char* resultat = (char*)malloc(sizeof(char));
    long val=key->val;
    long n=key->n;
    sprintf(resultat, "(%lx,%lx)", val, n);
    return resultat;
}

Key* str_to_key(char* str){
    /*passe d’une représentation sous forme de chaine de caractères à sa variable de type Key */
    Key* nKey=(Key*)malloc(sizeof(Key));
    long val, n;

    sscanf(str, "(%lx,%lx)", &val, &n);
    init_key(nKey, val, n);
    return nKey;
}


Signature* init_signature(long* content, int size){
	/*alloue et remplit une signature avec un tableau de long déjà alloué et initialisé*/
    Signature* nSignature=(Signature*)malloc(sizeof(Signature));
    nSignature->content=content;
    nSignature->size=size;
    return nSignature;
}

Signature* sign(char* mess, Key* sKey){
	/* crée une signature à partir du message mess  et de la clé secrète de l'émetteur*/
    int size=strlen(mess);

    long u=sKey->val;
    long n=sKey->n;
    long* content = encrypt(mess, u, n);

    return init_signature(content, size);
}

char* signature_to_str(Signature* sgn){
	/* passe d’une Signature à sa représentation sous forme de chaine de caractères*/
	char* result = malloc(10*sgn->size*sizeof(char));
	result[0]= '#';
	int pos = 1;
	char buffer[156];
	for(int i=0; i<sgn->size; i++){
		sprintf(buffer, "%lx", sgn->content[i]);
		for(int j=0; j<strlen(buffer); j++){
			result[pos] = buffer[j];
			pos = pos+1;
		}
		result[pos] = '#';
		pos = pos+1;
	}
	result[pos] = '\0';
	result = realloc(result,(pos+1)*sizeof(char));
	return result;
}

Signature* str_to_signature(char* str){
	/* passe d’une représentation sous forme de chaine de caractères à sa Signature*/
	int len = strlen(str);
	long* content = (long*)malloc(sizeof(long)* len);
	int num = 0;
	char buffer[256];
	int pos = 0;
	for (int i=0; i<len; i++){
		if (str[i] != '#'){
			buffer[pos] = str[i];
			pos = pos+1;
		}else{
			if (pos != 0){
				buffer[pos] = '\0';
				sscanf (buffer, "%lx ", &(content[num]) );
				num = num+1;
				pos = 0;
			}
		}
	}
	content = realloc(content, num*sizeof(long));
	return init_signature(content, num);
}

Protected* init_protected(Key* pKey, char* mess, Signature* sgn){
	/*alloue et initialise cette structure*/
	Protected* nProtected=(Protected*)malloc(sizeof(Protected));
	nProtected->pKey=pKey;
	nProtected->mess=strdup(mess);
	nProtected->sgn=sgn;
	return nProtected;
}

int verify(Protected* pr){
	/*verifie que la signature contenue dans pr correspond bien au message et a la personne contenus dans pr*/
	if (strcmp(decrypt(pr->sgn->content,pr->sgn->size,pr->pKey->val,pr->pKey->n),pr->mess)==0){
		return 1;
	}else{
		return 0;
	}
}

char* protected_to_str(Protected* pr){
	/* passe d'un Protected a sa représentation sous forme de chaine de caractères*/
    char* buffer = (char*)malloc(sizeof(char)*256);
    char* key = key_to_str(pr->pKey) ;
    char* sgn = signature_to_str(pr->sgn) ;
    sprintf(buffer,"%s %s %s",key,pr->mess,sgn);
    free(key);
    free(sgn);
    return buffer;
}

Protected* str_to_protected(char* mess){
	/* passe d'une représentation sous forme de chaine de caractères à son Protected*/
    char key[256],str[256],sign[256];
	sscanf(mess,"%s %s %s",key,str,sign);
	return init_protected(str_to_key(key), strdup(str), str_to_signature(sign));
}

void generate_random_data(int nv, int nc){
    /*simule le processus de vote à l’aide de trois fichiers: keys.txt, candidates.txt et declarations.txt*/
	int i, rnd;
    Key* pTable[nv];
    Key* sTable[nv];
    Key *pKey, *sKey;
    Key *candidats[nc];
    Signature *sgn;
    Protected *pr;

    FILE *fk = fopen("keys.txt","w");
    if( fk == NULL ){
        printf("Impossible d'ouvrir le fichier : %s\n", "keys.txt");
        return;
    }
    for(i=0; i<nv; i++){
        pKey = (Key*) malloc(sizeof(Key));
        sKey = (Key*) malloc(sizeof(Key));
        init_pair_keys(pKey, sKey, 3, 7);
        pTable[i] = pKey;
        sTable[i] = sKey;
        char* buffer1 = key_to_str(pKey);
        char* buffer2 = key_to_str(sKey);
        fprintf(fk, "%s\t%s\n", buffer1, buffer2);
        free(buffer1);
        free(buffer2);
    }
    fclose(fk);

    FILE *fc = fopen( "candidates.txt" , "w" );
    if(fc == NULL){
        printf("Impossible d'ouvrir le fichier : %s\n","candidates.txt" );
        return;
    }
    for (i=0; i<nc; i++){
        rnd = (int)rand()%nv;
        candidats[i] = pTable[rnd];
        char *buffer1 = key_to_str(candidats[i]);
        fprintf(fc, "%s\n", buffer1);
        free(buffer1);
    }
    fclose(fc);

    FILE* fd = fopen("declarations.txt", "w");
    if(fd == NULL){
        printf("Impossible d'ouvrir le fichier : %s\n", "declarations.txt");
        return ;
    }
    for ( i = 0 ; i < nv; i++){
        rnd = (int) rand() % nc ;
        char *buffer = key_to_str(candidats[rnd]);
        sgn = sign(buffer, sTable[i]);
        pr = init_protected(pTable[i],buffer,sgn);
        free(buffer);
        buffer = protected_to_str(pr);
        fprintf(fd, "%s\n", buffer);
        free(buffer);
        free(sgn->content);
        free(sgn);
        free(pr);
    }
    fclose(fd);

    for (i=0; i<nv; i++){
        free(pTable[i]);
        free(sTable[i]);
    }

    for(i=0; i<nc; i++) free(candidats[i]);
}
