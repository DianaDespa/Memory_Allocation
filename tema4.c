/* Despa Diana-Alexandra 311CA */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Functiile implementate in acest program realizeaza actiunile 
corespunzatoare comenzilor cu acelasi nume, dupa cum sunt prezentate 
in cerinta. */

char *INITIALIZE(int N) {
	return (char *)calloc(1, N);
}

void DUMP(const char *arena, int N) {
	int i;
	
	for (i = 1; i <= N; i++) {
		if (i % 16 == 1) { 
			printf("%.8X\t", i - 1); 
		} /* Se afiseaza indicele curent la fiecare 16 octeti, in formatul 
		precizat. */
		printf("%.2X ", ((unsigned int)arena[i - 1]) & 0x000000FF);
		/* Se afiseaza fiecare octet, in formatul cerut. */
		if (i % 16 == 8) {
			printf(" ");
		} /* Se afiseaza un spatiu in plus la mijlocul unei linii de octeti. */
		if (i % 16 == 0 ) {
			printf("\n");
		} /* Se trece pe o linie noua dupa afisarea a 16 octeti. */
 	}
 	if (N % 16 != 0) {
 		printf("\n");
 	} /* Daca ultimul indice nu este multiplu de 16, el se va afisa pe o linie 
 	noua. */
 	printf("%.8X\n", N);
}

void ALLOC(char *arena, char *str, int N) {
	int SIZE, *ar, poz, *aux;
	char **err = (char **)malloc(sizeof(char *));
	
	SIZE = strtol(str, err, 0); /* Se face conversia de la sir de caractere la 
	intreg pentru parametrul SIZE al functiei. */
	free(err);
	
	ar = (int *)arena; /* Pointer-ul "ar" este folosit la parcurgerea si 
	modificarea zonelor de gestiune, deoarece acestea contin date de tip 
	"int". */
	
	/* In continuare, se cauta o zona libera de memorie. Pentru realizarea 
	alocarii, se disting mai multe cazuri:  */
	
	if (SIZE <= N - 16) { /* Dimensiunea blocului ce trebuie alocat nu poate 
	depasi dimensiunea arenei-16 (= indexul de start + o zona de gestiune). */
		
		if (ar[0] == 0) {/* Cazul in care arena este complet libera -  */
			ar[0] = 4; /* Indexul de start al arenei devine 4. */
			ar = (int *)(arena + sizeof(int)); /* "Ar" pointeaza la prima zona 
			de gestiune. */
			ar[0] = 0; /* Deoarece arena nu mai are alocat niciun bloc, 
			indexul de start al blocului urmator e 0. */
			ar[1] = 0; /* Fiind primul bloc alocat, indexul de start al 
			blocului anterior va fi indexul de start al arenei. */
			ar[2] = SIZE + 12; /* Dimensiunea blocului va fi cea ceruta + cei 
			12 octeti de gestiune. */ 
			printf("16\n"); /* Indexul afisat va fi indexul de start al 
			blocului + cei 12 octeti de gestiune. */
		} else {
			
			if (ar[0] >= 16 + SIZE) { /* Cazul in care blocul cerut are loc 
			intre indexul de start al arenei si primul bloc care exista deja 
			in arena. */
				ar = (int *)(arena + sizeof(int));
				ar[0] = ((int *)arena)[0]; /* Indexul de start al urmatorului 
				bloc se obtine din indexul de start al arenei. */
				ar[1] = 0; 
				ar[2] = SIZE + 12; 
				((int *)arena)[0] = 4; /* Indexul de start al arenei este 4. */
				((int *)(arena + ar[0]))[1] = 4; /* Este actualizat indexul de 
				start al blocului anterior, pentru blocul urmator. */
				printf("16\n");
			} else {
				/* Se cauta spatiu pentru alocare intre blocurile din arena. */
				
				poz = ar[0]; 
				ar = (int *)(arena + poz);
				/* Variabila "poz" retine indexul blocului curent (la care 
				pointeaza ar). */
				
				if (ar[0] != 0) { /* Conditia de oprire a cautarii este ca "ar"
				sa pointeze la ultimul bloc din arena sau gasirea unui spatiu 
				suficient pentru blocul ce trebuie alocat. */
					while (SIZE + 12 > ar[0] - poz - ar[2]) {
						poz = ar[0];
						ar = (int *)(arena + poz);/* "Ar" se deplaseaza la 
						urmatorul bloc. */
						if (ar[0] == 0) {
							break;
						} 
					}
				}
				
				if (ar[0] != 0) {
				/* Cazul in care exista spatiu intre 2 blocuri consecutive. */
					if (SIZE + 12 <= ar[0] - poz - ar[2]) {
						printf("%d\n", poz + ar[2] + 12);
						aux = (int *)(arena + poz + ar[2]);
						/* Pozitia noului bloc este data de pozitia blocului 
						precedent + dimesiunea blocului precedent. Variabila 
						"aux" pointeaza la zona de gestiune a noului bloc. */
						aux[2] = SIZE + 12;
						aux[0] = ar[0];
						aux[1] = poz;
						ar[0] = poz + ar[2];
						((int *)(arena + aux[0]))[1] = ar[0];
						/* Se actualizeaza zona de gestiune a blocului curent 
						si legaturile (indecsii de start) cu blocurile vecine. 
						*/	
					}
					
				} else if (SIZE + 12 <= N - poz - ar[2]) {
				/* Cazul in care exista spatiu pentru noul bloc la sfarsitul 
				arenei, dupa ultimul bloc deja existent. */
					printf("%d\n", poz + ar[2] + 12);
					aux = (int *)(arena + poz + ar[2]);
					aux[2] = SIZE + 12;
					aux[0] = 0;
					aux[1] = poz;
					ar[0] = poz + ar[2];
					/* Se actualizeaza zona de gestiune a blocului curent si 
					legaturile (indecsii de start) cu blocurile vecine. */	
				} else {
					printf("0\n");
				}
			}
		}
	} else {
		printf("0\n");
	}/* Altfel nu se aloca spatiu in arena. */
}

