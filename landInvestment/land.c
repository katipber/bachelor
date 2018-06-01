#include <stdio.h>
#include <limits.h>
#include <string.h>
#define GMAX 2000

int getMap(int *x, int *y, int pmap[][GMAX]);
int getQuery(int *q1, int *q2);
int countQuery (int x, int y, int q2, int arr[][GMAX], int org[][GMAX]);
int listQuery (int lvl, int x, int y, int q2, int arr[][GMAX], int org[][GMAX]);

int getMap(int *x, int *y, int pmap[][GMAX]){
	
	int i, j;
	long int li;
	
	printf("Map size:\n");
	if(scanf("%d %d", x, y) != 2) return 1;
	if( *x<1 || *x>GMAX || *y<1 || *y>GMAX) return 1;
	
	printf("Price map:\n");
	for(j=0; j<*y; j++){
		for(i=0; i<*x; i++){
			if(scanf("%ld", &li) != 1) return 1;
			if(li > INT_MAX) return 1;
			pmap[j][i] = li;
		}
	}
//	printf("x:%d y:%d\n", *x, *y); // ***DB***
	return 0;
}

int getQuery(int *q1, int *q2){
	
	char q[6];
	long int li;
	
	if(scanf("%5s %ld", q, &li) != 2){
		if(feof(stdin)) return -1;
	}
	if(li > INT_MAX) return 1;
	*q2 = li;
	if(!strcmp(q, "count")) *q1 = 0;
	else if(!strcmp(q, "list")) *q1 = 1;
	else return 1;
		
	return 0;
}

int countQuery (int x, int y, int q2, int arr[][GMAX], int org[][GMAX]){
	
	int i, j, k, count=0, val=0;
	static int narr[GMAX][GMAX];
	
	for(i=0; i<y; i++){
		for(j=0; j<x; j++){
			if(j) narr[i][j-1] = arr[i][j]+org[i][j-1];
			for(k=i; k<y; k++){
				val += arr[k][j];
				if(val == q2) count++;
			}
			val = 0;
		}
	}
	x--;
	if(x>0) count += countQuery(x, y, q2, narr, org);
	
	return count;	
}

int listQuery (int lvl, int x, int y, int q2, int arr[][GMAX], int org[][GMAX]){
	
	int i, j, k, count=0, val=0;
	static int narr[GMAX][GMAX];
	
	for(i=0; i<y; i++){
		for(j=0; j<x; j++){
			if(j) narr[i][j-1] = arr[i][j]+org[i][j-1];
			for(k=i; k<y; k++){
				val += arr[k][j];
				if(val == q2){
					count++;
					printf("%d @ (%d,%d) - (%d,%d)\n", q2, j, i, j+lvl, k);
				}
			}
			val = 0;
		}
	}
	lvl++; x--;
	if(x>0) count += listQuery(lvl, x, y, q2, narr, org);
	
	return count;	
}

int main(void){
	
	int x, y, q1, q2, count, lvl=0, f=0;
	static int pmap[GMAX][GMAX];
	
	if(getMap(&x, &y, pmap)){
		printf("Invalid input.\n");
		return 1;
	}
	printf("Queries:\n");
	while(!f){
		f = getQuery(&q1, &q2);
		if(!f){
			if(q1 == 0) count = countQuery(x, y, q2, pmap, pmap);
			else count = listQuery(lvl, x, y, q2, pmap, pmap);
			printf("Total: %d\n", count);	
		}
	}	
	if(f == 1){ 
		printf("Invalid input.\n");
		return 1;
	}
/*	
int i, j;

for(j=0; j<y; j++){
for(i=0; i<x; i++){
printf("%d ", pmap[j][i]);
}
printf("\n");
}
printf("x:%d y:%d\nq1:%d q2:%d\ncount:%d\n", x, y, q1, q2, count);
*/	
	return 0;
}
