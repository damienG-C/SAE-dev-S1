#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "fonction.h"

int nb_modif = 0;
extern int nb_op;

void separateur(char tab[61][50], FILE* trajets, char carac){//Damien
    int ind_car = 0, ind_tab = 0;
    nb_op += 5;
    while(carac != '\n' && carac != EOF){
        while(carac != ',' && carac != '\n' && carac != EOF){
            tab[ind_tab][ind_car] = carac;
            ind_car++;
            carac = fgetc(trajets);
            nb_op += 11;
        }
        tab[ind_tab][ind_car] = '\0';
        ind_tab++;
        ind_car = 0;
        if (carac != '\n' && carac != EOF){
            carac = fgetc(trajets);
            nb_op += 2;
            if (carac == ','){
                strcpy(tab[ind_tab], "inconnu");
                ind_tab++;
                carac=fgetc(trajets);
                nb_op += 4;
            }
        }
        nb_op += 17;
    }
    nb_op++;
    while (ind_tab < 61){
        tab[ind_tab][0] = '\0';
        ind_tab++;
        nb_op += 6;
    }
}

void date_arrivee(char* date, long h_deb, long h_fin, int res[3]){//Damien
    int j, m, a;
    sscanf(date, "%d/%d/%d", &j, &m, &a);
    nb_op += 4;
    if (h_deb < h_fin || h_deb + h_fin > 9998){
        res[0] = j;
        res[1] = m;
        res[2] = a;
        nb_op += 6;
    }
    else
    {
        nb_op += 2;
        if (j == 31){
            nb_op += 1;
            if (m == 12){
                nb_op += 4;
                a++;
                j=1;
                m=1;
            }
            else {
                nb_op += 3;
                m++;
                j=1;
            }
        }
        else if (j == 30){
            nb_op += 5;
            switch (m){
                case 4: nb_op -= 1;
                case 6: nb_op -= 1;
                case 9: nb_op -= 1;
                case 11:
                    m++;
                    j=1;
                    nb_op += 3;
            }
        }
        else if (m == 2){
            nb_op += 1;
            if (j == 29){
                nb_op += 1;
                m = 3;
            }
            nb_op += 1;
            if (j == 29){
                nb_op += 2;
                if(a % 4 == 0){
                    nb_op += 2;
                    if( a % 100 == 0){
                        nb_op += 2;
                        if ( a % 400 == 0){
                            nb_op += 2;
                            j++;
                        }
                        else{
                            nb_op += 1;
                            m = 3;
                        }
                    }
                    else{
                        nb_op += 1;
                        m = 3;
                    }
                }
                else{
                    nb_op += 1;
                    m = 3;
                }
            }
        }
        else{
            j++;
            nb_op += 2;
        }
        res[0] = j;
        res[1] = m;
        res[2] = a;
        nb_op += 6;
    }
}

void affiche_trajets(FILE* trajets){//Damien
    rewind(trajets);
    char tab[61][50];
    long h_deb, h_fin;
    nb_op += 2;
    char carac = '\0';
    while (carac != EOF){
        nb_op += 1;
        carac = fgetc(trajets);
        separateur(tab, trajets, carac);

        if (!(strcmp(tab[4], "inconnu"))){
            h_deb = 9999;
        }
        else
            sscanf(tab[4], "%ld", &h_deb);
        if (!(strcmp(tab[5], "inconnu"))){
            h_fin = 9999;
        }
        else
            sscanf(tab[5], "%ld", &h_fin);
        int arrivee[3];
        date_arrivee(tab[3], h_deb, h_fin, arrivee);
        if (arrivee[2] != 0){
            if (h_deb + h_fin < 9999)
                printf("La ville de depart est : %s\nLa ville d'arrivee est : %s\nLa date de depart est : %s a %02ld:%02ld\nLa date d'arrivee est : %02d/%02d/%d a %02ld:%02ld\n\n", tab[1], tab[2], tab[3], h_deb/100, h_deb%100, arrivee[0], arrivee[1], arrivee[2], h_fin/100, h_fin%100);
            else if (h_deb == 9999 && h_fin != 9999)
                printf("La ville de depart est : %s\nLa ville d'arrivee est : %s\nLa date de depart est : %s a ??:??\nLa date d'arrivee est : %02d/%02d/%d a %02ld:%02ld\n\n", tab[1], tab[2], tab[3], arrivee[0], arrivee[1], arrivee[2], h_fin/100, h_fin%100);
            else if (h_deb != 9999 && h_fin == 9999)
                printf("La ville de depart est : %s\nLa ville d'arrivee est : %s\nLa date de depart est : %s a %02ld:%02ld\nLa date d'arrivee est : %02d/%02d/%d a ??:??\n\n", tab[1], tab[2], tab[3], h_deb/100, h_deb%100, arrivee[0], arrivee[1], arrivee[2]);
            else
                printf("La ville de depart est : %s\nLa ville d'arrivee est : %s\nLa date de depart est : %s a ??:??\nLa date d'arrivee est : %02d/%02d/%d a ??:??\n\n", tab[1], tab[2], tab[3], arrivee[0], arrivee[1], arrivee[2]);
        }
    }
}

