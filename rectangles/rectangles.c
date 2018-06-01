#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>

typedef struct TPlay{
	int data[240][6], index;
}TPLAY;

typedef struct TCell{
	int y, x, val;
}TCELL;

typedef struct TPuzzle{
	int **move, movei, cellc;
	struct TCell *cell;
	struct TPuzzle *next;
}TPUZZLE;

char PUZZLE[66][99];
int ORIGINAL[32][32], BOARD[32][32], OSIZE[2], PSIZE[2], CELLC;
TPUZZLE *SOLLIST, *CURSOL, *NEXTSOL, *LASTSOL, *REZ, *LASTREZ;
TPLAY BESTPLAY, DEEPPLAY, TMPPLAY;
TCELL CELL[200];

/*

	DEBUG START



void cS(void){

	int i = 0;

	CURSOL = REZ;
	while(CURSOL){
		CURSOL = CURSOL->next;
		i++;
	}
	printf("sol %5d\n", i);
	CURSOL = SOLLIST;

}

void pB(void){

	int i = 0;
	int j;

	for(i=0; i<OSIZE[0]; i++){
		for(j=0; j<OSIZE[1]; j++){
			printf("%2d ", BOARD[i][j]);
		}
		printf("\n");
	}
	printf("\n");

}



	DEBUG END - GET START

*/

void addOriginal(int val, int ind){
	
	ORIGINAL[OSIZE[0]][ind] = val;
	if(val){
		CELL[CELLC].y = OSIZE[0];
		CELL[CELLC].x = ind;
		CELL[CELLC].val = val;
		CELLC++;
	}
}

int validFirst(char *p){
	
	if(*p++ != '+') return -1;
		while(*p != '\n')
			if(*p++ != '-' || *p++ != '-' || *p++ != '+') return -1;
	
		return 1;
}

int validMid(char *p, int **tot){
	
	int index, val;

	if(*p++ != '|') return -1;

	index = 0;

	while(*p != '\n'){
	
		val = 0;
	
		if(*p != ' '){
		
			if(!isdigit(*p) || !isdigit(*(p+1)) || !(*p-'0')) return -1;
		
			**tot += val = ((*p-'0')*10)+(*(p+1)-'0');
			p += 2;
		}
		else if(*++p != ' '){
			
			if(!isdigit(*p) || !(*p-'0')) return -1;
		
			**tot += val = (*p-'0');
			p++;
		}
		else p++;
	
		if(*p != ' '){
			if(*p++ != '|' || *p != '\n') return -1;
		}
		else if(*++p == '\n') return -1;
	
		if(CELLC > 200) return -1;
		addOriginal(val, index++);
	}

	return 2;

}

int validLast(char *p){
	
	if(*p++ != '+') return -1;

	OSIZE[0]++;

	if(*p == ' '){
		while(*p != '\n')
			if(*p++ != ' ' || *p++ != ' ' || *p++ != '+') return -1;
	
		return 1;
	}
	else if(*p == '-'){
		while(*p != '\n')
			if(*p++ != '-' || *p++ != '-' || *p++ != '+') return -1;
	
		return 0;
	}
	else return -1;
}

int validateLine(char *p, int stage, int *tot){
	
	int flag = -1;
	
	if(stage == 1) flag = validMid(p, &tot);
	else if(stage == 2) flag = validLast(p);
	else if(!stage) flag = validFirst(p);
	return flag;
}

int getPuzzle(void){
	
	char *line;
	size_t len = 99;
	int flag = 0, total = 0, nlen;
	
	line = (char*) malloc (len * sizeof(*line));
	
	if((PSIZE[1] = getline(&line, &len, stdin)) < 5 || PSIZE[1] > 98){
		free(line); return -1;
	}	
	strcpy(PUZZLE[PSIZE[0]++], line);
	
	
	OSIZE[1] = (PSIZE[1]-2)/3;
	flag = validateLine(line, flag, &total);
	
	while(flag != -1){
		
		nlen = getline(&line, &len, stdin);

		if(!flag && nlen > -1){
			free(line); return -1;
		}
		if(feof(stdin)) break;
		if(PSIZE[1] != nlen){
			free(line); return -1;
		}
		flag = validateLine(line, flag, &total);
		strcpy(PUZZLE[PSIZE[0]++], line);
		if(PSIZE[0] > 65){
			free(line); return -1;
		}
	}

	if(flag) flag = -1;
	else if(total != (OSIZE[0])*(OSIZE[1])) flag = -2;
	free(line);
	return flag;
}

/*

	GET END - SOLVE START

*/

