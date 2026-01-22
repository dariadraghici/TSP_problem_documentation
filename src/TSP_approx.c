#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define INF INT_MAX / 2

int distante[20][20]; // matricea de distante intre noduri
bool vizitat[20]; // vector pentru a marca nodurile vizitate
int nr_noduri;

void initializeaza() {
	for (int i = 0; i < nr_noduri; i++) {
		for (int j = 0; j < nr_noduri; j++)
			distante[i][j] = INF;
		distante[i][i] = 0; // distanta de la un nod la el insusi este 0
		vizitat[i] = false; // initial toate nodurile sunt nevizitate
	}
}

// verifica daca exista un drum Hamiltonian in graf
bool hamiltonian() {
	if (nr_noduri == 1)
		return true;

	if (nr_noduri == 2) {
		if (distante[0][1] != INF) // exista muchie intre cele doua noduri
			return true;
		else
			return false;
	}

	bool accesibil[20] = {false}; // vector pentru a marca nodurile accesibile
	int stiva[20], varf = 0; // stiva pentru DFS
	stiva[varf] = 0; // pornim DFS din nodul 0
	varf++;
	accesibil[0] = true; // marcam nodul 0 ca accesibil
	
	while (varf > 0) {
		varf = varf - 1;
		int curent = stiva[varf];
		for (int i = 0; i < nr_noduri; i++) {
			if (i != curent && distante[curent][i] != INF && !accesibil[i]) { // daca exista muchie si nodul nu e marcat
				accesibil[i] = true; // marcam nodul ca accesibil
				stiva[varf] = i; // adaugam nodul in stiva
				varf++;
			}
		}
	}
	
	for (int i = 0; i < nr_noduri; i++)
		if (accesibil[i] == false) 
			return false;
	
	return true;
}

// algoritmul nearest neighbor (alege mereu nodul nevizitat cel mai apropiat)
int cel_mai_apropiat_vecin(int nod_start, int* traseu) {
	int cost_total = 0;
	int curent = nod_start;
	int index_traseu = 0;

	for (int i = 0; i < nr_noduri; i++)
		vizitat[i] = false; // resetam vectorul de vizitat

	traseu[index_traseu] = curent; // adaugam nodul de start in traseu
	index_traseu++;
	vizitat[curent] = true;

	for (int i = 1; i < nr_noduri; i++) {
		int urmator = -1;
		int dist_min = INF;
		
		for (int j = 0; j < nr_noduri; j++) {
			if (vizitat[j]	== false && distante[curent][j] < dist_min) { // nod nevizitat si distanta mai mica
				dist_min = distante[curent][j];
				urmator = j;
			}
		}
		
		if (urmator == -1 || dist_min == INF) // nu mai exista noduri accesibile
			return INF;

		cost_total = cost_total + dist_min;
		curent = urmator; // trecem la urmatorul nod
		vizitat[curent] = true;
		traseu[index_traseu] = curent;
		index_traseu++;
	}

	if (distante[curent][nod_start] == INF) // nu exista muchie inapoi la nodul de start
		return INF;

	cost_total = cost_total + distante[curent][nod_start];
	traseu[index_traseu] = nod_start; // inchidem circuitul inapoi la nodul de start
	index_traseu++;
	return cost_total;
}

// algoritm de optimizare 2-OPT (schimba doua muchii intre ele pentru a elimina intersectiile costisitoare)
int optimizare_2opt(int* traseu, int cost_curent) {
	bool imbunatatit = true;
	int cel_mai_bun_cost = cost_curent;
	int iteratii_max = 100;
	
	int traseu_lucru[21];
	for (int i = 0; i <= nr_noduri; i++)
		traseu_lucru[i] = traseu[i]; // copiem traseul curent

	while (imbunatatit == true) {
		if (iteratii_max <= 0) // limitam numarul de iteratii pentru a evita bucle infinite
			break;
		iteratii_max--;
		
		imbunatatit = false;
		
		for (int i = 1; i < nr_noduri - 1; i++) {
			for (int j = i + 1; j < nr_noduri; j++) {
				int a = traseu_lucru[i-1]; // nodul anterior lui i
				int b = traseu_lucru[i];
				int c = traseu_lucru[j];
				int d = traseu_lucru[j+1]; // nodul urmator lui j
				
				if (distante[a][b] == INF || distante[c][d] == INF || distante[a][c] == INF || distante[b][d] == INF)
					continue; // daca oricare dintre muchii nu exista sarim peste
				
				int cost_segment_vechi = distante[a][b] + distante[c][d];
				int cost_segment_nou = distante[a][c] + distante[b][d];
				
				if (cost_segment_nou < cost_segment_vechi) { // daca noul cost este mai mic facem schimbarea
					for (int k = 0; k < (j - i + 1) / 2; k++) { // inversam segmentul dintre i si j
						int aux = traseu_lucru[i + k];
						traseu_lucru[i + k] = traseu_lucru[j - k];
						traseu_lucru[j - k] = aux;
					}
					
					cel_mai_bun_cost = cel_mai_bun_cost + cost_segment_nou - cost_segment_vechi;
					imbunatatit = true;
					
					for (int k = i; k <= j; k++)
						traseu[k] = traseu_lucru[k]; // actualizam traseul optim
				}
			}
		}
	}
	
	return cel_mai_bun_cost;
}

