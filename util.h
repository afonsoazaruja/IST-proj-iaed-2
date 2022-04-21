/*
 * File: util.h
 * Author: Afonso Azaruja, 103624 - LEIC-A
 * Description: Macros, structs, prototypes and global variables for proj.c
*/

/*------------------------------------------------------------------------------
                                    MACROS
------------------------------------------------------------------------------*/

/* max size of airport id */
#define MAX_ID 4
/* max size of countries */
#define MAX_COUNTRY 31
/* max size of cities */     
#define MAX_CITY 51
/* max size of string */
#define MAX_STR 65536
/* max size of flight code */
#define MAX_CODE 8
/* min size of reservation code */
#define MIN_RES 10

/* max number of airports */
#define MAX_AIRPORT 40
/* max number of flights */
#define MAX_FLIGHTS 30000
/* min number of passengers */
#define MIN_PASS 10

/* for calculating minutes - calcMinutes function */
#define YEAR 2022
#define MON 1
#define DAY 1
#define MIN_DAY 1440
#define MIN_YEAR 525600

/* for validating input */
#define ERROR -1
#define SUCCESS 1

/* airport errors */
#define ERR_INV_ID "invalid airport ID\n"
#define ERR_TOO_MANY_AIR "too many airports\n"
#define ERR_DUP_ID "duplicate airport\n"
#define ERR_NO_ID "no such airport ID\n"

/* flight errors */
#define ERR_INV_COD "invalid flight code\n"
#define ERR_DUP_FLIGHT "flight already exists\n"
#define ERR_TOO_MANY_FLIGHTS "too many flights\n"
#define ERR_INV_DATE "invalid date\n" /* system date and reservation error aswell */
#define ERR_INV_DURATION "invalid duration\n"
#define ERR_INV_CAP "invalid capacity\n"

/* reservation errors */
#define ERR_INV_RES "invalid reservation code\n"
#define ERR_NO_FLY "flight does not exist\n"
#define ERR_DUP_RES "flight reservation already used\n"
#define ERR_TOO_MANY_RES "too many reservations\n"
#define ERR_INV_PASS "invalid passenger number\n"

/* deletion error */
#define ERR_NOT_FOUND "not found\n"

/* program error */
#define ERR_NO_MEM "No memory.\n"

/*------------------------------------------------------------------------------
                                    STRUCTS
------------------------------------------------------------------------------*/

typedef struct airports {
    char id[MAX_ID];
    char country[MAX_COUNTRY];
    char city[MAX_CITY];
    int n_flights;
}Airports;

typedef struct date {
    int d;
    int m;
    int y;
}Date;

typedef struct time {
    int h;
    int m;
}Time;

typedef struct flights {
    char code[MAX_CODE];
    char id_dep[MAX_ID];
    char id_arr[MAX_ID];
    Date date_dep;
    Date date_arr;
    Time time_dep;
    Time time_arr;
    Time duration;
    int cap;
    int n_pass;
    int min_dep;
    int min_arr;
}Flights; 

typedef struct result {
    int index;
    int value;
    Date date;
}Result;

typedef struct reserv {
    char cod_f[MAX_CODE];
    char *cod_r;
    int n_pass;
    Date date;
    struct reserv *next;
}reserv;

/*------------------------------------------------------------------------------
                                    PROTOTYPES
------------------------------------------------------------------------------*/

/*--------------------------------MAIN FUNCTIONS------------------------------*/
void addAirport(char input[]);
void listAirports(char input[]);
void addFlight(char input[]);
void listFlights(char input[]);
void changeDate(char input[]);
void addReservation(char input[]);
void deletion(char input[]);
void listReservations(int d, int m, int y);

/*------------------------------AUXILIRY FUNCTIONS----------------------------*/
/*retrieve functions*/
int retrieveID(char input[], int i);
void retrieveLocation(char input[]);
void retrieveTime(char input[], int i, int j);
int retrieveCap(char input[], int i);
int retrieveReservation(char input[], int i);

/*reservation functions*/
reserv * newReserv(int n_pass, int d, int m, int y, int l);
void pushReserv(reserv *r);
void popReserv();
void destroy();
void delReservation();
void delFlight();
void delFlightReservations();

/*search functions*/
int findID(char id[]);
int findFlight(int d, int m, int y);
int findReserv();

/*verification functions*/
int verifyAirport(char id[]);
int verifyFlight(char code[], char id_dep[], char id_arr[], int day, int month, 
                int year, int hour_d, int min_d, int capacity);
int verifyDate(int d, int m, int y);
int verifyReservation(int l, int d, int m, int y, int n_pass);
int verifyMemory(reserv *r);
int isdig(char input);
int isup(char input);

/*print functions*/
void printAirport(int i);
void printFlight(int i);

/*general functions*/
int powerTen(int i);
int calcMinutes(int day, int mon, int year, int h, int m);
void arrivalTime(int i);
void bubbleSort(int right);
void commandSelec(char input[]);

/*------------------------------------------------------------------------------
                                GLOBAL VARIABLES
------------------------------------------------------------------------------*/

int state = ' '; /* which command is active */

char id[MAX_CODE]; /* id buffer, for airport id and flight code */
char buffer[MAX_STR]; /* buffer for input string */
Airports aux_air;
int v_aux[MAX_FLIGHTS];
Result res_aux;

/* initialization of the number of airports and flights */
int n_air = 0;
int n_flights = 0;

/* initialization of the airports and flights databases, system date and head */
Airports airport_db[MAX_AIRPORT];
Flights flight_db[MAX_FLIGHTS];
Date sys_date = {1,1,2022};
reserv *head = NULL;
