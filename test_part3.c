#include "partie3.h"

int main(){
    //CellKey
    printf("CellKey :\n");
    CellKey* ck = read_public_keys("candidates.txt");
    print_list_keys(ck);
    delete_list_keys(ck);

    //CellProtected
    printf("CellProtected :\n");
    CellProtected* cp = read_protected("declarations.txt");
    print_list_protected(cp);
    delete_list_protected(cp);

    //emulation elections
    CellKey* listeVoters = read_public_keys("keys.txt");
    CellKey* listeCandidates = read_public_keys("candidates.txt");
    CellProtected* listeDeclaration = read_protected("declarations.txt");
    Key* Winner = compute_winner(listeDeclaration, listeCandidates, listeVoters, 20, 40);
    char* win = key_to_str(Winner);
    printf("L'elu est %s\n",win);
    free(win);
    delete_list_keys(listeVoters);
    delete_list_keys(listeCandidates);
    delete_list_protected(listeDeclaration);

    return 0;
}