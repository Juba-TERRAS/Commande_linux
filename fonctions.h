#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <malloc.h>
#include <time.h>

#ifndef TP_SYSTEME_FONCTIONS_H
#define TP_SYSTEME_FONCTIONS_H
#endif

/*=======================================================================================*/
struct commande_recherche {//Structure de données qui regroupes tous les champs d'une commande search
    char repertoire[255];
    char nom_fichier[255];
    int rech_repertoire;
    int rech_un_niveau;
    int rech_n_niveau;
    int rech_m;
    int rech_s;
    int rech_t;
    int rech_p;
    int rech_a;
};
/*=======================================================================================*/
void input(char str[]){
    int ch,i=0;
    printf("\nSaisir la commande de recherche : ");
    while((ch=getchar())!='\n'){
        str[i++]=ch;
    }
    str[i]='\0';
}
/*=======================================================================================*/
void heure_modif(time_t temp) {/*Fonction de transformation d'un champs de type time_t en format date:heure*/
    struct tm *time;
    char buffer[200];
    time = localtime(&temp);
    strftime(buffer, sizeof(buffer), "%d.%m.%Y %H:%M:%S", time);
    printf("     %s", buffer);
}
/*=======================================================================================*/
void type_fichier(struct stat *st) {/*Fonction pour savoir le type d'un fichier (Fichier ou repertoire)*/
    if (S_ISDIR(st->st_mode) == 1) {
        printf("     Repertoire");
    } else {
        printf("     Fichier");
    }
}
/*=======================================================================================*/
int compare_chaine(char fich[255], char fich_rep[255]) {/*Fonction qui permet de comparer deux chaines de caracteres
 contenant toutes deux le caractere '?' . Retourne 1 si les deux chaines en entrée sont egales sinon retourne 0*/
    int inter = 1;
    int i = 0;
    while (inter == 1 && fich_rep[i] != '\0') {
        if (fich_rep[i] == '?' || fich[i] == '?') {
        } else if (fich_rep[i] != fich[i]) {
            inter = 0;
        }
        i++;
    }
    return inter;
}
/*=======================================================================================*/
int compare_chaine_replace(char fich[255], char fich_rep[255]) {/*Fonction qui permet de comparer deux chaines de caracteres
 contenant toutes deux le caractere '*' . Retourne 1 si les deux chaines en entrée sont egales sinon retourne 0*/
    int inter = 1;
    int i = 0;
    while (inter == 1 && fich_rep[i] != '\0' && fich_rep[i] != '*') {
        if (fich_rep[i] != fich[i]) {
            inter = 0;
        }
        i++;
    }
    return inter;
}
/*=======================================================================================*/
int extract(int from, int to, char *chaine, char *sousChaine) {/*Fonction qui permet d'extraire une sous chaine
 d'une chaine de caractére a partir d'un index de debut qu'on appellera 'from' a index de fin qu'on appellera 'to'.
 Retourne 1 si l'operation a echoué sinon 0*/
    int i, j;
    //récupérer la longueur de la chaîne
    int length = strlen(chaine);

    if (from > length || from < 0) {;
        return 1;
    }
    if (to > length) {
        return 1;
    }
    for (i = from, j = 0; i <= to; i++, j++) {
        sousChaine[j] = chaine[i];
    }
    return 0;
}
/*=======================================================================================*/
void affichage_structure(struct commande_recherche *P) {/*Fonction d'affichage des differents champs d'unc commande*/
    printf("\nRepertoire : %s\n", P->repertoire);
    printf("Nom fichier :%s\n", P->nom_fichier);
    printf("rep: %d\n", P->rech_repertoire);
    printf("un_niveau : %d\n", P->rech_un_niveau);
    printf("n_niveau : %d\n", P->rech_n_niveau);
    printf("-a : %d\n", P->rech_a);
    printf("-m : %d\n", P->rech_m);
    printf("-s : %d\n", P->rech_s);
    printf("-t : %d\n", P->rech_t);
    printf("-p : %d\n", P->rech_p);
}
/*=======================================================================================*/
int extract_commande(char commande[255], struct commande_recherche *P, int *erreur) {/*Fonction qui permet d'extraire
 * tous les champs d'unc commande search(Repertoire+Nom du fichier+Les options)*/
    char c;
    int i = 7;
    int j = 0;
    int cpt = 0;
    char *com = malloc(sizeof(char) * 255);
    //Initisialisation des champs d'options de la commandes
    strcpy(P->nom_fichier, "");
    strcpy(P->repertoire, "");
    P->rech_repertoire = 0;
    P->rech_un_niveau = 0;
    P->rech_n_niveau = 0;
    P->rech_m = 0;
    P->rech_s = 0;
    P->rech_t = 0;
    P->rech_p = 0;
    P->rech_a = 0;
    //Extraction de la commande insérée. Retourner une erreur si la commande est differente de search*/
    extract(0, 5, commande, com);
    if (strcmp(com, "search") != 0) {
        *erreur = -1;
        return 0;
    }
    //Extraction du nom de repertoire
    if (commande[i] == '-') {
        strcpy(P->repertoire, getcwd(P->repertoire,sizeof (P->repertoire)));
        strcat(P->repertoire,"/");
    } else {
        while (commande[i] != ' ' && commande[i] != '\0') {
            P->repertoire[j] = commande[i];
            i++;
            j++;
        }
        P->repertoire[j] = '\0';
        i++;
    }
    //Extraction des options de la commande
    //L'option -0
    c = strstr(commande, "-0");
    if (c != NULL) {
        P->rech_repertoire = 1;
        cpt++;
    } else {
        //L'option -1
        c = strstr(commande, "-1");
        if (c != NULL) {
            P->rech_un_niveau = 1;
            cpt++;
        } else {
            //L'option -n
            c = strstr(commande, "-n");
            if (c != NULL) {
                P->rech_n_niveau = 1;
                cpt++;
            }
        }
    }
    //L'option -m
    c = strstr(commande, "-m");
    if (c != NULL) {
        P->rech_m = 1;
        cpt++;
    }
    //L'option -s
    c = strstr(commande, "-s");
    if (c != NULL) {
        P->rech_s = 1;
        cpt++;
    }
    //L'option -t
    c = strstr(commande, "-t");
    if (c != NULL) {
        P->rech_t = 1;
        cpt++;
    }
    //L'option -p
    c = strstr(commande, "-p");
    if (c != NULL) {
        P->rech_p = 1;
        cpt++;
    }
    //L'option -a
    c = strstr(commande, "-a");
    if (c != NULL) {
        P->rech_a = 1;
        cpt++;
    }
    //Deplacer le curseur jusqu'au nom de fichier
    i = i + cpt * 3;
    j = 0;
    //Extraction du nom du fichier
    while (commande[i] != ' ' && commande[i] != '\0') {
        P->nom_fichier[j] = commande[i];
        i++;
        j++;
    }
    P->nom_fichier[j] = '\0';
    return 0;
}
/*=======================================================================================*/
void affichage_info_fichier(struct stat *P, struct commande_recherche *Q) {/*Fonction d'affichage des caracteristiques
 d'un fichier (heure de derniere modification, Taille, type, protection) */
    if (Q->rech_m == 1 || Q->rech_a == 1) {
        heure_modif(P->st_mtime);
    } else {
        printf("                *              ");
    }
    if (Q->rech_s == 1 || Q->rech_a == 1) {
        printf("     %ld", P->st_size);
    } else {
        printf("      *       ");
    }
    if (Q->rech_t == 1 || Q->rech_a == 1) {
        type_fichier(P);
    } else {
        printf("      *       ");
    }
    if (Q->rech_p == 1 || Q->rech_a == 1) {
        printf("     %d", P->st_mode);
    } else {
        printf("      *       ");
    }
}
/*=======================================================================================*/
void module_recherche(DIR *dir, struct commande_recherche *P) {/*Procedure qui permet d'effectuer une recherche
 * soit dans le repertoire courant soit dans un niveau de l'arborescence*/
    printf("\n---------------------------------------------------------------------------------------------------\n");
    printf("|  Chemin absolu               | Date derniere modification | Taille(octects) | Type | Protection |\n");
    printf("---------------------------------------------------------------------------------------------------");
    //Inisialisation des differentes variables utilisées*/
    struct dirent *fich = malloc(sizeof(struct dirent));
    struct dirent *fich2 = malloc(sizeof(struct dirent));
    DIR *dir2;
    struct stat *st = malloc(sizeof(struct stat));
    char chemin_absolu[255];
    //Si la recherche se fait uniquement dans le repertoire courant
    if (P->rech_repertoire == 1) {
        while ((fich = readdir(dir)) != NULL) {
            //Comparaison des noms des fichiers du repertoire courant avec le nom du fichier recherché
            if (compare_chaine_replace(fich->d_name, P->nom_fichier) == 1 ||
                compare_chaine(fich->d_name, P->nom_fichier) == 1 || (strcmp(fich->d_name, P->nom_fichier) == 0)) {
                strcpy(chemin_absolu, "");
                strcat(chemin_absolu, P->repertoire);
                strcat(chemin_absolu, fich->d_name);
                printf("\n%s", chemin_absolu);
                stat(chemin_absolu, st);
                affichage_info_fichier(st, P);
            }
        }
        //Si la recherche se fait dans le repertoire courant et dans un niveau de plus dans l'arborescence
    } else if (P->rech_un_niveau == 1) {
        while ((fich = readdir(dir)) != NULL) {
            strcpy(chemin_absolu, "");
            strcat(chemin_absolu, P->repertoire);
            strcat(chemin_absolu, fich->d_name);
            stat(chemin_absolu, st);
            //Si le fichier du repertoire courant est un repertoire alors rechercher dans le repertoire
            if (S_ISDIR(st->st_mode) == 1) {
                dir2 = opendir(chemin_absolu);
                if (dir2 == NULL) {
                    perror("**Chemin invalid**");
                    return;
                }
                while ((fich2 = readdir(dir2)) != NULL) {
                    /*Comparaison des noms des fichiers du premier niveau de l'arborescence avec
                    le nom du fichier recherché*/
                    if (((compare_chaine_replace(fich2->d_name, P->nom_fichier) == 1) ||
                         (compare_chaine(fich2->d_name, P->nom_fichier) == 1) ||
                         (strcmp(fich2->d_name, P->nom_fichier) == 0)) && (strcmp(fich->d_name, ".") != 0) &&
                        (strcmp(fich->d_name, "..") != 0)) {
                        strcat(chemin_absolu, "/");
                        strcat(chemin_absolu, fich2->d_name);
                        printf("\n%s", chemin_absolu);
                        stat(chemin_absolu, st);
                        affichage_info_fichier(st, P);
                    }
                }
                //Si le fichier du repertoire courant alors comparer son nom avec le nom du fichier recherché
            } else {
                if ((compare_chaine_replace(fich->d_name, P->nom_fichier) == 1) ||
                    (compare_chaine(fich->d_name, P->nom_fichier) == 1) ||
                    (strcmp(fich->d_name, P->nom_fichier) == 0)) {
                    printf("\n%s", chemin_absolu);
                    stat(chemin_absolu, st);
                    affichage_info_fichier(st, P);
                }
            }
        }
    }
    printf("\n---------------------------------------------------------------------------------------------------");
}
/*=======================================================================================*/
void recur(char *s, struct commande_recherche *P) {/*Fonction qui permet de rechercher un fichier dans le repertoire
courant ansi que tous ses decendants*/
    char chemin[255];
    struct dirent *lecture;
    DIR *rep;
    char chemin_absolu[255];
    struct stat *st;
    st = malloc(sizeof(struct stat));
    rep = opendir(s);
    if (rep == NULL) {
        perror(s);
        return;
    }
    strcpy(chemin, "");
    while ((lecture = readdir(rep)) != NULL) {
        strcpy(chemin, s);
        strcat(chemin, lecture->d_name);
        stat(chemin, st);
        /*Si le repertoire courant contient des sous repertoire*/
        if (S_ISDIR(st->st_mode) == 1) {
            if (strcmp(lecture->d_name, ".") != 0 && strcmp(lecture->d_name, "..") != 0) {
                strcat(chemin, "/");
                recur(chemin, P);//Parcourir recursivement les repertoires
            }
            /*Parcours du repertoire courant*/
        } else {
            if ((compare_chaine_replace(lecture->d_name, P->nom_fichier) == 1) ||
                (compare_chaine(P->nom_fichier, lecture->d_name) == 1) ||
                (strcmp(P->nom_fichier, lecture->d_name) == 0)) {
                strcpy(chemin_absolu, s);
                strcat(chemin_absolu, lecture->d_name);
                printf("\n%s", chemin_absolu);
                affichage_info_fichier(st, P);
            }
        }
    }
    closedir(rep);
}