int calc_num_bus(FILE* fic, char num[4]){//Ewen
    int num_trajet = 0;
    char carac;
    for(int i = 0; i < 4; i++){
        carac = fgetc(fic);
        num[i] = carac;
        num_trajet = num_trajet + (carac-'0');
        num_trajet *= 10;
    }
    num_trajet /= 10;
}

void recup_bus(FILE* fic, int num_bus, char tab[61][50]){//Ewen
    char carac = 'a';
    char ligne[1200];
    int fin = 0;
    char num[4];

    rewind(fic);
    while(carac != EOF && fin == 0){
        int num_trajet = calc_num_bus(fic, num);
        carac = fgetc(fic);
        if(num_bus == num_trajet){
            fin = 1;
            separateur(tab, fic, carac);
        }
        fgets(ligne, 1200, fic);
    }
    for(int j = 0; j < 4; j++)
        tab[0][j] = num[j];
}

int affiche_client(int num_bus, char tab[61][50]){//Ewen
    int cnt = 1, test;

    printf("Passagers du bus %d :\n", num_bus);
    for(int i = 6; i < 61 && tab[i][0] != '\0'; i++){
        test = 0;
        printf("    %d) ", cnt);

        for(int j = 0; test < 2; j++){
            if(test)
                printf("%c", tab[i][j]);
            if(tab[i][j] == ':')
                test++;
            else if(tab[i][j+1] == ':' && test == 1)
                test++;
        }
        printf("\n");
        cnt++;
    }
    return cnt;
}

void affiche_infos_trajet(FILE* fic, char modif[100][61][50]){//Ewen
    char * bin;
    int test, num_test, ind, num_bus;
    char tab[61][50];
    int date[3];
    long h_deb, h_fin;

    printf("Entrez un numero de bus : ");
    scanf("%d", &num_bus);
    for(ind = 0; ind < 100 && test != 0; ind++){
        num_test = strtol(modif[ind][0], &bin, 10);
            if(num_test == num_bus)
                test = 0;
            else
                test = 1;
    }
    if(test)
        recup_bus(fic, num_bus, tab);
    else{
        for(int i = 0; i < 61; i++){
            strcpy(tab[i], modif[ind-1][i]);
        }
    }

    test = 0;
    h_deb = strtol(tab[4], &bin, 10);
    h_fin = strtol(tab[5], &bin, 10);
    date_arrivee(tab[3], h_deb, h_fin, date);
    printf("Depart a %s le %s a %ld:%02ld\nArrivee a %s le %02d/%02d/%d a %ld:%02ld\n", tab[1], tab[3], h_deb/100, h_deb%100, tab[2], date[0], date[1], date[2], h_fin/100, h_fin%100);
    affiche_client(num_bus, tab);
}

int cnt_ligne(FILE* fic){//Ewen
    int nb_ligne = 0;
    char carac = 'a';
    rewind(fic);
    while(carac != EOF){
        carac = fgetc(fic);
        if(carac == '\n' || carac == EOF)
            nb_ligne++;
    }
    rewind(fic);
    return nb_ligne;
}

/*void tri_fusion_date(int taille, int tab_ind[taille], int taille_totale,  int dates[taille_totale]){//Damien
    nb_op += 1;
    if (taille != 1){
        int t = taille / 2;
        int tab1[t];
        int tab2[taille - t];
        nb_op += 7;
        for (int i = 0; i < t; i++){
            tab1[i] = tab_ind[i];
            nb_op += 6;
        }
        nb_op += 2;
        for (int j = t; j < taille; j++){
            tab2[j - t] = tab_ind[j];
            nb_op += 7;
        }
        tri_fusion_date(t, tab1, taille_totale, dates);
        tri_fusion_date(taille - t, tab2, taille_totale, dates);
        nb_op += 1;
        int i1 = 0, i2 = 0, itot = 0;
        nb_op += 7;
        while (i1 < t && i2 < taille - t){
            nb_op += 9;
            if (dates[tab1[i1]] < dates[tab2[i2]]){
                tab_ind[itot] = tab1[i1];
                i1++;
                itot++;
                nb_op += 11;
            }
            else{
                tab_ind[itot] = tab2[i2];
                i2++;
                itot++;
                nb_op += 7;
            }
        }
        nb_op += 1;
        if (i1 == t){
            nb_op += 2;
            while (i2 < taille - t){
                tab_ind[itot] = tab2[i2];
                i2++;
                itot++;
                nb_op += 9;
            }
        }
        else{
            nb_op += 1;
            while (i1 < t){
                tab_ind[itot] = tab1[i1];
                i1++;
                itot++;
                nb_op += 8;
            }
        }
    }
}*/

