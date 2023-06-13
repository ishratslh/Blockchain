#include "partie2.h"
#include <time.h>

void print_long_vector(long *result, int size){
    printf ("Vector : [ ") ;
    for(int i=0; i<size;i++){
        printf ("%lx ", result[i]) ;
    }
    printf ("] \n") ;
}

int main(){
    //Batterie de tests 3
    srand(time(NULL));
    char* buffer1;
    char* buffer2;

    //Testing Init Keys
    Key* pKey = malloc(sizeof(Key));
    Key* sKey = malloc(sizeof(Key));
    init_pair_keys(pKey,sKey,3,7);
    printf("pKey: %lx, %lx \n",pKey->val, pKey->n);
    printf("sKey: %lx, %lx \n",sKey->val, sKey->n);

    //Testing Key Serialization
    char* chaine = key_to_str(pKey);
    printf("key_to_str: %s \n",chaine);
    Key* k = str_to_key(chaine);
    printf("str_to_key: %lx, %lx \n", k->val, k->n);
    free(chaine);
    free(k);

    //Testing signature
    //Candidate keys:
    Key* pKeyC = malloc(sizeof(Key));
    Key* sKeyC = malloc(sizeof(Key));
    init_pair_keys(pKeyC, sKeyC, 3, 7);
    //Declaration:
    char* mess = key_to_str(pKeyC);
    buffer1 = key_to_str(pKey);
    printf("%s vote pour %s\n", buffer1, mess);
    free(buffer1);
    Signature* sgn = sign(mess, sKey);
    printf("signature :");
    print_long_vector(sgn->content, sgn->size);
    chaine=signature_to_str(sgn);
    printf("signature_to_str: %s\n",chaine);
    sgn=str_to_signature(chaine);
    printf("str_to_signature: ");
    print_long_vector(sgn->content, sgn->size);
    free(chaine);

	//Testing protected:
	Protected* pr = init_protected(pKey,mess,sgn);
	//Verification:
	if (verify(pr)){
		printf("Signature valide\n");
	}else{
		printf("Signature non valide\n");
	}
	chaine = protected_to_str(pr);
	printf ("protected_to_str: %s\n", chaine);
	pr = str_to_protected(chaine);
    buffer1 = key_to_str(pr->pKey);
    buffer2 = signature_to_str(pr->sgn);
	printf ("str_to_protected: %s %s %s \n", buffer1, pr->mess, buffer2);
    free(buffer1);
    free(buffer2);
    generate_random_data(20,10);
    free(chaine);
    free(mess);
    free(sgn->content);
    free(sgn);
    free(pr);
	free(pKey);
	free(sKey);
	free(pKeyC);
	free(sKeyC);

    return 0;
}