Acest README.md conține prezentarea generală a structurii arhivei. Acest README este împărțit în 4 părți
1. Prezentarea conținutul arhivei
2. Instrucțiuni de evaluare
3. Surse externe și referințe
4. Detaliera individuală a alegerii testelor

		Prezentarea conținutul repositoriului

Prezenta arhivă conține o implementare completă pentru rezolvarea Problemei Comis-voiajorului (TSP), 
incluzând atât o soluție exactă, cât și o soluție aproximativă. Structura proiectului este următoarea:

1. Raport.pdf
	Documentația completă a proiectului, care include:
	- Introducere
	- Demonstrație NP-Hard
	- Prezentare Algoritmi 
	- Evaluare
	- Concluzii
	- Bibliografie

2. Directorul src/
	a. Makefile
		Acest fișier reprezintă sistemul de build și automatizare definește următoarele reguli esențiale:
			- all: compilează ambele soluții (exactă și aproximativă)
			- TSP_exact și TSP_approx: reguli individuale pentru construcția fiecărui executabil
			- clean: șterge executabilele și conținutul folderelor output_exact și output_approx din
					 directorul tests/, păstrând structura directoarelor
			- clean-tests: Șterge complet toate testele generate, inclusiv fișierele de input și output-urile
						   asociate
			- test: Rulează întreaga suită de teste utilizând scriptul run_tests.sh situat în directorul tests/
			- generate-tests: Generează automat testele de intrare folosind scriptul Python din directorul tests/
	
	b. TSP_exact.c
			Acest fișier reprezintă soluția care obține întotdeauna rezultatul corect utilizând algoritmul Held-Karp
		(Dynamic Programming with Bitmasking), care garantează găsirea ciclului Hamiltonian de cost minim pentru
		grafuri complete cu până la 20 de noduri. Acest algoritm are complexitatea O(n²·2ⁿ), oferind soluția optimă
		într-un timp rezonabil pentru instanțe de dimensiuni moderate.
	
	c. TSP_approx.c
		Acest fișier reprezintă soluția care obține un răspuns apropiat de cel corect și consumă o cantitate
		rezonabilă de resurse (spațiu/timp) pentru orice instanță a problemei. Este o implementare hibridă care
		combină mai multe tehnici:
			- Verificare existență ciclu Hamiltonian: Folosește DFS pentru a detecta dacă graful conține un ciclu
													  Hamiltonian
			- Algoritmi de construcție:
						~ Nearest Neighbor: construiește un tur pornind de la un nod și adăugând mereu cel mai apropiat
											nod nevizitat
						~ Simple Insertion: inserează noduri în turul curent în poziția care minimizează creșterea
											costului
			- Algoritmul de optimizare 2-OPT: îmbunătățește soluția inițială prin schimbarea sistematică a muchiilor
											  pentru reducerea costului total

3. Directorul tests/
	a. generate_tests.py
		Acest script Python generează 34 de teste cu caracteristici diverse, detaliate individual în ultima parte a
		README-ului. Fiecare test este generat cu parametri controlați (seed-uri fixe pentru reproducibilitate) și
		salvat în formatul specificat.
	
	b. run_tests.sh
		Acest fișier reprezintă scriptul de testare, care compilează automat ambele programe, rulează fiecare test
		prin ambele implementări(exactă și aproximativă), implementează timeout (10 secunde pentru soluția exactă,
		5 secunde pentru cea aproximativă), compară rezultatele și afișează un tabel sumar care include:
			- numărul testului
			- costul soluției exacte
			- costul soluției aproximative
			- raportul aproximativ/exact (ratio)
			- indicarea "Match" pentru rezultate identice sau "Different" pentru rezultate divergente
	
	c. directorul input/
		Acesta conține fișierele .in generate de generate_tests.py
	
	d. output_exact/
		Acesta conține rezultatele produse de algoritmul Held-Karp
	
	e. output_approx/
		Acesta conține rezultatele produse de algoritmul aproximativ

4. Prezentul README.md

		Instrucțiuni de evaluare

a. Compilare
	cd src
	make

b. Generare teste
	make generate-tests
	# SAU
	cd tests
	python3 generate_tests.py

c. Rulare teste
	make test
	# SAU
	cd tests
	./run_tests.sh

d. Clean
	make clean          # Șterge executabilele și output-urile testelor
	make clean-tests    # Șterge toate testele generate (input + output)


		Surse externe și referințe

Pentru implementarea algoritmilor și structurarea soluțiilor, au fost consultate următoarele resurse tehnice și
teoretice:

1. Algoritmul Held-Karp (Programare Dinamică cu Bitmasking)
   - Sursa: "Introduction to Algorithms" (Cormen et al.) și GeeksforGeeks - TSP Dynamic Programming
   - Descriere: Documentația explică recurența folosită pentru a calcula costul minim vizitând un subset de orașe,
                folosind măști de biți pentru reprezentarea stării.
   - Utilizat în: src/TSP_exact.c pentru implementarea funcției principale de rezolvare exactă.

