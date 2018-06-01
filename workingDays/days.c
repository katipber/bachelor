#ifndef __PROGTEST__
#include <stdio.h>
#include <assert.h>
#endif /* __PROGTEST__ */

int IsHoliday(int m, int d);
int ValidateOne(int y, int m, int d);
int ValidateTwo(int y1, int m1, int d1, int y2, int m2, int d2);
int BaseDay(int m, int d);
int FindDay(int y, int m, int d);
int CountLeapYears(int y);
int IsLeapYear(int y);
int YearInfo(int y);
int MacroWorkDays(int y1, int y2);
int MonthLength(int y, int m);
int FindIndex(int y, int m, int d);
int IsWeekDay(int y, int m, int d);
int Holidays(int y, int m, int d);
int WeekDays(int y, int m, int d);
int TrimY1(int y, int m, int d);
int TrimY2(int y, int m, int d);


int IsHoliday(int m, int d){
	return ((m==1 && d==1)||(m==5 && d==1)||(m==5 && d==8)||(m==7 && d==5)||(m==7 && d==6)||(m==9 && d==28)||(m==10 && d==28)||(m==11 && d==17)||(m==12 && d==24)||(m==12 && d==25)||(m==12 && d==26));
}

int ValidateOne(int y, int m, int d){
	if(y>1999){
		switch(m){
			case 1: return(d>0 && d<32);
			case 2: if(IsLeapYear(y)) return(d>0 && d<30);
					else return(d>0 && d<29);
			case 3: return(d>0 && d<32);
			case 4: return(d>0 && d<31);
			case 5: return(d>0 && d<32);
			case 6: return(d>0 && d<31);
			case 7: return(d>0 && d<32);
			case 8: return(d>0 && d<32);
			case 9: return(d>0 && d<31);
			case 10: return(d>0 && d<32);
			case 11: return(d>0 && d<31);
			case 12: return(d>0 && d<32);
			default: return 0;
		}
	}
	return 0;
}

int ValidateTwo(int y1, int m1, int d1, int y2, int m2, int d2){
	int i;
//	printf("validate2\n");
	if((372*(y2-y1)+31*(m2-m1)+d2-d1)<0) return 0;
	i = ValidateOne(y1, m1, d1) && ValidateOne(y2, m2, d2);
	return i;
}

int BaseDay(int m, int d){
	int i=4, j;
	for(j=0; j<m-1; j++){
		switch(j){
			case 0: i += 3; break;
			case 1: i += 1; break;
			case 2: i += 3; break;
			case 3: i += 2; break;
			case 4: i += 3; break;
			case 5: i += 2; break;
			case 6: i += 3; break;
			case 7: i += 3; break;
			case 8: i += 2; break;
			case 9: i += 3; break;
			case 10: i += 2; break;
			case 11: i += 3; break;
		}
	}
	return (i+d)%7;
	
}

int FindDay(int y, int m, int d){
	int i;
	if(m>2){
		i = BaseDay(m, d);
		i += (y-2000)+CountLeapYears(y);
		i %= 7;
	}
	else{
		i = BaseDay(m, d);
		i += (y-2000)+CountLeapYears(y-1)+!!(y-2000);
		i %= 7;
	}
	return i;
}

int CountLeapYears(int y){
	int i=y-2000;
	return -y/4000 + i/400 - i/100 + i/4;
}

int IsLeapYear(int y){
	return (!(y%4) && (y%100 || !(y%400)) && y%4000);
}

int YearInfo(int y){
	int i;
	if 		(!(y%4000))	i = 4;
	else if	(!(y%400)) 	i = 3;
	else if	(!(y%100)) 	i = 7;
	else if	(!(y%4)) 	i = 1;
	else 				i = 5;
	return i;
}

