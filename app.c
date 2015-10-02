#include <lcd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#include <mysql.h>

#include "lib/getch.h"
#include "lib/const.h"
#include "lib/funct.h"

int main(){

	signal(SIGINT, signalHandler);

	wiringPiSetup();
	int lcdDisplay = lcdInit (LCDRows, LCDCols, LCDDataBits,
							  LCDRS, LCDStrobe,
							  DB0, DB1, DB2, DB3,
							  DB4, DB5, DB6, DB7);

	char pNumber[pNumLen];
	char Nominal[nmLen];
	char *stringHandler;
	
	MYSQL *Con;
	MYSQL *realCon;
	
	char query[queryLen];
	int queryBuff = sizeof(query);

	int Key, x = 0;

	while(1){
		int i = 0;
		memset(pNumber, '\0', sizeof(pNumber));
		memset(Nominal, '\0', sizeof(Nominal));
		
		Con = mysql_init(NULL);
		realCon = mysql_init(NULL);

		lcdClear(lcdDisplay);
		stringHandler = inpNum;
		render(lcdDisplay, "_", stringHandler);

		do {
			Key = getch();
			pNumber[i] = Key;
			
			if (Key == KeyBspace) {
				i -= 1;
				pNumber[i] = '\0';
				render(lcdDisplay, pNumber, stringHandler);
			} else if (Key == KeyDel) {
				i = 0;
				render(lcdDisplay, pNumber, stringHandler);
			} else if(i == pNumLen - 1){
				render(lcdDisplay, "Max input reached!", stringHandler);
				continue;
			} else {
				render(lcdDisplay, pNumber, stringHandler);
				i++;
			}
		} while (Key != KeyEnter);
		pNumber[i-1] = '\0';
		lcdClear(lcdDisplay);

		stringHandler = slcPvd;
		render(lcdDisplay, pv[x].pvName, stringHandler);

		do {
			Key = getch();
			if (Key == KeyUp) {
				x = scrollUp(x);
				render(lcdDisplay, pv[x].pvName, stringHandler);
			} else if (Key == KeyDown) {
				x = scrollDown(x);
				render(lcdDisplay, pv[x].pvName, stringHandler);
			}
		} while (Key != KeyEnter);
		lcdClear(lcdDisplay);

		i = 0;
		stringHandler = inpNom;
		render(lcdDisplay, "_", stringHandler);
		
		do {
			Key = getch();
			Nominal[i] = (char)Key;

			if (Key == KeyBspace) {
				i -= 1;
				Nominal[i] = '\0';
				render(lcdDisplay, Nominal, stringHandler);
			} else if (Key == KeyDel) {
				i = 0;
				render(lcdDisplay, Nominal, stringHandler);
			} else if(i == nmLen - 1){
				render(lcdDisplay, "Max input reached!", stringHandler);
				continue;
			}else {
				render(lcdDisplay, Nominal, stringHandler);
				i++;
			}
		} while (Key != KeyEnter);
		Nominal[i-1] = '\0';
		
		realCon = mysql_real_connect(Con, hostname, sqlUser, sqlPass, dataBase, 0, NULL, 0);
		if (realCon == NULL){
			exit(1);
		}
		
		snprintf(query, queryBuff, "INSERT INTO outbox(DestinationNumber, Class, TextDecoded) VALUES ('%s', '%s', '%s.%c.%s')", dataCenter, nonFlashFormat, pNumber, pv[x].pvCode, Nominal);
		
		mysql_query(Con, query);
		mysql_close(Con);
		
		printf("%s, %s, %s\n", pNumber, pv[x].pvName, Nominal);
		
		lcdClear(lcdDisplay);
	}
	return 0;
}