2. Euristica Nearest Neighbor și Optimizarea 2-OPT
   - Sursa: Wikipedia "2-opt algorithm" și articole de optimizare combinatorie
   - Descriere: Detaliază mecanismul de construcție rapidă a unui tur (Greedy) și tehnica de inversare a muchiilor
                (swapping) pentru eliminarea încrucișărilor din traseu.
   - Utilizat în: src/TSP_approx.c pentru funcțiile solve_approx și optimize_2opt.

3. Generarea numerelor pseudo-aleatoare și Scripting
   - Sursa: Documentația oficială Python (module random, argparse) și Bash Scripting Guide
   - Descriere: Metode pentru generarea distribuțiilor uniforme și automatizarea proceselor de testare.
   - Utilizat în: tests/generate_tests.py (pentru crearea instanțelor de test) și tests/run_tests.sh (pentru
                  pipeline-ul de evaluare).

4. Standarde de I/O și manipularea fișierelor în C
   - Sursa: Documentația GNU C Library
   - Descriere: Gestionarea pointerilor la fișiere și parsarea eficientă a input-ului.


			Justificarea setului exhaustiv de teste pentru problema TSP (Travelling salesman problem)

	În continuare se găsește explicația pentru fiecare test din setul exhaustiv de teste pentru problema
Traveling Salesman Problem (TSP). Setul acoperă toate scenariile posibile, de la cazuri simple la edge 
cases complexe, asigurând o verificare completă a corectitudinii, robusteței și performanței algoritmilor. 
Testele sunt organizate într-o progresie logica: de la verificări de bază, cazuri simple cu soluție 
cunoscută, la cazuri practice, realiste, ba chiar extreme.


Test 00: număr negativ de puncte (n ≤ 0)
Justificare matematică: TSP este definit pentru n ≥ 1, altfel fiind imposibil matematic
Testează:
- Gestionarea unui caz imposibil
- Validarea strictă a inputului
- Afișarea mesajului corect "Nu există soluție."
- Adaptare: n = -1


Test 01: Un singur oraș (n = 1)
Justificare matematică: Reprezintă cazul trivial: Un TSP cu un singur oraș are distanța 0
Testează:
- Gestionarea cazului limită n=1
- Verificarea validării inputului
- Output-ul corect pentru cazul degenerat
Riscuri acoperite: Segmentation fault pentru n=1, împărțire la zero, alocare memorie


Test 02: Doar 2 orașe (n = 2)
Justificare matematică: Distanța pe o linie (dus-întors)
Testează:
- Calculul corect al distanței
- Logica de bază a TSP pentru cel mai mic caz non-trivial
- Gestionarea tururilor cu 2 noduri
- Orice algoritm TSP trebuie să rezolve corect acest caz


Test 03: Triunghi echilateral (n = 3)
Justificare geometrică: Toate tururile au aceeași distanță (300)
Testează:
- Multiple soluții optime
- Simetria problemei
- Calculul precis al distanțelor
Complexitate matematică: Introduce prima problematică de permutări (3! = 6 tururi posibile)


Test 04: Pătrat (n = 4)
Justificare geometrică: Optimul este perimetrul (400)
Testează:
- Structuri geometrice regulate
- Conceptul de convex hull
- Calculul turului optim pentru poligoane convexe


Test 05: Puncte coliniare (n = 7)
Justificare matematică: Caz degenerat
Testează:
- Gestionarea cazurilor degenerate
- Algoritmi care se bazează pe unghiuri sau orientări
- Stabilitatea numerică pentru puncte coliniare
Risc acoperit: Algoritmii care folosesc determinantul pentru orientare pot da erori la coliniaritate perfectă


Test 06: Cerc perfect (n = 10)
Justificare geometrică: Puncte uniform distribuite pe cerc
Testează:
- Distribuția circulară (comună în probleme reale)
- Multiple tururi optime datorită simetriei
- Algoritmi de nearest neighbor pe structuri circulare
Complexitate: Introduce noțiunea de puncte echidistante de un centru


Test 07: Random mic (n = 8)
Justificare statistică: Distribuție uniformă aleatoare
Testează:
- Comportamentul pe date nepredictibile
- Calitatea soluțiilor euristice pe input aleator
- Lipsa pattern-urilor evidente
Importanță: Simulează cazuri generale fără structură particulară


Test 08: Grid regulat (n = 12) (costuri Manhattan)
Justificare practică: Orașe așezate în grid (ca o rețea de străzi)
Testează:
- Algoritmi pe structuri regulate
- Conceptul de distanțe Manhattan-like
- Gestionarea punctelor echidistante
Aplicație reală: Similar cu orașe planificate în caroiaj