void tri_selection(int taille, int tab_ind[taille], char tab[taille][61][50], int dates[taille]){
    int min;
    int temp;
    int res;
    for (int i = 0; i < taille - 1; i++){
        min = i;
        for (int j = i + 1; j < taille; j++){
            res = strcmp(tab[tab_ind[j]][1], tab[tab_ind[min]][1]);
            if (res < 0 || (res == 0 && dates[tab_ind[j]] < dates[tab_ind[min]])){
                min = j;
            }
        }
        temp = tab_ind[i];
        tab_ind[i] = tab_ind[min];
        tab_ind[min] = temp;
    }
}

/*void tri_fusion(int taille, int tab_ind[taille], int taille_totale, char tab[taille][61][50], int ind){//Damien
    nb_op += 1;
    if (taille != 1){
        int t = taille / 2;
        int tab1[t];
        int tab2[taille - t];
        nb_op += 7;
        for (int i = 0; i < t; i++){
            tab1[i] = tab_ind[i];
            nb_op += 6;
        }
        nb_op += 2;
        for (int j = t; j < taille; j++){
            tab2[j - t] = tab_ind[j];
            nb_op += 7;
        }
        tri_fusion(t, tab1, taille_totale, tab, ind);
        tri_fusion(taille - t, tab2, taille_totale, tab, ind);
        nb_op += 1;
        int i1 = 0, i2 = 0, itot = 0;
        nb_op += 7;
        while (i1 < t && i2 < taille - t){
            nb_op += 5;
            if (strcmp(tab[tab1[i1]][ind], tab[tab2[i2]][ind]) <= 0){
                tab_ind[itot] = tab1[i1];
                i1++;
                itot++;
                nb_op += 7;
            }
            else{
                tab_ind[itot] = tab2[i2];
                i2++;
                itot++;
                nb_op += 7;
            }
        }
        nb_op += 1;
        if (i1 == t){
            nb_op += 2;
            while (i2 < taille - t){
                tab_ind[itot] = tab2[i2];
                i2++;
                itot++;
            nb_op += 7;
            }
        }
        else{
            nb_op += 1;
            while (i1 < t){
                tab_ind[itot] = tab1[i1];
                i1++;
                itot++;
            nb_op += 8;
            }
        }
    }
}*/

void affiche_trajets_tries(FILE* trajets, int taille){//Damien
    rewind(trajets);
    char tab[taille][61][50];
    long h_deb, h_fin;
    char carac = fgetc(trajets);
    int dates[taille];
    int j, m, a;
    nb_op = 1;
    for (int i = 0; i < taille; i++){
        nb_op += 3;
        separateur(tab[i], trajets, carac);
        if (i < taille){
            carac = fgetc(trajets);
            nb_op += 1;
        }
    }
    nb_op += 4;
    for (int i = 0; i < taille; i++){
        sscanf(tab[i][3], "%d/%d/%d", &j, &m, &a);
        dates[i] = a * 10000 + m * 100 + j;
        nb_op += 8;
    }
    int indices[taille];
    nb_op += 2;
    for(int i = 0; i < taille; i++){
        indices[i] = i;
        nb_op += 4;
    }
    //tri_fusion_date(taille, indices, taille, dates);
    //tri_fusion(taille, indices, taille, tab, 1);
    tri_selection(taille, indices, tab, dates);
    for (int i = 0; i < taille; i++){
        if (!(strcmp(tab[indices[i]][4], "inconnu"))){
            h_deb = 9999;
        }
        else{
            sscanf(tab[indices[i]][4], "%ld", &h_deb);
        }
        if (!(strcmp(tab[indices[i]][5], "inconnu"))){
            h_fin = 9999;
        }
        else{
            sscanf(tab[indices[i]][5], "%ld", &h_fin);
        }
        int arrivee[3];
        date_arrivee(tab[indices[i]][3], h_deb, h_fin, arrivee);
        if (arrivee[2] != 0){
            if (h_deb + h_fin < 9999)
                printf("La ville de depart est : %s\nLa ville d'arrivee est : %s\nLa date de depart est : %s a %02ld:%02ld\nLa date d'arrivee est : %02d/%02d/%d a %02ld:%02ld\n\n", tab[indices[i]][1], tab[indices[i]][2], tab[indices[i]][3], h_deb/100, h_deb%100, arrivee[0], arrivee[1], arrivee[2], h_fin/100, h_fin%100);
            else if (h_deb == 9999 && h_fin != 9999)
                printf("La ville de depart est : %s\nLa ville d'arrivee est : %s\nLa date de depart est : %s a ??:??\nLa date d'arrivee est : %02d/%02d/%d a %02ld:%02ld\n\n", tab[indices[i]][1], tab[indices[i]][2], tab[indices[i]][3], arrivee[0], arrivee[1], arrivee[2], h_fin/100, h_fin%100);
            else if (h_deb != 9999 && h_fin == 9999)
                printf("La ville de depart est : %s\nLa ville d'arrivee est : %s\nLa date de depart est : %s a %02ld:%02ld\nLa date d'arrivee est : %02d/%02d/%d a ??:??\n\n", tab[indices[i]][1], tab[indices[i]][2], tab[indices[i]][3], h_deb/100, h_deb%100, arrivee[0], arrivee[1], arrivee[2]);
            else
                printf("La ville de depart est : %s\nLa ville d'arrivee est : %s\nLa date de depart est : %s a ??:??\nLa date d'arrivee est : %02d/%02d/%d a ??:??\n\n", tab[indices[i]][1], tab[indices[i]][2], tab[indices[i]][3], arrivee[0], arrivee[1], arrivee[2]);
        }
    }
}

