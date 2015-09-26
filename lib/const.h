#define LCDRows		2
#define LCDCols		16
#define LCDDataBits	4

/* WiringPi Interfaces */
#define LCDRS		11
#define LCDStrobe	10
#define DB0			0
#define DB1			1
#define DB2			2
#define DB3			3
#define DB4			0
#define DB5			0
#define DB6			0
#define DB7			0

/* LCD Screen */
#define FirstCol	0
#define FirstRow	0
#define SecondRow	1

/* Define KeyPressed */
#define KeyUp		65
#define KeyDown		66
#define KeyEnter	10
#define KeyBspace	8
#define KeyDel		27

/* Define StringHandler */
#define inpNum		"Input Number"
#define slcPvd		"Select Provider"
#define inpNom		"Input Nominal"

#define stringLen	25
#define pNumLen		15
#define nmLen		9

#define pvQty		3

typedef struct s_providerData {
	int pvCode;
	char pvName[25];
} Provider;

Provider pv[pvQty] = {
	{1, "Telkomsel"},
	{2, "Indosat"},
	{3, "XL"}
};