Test 09: Două clustere (n = 10)
Justificare: Două grupuri de orașe (ca două orașe cu suburbii)
Testează:
- Algoritmi de clustering
- Salturile între clustere
- Conceptul de "spanner" între grupuri dense
Complexitate: Introduce conceptul de densitate variabilă


Test 10: Linie dreaptă cu zgomot (n = 10)
Justificare matematică: Varianta "noisy" a testului 05
Testează:
-Algoritmi pe structuri aproape-liniare
- Stabilitatea la mici abateri de la coliniaritate
- Distincția între puncte perfect și aproape coliniare
Adaptare: Costuri aproape liniare cu perturbări mici


Test 11: Costuri mari (simulând coordonate negative mari) (n = 15)
Justificare practică: Coordonatele negative sunt comune în CAD, grafică, GPS
Testează:
- Utilizarea tipurilor de date adecvate (long long vs int)
- Stabilitatea numerică la magnitudini mari
- Posibilele erori de overflow în calcule intermediare
Problemă matematică: Suma costurilor poate depăși limita pentru numere mari
Adaptare: Costuri 500k-1M pentru a testa limitele


Test 12: Dimensiune maximă (n = 20)
Justificare numerică: Testarea limitelor superioare permise
Testează:
- Complexitatea algoritmului Held-Karp (O(n²2ⁿ))
- Utilizarea eficientă a memoriei
- Scalabilitatea la n maxim


Test 13: Costuri foarte mici (n = 10)
Justificare numerică: Verifică precizia la numere mici
Testează:
- Gestionarea costurilor unitare
- Erori de rotunjire
- Comparații cu valori mici


Test 14: Scale mixte (n = 15)
Strategie de testare: Combinație de numere foarte mici, medii și foarte mari
Testează:
- Stabilitatea algoritmilor pe scale variate
- Problemele de scalare (scaling issues)
- Normalizarea inadecvată a datelor
Risc acoperit: Algoritmi care normalizează datele pot distorsiona distanțele relative


Test 15: Numere întregi puteri ale lui 2 (n = 8)
Justificare practică: Multe aplicații reale folosesc valori discrete
Testează:
- Operații pe puteri ale lui 2
- Optimizări posibile pentru numere întregi
- Calculul precis al distanțelor
Observație: Testează manipularea corectă a valorilor discrete
Adaptare: Costuri puteri ale lui 2 (2-1024)


Test 16: Costuri cu structură complexă (n = 12)
Justificare numerică: Testare pe valori cu pattern complex
Testează:
- Algoritmi pe date fără structură evidentă
- Capacitatea de a găsi soluții în spații complexe
- Robustete la pattern-uri neregulate


Test 17: Muchii duplicate (n = 15)
Justificare practică: Datele reale pot conține muchii duplicate
Testează:
- Selectarea costului minim pentru muchii multiple
- Parsarea corectă a inputului
- Eliminarea duplicatelor în preprocesare
Risc major: Alegerea greșită a costului mai mare
Adaptare: Muchii adăugate de două ori cu costuri diferite


Test 18: Structură spirală (n = 20)
Justificare geometrică: Puncte pe o structură ordonată crescător
Testează:
- Algoritmi pe structuri ordonate natural
- Conceptul de "nearest neighbor" pe structuri ordonate
- Distincția între puncte apropiate în index dar îndepărtate în cost
Complexitate: Nearest neighbor nu e optim pe structuri spiralate
Adaptare: Costuri crescătoare ca pe spirală


Test 19: Formă de stea (n = 16)
Justificare geometrică: Alternanță puncte apropiate și îndepărtate
Testează:
- Structuri cu puncte centrale și periferice
- Conceptul de convex hull vs interior points
- Algoritmi care optimizează local vs global
Problemă de optimizare: Turul optim trece prin punctele centrale la momentul potrivit
Adaptare: Centru (nodul 0) cu costuri diferite către celelalte


Test 20: Performanță medie (n = 20)
Justificare practică: Dimensiune medie pentru testare de performanță
Testează:
- Complexitatea timp O(n²2ⁿ) pentru Held-Karp
- Utilizarea memoriei
- Scalabilitatea algoritmului
Prag: n=20 este maximul permis, suficient pentru a testa performanța


Test 21: Graf incomplet (fără soluție) (n = 15)
Justificare: Test pentru grafuri fără ciclu Hamiltonian
Testează:
- Detectarea imposibilității soluției
- Gestionarea grafului insuficient de conectat
- Validarea existenței ciclului Hamiltonian
Adaptare: Graf cu insuficiente muchii pentru TSP


