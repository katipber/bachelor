#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define TOKENMAX 100

typedef struct TMove{
	int moves[TOKENMAX], len, s0, s1, sum;
} TMOVE;

typedef struct TLevel{
	int *token;
	TMOVE m[5], *mm;
} TLVL;

TLVL **LVL;
int TOKEN[TOKENMAX], TOT;

void DB(int lvl){
	
/*	int i, j, k, l;
	
	for(i=0; i<TOT-2; i++){
		for(j=0; j<i+1; j++){
			for(k=0; k<5; k++){
				for(l=0; l<2+2*i; l++) printf("%d ", LVL[i][j].m[k].moves[l]);
				printf("\n");
				printf("LVL[%d][%d]:m[%d]:%d:%d:%d:%d\n", i, j, k,
				LVL[i][j].m[k].len,
				LVL[i][j].m[k].s0,
				LVL[i][j].m[k].s1,
				LVL[i][j].m[k].sum);
			}
			printf("LVL[%d][%d]:mm:%d:%d:%d\n", i, j,
			LVL[i][j].mm->s0,
			LVL[i][j].mm->s1,
			LVL[i][j].mm->sum);
		}
	}
*/
	
	printf("%d:%d\n", LVL[lvl][0].mm->s0, LVL[lvl][0].mm->s1);

}

void createLevel(void){
	
	int i;
	
	LVL = (TLVL**) malloc (TOKENMAX*sizeof(*LVL));
	for(i=0; i<TOKENMAX; i++)
		LVL[i] = (TLVL*) malloc ((i+1)*sizeof(**LVL));
}

void compareTwo(int level){
	
	int *t = TOKEN+level;
	
	LVL[level][level].m[0].moves[0] = 0;
	LVL[level][level].m[0].moves[1] = 0;
	LVL[level][level].m[0].len = 2;
	LVL[level][level].m[0].s0 = t[0];
	LVL[level][level].m[0].s1 = t[1];
	LVL[level][level].m[0].sum = t[0]-t[1];
	
	LVL[level][level].m[2].moves[0] = 2;
	LVL[level][level].m[2].len = 1;
	LVL[level][level].m[2].s0 = t[0]+t[1];
	LVL[level][level].m[2].s1 = 0;
	LVL[level][level].m[2].sum = t[0]+t[1];
	
	LVL[level][level].m[4].moves[0] = 4;
	LVL[level][level].m[4].moves[1] = 4;
	LVL[level][level].m[4].len = 2;
	LVL[level][level].m[4].s0 = t[1];
	LVL[level][level].m[4].s1 = t[0];
	LVL[level][level].m[4].sum = t[1]-t[0];

	LVL[level][level].mm = &LVL[level][level].m[0];
	if(LVL[level][level].mm->sum < LVL[level][level].m[2].sum)
		LVL[level][level].mm = &LVL[level][level].m[2];	
	if(LVL[level][level].mm->sum < LVL[level][level].m[4].sum)
		LVL[level][level].mm = &LVL[level][level].m[4];
}

