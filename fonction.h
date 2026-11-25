#ifndef FONCTIONS_H_INCLUDED
#define FONCTIONS_H_INCLUDED

/**
* entrée tab: tableau de chaine de caractères
* entrée trajets: le fichier de travail
* entrée carac: un caractère char
* pré-cond: carac est le premier caractère d'une ligne de trajets et le curseur est juste après et trajets est le chemin d'un fichier existant
* sortie: aucune
* post-cond: entre dans tab la ligne commencée par carac
*/
void separateur(char tab[61][50], FILE* trajets, char carac);//Damien

/**
* entrée date: une chaine de carctères
* entrée h_deb, h_fin: long
* entrée res: tableau de long qui va stocker les résultat
* pré-cond: date est une date au format JJ/MM/AAAA et h_deb et h_fin sont 9999 si on ne connait pas l'heure sinon sont compris entre 0000 et 2359
* sortie: aucune
* post-cond: entre dans res le jour puis le mois puis l'année d'arrivée selon les informations d'entrée
*/

void date_arrivee(char* date, long h_deb, long h_fin, int res[3]);//Damien

/**
* entrée trajets: le fichier de travail
* pré-cond: trajets est le chemin d'un fichier existant
* sortie: aucune
* post-cond: affiche les trajets contenus dans trajets avec leurs villes, dates et heures de départ et d'arrivée
*/

void affiche_trajets(FILE* trajets);//Damien

int calc_num_bus(FILE* fic, char num[4]);//Ewen

void recup_bus(FILE* fic, int num_bus, char tab[61][50]);//Ewen

int affiche_client(int num_bus, char tab[61][50]);//Ewen

void affiche_infos_trajet(FILE* fic, char modif[100][61][50]);//Ewen

int cnt_ligne(FILE* fic);//Ewen

/**
* entrée taille: le nombre de ligne du fichier sous forme d'un entier
* entrée tab-ind: un tableau contenant les valeurs de 1 à taille
* entrée tab: un tableau de taille tableau de 61 chaines de caractères
* entrée dates: un tableau d'entiers
* pré-cond: tab contient le contenu du ficher de travail et dates les dates corespondantes
* sortie: aucune
* post-cond: tri le tableau d'indice selon les villes de départ de tab (indice 1) ou par leurs dates en cas d'égalité
*/

void tri_selection(int taille, int tab_ind[taille], char tab[taille][61][50], int dates[taille]);//Damien

/**
* entrée trajets: le fichier de travail
* entrée taille: le nombre de ligne du fichier sous forme d'un entier
* pré-cond: trajets est le chemin d'un fichier existant
* sortie: aucune
* post-cond: affiche les trajets contenus dans trajets avec leurs villes, dates et heures de départ et d'arrivée
* triés par ville de départ et par date en cas d'égalité
*/

void affiche_trajets_tries(FILE* trajets, int taille);//Damien

void keep_in_memory(char modif[100][61][50], int test, char * bin, int num_bus, char tab[61][50]);//Ewen

int modif_trajet(FILE* fic, int choix, char modif[100][61][50]);//Ewen

int modif_client(FILE* fic, int choix, char modif[100][61][50]);//Ewen

int sauvegarde(FILE* fic, char modif[100][61][50]);//Ewen

/**
* entrée trajets: le fichier de travail
* pré-cond: trajets est le chemin d'un fichier existant
* sortie: aucune
* post-cond: affiche les trajets contenus dans trajets avec leurs villes, dates et heures de départ et d'arrivée
* selon une ville et une date demandés par le programme et qui doivent arriver le lendemain
*/

void filtre_ville_lendemain(FILE* trajets);//Damien

/**
* entrée trajets: le fichier de travail
* pré-cond: trajets est le chemin d'un fichier existant
* sortie: aucune
* post-cond: affiche les trajets contenus dans trajets avec leurs villes, dates et heures de départ et d'arrivée
* selon une ville de départ, d'arrivée et une date demandés par le programme
*/

void filtre_ville2(FILE* trajets);//Damien

/**
* entrée tab: un tableau de chaines de caractères
* pré-cond: tab est une ligne du fichier de travail
* sortie benef: un nombre décimal 
* post-cond: prend en paramètre une ligne et renvoie le benefice du bus
*/

float benef(char tab[61][50]);//Damien

/**
* entrée taille: le nombre de ligne du fichier sous forme d'un entier
* entrée tab-ind: un tableau contenant les valeurs de 1 à taille
* entrée benef: un tableau d'entiers
* pré-cond: benef contient les benefces de chaques bus
* sortie: aucune
* post-cond: tri le tableau d'indice selon les benefices des bus
*/

void tri_selection_benefices(int taille, int tab_ind[taille], float benef[taille]);//Damien

/**
* entrée trajets: le fichier de travail
* pré-cond: trajets est le chemin d'un fichier existant
* sortie: aucune
* post-cond: affiche les trajets contenus dans trajets triés par les bénéfices engendrés
*/

void affiche_trajets_tries_benef(FILE* trajets, int taille);//Damien

void echange(int taille1, int taille2, double prix[taille1], double prx[taille2], char clients[taille1][3][20], char clix[taille2][3][20]);//Ewen

int repartition(char clients[55][3][20], double prix[55], int i_debut, int i_fin);//Ewen

void tri_rapide(char clients[55][3][20], double prix[55], int i_debut, int i_fin);//Ewen

int recup_clients(const char tab[61][50], char clients[55][3][20]);//Ewen

int suppression_bus(FILE* fic);//Ewen

#endif
