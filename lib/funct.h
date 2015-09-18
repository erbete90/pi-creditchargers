void signalHandler(int sig){
	char c;
	
	signal(sig, SIG_IGN);
	printf("Do you really want to quit ? [y/n]\n");
	c = getch();
	if (c == 'y' || c == 'Y') {
		exit(0);
	} else {
		signal(SIGINT, signalHandler);
	}
}

/*int renderCLI(char *pNumber){
	system("clear");
	printf("%s\n", pNumber);
}*/

void render(int lcdDisplay, const char *Value, char *String){
	lcdClear(lcdDisplay);
	lcdPosition(lcdDisplay, FirstCol, FirstRow);
	lcdPrintf(lcdDisplay, "%s", String);
	lcdPosition(lcdDisplay, FirstCol, SecondRow);
	lcdPrintf(lcdDisplay, "%s", Value);
	printf("%s\n", Value);
}

int scrollUp(int i) {
	if (i == 0)
		i = pvQty - 1;
	else
		i--;
	return i;
}

int scrollDown(int i) {
	if (i == pvQty - 1)
		i = 0;
	else
		i++;
	return i;
}

void flushMalloc(char *pNumber, char *Nominal) {
	free(pNumber);
	free(Nominal);
}