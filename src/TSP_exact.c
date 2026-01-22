#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define INFINIT (INT_MAX / 2) // evitam overflow la adunari

int distanta[20][20]; // matricea de distante intre noduri
int dp[1 << 20][20]; // dp[masca][ultimul_nod]
int parinte[1 << 20][20]; // pentru reconstructia drumului 
int nr_noduri;

void initializeaza_tabele() {
	// initializare matrice distante
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			if (i == j)
				distanta[i][j] = 0;
			else
				distanta[i][j] = INFINIT;
		}
	}

	// initializare dp si parinte
	for (int i = 0; i < (1 << 20); i++) { // pentru toate mascile posibile
		for (int j = 0; j < 20; j++) {
			dp[i][j] = INFINIT; // cost infinit initial
			parinte[i][j] = -1; // fara parinte initial
		}
	}
}

int held_karp() {
	// cazul de baza (pornim din nodul 0)
	dp[1 << 0][0] = 0; // doar nodul 0 vizitat, cost 0

	int masca_plina = (1 << nr_noduri) - 1; // masca cu toate nodurile vizitate

	// iteram toate submultimile posibile de noduri (masti)
	for (int masca = 1; masca <= masca_plina; masca++) {
		for (int u = 0; u < nr_noduri; u++) {
			if (dp[masca][u] == INFINIT) // daca starea nu e valida sarim peste
				continue;

			// incercam sa extindem drumul la toate nodurile nevizitate
			for (int v = 0; v < nr_noduri; v++) {
				if ((masca & (1 << v)) == 0) { // daca nodul v nu e in masca
					if (distanta[u][v] == INFINIT)  // daca nu exista muchie intre u si v
						continue;

					int masca_noua = masca | (1 << v); // adaugam nodul v in masca
					int cost_tranzitie = dp[masca][u] + distanta[u][v]; // costul tranzitiei

					if (cost_tranzitie < dp[masca_noua][v]) { // daca am gasit un drum mai ieftin
						dp[masca_noua][v] = cost_tranzitie; // actualizam costul minim
						parinte[masca_noua][v] = u; // actualizam parintele pentru reconstructia drumului
					}
				}
			}
		}
	}

	// calculam costul minim al circuitului complet (inchidem drumul inapoi la nodul 0)
	int cost_minim_total = INFINIT;
	for (int i = 1; i < nr_noduri; i++) {
		if (dp[masca_plina][i] != INFINIT && distanta[i][0] != INFINIT) { // daca exista drum si muchie inapoi la 0
			int cost_circuit = dp[masca_plina][i] + distanta[i][0]; // costul total al circuitului
			if (cost_circuit < cost_minim_total)
				cost_minim_total = cost_circuit;
		}
	}
	return cost_minim_total;
}

void afiseaza_drumul_optim(int cost_final) {
	int masca_completa = (1 << nr_noduri) - 1; // masca cu toate nodurile vizitate
	int ultimul_nod = -1;

	// gasim ultimul nod din drum care conduce la costul final
	for (int i = 1; i < nr_noduri; i++) {
		if (dp[masca_completa][i] != INFINIT && distanta[i][0] != INFINIT) { // daca exista drum si muchie inapoi la 0
			if (dp[masca_completa][i] + distanta[i][0] == cost_final) { // daca costul total corespunde costului final
				ultimul_nod = i;
				break;
			}
		}
	}

	if (ultimul_nod == -1)
		return;

	int drum[21];
	int index = 0;
	int nod_it = ultimul_nod; // incepem reconstructia de la ultimul nod
	int m = masca_completa;

	while (nod_it != -1) {
		drum[index++] = nod_it; // adaugam nodul curent in drum
		int p = parinte[m][nod_it]; // gasim parintele nodului curent
		m = m ^ (1 << nod_it); // scoatem nodul curent din masca
		nod_it = p; // trecem la parinte
	}

	printf("0 "); // incepem drumul de la nodul 0
	for (int i = 0; i < index - 1; i++)
		printf("%d ", drum[i]);
	printf("0\n"); // incheiem drumul inapoi la nodul 0
}

int main() {
	int nr_muchii;
	if (scanf("%d %d", &nr_noduri, &nr_muchii) != 2 || nr_noduri <= 0) { // daca nu s-au citit corect sau nr_noduri e invalid
		printf("Nu există soluție.\n");
		return 0;
	}

	if (nr_noduri == 1) { // cazul cu un singur nod
		printf("0\n0 0\n");
		return 0;
	}

	initializeaza_tabele();

	for (int i = 0; i < nr_muchii; i++) {
		int u, v;
		int w;
		if (scanf("%d %d %d", &u, &v, &w) == 3) {
			if (u >= 0 && u < nr_noduri && v >= 0 && v < nr_noduri) { // daca nodurile sunt valide
				if (w < distanta[u][v]) // pastram doar muchia cu costul minim
					distanta[u][v] = distanta[v][u] = w; // graf neorientat
			}
		}
	}

	int rezultat = held_karp();

	if (rezultat >= INFINIT) { // daca nu exista drum Hamiltonian
		printf("Nu există soluție.\n");
	} else {
		printf("%d\n", rezultat);
		afiseaza_drumul_optim(rezultat);
	}

	return 0;
}