void keep_in_memory(char modif[100][61][50], int test, char * bin, int num_bus, char tab[61][50]){//Ewen
    int num_test;
    int ind;
    for(ind = 0; ind < 100 && test != 0; ind++){
        num_test = strtol(modif[ind][0], &bin, 10);
        if(num_test == num_bus)
            test = 0;
        else
            test = 1;
    }
    if(test){
        for(int j = 0; j < 61; j++)
            strcpy(modif[nb_modif][j], tab[j]);
    }
    else{
        for(int j = 0; j < 61; j++)
            strcpy(modif[ind-1][j], tab[j]);
    }
    nb_modif++;
}

int modif_trajet(FILE* fic, int choix, char modif[100][61][50]){//Ewen
    int num_bus, num_client, num_test, test, ind, cnt, change = 1;
    char tab[61][50];
    char * bin;
    char ajout[3][50];

    while(1){
        if(change){
            printf("Entrez un numero de bus : ");
            scanf("%d", &num_bus);
            for(ind = 0; ind < 100 && test != 0; ind++){
                num_test = strtol(modif[ind][0], &bin, 10);
                    if(num_test == num_bus)
                        test = 0;
                    else
                        test = 1;
            }
            if(test)
                recup_bus(fic, num_bus, tab);
            else{
                for(int i = 0; i < 61; i++){
                    strcpy(tab[i], modif[ind-1][i]);
                }
            }
        }
        change = 0;
        system("cls");
        cnt = affiche_client(num_bus, tab);
        test = 1;
        printf("\n\nActions possibles :\n  1) Ajouter un client\n  2) Supprimer un client\n  3) Changer de bus\n  4) Sauvegarder les modifications\n  5) Revenir au menu\n  6) Quitter\n\nChoix : ");
        scanf("%d", &choix);
        switch(choix){
            case 1: printf("Entrez le prenom et le nom du client que vous voulez ajouter, separes par un espace : ");
                    scanf("%s %s", &ajout[0], &ajout[1]);
                    printf("Entrez le prix du billet du client, sous la forme \"12.12\" : ");
                    scanf("%s", &ajout[2]);
                    snprintf(tab[cnt+5], 50, "%d:%s %s:%s", cnt, ajout[0], ajout[1], ajout[2]);
                    keep_in_memory(modif, test, bin, num_bus, tab);
                break;
            case 2: printf("Entrez le numero du client que vous voulez supprimer : ");
                    scanf("%d", &num_client);
                    for(int i = num_client+5; i < 61 && tab[i][0] != '\0'; i++)
                        strcpy(tab[i], tab[i+1]);
                    keep_in_memory(modif, test, bin, num_bus, tab);
                break;
            case 3: change = 1;
                break;
            case 4: sauvegarde(fic, modif);
                break;
            case 5: return 0;
                break;
            case 6: return 11;
                break;
        }
    }
}

