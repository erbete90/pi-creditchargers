#include <lcd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
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

	char *pNumber;
	char *Nominal;
	char *stringHandler;

	int Key, x = 0;

	while(1){
		int i = 0;
		pNumber = (char *) malloc (pNumLen * sizeof(char));
		Nominal = (char *) malloc (nmLen * sizeof(char));
		stringHandler = (char *) malloc (stringLen * sizeof(char));
		memset(pNumber, 0, pNumLen * sizeof(char));
		memset(Nominal, 0, nmLen * sizeof(char));
		memset(stringHandler, 0, stringLen * sizeof(char));

		lcdClear(lcdDisplay);
		stringHandler = inpNum;
		render(lcdDisplay, '\0', stringHandler);

		do {
			Key = getch();
			pNumber[i] = Key;

			if (Key == KeyBspace) {
				i -= 1;
				pNumber[i] = '\0';
				render(lcdDisplay, pNumber, stringHandler);
			} else if (Key == KeyDel) {
				i = 0;
				memset(pNumber, 0, pNumLen * sizeof(char));
				render(lcdDisplay, pNumber, stringHandler);
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
		render(lcdDisplay, '\0', stringHandler);
		
		do {
			Key = getch();
			Nominal[i] = Key;

			if (Key == KeyBspace) {
				i -= 1;
				Nominal[i] = '\0';
				render(lcdDisplay, Nominal, stringHandler);
			} else if (Key == KeyDel) {
				i = 0;
				memset(Nominal, 0, nmLen * sizeof(char));
				render(lcdDisplay, Nominal, stringHandler);
			} else {
				render(lcdDisplay, Nominal, stringHandler);
				i++;
			}
		} while (Key != KeyEnter);
		Nominal[i-1] = '\0';
		lcdClear(lcdDisplay);

		lcdPosition(lcdDisplay, FirstCol, FirstRow);
		lcdPrintf(lcdDisplay, "%s $s", Nominal, pv[x].pvName);

		lcdPosition(lcdDisplay, FirstCol, FirstRow);
		lcdPrintf(lcdDisplay, "%s", pNumber);

		flushMalloc(pNumber, Nominal);
	}
	flushMalloc(pNumber, Nominal);
	return 0;
}