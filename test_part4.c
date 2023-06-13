#include "partie4.h"
#include <time.h>

int main(){
    //verification installation
    const char *s = "Rosetta code";
    unsigned char *d = SHA256((const unsigned char*)s,strlen(s),0);
    int i;
    for(i=0;i<SHA256_DIGEST_LENGTH; i++) printf("%02x",d[i]);
    putchar('\n');
    unsigned char* vc = valhach((char*)s);
    for(i=0;i<SHA256_DIGEST_LENGTH; i++) printf("%02x",vc[i]);
    putchar('\n');

    //mesure temps compute_proof_of_work
    FILE *f = fopen("compute_pow","w");
    clock_t temps_initial,temps_final;
    double temps_cpu;
    Block* b = script_to_block("block.txt");
    int nb0 = 1;
    while(temps_cpu < 1){
        temps_initial = clock();
        compute_proof_of_work(b,nb0);
        temps_final = clock();
        temps_cpu = (double)(temps_final - temps_initial)/CLOCKS_PER_SEC;
        nb0++;
    }
    fclose(f);
    return 0;
}