int MacroWorkDays(int y1, int y2){
	
	int i=0, m, n; // i: working days; m: year info, n:years to be calculated, starting day of next m years
	int d[] = {2, 5, 6, 0, 6, 1, 4, 5, 6, 6}; // year first day incrementation for 1LY, 4LY, 100LY, 400LY, 4000LY, 1!LY, 4!LY.. years
	int wd[10][7] = {{254, 254, 253, 255, 254, 252, 253}, 
					{1013, 1014, 1013, 1013, 1011, 1010, 1011}, 
					{25305, 25304, 25304, 25304, 25302, 25303, 25303}, 
					{101214, 101211, 101214, 101211, 101212, 101211, 101212}, 
					{50606034, 50606054, 50606024, 50606054, 50606024, 50606035, 50606025}, 
					{252, 253, 253, 252, 254, 254, 252}, 
					{1010, 1012, 1013, 1012, 1012, 1011, 1010}, 
					{25302, 25304, 25303, 25303, 25303, 25302, 25303}, 
					{101211, 101213, 101210, 101213, 101210, 101212, 101211}, 
					{1012119, 1012139, 1012109, 1012139, 1012109, 1012120, 1012110}
					}; // working days for 1LY, 4LY, 100LY, 400LY, 4000LY, 1!LY, 4!LY.. years starting with corresponding days
	
	n = FindDay(y1, 1, 1); // first day of y1
	m = YearInfo(y1);
	int q=0;
	while(y1 <= y2){
//		printf("y/m/macro: %8d / %d / %10d\n", y1, m, i);
		switch(m){
			case 0:
				i += wd[m][n];
				n = (n+d[m])%7;
				y1++;
				m = YearInfo(y1); break;
			case 1:
				if((y2-y1)/4){
					i += wd[m][n];
					n = (n+d[m])%7;
					y1 += 4;
					m = YearInfo(y1);
				}
				else m = 0; break;
			case 2:
				if((y2-y1)/100){
					i += wd[m][n];
					n = (n+d[m])%7;
					y1 += 100;
					m = YearInfo(y1);
				}
				else m = 1; break;
			case 3:
				if((y2-y1)/400){
					i += wd[m][n];
					n = (n+d[m])%7;
					y1 += 400;
					m = YearInfo(y1);
				}
				else m = 2; break;
			case 4:
				if((y2-y1)/200000){
					i += wd[m][n];
					n = (n+d[m])%7;
					y1 += 200000;
					m = YearInfo(y1);
				}
				else m = 9; break;
			case 5:
				i += wd[m][n];
				n = (n+d[m])%7;
				y1++;
				m = YearInfo(y1); break;
			case 6:
				if((y2-y1)/4){
					i += wd[m][n];
					n = (n+d[m])%7;
					y1 += 4;
					m = YearInfo(y1);
				}
				else m = 5; break;
			case 7:
				if((y2-y1)/100){
					i += wd[m][n];
					n = (n+d[m])%7;
					y1 += 100;
					m = YearInfo(y1);
				}
				else m = 6; break;
			case 8:
				if((y2-y1)/400){
					i += wd[m][n];
					n = (n+d[m])%7;
					y1 += 400;
					m = YearInfo(y1);
				}
				else m = 7; break;
			case 9:
				if((y2-y1)/4000){
					i += wd[m][n];
					n = (n+d[m])%7;
					y1 += 4000;
					m = YearInfo(y1);
					q++;
				}
				else m = 8; break;
		}
	}
//	printf("%d %d\n", i, q);
	return i;
}

int MonthLength(int y, int m){
	switch(m){
		case 1: return 31;
		case 2: if(IsLeapYear(y))return 29;
				else return 28;
		case 3: return 31;
		case 4: return 30;
		case 5: return 31;
		case 6: return 30;
		case 7: return 31;
		case 8: return 31;
		case 9: return 30;
		case 10: return 31;
		case 11: return 30;
	}
	return 0;
}

int FindIndex(int y, int m, int d){
	int i, j;
	i = d;
	for(j=0; j<m; j++){
		i += MonthLength(y, j);
	}
	return i;
}

int IsWeekDay(int y, int m, int d){
	if(FindDay(y, m, d)<5) return 1;
	return 0;
}

int Holidays(int y, int m, int d){
	int i, j=0, di, hi=1, count=0; // di: day index, hi: holiday index
	int hd[] = {1, 1, 8, 5, 6, 28, 28, 17, 24, 25, 26};
	int hm[] = {1, 5, 5, 7, 7, 9, 10, 11, 12, 12, 12};
	
	di = FindIndex(y, m, d);
	while(di>=hi && j<11){
		j++;
		hi = FindIndex(y, hm[j], hd[j]);
	}
	for(i=0; i<j; i++){
		count += IsWeekDay(y, hm[i], hd[i]);
//		printf("IWD: %d %d/%d\n", IsWeekDay(y, hm[i], hd[i]), hd[i], hm[i]);
	}
	return count;
}

int WeekDays(int y, int m, int d){
	int weks = FindIndex(y, m, d)/7;
	int days = FindIndex(y, m, d)%7;
	int i, wd = weks*5;
	for(i=0; i<days; i++){
		wd += IsWeekDay(y, 1, 1+i);
	}
	return wd;
}

int TrimY1(int y, int m, int d){
	int i = WeekDays(y, m, d) - Holidays(y, m, d);
//	printf("trim1: %d\n", i);
	return i;
}