int modif_client(FILE* fic, int choix, char modif[100][61][50]){//Ewen
    int j, k, l, num_bus, num_client, num_test, test = 1, ind, cnt, change_bus = 1, change_client = 1;
    char tab[61][50];
    char * bin;
    char client[4][50];

    while(1){
        system("cls");
        if(change_bus){
            printf("Entrez un numero de bus : ");
            scanf("%d", &num_bus);
            for(ind = 0; ind < 100 && test != 0; ind++){
                num_test = strtol(modif[ind][0], &bin, 10);
                if(num_test == num_bus){
                    test = 0;
                }
                else{
                    test = 1;
                }
            }
            if(test){
                recup_bus(fic, num_bus, tab);
            }
            else{
                for(int i = 0; i < 61; i++){
                    strcpy(tab[i], modif[ind-1][i]);
                }
            }
        }
        system("cls");
        if(change_client){
            for(int i = 0; i < 2; i++){
                client[0][i] = '\0';
                for(int j = 0; j < 50; j++){
                    client[1][j] = '\0';
                    client[2][j] = '\0';
                }
            }
            cnt = affiche_client(num_bus, tab);
            printf("\nChoisissez le client que vous voulez modifier : ");
            scanf("%d", &num_client);
            snprintf(client[0], 50, "%d", num_client);
            test = 0, j = 0, k = 0, l = 0;
            for(int i = 0; i < 50 && tab[num_client+5][i] != '\0'; i++){
                if(test == 1){
                    client[1][j] = tab[num_client+5][i];
                    j++;
                }
                else if(test == 2){
                    client[2][k] = tab[num_client+5][i+1];
                    k++;
                }
                else if(test == 3){
                    client[3][l] = tab[num_client+5][i+2];
                    l++;
                }
                if((tab[num_client+5][i] == ':' && test == 0) || (tab[num_client+5][i+1] == ' ' && test == 1) || (tab[num_client+5][i+2] == ':' && test == 2))
                    test++;
            }
        }
        change_bus = 0;
        change_client = 0;
        system("cls");
        test = 1;
        printf("Passager numero %d du bus %d :\n  Prenom : %s\n  Nom : %s\n  Billet : %s euros \n\nActions possibles :\n  1) Modifier le nom / prenom\n  2) Modifier le prix du billet\n  3) Changer de client\n  4) Changer de bus\n  5) Sauvegarder les modifications\n  6) Revenir au menu\n  7) Quitter\n\nChoix : ", num_client, num_bus, client[1], client[2], client[3]);
        scanf("%d", &choix);
        switch(choix){
            case 1: printf("Entrez les nouveaux prenom et nom du client, separes par un espace : ");
                    scanf("%s %s", &client[1], &client[2]);
                    snprintf(tab[num_client+5], 50, "%s:%s %s:%s", client[0], client[1], client[2], client[3]);
                    keep_in_memory(modif, test, bin, num_bus, tab);
                break;
            case 2: printf("Entrez le nouveau prix du billet du client, sous la forme \"12.12\" : ");
                    scanf("%s", &client[3]);
                    snprintf(tab[num_client+5], 50, "%s:%s %s:%s", client[0], client[1], client[2], client[3]);
                    keep_in_memory(modif, test, bin, num_bus, tab);
                break;
            case 3: change_client = 1;
                break;
            case 4: change_bus = 1;
                    change_client = 1;
                break;
            case 5: sauvegarde(fic, modif);
                break;
            case 6: return 0;
                break;
            case 7: return 11;
                break;
        }
    }
}

int sauvegarde(FILE* fic, char modif[100][61][50]){//Ewen
    FILE* copie = fopen("trajets/trajets_copie.csv", "w");
    char carac = 'a';
    char ligne[1200];
    int test, cnt = 0, avion = 0;
    char num_trajets[4];

    rewind(fic);
    while(carac != EOF){
        for(int i = 0; i < 4; i++){
            num_trajets[i] = fgetc(fic);
        }
        carac = fgetc(fic);
        test = 1;
        for(int j = 0; j < 100 && test; j++){
            if((modif[j][0][0] == num_trajets[0]) && (modif[j][0][1] == num_trajets[1]) && (modif[j][0][2] == num_trajets[2]) && (modif[j][0][3] == num_trajets[3])){
                fputs(modif[j][0], copie);
                for(int k = 1; k < 61 && test == 1; k++){
                    if(modif[j][k][0] == '\0')
                        test = 0;
                    if(test){
                        fputc(',', copie);
                        fputs(modif[j][k], copie);
                    }
                }
                fputc('\n', copie);
                while(carac != '\n' && carac != '\0')
                    carac = fgetc(fic);
            }
        }
        if(test){
            carac = fgetc(fic);
            fputs(num_trajets, copie);
            fputc(',', copie);
            while(carac != '\n' && carac != EOF){
                fputc(carac, copie);
                carac = fgetc(fic);
            }
            if(carac != EOF)
                fputc('\n', copie);
        }
    }
    fclose(copie);
    return 0;
}