void FREE(char *arena, char *str, int n) {
	int INDEX, *ar;
	char **err = (char **)malloc(sizeof(char *));
	
	INDEX = strtol(str, err, 0);/* Se face conversia de la sir de caractere 
	la intreg pentru parametrul INDEX al functiei. */
	free(err);
	
	ar = (int *)(arena + INDEX - 12);/* "Ar" pointeaza la blocul ce trebuie 
	eliberat. */
	
	if (ar[0] != 0) {
		((int *)(arena + ar[0]))[1] = ar[1];
	}
	((int *)(arena + ar[1]))[0] = ar[0];
	/* Se actualizeaza legaturile (indecsii de start) dintre blocurile vecine. 
	*/
}

void FILL(char *arena, char *str3, char *str2, char *str1, int N) {
	int INDEX, SIZE, VALUE, i;
	char **err = (char **)malloc(sizeof(char *));
	
	INDEX = strtol(str1, err, 0);
	SIZE = strtol(str2, err, 0);
	VALUE = strtol(str3, err, 0);
	/* Se face conversia de la sir de caractere la intreg pentru parametrii 
	functiei. */
	free(err);
	
	for (i = INDEX; i < INDEX + SIZE; i++) {
		arena[i] = VALUE;/* Fiecare octet va fi actualizat cu valoarea 
		VALUE. */
	}
}

