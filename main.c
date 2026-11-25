#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "fonction.h"

char modif[100][61][50];
int nb_op = 0;

int main(int nbarg, char* argv[]){//Ewen
    FILE* fic = fopen(argv[1], "r");
    if (fic == NULL){
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    char tab[61][50];
    int num_bus;
    int choix = 0;
    char * bin;
    while(choix != 11){
        switch(choix){
            case 0: system("cls");
                    printf("Menu :\n  1) Affichage des trajets prevus\n  2) Affichage des informations sur un trajet\n  3) Affichage des trajets tries par ville et date de depart\n  4) Modification des clients pour un trajet\n  5) Modification des informations d'un client pour un trajet\n  6) Sauvegarde des donnees\n  7) Affichage des trajets arrivant le lendemain du depart\n  8) Affichage des trajets dont les informations sont completes\n  9) Affichage des trajets tries par chiffre d'affaire\n  10) Suppression des bus non-rentables et repartition des clients\n  11) Quitter\n\nChoix : ");
                    scanf("%d", &choix);
                break;
            case 1: system("cls");//Done
                    affiche_trajets(fic);
                    printf("Entrez 0 pour revenir au menu, 11 pour quitter : ");
                    scanf("%d", &choix);
                break;
            case 2: system("cls");//Done
                    affiche_infos_trajet(fic, modif);
                    printf("\n\nEntrez sur 0 pour revenir au menu, 11 pour quitter : ");
                    scanf("%d", &choix);
                break;
            case 3: system("cls");//Done
                    affiche_trajets_tries(fic, cnt_ligne(fic));
                    printf("Entrez 0 pour revenir au menu, 11 pour quitter : ");
                    scanf("%d", &choix);
                break;
            case 4: system("cls");//Done
                    choix = modif_trajet(fic, choix, modif);
                break;
            case 5: system("cls");//Done
                    choix = modif_client(fic, choix, modif);
                break;
            case 6: system("cls");//Done
                    choix = sauvegarde(fic, modif);
                break;
            case 7: system("cls");//Done
                    filtre_ville_lendemain(fic);
                    printf("\nActions possibles :\n  0) Revenir au menu\n  1) Choisir d'autres informations\n  2) Quitter\n\nChoix : ");
                        scanf("%d", &choix);
                    switch(choix){
                        case 1: choix = 7;
                            break;
                        case 2: choix = 11;
                            break;
                    }
                break;
            case 8: system("cls");//Done
                    filtre_ville2(fic);
                    printf("\nActions possibles :\n  0) Revenir au menu\n  1) Choisir d'autres informations\n  2) Quitter\n\nChoix : ");
                        scanf("%d", &choix);
                    switch(choix){
                        case 1: choix = 8;
                            break;
                        case 2: choix = 11;
                            break;
                    }
                break;
            case 9: system("cls");
                    affiche_trajets_tries_benef(fic, cnt_ligne(fic));
                    printf("Entrez 0 pour revenir au menu, 11 pour quitter : ");
                    scanf("%d", &choix);
                break;
            case 10:system("cls");
                    choix = suppression_bus(fic);
                break;
            case 12:printf("num_bus : ");
                    scanf("%d", &num_bus);
                    recup_bus(fic, num_bus, tab);
                    for(int i = 0; i < 61; i++){
                        for(int j = 0; j < 50 && tab[i][j] != '\0'; j++)
                            printf("%c", tab[i][j]);
                        printf(",");
                    }
                    printf("\nChoix : ");
                    scanf("%d", &choix);
                break;
        }
    }
    fclose(fic);
    return EXIT_SUCCESS;
}