void filtre_ville_lendemain(FILE* trajets){//Damien
    rewind(trajets);
    char ville[50];
    char date[50];
    char tab[61][50];
    long h_deb, h_fin;
    char carac = '\0';
    printf("Entrez une ville de depart : ");
    scanf("%s", &ville);
    printf("Entrez une date de depart : ");
    scanf("%s", &date);
    while (carac != EOF){
        carac = fgetc(trajets);
        separateur(tab, trajets, carac);
        if (!(strcmp(tab[4], "inconnu"))){
            h_deb = 9999;
        }
        else
            sscanf(tab[4], "%ld", &h_deb);
        if (!(strcmp(tab[5], "inconnu"))){
            h_fin = 9999;
        }
        else
            sscanf(tab[5], "%ld", &h_fin);
        int arrivee[3];
        date_arrivee(tab[3], h_deb, h_fin, arrivee);
        if (arrivee[2] != 0 && strcmp(tab[1], ville) == 0 && !(strcmp(tab[3], date)) && arrivee[0] != (date[0] - '0') * 10 + date[1] - '0'){
            if (h_deb + h_fin < 9999)
                printf("La ville de depart est : %s\nLa ville d'arrivee est : %s\nLa date de depart est : %s a %02ld:%02ld\nLa date d'arrivee est : %02d/%02d/%d a %02ld:%02ld\n\n", tab[1], tab[2], tab[3], h_deb/100, h_deb%100, arrivee[0], arrivee[1], arrivee[2], h_fin/100, h_fin%100);
            else if (h_deb == 9999 && h_fin != 9999)
                printf("La ville de depart est : %s\nLa ville d'arrivee est : %s\nLa date de depart est : %s a ??:??\nLa date d'arrivee est : %02d/%02d/%d a %02ld:%02ld\n\n", tab[1], tab[2], tab[3], arrivee[0], arrivee[1], arrivee[2], h_fin/100, h_fin%100);
            else if (h_deb != 9999 && h_fin == 9999)
                printf("La ville de depart est : %s\nLa ville d'arrivee est : %s\nLa date de depart est : %s a %02ld:%02ld\nLa date d'arrivee est : %02d/%02d/%d a ??:??\n\n", tab[1], tab[2], tab[3], h_deb/100, h_deb%100, arrivee[0], arrivee[1], arrivee[2]);
            else
                printf("La ville de depart est : %s\nLa ville d'arrivee est : %s\nLa date de depart est : %s a ??:??\nLa date d'arrivee est : %02d/%02d/%d a ??:??\n\n", tab[1], tab[2], tab[3], arrivee[0], arrivee[1], arrivee[2]);
        }
    }
}

void filtre_ville2(FILE* trajets){//Damien
    rewind(trajets);
    char depart[50];
    char destination[50];
    char date[50];
    char tab[61][50];
    long h_deb, h_fin;
    char carac = '\0';
    printf("entrez une ville de depart : ");
    scanf("%s", &depart);
    printf("entrez une ville d'arrivee : ");
    scanf("%s", &destination);
    printf("entrez une date de depart : ");
    scanf("%s", &date);
    while (carac != EOF){
        carac = fgetc(trajets);
        separateur(tab, trajets, carac);
        if (!(strcmp(tab[4], "inconnu"))){
            h_deb = 9999;
        }
        else
            sscanf(tab[4], "%ld", &h_deb);
        if (!(strcmp(tab[5], "inconnu"))){
            h_fin = 9999;
        }
        else
            sscanf(tab[5], "%ld", &h_fin);
        int arrivee[3];
        date_arrivee(tab[3], h_deb, h_fin, arrivee);
        if (arrivee[2] != 0 && !(strcmp(tab[1], depart)) && !(strcmp(tab[2], destination)) && !(strcmp(tab[3], date))){
            if (h_deb + h_fin < 9999)
                printf("La ville de depart est : %s\nLa ville d'arrivee est : %s\nLa date de depart est : %s a %02ld:%02ld\nLa date d'arrivee est : %02d/%02d/%d a %02ld:%02ld\n\n", tab[1], tab[2], tab[3], h_deb/100, h_deb%100, arrivee[0], arrivee[1], arrivee[2], h_fin/100, h_fin%100);
            else if (h_deb == 9999 && h_fin != 9999)
                printf("La ville de depart est : %s\nLa ville d'arrivee est : %s\nLa date de depart est : %s a ??:??\nLa date d'arrivee est : %02d/%02d/%d a %02ld:%02ld\n\n", tab[1], tab[2], tab[3], arrivee[0], arrivee[1], arrivee[2], h_fin/100, h_fin%100);
            else if (h_deb != 9999 && h_fin == 9999)
                printf("La ville de depart est : %s\nLa ville d'arrivee est : %s\nLa date de depart est : %s a %02ld:%02ld\nLa date d'arrivee est : %02d/%02d/%d a ??:??\n\n", tab[1], tab[2], tab[3], h_deb/100, h_deb%100, arrivee[0], arrivee[1], arrivee[2]);
            else
                printf("La ville de depart est : %s\nLa ville d'arrivee est : %s\nLa date de depart est : %s a ??:??\nLa date d'arrivee est : %02d/%02d/%d a ??:??\n\n", tab[1], tab[2], tab[3], arrivee[0], arrivee[1], arrivee[2]);
        }
    }
}

