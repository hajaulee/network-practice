#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 4096
typedef struct Schedule {
	char code[30];
	char name[100];
	int weekDay;
	int am_pm;
	int periodStart;
	int preiodEnd;
	char week[20];
	char room[30];
} Schedule;

Schedule g_sechdule[50];
char g_response[MAX_SIZE];
int g_totalSize = 0;
const char* g_DateOfWeek[] = {"", "", 
							"Monday",
							"Tuesday",
                            "Wednesday",
                            "Thursday",
                            "Friday",
                            "Saturday",
                            "Sunday"
                           };
const char * g_AMPM[] = {"", "Morning", "Afternoon"};
int lastIndexOf(char * s, char c) {
	int j;
	for(j=strlen(s)-1; j>=0; j--) {
		if(s[j] == c)
			return j;
	}
}
void readScheduleFile(char *fname) {
	FILE *fp;
	char buf[1024];
	fp = fopen(fname,"r");
	int i = 0;
	while((fgets(buf,sizeof(buf),fp))!=NULL) {
		int a, b, c;
		char d[100];
		sscanf(buf, "%[^\t]%*c%[^\t]%*c%d,%d,%d,%[^;]", g_sechdule[i].code,g_sechdule[i].name, &a, &b, &c,d);
		g_sechdule[i].weekDay = b/100;
		g_sechdule[i].am_pm = (b/10)%10;
		g_sechdule[i].periodStart = b%10;
		g_sechdule[i].preiodEnd = c%10;
		int lastQuote = lastIndexOf(d, ',');
		d[lastQuote] = '\0';
		strcpy((char*)g_sechdule[i].week, (char*)d);
		strcpy(g_sechdule[i].room, d + lastQuote +1);
		i++;
	}
	g_totalSize = i;
}

char* displaySchedule(char* dayOfWeek) {
	memset(g_response, 0, MAX_SIZE);
	int printedLength = sprintf(g_response,"\n%-10s|%-30s|%-10s|%-10s|%-10s|%-20s|%-10s\n", "Code", "Course", "Week Day", "AM/PM", "Period", "Week", "Room");
	int i;
	for(i = 0; i < g_totalSize; i++){
		if(strcmp(g_DateOfWeek[g_sechdule[i].weekDay], dayOfWeek) == 0){
			printedLength += sprintf(g_response + printedLength, "%-10s|%-30s|%-10s|%-10s|%-1d-%-8d|%-20s|%-10s\n",
				g_sechdule[i].code,
				g_sechdule[i].name,
				g_DateOfWeek[g_sechdule[i].weekDay],
				g_AMPM[g_sechdule[i].am_pm],
				g_sechdule[i].periodStart,
				g_sechdule[i].preiodEnd,
				g_sechdule[i].week,
				g_sechdule[i].room);
		}
	}
	return g_response;	
}

void displayBusySchedule(){
	printf("%-3s|%-10s|%-10s|%-10s|%-10s|%-10s|\n","..." ,
							"Monday",
							"Tuesday",
                            "Wednesday",
                            "Thursday",
                            "Friday");
    int i, j, k, has;
    for(i = 1; i < 13; i++){
    	printf("%-3d|", i);
		for(j = 2; j < 7; j++){
			has = 0;
			for(k = 0; k < g_totalSize; k++){
				int ca = ((g_sechdule[k].am_pm-1)*6);
				if(g_sechdule[k].weekDay == j && g_sechdule[k].periodStart + ca  <= i && g_sechdule[k].preiodEnd+ca >= i){
					printf("%-10s|", g_sechdule[k].room);
					has = 1;
					break;
				}
			}
			if(!has)
				printf("%-10s|", "");
		}
		puts("");	
	}
}