// algortimul de insertie simpla (adauga noduri in traseu acolo unde sporesc cel mai putin costul total)
int insertie_simpla(int* traseu) {
	for (int i = 0; i < nr_noduri; i++)
		vizitat[i] = false;
	
	int lungime_traseu = 0;
	int cost_total = 0;
	int triunghi_gasit = 0;

	for (int i = 0; i < nr_noduri && triunghi_gasit == 0; i++) { // parcurgem nodurile
		for (int j = i + 1; j < nr_noduri && triunghi_gasit == 0; j++) { // parcurgem nodurile dupa i
			for (int k = j + 1; k < nr_noduri && triunghi_gasit == 0; k++) { // parcurgem nodurile dupa j
				if (distante[i][j] != INF && distante[j][k] != INF && distante[k][i] != INF) { // daca exista muchii intre toate cele trei noduri
					traseu[0] = i;
					traseu[1] = j;
					traseu[2] = k;
					lungime_traseu = 3;
					cost_total = distante[i][j] + distante[j][k] + distante[k][i]; // costul triunghiului
					vizitat[i] = true;
					vizitat[j] = true;
					vizitat[k] = true;
					triunghi_gasit = 1;
				}
			}
		}
	}
	
	if (triunghi_gasit == 0) { // daca nu am gasit triunghi cautam o muchie
		for (int i = 0; i < nr_noduri && triunghi_gasit == 0; i++) {
			for (int j = i + 1; j < nr_noduri && triunghi_gasit == 0; j++) {
				if (distante[i][j] != INF) {
					traseu[0] = i;
					traseu[1] = j;
					lungime_traseu = 2;
					cost_total = 2 * distante[i][j];
					vizitat[i] = true;
					vizitat[j] = true;
					triunghi_gasit = 1;
				}
			}
		}
	}
	
	if (triunghi_gasit == 0)
		return INF;
	
	while (lungime_traseu < nr_noduri) { // cat timp nu am adaugat toate nodurile
		int nod_optim = -1;
		int pozitie_optima = -1;
		int crestere_minima = INF;
		
		for (int nod = 0; nod < nr_noduri; nod++) {
			if (vizitat[nod] == true)
				continue;
			
			for (int poz = 0; poz < lungime_traseu; poz++) {
				int a = traseu[poz];
				int b = traseu[(poz + 1) % lungime_traseu]; // nodul urmator circular
				
				if (distante[a][nod] == INF || distante[nod][b] == INF) // daca nu exista muchii necesare
					continue;
				
				int crestere = distante[a][nod] + distante[nod][b] - distante[a][b]; // cresterea costului daca inseram nodul aici
				
				if (crestere < crestere_minima) {
					crestere_minima = crestere;
					nod_optim = nod;
					pozitie_optima = poz;
				}
			}
		}
		
		if (nod_optim == -1) // nu am gasit nod de inserat
			return INF;
		
		for (int i = lungime_traseu; i > pozitie_optima + 1; i--) // mutam nodurile pentru a face loc noului nod
			traseu[i] = traseu[i-1];

		traseu[pozitie_optima + 1] = nod_optim;
		lungime_traseu++;
		vizitat[nod_optim] = true;
		cost_total = cost_total + crestere_minima;
	}
	
	traseu[nr_noduri] = traseu[0];
	return cost_total;
}

int main() {
	int nr_muchii;
	
	if (scanf("%d %d", &nr_noduri, &nr_muchii) != 2) {
		printf("Nu există soluție.\n");
		return 0;
	}
	
	if (nr_noduri <= 0 || nr_noduri > 20) {
		printf("Nu există soluție.\n");
		return 0;
	}
	
	initializeaza();
	
	for (int i = 0; i < nr_muchii; i++) {
		int u, v, w;
		if (scanf("%d %d %d", &u, &v, &w) != 3) {
			printf("Nu există soluție.\n");
			return 0;
		}
		
		if (u < 0 || u >= nr_noduri || v < 0 || v >= nr_noduri || w < 1) {
			printf("Nu există soluție.\n");
			return 0;
		}
		
		if (w < distante[u][v]) {
			distante[u][v] = w;
			distante[v][u] = w;
		}
	}
	
	if (nr_noduri == 1) {
		printf("0\n0 0\n");
		return 0;
	}
	
	if (nr_noduri == 2) {
		if (distante[0][1] == INF) {
			printf("Nu există soluție.\n");
		} else {
			int cost = 2 * distante[0][1];
			printf("%d\n", cost);
			printf("0 1 0\n");
		}
		return 0;
	}
	
	if (hamiltonian() == false) {
		printf("Nu există soluție.\n");
		return 0;
	}
	
	int traseu_optim[21];
	int cost_optim = INF;
	
	for (int start = 0; start < nr_noduri && start < 3; start++) {
		int traseu_temp[21];
		int cost_temp = cel_mai_apropiat_vecin(start, traseu_temp); // algoritmul nearest neighbor
		
		if (cost_temp < cost_optim) { // actualizam solutia optima gasita
			cost_optim = cost_temp;
			for (int i = 0; i <= nr_noduri; i++)
				traseu_optim[i] = traseu_temp[i];
		}
	}
	
	int traseu_ins[21];
	int cost_ins = insertie_simpla(traseu_ins);
	
	if (cost_ins < cost_optim) { // actualizam solutia optima gasita
		cost_optim = cost_ins;
		for (int i = 0; i <= nr_noduri; i++)
			traseu_optim[i] = traseu_ins[i]; // copiem traseul gasit prin insertie simpla
	}
	
	if (cost_optim < INF / 2) { // daca am gasit o solutie valida
		cost_optim = optimizare_2opt(traseu_optim, cost_optim); // optimizare 2-OPT
		
		printf("%d\n", cost_optim);

		for (int i = 0; i <= nr_noduri; i++) {
			printf("%d", traseu_optim[i]);
			if (i < nr_noduri)
				printf(" ");
		}

		printf("\n");
	} else {
		printf("Nu există soluție.\n");
	}
	
	return 0;
}