float benef(char tab[61][50]){//Damien
    long h_deb, h_fin;
    float benef = 0.0, prix = 0.0;
    int cpt = 0;
    int indice = 0;
    char prix_str[10];
    int nb_heures;
    for (int i = 6; i < 61 && tab[i][0] != '\0'; i++){
        while (cpt < 2){
            if (tab[i][indice] == ':'){
                cpt++;
            }
            indice++;
        }
        for (int j = 0; tab[i][j + indice] != '\0'; j++){
            prix_str[j] = tab[i][j + indice];
        }
        sscanf(prix_str, "%f", &prix);
        benef += prix;
        cpt = 0;
        indice = 0;
    }
    if (!(strcmp(tab[4], "inconnu"))){
        h_deb = 9999;
    }
    else{
        sscanf(tab[4], "%ld", &h_deb);
    }
    if (!(strcmp(tab[5], "inconnu"))){
        h_fin = 9999;
    }
    else{
        sscanf(tab[5], "%ld", &h_fin);
    }
    if (h_deb + h_fin >= 9999){
        benef = -99999999999999999999999;
    }
    else{
        nb_heures = h_fin / 100 - h_deb / 100;
        if (nb_heures < 0 || (nb_heures == 0 && h_fin % 100 < h_deb % 100)){
            nb_heures += 24;
        }
        if (h_fin % 100 > h_deb % 100){
            nb_heures += 1;
        }
        benef -= 50 * nb_heures;
        benef -= 100;
    }
    return benef;
}

void tri_selection_benefices(int taille, int tab_ind[taille], float benef[taille]){
    int min;
    int temp;
    for (int i = 0; i < taille - 1; i++){
        min = i;
        for (int j = i + 1; j < taille; j++){
            if (benef[tab_ind[j]] < benef[tab_ind[min]]){
                min = j;
            }
        }
        temp = tab_ind[i];
        tab_ind[i] = tab_ind[min];
        tab_ind[min] = temp;
    }
}


void affiche_trajets_tries_benef(FILE* trajets, int taille){//Damien
    rewind(trajets);
    char tab[taille][61][50];
    long h_deb, h_fin;
    char carac = fgetc(trajets);
    float benefs[taille];
    int indices[taille];
    for (int i = 0; i < taille; i++){
        separateur(tab[i], trajets, carac);
        if (i < taille)
            carac = fgetc(trajets);
    }
    for (int i = 0; i < taille; i++){
        indices[i] = i;
        benefs[i] = benef(tab[i]);
    }
    tri_selection_benefices(taille, indices, benefs);
    for (int j = 0; j < taille; j++){
        int i = indices[j];
        if (!(strcmp(tab[i][4], "inconnu"))){
            h_deb = 9999;
        }
        else{
            sscanf(tab[i][4], "%ld", &h_deb);
        }
        if (!(strcmp(tab[i][5], "inconnu"))){
            h_fin = 9999;
        }
        else{
            sscanf(tab[i][5], "%ld", &h_fin);
        }
        int arrivee[3];
        date_arrivee(tab[i][3], h_deb, h_fin, arrivee);
        if (arrivee[2] != 0){
            if (h_deb + h_fin < 9999)
                printf("Le numero du trajet est : %s\nLa ville de depart est : %s\nLa ville d'arrivee est : %s\nLa date de depart est : %s a %02ld:%02ld\nLa date d'arrivee est : %02d/%02d/%d a %02ld:%02ld\nle benefice est %.2f\n\n", tab[i][0], tab[i][1], tab[i][2], tab[i][3], h_deb/100, h_deb%100, arrivee[0], arrivee[1], arrivee[2], h_fin/100, h_fin%100, benefs[i]);
            else if (h_deb == 9999 && h_fin != 9999)
                printf("Le numero du trajet est : %s\nLa ville de depart est : %s\nLa ville d'arrivee est : %s\nLa date de depart est : %s a ??:??\nLa date d'arrivee est : %02d/%02d/%d a %02ld:%02ld\nle benefice est inconnu\n\n", tab[i][0], tab[i][1], tab[i][2], tab[i][3], arrivee[0], arrivee[1], arrivee[2], h_fin/100, h_fin%100);
            else if (h_deb != 9999 && h_fin == 9999)
                printf("Le numero du trajet est : %s\nLa ville de depart est : %s\nLa ville d'arrivee est : %s\nLa date de depart est : %s a %02ld:%02ld\nLa date d'arrivee est : %02d/%02d/%d a ??:??\nle benefice est inconnu\n\n", tab[i][0], tab[i][1], tab[i][2], tab[i][3], h_deb/100, h_deb%100, arrivee[0], arrivee[1], arrivee[2]);
            else
                printf("Le numero du trajet est : %s\nLa ville de depart est : %s\nLa ville d'arrivee est : %s\nLa date de depart est : %s a ??:??\nLa date d'arrivee est : %02d/%02d/%d a ??:??\nle benefice est inconnu\n\n", tab[i][0], tab[i][1], tab[i][2], tab[i][3], arrivee[0], arrivee[1], arrivee[2]);
        }
    }
}