TPUZZLE *createSol(void){
	
	TPUZZLE *p;
	int i;
	
	p = (TPUZZLE*) malloc (sizeof(*p));
	
	p->cellc = CELLC;
	p->cell = (TCELL*) malloc (CELLC*sizeof(*(p->cell)));
	
	for(i=0; i<CELLC; i++){
		p->cell[i].val = CELL[i].val;
		p->cell[i].y = CELL[i].y;
		p->cell[i].x = CELL[i].x;
	}
	
	p->movei = 0;
	
	return p;
}

void resetBoard(void){
	
	int i, j;
	
	for(i=0; i<OSIZE[0]; i++){
		for(j=0; j<OSIZE[1]; j++){
			BOARD[i][j] = ORIGINAL[i][j];
		}
	}
}

void setBoard(TPUZZLE *pz){
	
	int k, i, j, ymin, ymax, xmin, xmax;
	
	resetBoard();
	for(k=0; k<pz->movei; k++){
		
		ymin = pz->move[k][0];
		xmin = pz->move[k][1];
		ymax = pz->move[k][2];
		xmax = pz->move[k][3];
		
		for(i=ymin; i<=ymax; i++)
			for(j=xmin; j<=xmax; j++)
				BOARD[i][j] = -1;
		
	}
}

int checkVar(int y, int x, int ymin, int ymax, int xmin, int xmax, int val){
	
	int i, j, flag = 1;
	
	if(ymin < 0 || ymax >= OSIZE[0] || xmin < 0 || xmax >= OSIZE[1]) return 0;
	
	BOARD[y][x] = 0;
	
	for(i=ymin; i<=ymax && flag; i++){
		for(j=xmin; j<=xmax && flag; j++){
			if(BOARD[i][j]) flag = 0;
		}
	}
		
	if(flag){
		TMPPLAY.data[TMPPLAY.index][0] = ymin;
		TMPPLAY.data[TMPPLAY.index][1] = ymax;
		TMPPLAY.data[TMPPLAY.index][2] = xmin;
		TMPPLAY.data[TMPPLAY.index][3] = xmax;
		TMPPLAY.index++;
	}
	
	BOARD[y][x] = val;

	return flag;
}

int positionIndex(int y, int x, int val, int row){
	
	int i, j, col = val/row, var = 0;

	for(i=0; i<row; i++){
		for(j=0; j<col; j++){
			var += checkVar(y, x, y-i, y+row-i-1, x-j, x+col-j-1, val);
		}
	}
	return var;
}

int countVar(int i, int j, int val){
	
	int div, var = 0;
	
	for(div=1; div<=val; div++){
		if(!(val%div)) var += positionIndex(i, j, val, div);
	}
	return var;
}

TPUZZLE *copySol(TPUZZLE *orj, int cindex){
	
	TPUZZLE *p;
	int i = 0, j = 0;
	
	p = (TPUZZLE*) malloc (sizeof(*p));
	
	p->cellc = orj->cellc - 1;
	p->cell = (TCELL*) malloc (p->cellc*sizeof(*(p->cell)));

	for(i=0; i<orj->cellc; i++){
		if(i!=cindex){
			p->cell[j].val = orj->cell[i].val;
			p->cell[j].y = orj->cell[i].y;
			p->cell[j].x = orj->cell[i].x;
			j++;
		}
	}
	
	p->movei = orj->movei+1;
	p->move = (int**) malloc (p->movei*sizeof(*(p->move)));
	
	for(i=0; i<p->movei; i++){
		p->move[i] = (int*) malloc (4*sizeof(**(p->move)));
	}

	for(i=0; i<orj->movei; i++){
		for(j=0; j<4; j++){
			p->move[i][j] = orj->move[i][j];
		}
	}
	
	return p;
}

void killSol(TPUZZLE *p){
	
	int i;
	
	for(i=0; i<p->movei; i++)
		free(p->move[i]);
	
	free(p->move);
	free(p->cell);
	free(p);
}

TPUZZLE *modifySol(TPUZZLE *orj, int cindex){
	
	TPUZZLE *p;
	int i = 0, j = 0;
	
	p = (TPUZZLE*) malloc (sizeof(*p));
	
	p->cellc = orj->cellc - 1;
	p->cell = (TCELL*) malloc (p->cellc*sizeof(*(p->cell)));

	for(i=0; i<orj->cellc; i++){
		if(i!=cindex){
			p->cell[j].val = orj->cell[i].val;
			p->cell[j].y = orj->cell[i].y;
			p->cell[j].x = orj->cell[i].x;
			j++;
		}
	}
	
	p->movei = orj->movei+1;
	p->move = (int**) malloc (p->movei*sizeof(*(p->move)));
	
	for(i=0; i<p->movei; i++){
		p->move[i] = (int*) malloc (4*sizeof(**(p->move)));
	}

	for(i=0; i<orj->movei; i++){
		for(j=0; j<4; j++){
			p->move[i][j] = orj->move[i][j];
		}
	}
	
	killSol(orj);
	return p;
}