void SHOW_FREE(const char *arena, int N) {
	int bytes = 0, blocks = 0, *ar;
	
	ar = (int *)arena;
	
	if (ar[0] == 0) {
		printf("1 blocks (%d bytes) free\n", N - 4);
		/* Daca arena este goala, numarul de octeti liberi va fi N - numarul 
		de octeti ai zonei de gestiune(4). */
	} else {
		
		if (ar[0] > 4) {
			blocks++;
			bytes += ar[0] - 4;
			/* Daca primul bloc nu se afla imediat dupa indexul de start al 
			arenei, se numara un bloc liber si se aduna diferenta de octeti la 
			numarul de octeti liberi. */
		}
		ar = (int *)(arena + ar[0]);
		
		while (ar[0] != 0) {
			if (ar[0] - ar[2] - ((int *)(arena + ar[1]))[0] > 0) {
				blocks++;
				bytes += ar[0] - ar[2] - ((int *)(arena + ar[1]))[0];
			}
			ar = (int *)(arena + ar[0]);
			/* Se parcurge arena bloc cu bloc (blocuri ocupate), se verifica 
			si se calculeaza numarul de octeti liberi dintre ele si numarul de 
			blocuri libere.*/
		} 
		
		if (N - ar[2] - ((int *)(arena + ar[1]))[0] > 0) {
			blocks++;
			bytes += N - ar[2] - ((int *)(arena + ar[1]))[0];
		}/* Se fac calcule si pentru cazul in care exista spatiu nealocat la 
		sfarsitul arenei. */
		
		printf("%d blocks (%d bytes) free\n", blocks, bytes);
	}
}

void SHOW_USAGE(const char *arena, int N) {
	int blocks = 0, bytes = 0, rez = 4, *ar, fragm = 0, free = 0;
	
	ar = (int *)arena;
	
	if (ar[0] > 4) {
		free++;
	}
	
	while (ar[0] != 0) {
		ar = (int *)(arena + ar[0]);
		if (ar[1] > 0) {
			if (((int *)(arena + ar[1]))[0] - ar[1] - 
					((int *)(arena + ar[1]))[2] > 0) {	
				free++;
			}
		}
		blocks++;
		bytes += ar[2] - 12;
		rez += ar[2];
	}
	/* Se numara blocurile ocupate si blocurile libere intr-un mod similar cu 
	cel din functia SHOW_FREE. Numarul de octeti rezervati este suma 
	dimensiunilor tuturor blocurilor ocupate, iar numarul de octeti folositi 
	efectiv este dimensiunilor tuturor blocurilor ocupate minus dimensiunea 
	tuturor zonelor de gestiune. */
	
	if (((int *)arena)[0] != 0) {
		if (N - ar[2] - ((int *)(arena + ar[1]))[0] > 0) {
			free++;
		}
	}
	
	if (blocks != 0) {
		fragm = (free - 1) * 100 / blocks;
	}
	
	printf("%d blocks (%d bytes) used\n", blocks, bytes);
	printf("%d%s", (int)bytes*100/rez, "%");
	printf(" efficiency\n");
	printf("%d%s", (int)fragm, "%");
	printf(" fragmentation\n");
	
	/* Se calculeaza datele cerute si se afiseaza conform enuntului. */
}

void SHOW_ALLOCATIONS(const char *arena, int N) {
	int *ar;
	
	
	/* Se parcurge arena si se verifica intr-un mod similar cu cel din 
	functiile SHOW_FREE si SHOW_USAGE care sunt zonele ocupate si zonele 
	libere. */
	
	ar = (int *)arena;
	printf("OCCUPIED 4 bytes\n");
	
	if (ar[0] != 0) {
		
		if (ar[0] > 4) {
			printf("FREE %d bytes\n", ar[0] - 4);
		}
		ar = (int *)(arena + ar[0]);
		printf("OCCUPIED %d bytes\n", ar[2]);
		
		while (ar[0] != 0) {
			ar = (int *)(arena + ar[0]);
			if (((int *)(arena + ar[1]))[0] - ar[1] - 
				((int *)(arena + ar[1]))[2] > 0) {
				printf("FREE %d bytes\n", ((int *)(arena + ar[1]))[0] - ar[1] 
					-((int *)(arena + ar[1]))[2]);
			}
			printf("OCCUPIED %d bytes\n", ar[2]);
		}
		
		if (N - ar[2] - ((int *)(arena + ar[1]))[0] > 0) {
			printf("FREE %d bytes\n", N - ar[2] - ((int *)(arena + ar[1]))[0]);
		}
	} else {
		printf("FREE %d bytes\n", N - 4);
	}
}