void echange(int taille1, int taille2, double prix[taille1], double prx[taille2], char clients[taille1][3][20], char clix[taille2][3][20]){//Ewen
    prx[taille2] = prix[taille1];

    for(int i = 0; i < 3; i++){
        strcpy(clix[taille2][i], clients[taille1][i]);
    }
}

void tri_clients(int taille, double prix[taille], char clients[taille][3][20]){//Damien
    if (taille != 1){
        int t = taille / 2;
        char cli1[t][3][20];
        char cli2[taille-t][3][20];
        double pr1[t];
        double pr2[taille-t];

        for (int i = 0; i < t; i++){
            echange(i, i, prix, pr1, clients, cli1);
        }
        for (int j = t; j < taille; j++){
            echange(j, j-t, prix, pr2, clients, cli2);
        }
        tri_clients(t, pr1, cli1);
        tri_clients(taille - t, pr2, cli2);
        int i1 = 0, i2 = 0, itot = 0;
        while (i1 < t && i2 < taille - t){
            if (pr1[i1] > pr2[i2]){
                echange(i1, itot, pr1, prix, cli1, clients);
                i1++;
                itot++;
            }
            else{
                echange(i2, itot, pr2, prix, cli2, clients);
                i2++;
                itot++;
            }
        }
        if (i1 == t){
            while (i2 < taille - t){
                echange(i2, itot, pr2, prix, cli2, clients);
                i2++;
                itot++;
            }
        }
        else{
            while (i1 < t){
                echange(i1, itot, pr1, prix, cli1, clients);
                i1++;
                itot++;
            }
        }
    }
}

int recup_clients(const char tab[61][50], char clients[55][3][20]){//Ewen
    int ind;
    double prix[55];
    int res = 0;

    for(int i = 0; i < 55 && tab[i+6][0] != '\0'; i++){
        ind = 0;
        for(int j = 0; j < 3; j++){
            for(int k = 0; k < 20 && tab[i+6][ind] != ':' && tab[i+6][ind] != '\0'; k++){
                clients[i][j][k] = tab[i+6][ind];
                ind++;
            }
            ind++;
        }
        res++;
    }

    for(int i = 0; i < 54 && clients[i][0][0] != '\0'; i++){
        prix[i] = strtod(clients[i][2], NULL);
    }
    tri_clients(55, prix, clients);
    return res;
}

int suppression_bus(FILE* fic){//Ewen
    int nb_lignes = cnt_ligne(fic);
    char all[nb_lignes][61][50];
    int faits[nb_lignes];
    int nb_faits = 0, nb_negatif;
    char* bin;

    rewind(fic);
    for(int ind = 0; ind < nb_lignes; ind++){
        char carac = fgetc(fic);
        separateur(all[ind], fic, carac);
    }

    for(int ind = 0; ind < nb_lignes; ind++){
        int num_bus = strtol(all[ind][0], &bin, 10);
        float benefice = benef(all[ind]);
        char clients[55][3][20];
        if(benefice < 0){
            nb_negatif = recup_clients(all[ind], clients);
            for(int i = 0; i < nb_negatif; i++){
                for(int j = 0; j < 3; j++){
                    for(int k = 0; k < 20 && clients[i][j][k] != '\0'; k++){
                        printf("%c", clients[i][j][k]);
                    }
                    printf(" ");
                }
                printf("\n");
            }
            printf("\n");
        }
    }
    return -1;
}