int TrimY2(int y, int m, int d){
	int i = MacroWorkDays(y,y)-(WeekDays(y, m, d) - Holidays(y, m, d));
//	printf("trim2: %d Holidays:%d\n", i, Holidays(y, m, d));
	return i;
}

int IsWorkDay ( int y, int m, int d )
{
	if (!ValidateOne(y, m, d)) return 0;
	if (IsHoliday(m, d)) return 0;
	return IsWeekDay(y, m, d);
}

int CountWorkDays ( int y1, int m1, int d1,
                    int y2, int m2, int d2,
                    int * cnt )
{
	if (!ValidateTwo(y1, m1, d1, y2, m2, d2)) return 0;
//	printf("valid\n");
	*cnt = MacroWorkDays(y1, y2)-TrimY1(y1, m1, d1)-TrimY2(y2, m2, d2)+IsWorkDay(y1, m1, d1);
//	printf("isworkday: %d\n", IsWorkDay(y1, m1, d1));
	return 1;
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
  int cnt;
  
// my code

/*	printf("IWD? %d\n",IsWorkDay ( 2200, 12, 31 ));
	printf("CWD? %d\n",CountWorkDays ( 2475, 5, 17, 5037824, 10, 25, &cnt ));
	printf("cnt: %d\n\n", cnt);

	int y, m, d;
	printf("start\n");
	while(cnt>0){
		for(y=0; y<1000000 ;y++){
			for(m=0; m<11 ;m++){
				for(d=0; d<31 ;d++){
					if (CountWorkDays ( 2000+y, 1+m, 1+d, 2000+y*y, 1+(m*m)%12, 1+(d*d)%31, &cnt )) printf("%d.%d.%d | %d.%d.%d = %d\n", 2000+y, 1+m, 1+d, 2000+y*y, 1+(m*m)%12, 1+(d*d)%31, cnt);
					else printf("%d.%d.%d | %d.%d.%d = invalid\n", 2000+y, 1+m, 1+d, 2000+y*y, 1+(m*m)%12, 1+(d*d)%31);
				}
			}
		}
	}
	
   

	int y, m, d;
  
  scanf("%d %d %d", &y, &m, &d);
  printf("LY:%d\n", IsLeapYear(y));
  printf("day:%d\n", BaseDay(m, d));
  printf("IWD? %d\n", IsWorkDay(y, m, d));
  
  scanf("%d %d", &y, &m);
  MacroWorkDays(y,m);
    
*/  
// end of my code

  assert ( IsWorkDay ( 2016, 11, 11 ) );
  assert ( ! IsWorkDay ( 2016, 11, 12 ) );
  assert ( ! IsWorkDay ( 2016, 11, 17 ) );
  assert ( ! IsWorkDay ( 2016, 11, 31 ) );
  assert ( IsWorkDay ( 2016,  2, 29 ) );
  assert ( ! IsWorkDay ( 2004,  2, 29 ) );
  assert ( ! IsWorkDay ( 2001,  2, 29 ) );
  assert ( ! IsWorkDay ( 1996,  1,  1 ) );
  assert ( CountWorkDays ( 2016, 11,  1,
                           2016, 11, 30, &cnt ) == 1
           && cnt == 21 );
  assert ( CountWorkDays ( 2016, 11,  1,
                           2016, 11, 17, &cnt ) == 1
           && cnt == 12 );
  assert ( CountWorkDays ( 2016, 11,  1,
                           2016, 11,  1, &cnt ) == 1
           && cnt == 1 );
  assert ( CountWorkDays ( 2016, 11, 17,
                           2016, 11, 17, &cnt ) == 1
           && cnt == 0 );
  assert ( CountWorkDays ( 2016,  1,  1,
                           2016, 12, 31, &cnt ) == 1
           && cnt == 254 );
  assert ( CountWorkDays ( 2015,  1,  1,
                           2015, 12, 31, &cnt ) == 1
           && cnt == 252 );
  assert ( CountWorkDays ( 2000,  1,  1,
                           2016, 12, 31, &cnt ) == 1
           && cnt == 4302 );
  assert ( CountWorkDays ( 2001,  2,  3,
                           2016,  7, 18, &cnt ) == 1
           && cnt == 3911 );
  assert ( CountWorkDays ( 2014,  3, 27,
                           2016, 11, 12, &cnt ) == 1
           && cnt == 666 );
  assert ( CountWorkDays ( 2001,  1,  1,
                           2000,  1,  1, &cnt ) == 0 );
  assert ( CountWorkDays ( 2001,  1,  1,
                           2015,  2, 29, &cnt ) == 0 );
  return 0;
}
#endif /* __PROGTEST__ */