TPUZZLE *goDeep(TPUZZLE *pz){
	
	int i, y, x, val = 0, nval, var = 1152, nvar, index = -1;
	setBoard(pz);
	for(i=0; i<pz->cellc; i++){
		
		y = pz->cell[i].y;
		x = pz->cell[i].x;
		nval = pz->cell[i].val;
		
		TMPPLAY.index = 0;
		nvar = countVar(y, x, nval);

		if(!nvar){
			killSol(pz);
			return NULL;
		}
		
		if(nval > 16 && nval > val){
			BESTPLAY = TMPPLAY;
			index = i;
			var = nvar;
			val = nval;
		}
	
		else if(val < 17 && nvar < var){
			BESTPLAY = TMPPLAY;
			index = i;
			var = nvar;
			val = nval;
		}

		else if(val < 17 && nvar == var && nval > val){
			BESTPLAY = TMPPLAY;
			index = i;
			var = nvar;
			val = nval;
		}

	}
	
	for(i=var; i>1; i--){
		NEXTSOL = copySol(pz, index);
		NEXTSOL->move[NEXTSOL->movei-1][0] = BESTPLAY.data[i-1][0];
		NEXTSOL->move[NEXTSOL->movei-1][1] = BESTPLAY.data[i-1][2];
		NEXTSOL->move[NEXTSOL->movei-1][2] = BESTPLAY.data[i-1][1];
		NEXTSOL->move[NEXTSOL->movei-1][3] = BESTPLAY.data[i-1][3];
		NEXTSOL->next = LASTSOL;
		LASTSOL = NEXTSOL;	
	}
	
	if(var){
		pz = modifySol(pz, index);
		pz->move[pz->movei-1][0] = BESTPLAY.data[0][0];
		pz->move[pz->movei-1][1] = BESTPLAY.data[0][2];
		pz->move[pz->movei-1][2] = BESTPLAY.data[0][1];
		pz->move[pz->movei-1][3] = BESTPLAY.data[0][3];
	}
	
	if(pz->cellc) pz = goDeep(pz);
	
	return pz;
}

void playAll(void){
/*
int memuse;
struct mallinfo before, after;
before = mallinfo();
*/
	while(SOLLIST){

		if((REZ = goDeep(SOLLIST))){
			REZ->next = LASTREZ;
			LASTREZ = REZ;
		}

		if((SOLLIST = LASTSOL))	LASTSOL = LASTSOL->next;
/*
after = mallinfo();
memuse = (after.uordblks - before.uordblks);
printf("\nmem %5d\n", memuse/1000000);
cS();
*/
	}
}

void solvePuzzle(void){
	
	CURSOL = SOLLIST = createSol();
	SOLLIST->next = LASTSOL = REZ = LASTREZ = NULL;
	playAll();
	
}

/*

	SOLVE END - PRINT START

*/

void drawBoarders(int c){

	int i;
	int ymin = LASTREZ->move[c][0];
	int xmin = LASTREZ->move[c][1];
	int ymax = LASTREZ->move[c][2];
	int xmax = LASTREZ->move[c][3];
	
	for(i=1+3*xmin; i<=1+3*xmax; i+=3){
		PUZZLE[2*ymin][i] = '-';
		PUZZLE[2*ymin][i+1] = '-';
		PUZZLE[2*(ymax+1)][i] = '-';
		PUZZLE[2*(ymax+1)][i+1] = '-';
	}
	
	for(i=1+2*ymin; i<=1+2*ymax; i+=2){
		PUZZLE[i][3*xmin] = '|';
		PUZZLE[i][3*(xmax+1)] = '|';
	}
	
}

void printPuzzle(void){

	int i, j;
	
	printf("One solution:\n");
	for(i=0; i<PSIZE[0]; i++){
		for(j=0; j<PSIZE[1]; j++){
			printf("%c", PUZZLE[i][j]);
		}
	}
}

void printResult(void){
	
	int i, c = 0;
	
	if(!LASTREZ) printf("No solution.\n");
	
	else if(!LASTREZ->next){
		for(i=0; i<LASTREZ->movei; i++) drawBoarders(i);
		printPuzzle();
		killSol(LASTREZ);
		LASTREZ = NULL;
	}
	
	else{
		while(LASTREZ){
			REZ = LASTREZ->next;
			killSol(LASTREZ);
			LASTREZ = REZ;
			c++;
		}
		REZ = NULL;
		printf("Total solutions: %d\n", c);
	}
}


/*

	PRINT END

*/

int main(void){
	
	int flag;

	printf("Enter the puzzle:\n");
	
	flag = getPuzzle();
	
	if(flag == -1){
		printf("Invalid input.\n");
		return 1;
	}
	
	if(flag == -2){
		printf("No solution.\n");
		return 0;
	}
	
	solvePuzzle();
	printResult();
	
	return 0;
}