void SHOW(const char *arena, char *str, int N) {
	/* Functia SHOW poate apela una dintre functiile SHOW_FREE, SHOW_USAGE 
	sau SHOW_ALLOCATIONS, in functie de sirul de caractere extras din comanda, 
	primit ca al doilea argument. */
	
	if ( 0 == strcmp(str, "FREE") ) {
		SHOW_FREE(arena, N);
	} else if ( 0 == strcmp(str, "USAGE") ) {
		SHOW_USAGE(arena, N);
	} else {
		SHOW_ALLOCATIONS(arena, N);
	}
}

int main() {
	char *arena; /* Am ales ca arena sa fie un pointer de tip char, deoarece 
	tipul "char" este reprezentat pe 1 octet, iar manipularea datelor este mai 
	facila astfel. */
	
	char comm[50], *tok;
	int N;
	char **err=(char **)malloc(sizeof(char *));
	
	/* Pentru inceput, functia main citeste de la tastatura prima comanda, 
	INITIALIZE <N>, a carei validare nu este necesara deoarece se cunoaste 
	faptul ca toate comenzile sunt valide. */

	fgets(comm, 50, stdin); 
	comm[strlen(comm) - 1] = '\0'; /* Deoarece functia fgets include si \n in 
	sirul de caractere citit, se deplaseaza sfarsitul de sir, \0, inapoi cu un 
	caracter*/
	tok = strtok(comm, " ");
	tok = strtok(NULL, " ");/* Se extrage sirul ce contine numarul N din 
	comanda introdusa. */
	N = strtol(tok, err, 0);/* Se face conversia de la sir de caractere la 
	intreg petru N. */
	free(err);
	
	arena = INITIALIZE(N);/* Se aloca spatiul cerut pentru arena. */  

	do {
		fgets(comm, 50, stdin);
		comm[strlen(comm) - 1] = '\0';
		/* Se citeste o comanda completa (cu parametri) si se deplaseaza 
		sfarsitul de sir inapoi cu o pozitie. */
		tok = strtok(comm, " ");/* Se extrage cuvantul cheie din comanda. */ 
		
		/* In continuare, se verifica ce comanda a fost introdusa de la 
		tastatura si se apeleaza o functie corespunzatoare. */
		if ( 0 == strcmp(tok, "DUMP") ) {
			DUMP( arena, N );
		
		} else if ( 0 == strcmp(tok, "ALLOC") ) {
			ALLOC( arena, strtok(NULL, " "), N );
		
		} else if ( 0 == strcmp(tok, "FREE") ) {
			FREE( arena, strtok(NULL, " "), N );
		
		} else if ( 0 == strcmp(tok, "FILL") ) {
			FILL( arena, strtok(NULL, " "), strtok(NULL, " "), 
				strtok(NULL, " "), N );
				
		} else if ( 0 == strcmp(tok, "SHOW") ) {
			SHOW( arena, strtok(NULL, " "), N);
		}
		/* Deoarece comenzile ALLOC, FREE, FILL si SHOW primesc parametri, 
		functiile care le corespund primesc ca argumente sirurile de caractere 
		corespunzatoare fiecarui argument, extrase din comanda completa cu 
		functia "strtok". */
		
	} while ( strcmp(tok, "FINALIZE") ); /* Se citesc instructiuni pana la 
	citirea comenzii "FINALIZE". */ 
	
	free(arena);/* Se elibereaza zona de memorie alocata pentru arena. */

	return 0;	
}
