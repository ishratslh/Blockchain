#include "partie1.h"


int is_prime_naive(long p){ //complexite O(p)
    /* renvoie 1 si p est premier et 0 sinon */
    for(long i=3; i<p; i++){
        if(p%i == 0) return 0;
    }
    return 1;
}

long modpow_naive(long a, long m, long n){
	/* retourne la valeur a^b mod n par la m´ethode naive*/
    int pow = a;
    for(int i=0; i<m; i++){
        pow = pow*a;
    }
    return pow%n;
}

int modpow(long a, long m, long n){
	/* réalisant cette succession d'élévations au carré*/
    if( m == 0 ) return 1;
    long b;
    if ( m % 2 == 0 ){
        b = modpow( a, m / 2, n );
        return b * b % n;
    }
    else{
        b = modpow ( a, floor( m / 2 ), n );
        return a * b * b % n;
    }
}

int witness(long a, long b, long d, long p){
	/*teste si a est un témoin de Miller pour p, pour un entier a donné*/
    long x = modpow(a,d,p);
    if(x==1){
        return 0;
    }
    for(long i=0; i<b; i++){
        if(x==p-1){
            return 0;
        }
        x = modpow(x,2,p);
    }
    return 1;
}

long rand_long(long low, long up){
	/*retourne un entier long généré aléatoirement entre low et up inclus*/
    return rand()%(up-low+1)+low;
}

int is_prime_miller(long p, int k){
	/*réalise le test de Miller-Rabin en générant k valeurs de a au hasard, et en testant si chaque valeur de a est un témoin de Miller pour p*/
    if(p==2){
        return 1;
    }
    if(!(p&1)||p<=1){
        return 0; //un témoin de Miller est trouvé (p n'est pas premier)
    }
    long b=0;
    long d=p-1;
    while(!(d&1)){
        d=d/2;
        b++;
    }
    long a;
    for(int i=0; i<k; i++){
        a=rand_long(2,p-1);
        if(witness(a,b,d,p)){
            return 0; //un témoin de Miller est trouvé
        }
    }
    return 1; //si aucun témoin de Miller n’a été trouvé (p est très probablement premier)
}

long random_prime_number(int low_size, int up_size, int k){
	/*retourne un nombre premier de taille comprise entre low size et up size (k=nombre de tests de Miller)*/
    long num;
    while(1){
        num = rand_long( pow( 2, low_size - 1 ), pow( 2, up_size )- 1 );
        if(is_prime_miller(num,k)) return num;
    }
}

long extended_gcd(long s, long t, long *u, long *v){
    if (s==0){
        *u = 0;
        *v = 1;
        return t;
    }
    long uPrim,vPrim;
    long gcd = extended_gcd(t%s, s, &uPrim, &vPrim);
    *u = vPrim - (t/s)*uPrim;
    *v = uPrim;
    return gcd;
}

void generate_key_values(long p, long q, long* n, long* s, long* u){
    long t = (p-1)*(q-1);
    long v = 0;
    *s=rand_long(0,t);
    *n=p*q;
    while(extended_gcd(*s,t,u,&v)!=1){
        *s=rand_long(0,t);
    }
}

long* encrypt(char* chaine, long s, long n){
    /*chiffre la chaine de caractères chaine avec pKey*/
    int taille = strlen( chaine );
    long* resultat = (long*) malloc(sizeof(long)*taille);
    if (!resultat){
        fprintf(stderr, "Erreur d'allocation\n");
        return NULL;
    }
    for (int i = 0; i < taille; i++ ) resultat[i] = modpow(((long)chaine[i]), s, n );
    return resultat;
}

char* decrypt(long* crypted, int size, long u, long n){
    /*déchiffre un message à l'aide de la clé secrète sKey*/
    char* message = (char*)malloc(sizeof(char)*(size+1));
    if(!message){
        printf("Erreur d'allocation\n");
        return NULL;
    }
    for(int i = 0; i < size; i++) message[i] = (char)(modpow(crypted[i], u, n ));
    message[size] = '\0';
    return message;
}