Test 22: Cluster extrem (n = 20)
Strategie: Cluster foarte dens + puncte dispersate
Testează:
- Algoritmi pe date cu densități extreme
- Problema salturilor între clustere
- Scalarea la clustere foarte dense
Observație: Într-un cluster dens, toate distanțele sunt apropiate, dificultând alegerea optimă
Adaptare: 10 noduri dense + 10 noduri îndepărtate, iar rezolvarea aproximativă găsită nu trece


Test 23: Structură pe "axe" (n = 15)
Justificare geometrică: Puncte grupate pe două dimensiuni
Testează:
- Cazuri degenerate bidimensionale
- Algoritmi care folosesc structura geometrică
- Calculul distanțelor în grupuri separate
Problemă: Multe puncte au structură similară pe o dimensiune
Adaptare: Două grupuri cu costuri mici în interior, mari între ele


Test 24: Simetrie perfectă (n = 12)
Justificare matematică: Simetrie în costuri
Testează:
- Multiple soluții optime datorită simetriei
- Algoritmi deterministi vs randomizați
- Echivalența tururilor prin permutări simetrice
Complexitate: Dacă o permutare este optimă, permutările simetrice sunt și ele optime
Adaptare: Costuri simetrice față de mijloc


Test 25: Model de tablă de șah (n = 16)
Justificare: Grid cu pattern alternant
Testează:
- Structuri periodice
- Pattern-uri regulate dar cu alternanță
- Conceptul de "metric learning" pe structuri cvasiregulate
- Simetrii multiple
Probleme acoperite: Algoritmii care văd structura câștigă, iar cei care tratează punctele independent pierd
Adaptare: 4×4 grid cu costuri bazate pe paritatea sumei coordonatelor


Test 26: Graf extrem de rar (n = 20)
Justificare: Test pentru grafuri cu număr minim de muchii
Testează:
- Gestionarea grafului sparse
- Capacitatea de a identifica cicluri Hamiltoniene minime
- Optimizări pentru grafuri rare
Adaptare: Ciclu Hamiltonian simplu (fiecare nod conectat doar cu următorul)


Test 27: Costuri unitare (n = 10)
Justificare numerică: Test pentru cazul cel mai simplu de costuri
Testează:
- Simplificări posibile pentru costuri constante
- Optimizări specifice
- Calculul corect al sumei
Cost așteptat: 10 (n×1)
Adaptare: Toate costurile = 1


Test 28: Input incomplet (invalid format)
Justificare practică: Verifică gestionarea inputului incomplet sau corupt
Testează:
- Detectarea EOF prematur
- Validarea completă înainte de procesare
- Gestionarea erorilor de I/O
- Principiul fail-early
Risc acoperit: Algoritmul începe să ruleze cu date incomplete, cauzând acces invalid la memorie
Adaptare: Fișier gol


Test 29: Caractere non-numerice (format invalid)
Justificare de robustețe: Verifică parsarea robustă și gestionarea formatelor neașteptate
Testează:
- Parsarea numerelor întregi
- Gestionarea caracterelor invalide (litere, simboluri)
- Recuperarea din erori de parsare
Risc acoperit: scanf() eșuează silențios, rămânând caractere în buffer
Adaptare: Linie cu "abc" în loc de număr


Test 30: Lipsă n (invalid format)
Justificare de validare: Verifică că programul cere formatul corect de la început
Testează:
- Prezența obligatorie a lui n pe prima linie
- Parsarea corectă a numărului de orașe
- Gestionarea fișierelor goale
- Gestionarea whitespace-ului în exces
Adaptare: Fișier cu doar whitespace


Test 31: "Spike" Pattern (n = 10)
Justificare algoritmică: Test pentru euristicile greedy și algoritmii de clustering
Testează:
- Nearest Neighbor: va merge la outlier și va rămâne blocat
- Algoritmi bazați pe densitate: vor trata outlier-ul ca noise
- Divide et Impera: partiție inechitabilă (1 vs multe)
- Convex Hull: outlier-ul domină complet hull-ul
Complexitate geometrică: Saltul enorm perturbă orice metrică locală
Adaptare: 9 noduri apropiate + 1 nod cu costuri enorme


Test 32: Grid cu găuri (n = 16)
Justificare structurală: Testează adaptabilitatea la obstacole sau restricții
Testează:
- Algoritmi care presupun graf complet
- Euristici care "sar" peste muchii lipsă
- Capacitatea de a ocoli zone inaccesibile
- Gestionarea discontinuităților
Problemă: Găurile creează căi ocolitoare
Adaptare: Grid 4×4 cu câteva muchii lipsă


Test 33: Structură fractală (simulată) (n = 15)
Justificare teoretică: Structură recursivă care testează adâncimea de analiză a algoritmului
Testează:
- Recursivitatea algoritmului (dacă e cazul)
- Auto-similaritatea la diferite scale
- Nearest Neighbor pe structuri care seamănă dar nu sunt identice
Problema algoritmică: Un algoritm care optimizează local va rămâne blocat într-o sub-structură
