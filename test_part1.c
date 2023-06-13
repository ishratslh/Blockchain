#include "partie1.h"
#include <time.h>

void print_long_vector(long *result, int size){
    printf ("Vector : [ ") ;
    for(int i=0; i<size;i++){
        printf ("%lx ", result[i]) ;
    }
    printf ("] \n") ;
}

int main(){
    /*
    //Batterie de tests 1
    FILE *f = fopen("modpow.txt","w");
    clock_t temps_initial,temps_final;
    double temps_cpu = 1;
    long p = 1;
    long a = 654316;
    long n = 7;
    for(long m=0; m<50000; m++){
        temps_initial = clock();
        modpow_naive(a,m,n);
        temps_final = clock();
        temps_cpu = (double)(temps_final - temps_initial)/CLOCKS_PER_SEC;
        fprintf(f,"%ld\t%f\t",m,temps_cpu);
        temps_initial = clock();
        modpow(a,m,n);
        temps_final = clock();
        temps_cpu = (double)(temps_final - temps_initial)/CLOCKS_PER_SEC;
        fprintf(f,"%f\n",temps_cpu);
    }
    fclose(f);
    while(temps_cpu < 0.002){
        temps_initial = clock();
        int b = is_prime_naive(p);
        temps_final = clock();
        temps_cpu = (double)(temps_final - temps_initial)/CLOCKS_PER_SEC;
        printf("p = %ld, est premier : %d temps_cpu = %f\n",p,b,temps_cpu);
        p+=2;
    }
    */
    //Batterie de tests 2
	srand(time(NULL));
	//Generation de cle :
	long p = random_prime_number(3, 7, 5000);
	long q = random_prime_number(3, 7, 5000);
	while (p == q) {
		q = random_prime_number(3, 7, 5000);
	}
	long n, s, u ;
	generate_key_values(p, q, &n, &s, &u) ;
	//Pour avoir des cles positives :
	if(u <0){
		long t = (p-1)*(q-1);
		u=u+t; //on aura toujours s*u mod t = 1
	}

	//Afichage des cles en hexadecimal
	printf("clé publique = (%lx, %lx) \n", s, n);
	printf("clé privee = (%lx , %lx) \n", u, n);

	//Chiffrement:
	char mess[10] =  "Hello";
	int len = strlen ( mess ) ;
	long *crypted = encrypt(mess, s, n);

	printf ("Initial message : %s \n", mess);
	printf ("Encoded representation: \n");
	print_long_vector(crypted, len);

	//Dechiffrement
	char *decoded = decrypt(crypted, len, u, n) ;
	printf ("Decoded : %s \n", decoded);

    return 0;
}