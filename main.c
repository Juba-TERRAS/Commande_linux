#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <malloc.h>
#include "fonctions.h"
#include <string.h>

int main(int argc, char *argv[]) {
    //===============================================================================
    //Variables de manipulation des fichiers
    char commande[255];
    DIR *dir;
    struct commande_recherche *P = NULL;
    int erreur = 0;
    //===============================================================================
    //Debut du programme
    debut :
    input(commande);//saisir la commande
    P = malloc(sizeof(struct commande_recherche));
    erreur = 0;
    if (strlen(commande) < 5) {/*Si la longueur de la commande est inferieur a 5 caracteres donc automatiquement
                elle est erronée*/
        printf("\nErreur de commande. Inserez la commande search!.\n");
        goto fin;
    }
    //Extraire tous les champs de la commande
    extract_commande(commande, P, &erreur);
    if (erreur == -1) {/*Si la commande est invalide*/
        printf("\nErreur commande non reconnue.\n");
        goto fin;
    }
    //Ouverture du repertoire
    dir = opendir(P->repertoire);
    if (dir == NULL) {/*Si l'ouverture du repertoire a échouée*/
        printf("\nErreur. Commande non reconnue ou le repertoire cible n'existe pas.\n");
        goto fin;
    } else {
        if (P->rech_repertoire == 0 && P->rech_un_niveau == 0) {//Si il s'agit d'une recherche a n niveaux
            printf("\n---------------------------------------------------------------------------------------------------\n");
            printf("|  Chemin absolu               | Date derniere modification | Taille(octects) | Type | Protection |\n");
            printf("---------------------------------------------------------------------------------------------------");
            recur(P->repertoire, P);
            printf("\n---------------------------------------------------------------------------------------------------");
        } else { /*Si il s'agit d'une recherche dans le repertoire courant ou d'une recherche a un niveau*/
            module_recherche(dir, P);
        }
    }
    fin :
    goto debut;
    return 0;
}