void setStage(int level, int stage){
	
	int i, len, s0, s1;
	int *h = TOKEN+stage;
	int *t = TOKEN+level+1;
	
	len = LVL[level][stage+1].mm->len;
	for(i=0; i<len; i++){
		LVL[level][stage].m[0].moves[i] = LVL[level][stage+1].mm->moves[i];
	}
	LVL[level][stage].m[0].moves[i] = 0;
	LVL[level][stage].m[0].len = len+1;
	s0 = LVL[level][stage].m[0].s0 = *h + LVL[level][stage+1].mm->s1;
	s1 = LVL[level][stage].m[0].s1 = LVL[level][stage+1].mm->s0;
	LVL[level][stage].m[0].sum = s0-s1;

	len = LVL[level][stage+1].m[0].len;
	for(i=0; i<len-1; i++){
		LVL[level][stage].m[1].moves[i] = LVL[level][stage+1].m[0].moves[i];
	}
	LVL[level][stage].m[1].moves[i] = 1;
	LVL[level][stage].m[1].len = len;
	s0 = LVL[level][stage].m[1].s0 = *h + LVL[level][stage+1].m[0].s0;
	s1 = LVL[level][stage].m[1].s1 = LVL[level][stage+1].m[0].s1;
	LVL[level][stage].m[1].sum = s0-s1;
	
	len = LVL[level-1][stage].m[0].len;
	for(i=0; i<len-1; i++){
		LVL[level][stage].m[2].moves[i] = LVL[level-1][stage].m[0].moves[i];
	}
	LVL[level][stage].m[2].moves[i] = 2;
	LVL[level][stage].m[2].len = len;
	s0 = LVL[level][stage].m[2].s0 = *t + LVL[level-1][stage].m[0].s0;
	s1 = LVL[level][stage].m[2].s1 = LVL[level-1][stage].m[0].s1;
	LVL[level][stage].m[2].sum = s0-s1;	

	len = LVL[level-1][stage].m[4].len;
	for(i=0; i<len-1; i++){
		LVL[level][stage].m[3].moves[i] = LVL[level-1][stage].m[4].moves[i];
	}
	LVL[level][stage].m[3].moves[i] = 3;
	LVL[level][stage].m[3].len = len;
	s0 = LVL[level][stage].m[3].s0 = *t + LVL[level-1][stage].m[4].s0;
	s1 = LVL[level][stage].m[3].s1 = LVL[level-1][stage].m[4].s1;
	LVL[level][stage].m[3].sum = s0-s1;	
	
	len = LVL[level-1][stage].mm->len;
	for(i=0; i<len; i++){
		LVL[level][stage].m[4].moves[i] = LVL[level-1][stage].mm->moves[i];
	}
	LVL[level][stage].m[4].moves[i] = 4;
	LVL[level][stage].m[4].len = len+1;
	s0 = LVL[level][stage].m[4].s0 = *t + LVL[level-1][stage].mm->s1;
	s1 = LVL[level][stage].m[4].s1 = LVL[level-1][stage].mm->s0;
	LVL[level][stage].m[4].sum = s0-s1;	
}

void setMax(int level, int stage){
	
	int i;
		
	LVL[level][stage].mm = &LVL[level][stage].m[0];
	for(i=1; i<5; i++){
		if(LVL[level][stage].mm->sum < LVL[level][stage].m[i].sum)
			LVL[level][stage].mm = &LVL[level][stage].m[i];
	}
	
}

void setLevel(int level, int stage){
	
	if(level != stage){
		setLevel(level, stage+1);
		setStage(level, stage);
		setMax(level, stage);
	}
	else{
		compareTwo(level);
	}
}

void printMoves(int level){
	
	int i = LVL[level][0].mm->len-1;
	int *h = TOKEN;
	int *t = TOKEN+TOT-1;
	int *f = h;
	int player = 0;
	
	while(i>-1){
		if(!player) printf("A ");
		else printf("B ");
		switch(LVL[level][0].mm->moves[i]){
		case 0: printf("%ld (%d)\n", h-f, *h); h++; break;
		case 1: printf("%ld, %ld (%d,%d)\n", h-f, h-f+1, *h, *(h+1)); h += 2; break;
		case 2: printf("%ld, %ld (%d,%d)\n", h-f, t-f, *h, *t); h++; t--; break;
		case 3: printf("%ld, %ld (%d,%d)\n", t-f, t-f-1, *t, *(t-1)); t -= 2; break;
		case 4: printf("%ld (%d)\n", t-f, *t); t--; break;
		}
		player = (player+1)%2;
		i--;
	}
	printf("A: %d, B: %d\n", LVL[level][0].mm->s0, LVL[level][0].mm->s1);
}

void printOne(void){
	printf("A 0 (%d)\nA: %d, B: 0\n", TOKEN[0], TOKEN[0]);
}

void deleteLevel(void){
	
	int i;
	
	for(i=0; i<TOKENMAX; i++)
		free(LVL[i]);
	free(LVL);
}

int main(void){
	
	int level = -2;
	
	createLevel();
	printf("Tokens:\n");
	while(scanf("%d", &TOKEN[TOT]) == 1){
		TOT++;
		if(++level>-1) setLevel(level, 0);
	}
	if(!feof(stdin) || !TOT){
		printf("Invalid input.\n");
		deleteLevel();
		return 1;
	}
	if(TOT>1) printMoves(level);
	else printOne();
	deleteLevel();
//	DB(level);
	return 0;
}
