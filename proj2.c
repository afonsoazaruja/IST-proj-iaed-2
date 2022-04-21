/*
 * File: proj1.c
 * Author: Afonso Azaruja, 103624 - LEIC-A
 * Description: Airport flight management system
*/

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "util.h"

int main() {    
    char input[MAX_STR];
    do {
        fgets(input, MAX_STR, stdin);
        commandSelec(input);
    } while (state != 'q');
    destroy();
    return 0;
}

/* adds new airport to airport_db */
void addAirport(char input[]) {
    retrieveID(input, 2);
    if (verifyAirport(id) == ERROR) {
        return;
    }
    strcpy(airport_db[n_air].id ,id); /* if input correct write to db */
    retrieveLocation(input); /* write location details to db */
    airport_db[n_air].n_flights = 0;
    
    printf("airport %s\n", airport_db[n_air].id);
    n_air++;
}

/* lists the airports id, country and city */
void listAirports(char input[]) {
    int i, j = 2, k, len = strlen(input);
    if (len == 2) { /* list all airports */ 
        bubbleSort(n_air-1);
        for (i = 0; i < n_air; i++) {
            printAirport(i);
        }
    }
    else { /* len/MAX_ID -> iterate for the number of IDs in input */
        for (i = 0; i < len/MAX_ID; i++) {
            retrieveID(input, j);
            k = findID(id);
            if (k == ERROR) { /* if id does not exist */
                printf("%s: %s", id, ERR_NO_ID);
            }
            else {
                printAirport(k);
            }
            j += MAX_ID;
        }
    }
}

/* add new flight to flight_db */
void addFlight(char input[]) {
    int i = 0, j = 2;
    char code[MAX_CODE], id_dep[MAX_ID], id_arr[MAX_ID];
    int day = 0, month = 0, year = 0; 
    int hour = 0, min = 0, hour_d = 0, min_d = 0, capacity = 0;
    
    if (input[1] == '\n') {
        while (i < n_flights) {    
            printFlight(i);
            i++;
        }
        return;
    }
    
    j = retrieveID(input, j); strcpy(code, id); 

    j = retrieveID(input, j); strcpy(id_dep, id);
    j = retrieveID(input, j); strcpy(id_arr, id);
    /* date */
    retrieveTime(input, j, 10); j = res_aux.index; day = res_aux.value;
    retrieveTime(input, j, 10); j = res_aux.index; month = res_aux.value;
    retrieveTime(input, j, 1000); j = res_aux.index; year = res_aux.value;
    /* departure time */
    retrieveTime(input, j, 10); j = res_aux.index; hour = res_aux.value;
    retrieveTime(input, j, 10); j = res_aux.index; min = res_aux.value;
    /* flight duration */
    retrieveTime(input, j, 10); j = res_aux.index; hour_d = res_aux.value;
    retrieveTime(input, j, 10); j = res_aux.index; min_d = res_aux.value;
    /* flight capacity */ 
    capacity = retrieveCap(input, j);

    if (verifyFlight(code, id_dep, id_arr, day, month, year,
                         hour_d, min_d, capacity) == ERROR) {
        return;
    }
    
    strcpy(flight_db[n_flights].code, code);
    strcpy(flight_db[n_flights].id_dep, id_dep);
    strcpy(flight_db[n_flights].id_arr, id_arr);
    flight_db[n_flights].date_dep.d = day;
    flight_db[n_flights].date_dep.m = month;
    flight_db[n_flights].date_dep.y = year;
    flight_db[n_flights].time_dep.h = hour;
    flight_db[n_flights].time_dep.m = min;
    flight_db[n_flights].duration.h = hour_d;
    flight_db[n_flights].duration.m = min_d;
    flight_db[n_flights].min_dep = calcMinutes(day, month, year, hour, min);
    
    arrivalTime(n_flights); /* writes arrival time in db */

    flight_db[n_flights].min_arr = calcMinutes(flight_db[n_flights].date_arr.d, 
                                            flight_db[n_flights].date_arr.m, 
                                            flight_db[n_flights].date_arr.y, 
                                            flight_db[n_flights].time_arr.h, 
                                            flight_db[n_flights].time_arr.m);

    flight_db[n_flights].cap = capacity;
    flight_db[n_flights].n_pass = 0;
    airport_db[findID(id_dep)].n_flights++; 

    n_flights++;
}

/* list flights ordered by date and time of departure */
void listFlights(char input[]) {
    int i, count = 0;
    
    retrieveID(input, 2); /* id starts at index 2 in input string */

    for (i = 0; i < n_flights; i++) {
        if (state == 'p') {
            /* add integer of the flight_db to a vector */
            if (strcmp(flight_db[i].id_dep, id) == 0) {
                v_aux[count] = i;
                count++;
            }
        }
        if (state == 'c') {
            if (strcmp(flight_db[i].id_arr, id) == 0) {
                v_aux[count] = i;
                count++;
           }
        }
    }
    if (count == 0 && findID(id) == ERROR) {
            printf("%s: %s", id, ERR_NO_ID);
            return;
    }
    bubbleSort(count-1);

    for (i = 0; i < count; i++) {
        printFlight(v_aux[i]);  
    }
}

/* changes system date */
void changeDate(char input[]) {
    int i = 2, d = 0, m = 0, y = 0;
    
    retrieveTime(input, i, 10); i = res_aux.index;
    d = res_aux.value;
    retrieveTime(input, i, 10); i = res_aux.index;
    m = res_aux.value;
    retrieveTime(input, i, 1000); i = res_aux.index;
    y = res_aux.value;

    if(verifyDate(d, m, y) == ERROR) {
        return;
    }

    sys_date.d = d; sys_date.m = m; sys_date.y = y;

    printf("%02d-%02d-%d\n", sys_date.d, sys_date.m, sys_date.y);
}

/* adds new reservation to list */
void addReservation(char input[]) {
    int i, len;
    int d, m, y, n_pass;
    
    /* flight code */
    i = retrieveID(input, 2);
    /* date */
    retrieveTime(input, i, 10); i = res_aux.index; d = res_aux.value;
    retrieveTime(input, i, 10); i = res_aux.index; m = res_aux.value;
    retrieveTime(input, i, 1000); i = res_aux.index; y = res_aux.value;
    
    if(input[i-1] == '\n') {
        listReservations(d, m, y);
        return;
    }

    len = i;
    i = retrieveReservation(input, i); 
    len = i - 1 - len; /* reservation code length */

    n_pass = retrieveCap(input, i);

    
    if(verifyReservation(len, d, m, y, n_pass) == ERROR)
        return;

    pushReserv(newReserv(n_pass, d, m, y, len));
    flight_db[findFlight(d, m, y)].n_pass += n_pass;
}

/* list all reservations with a certain flight code and date */
void listReservations(int d, int m, int y) {
    reserv *aux;
    if(findFlight(d, m, y) == ERROR) {
        printf("%s: %s", id, ERR_NO_FLY);
        return;
    }
    if(verifyDate(d, m, y) == ERROR) {
        return;
    }
    for(aux = head; aux != NULL; aux = aux->next) 
        if(!strcmp(aux->cod_f, id) 
            && (aux->date.d == d) && (aux->date.m == m) && (aux->date.y == y)) 
            printf("%s %d\n", aux->cod_r, aux->n_pass);
}

/* deletes one reservation or flight and all its reservations */
void deletion(char input[]) {
    if(strlen(input) > MIN_RES) {
        retrieveReservation(input, 2);
        delReservation();
    }
    else {
        retrieveID(input, 2);
        delFlight();
        delFlightReservations();
    }
}


/*------------------------------------------------------------------------------
                                AUX FUNCTIONS
------------------------------------------------------------------------------*/

/*---------------------------retrieve functions-------------------------------*/

/* retrieves airport id or flight code, returns index where ID ends in input */
int retrieveID(char input[], int i) {
    int j = 0;
    while (input[i] != ' ' && input[i] != '\n' && input[i] != '\t') {
        id[j] = input[i];
        i++; j++;
    }
    id[j] = '\0'; 
    i++; /* so index is not in whitespace */
    return i;
}

/* retrieves location details from user input */
void retrieveLocation(char input[]) {
    int j = 0, i = 6;
    while (input[i] == ' ' || input[i] == '\t') {
        i++;
    }
    while (input[i] != ' ' && input[i] != '\t') { /* FIND COUNTRY */
        airport_db[n_air].country[j] = input[i]; i++; j++;
    }
    airport_db[n_air].country[j] = '\0';
    
    while (input[i] == ' ' || input[i] == '\t') {
        i++;
    }
    j = 0;
    while (input[i] != '\n') { /* FIND CITY */
        airport_db[n_air].city[j] = input[i]; i++; j++;
    }
    airport_db[n_air].city[j] = '\0';
}

/* retrieves time based input, date and hours */
void retrieveTime(char input[], int i, int j) {
    int value = 0;
    while (isdig(input[i]) == SUCCESS) {
        value += (input[i] - '0') * j; j /= 10; i++;
    }
    i++; /* so index is not in whitespace */
    res_aux.index = i;
    res_aux.value = value;
}

/* retrieves flight capacity or number of passengers */
int retrieveCap(char input[], int i) {
    int cap = 0, count = -1, j = i, negative = 0;
    
    if(input[i] == '-') {
        i++; j++; negative = 1;
    }

    while(input[i] != '\n') {
        count++;
        i++;
    }

    while(input[j] != '\n' && count != 0) {
        cap += (input[j] - '0') * powerTen(count);
        count -= 1;
        j++;
    }
    cap += input[j] - '0';

    if(negative)
        return -cap;
    return cap;
}

/* retrieves reservation code */
int retrieveReservation(char input[], int i) {
    int j = 0;

    while(input[i] != ' ' && input[i] != '\n') {
        buffer[j] = input[i];
        j++; i++;
    }
    buffer[j] = '\0'; i++;
    return i;
}

/*-------------------------reservation functions------------------------------*/

/* creates new reservation */
reserv* newReserv(int n_pass, int d, int m, int y, int l) {
    reserv *new_reserv = (reserv*) malloc(sizeof(reserv));

    new_reserv->cod_r = (char*) malloc(sizeof(char) * (l + 1));
    
    if(verifyMemory(new_reserv) == ERROR) {
        exit(0);
    }

    strcpy(new_reserv->cod_f, id);
    strcpy(new_reserv->cod_r, buffer);

    new_reserv->n_pass = n_pass;

    new_reserv->date.d = d;
    new_reserv->date.m = m;
    new_reserv->date.y = y;

    new_reserv->next = NULL;

    return new_reserv;
}

/* inserts reservation in list, ordered */
void pushReserv(reserv *r) {
    reserv *b = head, *f;
    if(head == NULL)
        head = r;

    else if(strcmp(r->cod_r, b->cod_r) < 0) { /* before head */
        r->next = b;
        head = r;
    }
    else {
        for(f = head->next; f != NULL; b = b->next, f = f->next)
            if(strcmp(r->cod_r, f->cod_r) < 0) {
                b->next = r;
                r->next = f;
                break;
            }
        b->next = r;
    }
}

/* deletes memory allocated from head and declares new head */
void popReserv() {
    reserv *aux = NULL;
    if(head != NULL) {
        aux = head;
        head = aux->next;
        free(aux->cod_r);
        free(aux);
    }
}

/* deletes entire reservation list */
void destroy() {
    while(head != NULL)
        popReserv();
}

/* deletes reservation from list */
void delReservation() {
    int i, change = 0;
    reserv *b, *f;
    
    if(findReserv() == SUCCESS) {
        printf("%s", ERR_NOT_FOUND);
        return;
    }
    i = findFlight(res_aux.date.d, res_aux.date.m, res_aux.date.y);
    flight_db[i].n_pass -= res_aux.value;

    if(head != NULL && !strcmp(head->cod_r, buffer)) { 
        popReserv();
        change = 1;
    }
    if(change == 0 && head != NULL)
        for(b = head, f = head->next; f != NULL; b = b->next, f = f->next) {
            if(!strcmp(f->cod_r, buffer)) {
                b->next = f->next;
                free(f->cod_r);
                free(f);
                break;
            }
        }
}

/* deletes flights */
void delFlight() {
    int i = 0, j, count = 0, k;

    while(i < n_flights - 1 && n_flights > 1) {
        if(!strcmp(flight_db[i].code, id)) {
            k = findID(flight_db[i].id_dep);
            airport_db[k].n_flights--;
            count = 1;

            for(j = i; j < n_flights - 1; j++) {
                flight_db[j] = flight_db[j+1];
            }
            n_flights--;
            i--;
        }
        i++;
    }

    if(n_flights != 1 && n_flights == (i + 1) && !strcmp(flight_db[i].code, id)) {
        k = findID(flight_db[i].id_dep);
        airport_db[k].n_flights--;
        n_flights--;
        count = 1;
    }

    if(n_flights == 1 && !strcmp(flight_db[0].code, id)) {
        k = findID(flight_db[0].id_dep);
        airport_db[k].n_flights--;
        n_flights--;
        count = 1;
    }

    if(!count) {
        printf("%s", ERR_NOT_FOUND);
        return;
    }
}

/* deletes reservations with certain flight code */
void delFlightReservations() {
    reserv *b, *f;

    while(head != NULL && !strcmp(head->cod_f, id))
        popReserv();

    if(head != NULL) {
        b = head; f = head->next;
        while(f != NULL) {
            if(!strcmp(f->cod_f, id)) {
                b->next = f->next;
                free(f->cod_r);
                free(f);
                f = b->next;
            }
            else {    
                b = b->next;
                f = f->next;
            }
        }
    }
}

/*----------------------------search functions--------------------------------*/

/* searchs airport id in db, if it exists return the index position in db */
int findID(char id[]) {
    int i;
    for (i = 0; i < n_air; i++) {
        if(!strcmp(airport_db[i].id, id)) {
            return i;
        }
    }
    return ERROR;
}

int findFlight(int d, int m, int y) {
    int i;
    for (i = 0; i < n_flights; i++) {
        if(flight_db[i].date_dep.d == d 
            && flight_db[i].date_dep.m == m 
            && flight_db[i].date_dep.y == y
            && !strcmp(flight_db[i].code, id)) {
        return i;
        }
    }
    return ERROR;
}

/* verifies if reservation already exists */
int findReserv() {
    reserv *aux = head;
    for(; aux != NULL; aux = aux->next)
        if(!strcmp(aux->cod_r, buffer)) {
            res_aux.value = aux->n_pass;
            strcpy(id, aux->cod_f);
            res_aux.date.d = aux->date.d;
            res_aux.date.m = aux->date.m;
            res_aux.date.y = aux->date.y;
            return ERROR;
        }
    return SUCCESS;
}

/*--------------------------verification functions----------------------------*/

/* verifies if id is correct and if max airport is reached */
int verifyAirport(char id[]) {
    int i;
    for (i = 0; id[i] != '\0'; i++) {
        if (isup(id[i]) == ERROR) {
            printf("%s", ERR_INV_ID);
            return ERROR; 
        }
    }
    if (n_air + 1 > MAX_AIRPORT) {
        printf("%s", ERR_TOO_MANY_AIR);
        return ERROR;
    }
    if (findID(id) != ERROR) { /* findID returns ERROR if no duplicate id */ 
        printf("%s", ERR_DUP_ID);
        return ERROR;
    }
    return SUCCESS;
}

/* verifies if flight code is correct */
int verifyFlight(char code[], char id_dep[], char id_arr[], 
    int day, int month, int year, int hour_d, int min_d,
    int capacity) 
{   
    int i = 0, count_dig = 0;
    /* validates if flight code has 2 uppercase letters */
    while(isup(code[i]) == SUCCESS && i < 2)
        i++;
    if(i != 2) {
        printf("%s", ERR_INV_COD);
        return ERROR;
    }

    if (code[2] == '0') {
        printf("%s", ERR_INV_COD);
        return ERROR;
    }
    /* validates if flight code has between 1 and 4 digits */
    for (i = 2; code[2] != '\0' && code[i] != '\0' && count_dig < 5; i++) {
        if (isdig(code[i]) == ERROR) {
            printf("%s", ERR_INV_COD);
            return ERROR;
        }
        count_dig++;
    }
    /* validates if there is no flight with same code in the same day */
    for(i = 0; i < n_flights; i++) {
        if (strcmp(flight_db[i].code, code) == 0 
        && (day == flight_db[i].date_dep.d
        && month == flight_db[i].date_dep.m 
        && year == flight_db[i].date_dep.y)) {
            printf("%s", ERR_DUP_FLIGHT);
            return ERROR;
        }
    }
    /* validates if airport id exists */
    if (findID(id_dep) == ERROR) {
        printf("%s: %s", id_dep, ERR_NO_ID);
        return ERROR;
    }
    if (findID(id_arr) == ERROR) {
        printf("%s: %s", id_arr, ERR_NO_ID);
        return ERROR;
    }
    /* validates max number of flights has been reached */
    if (n_flights + 1 > MAX_FLIGHTS) {
        printf("%s", ERR_TOO_MANY_FLIGHTS);
        return ERROR;
    }

    if(verifyDate(day, month, year) == ERROR) {
        return ERROR;
    }
    
    /* if flight duration is greater than 12 hours */
    if (hour_d > 12 || (hour_d == 12 && min_d > 0)) {
        printf("%s", ERR_INV_DURATION);
        return ERROR;
    }
    /* if number of passenger is less than 10 */
    if (capacity < MIN_PASS) {
        printf("%s", ERR_INV_CAP);
        return ERROR;
    }
    return SUCCESS;
}

/* verifies if date is not in the past or 1 year into the future */
int verifyDate(int d, int m, int y) {
    int min_flight, min_sys, diff_min;
    
    min_flight = calcMinutes(d, m, y, 00, 00);
    min_sys = calcMinutes(sys_date.d, sys_date.m, sys_date.y, 00, 00);
    diff_min = min_flight - min_sys;

    /* if date is in the past or 1 year into the future */
    if (diff_min < 0 || diff_min > MIN_YEAR || y < 2022) {
        printf("%s", ERR_INV_DATE);
        return ERROR;
    }
    return SUCCESS;
}

/* verifies if reservation info is correct */
int verifyReservation(int l, int d, int m, int y, int n_pass) {
    int i;
    if(l < MIN_RES) {
        printf("%s", ERR_INV_RES);
        return ERROR;
    }

    for(i = 0; buffer[i] != '\0'; i++) {
        if(isdig(buffer[i]) == ERROR && isup(buffer[i]) == ERROR) {
            printf("%s", ERR_INV_RES);
            return ERROR;
        }
    }
    i = findFlight(d, m, y);
    if(i == ERROR) {
        printf("%s: %s", id, ERR_NO_FLY);
        return ERROR;
    }

    if(findReserv() == ERROR) {
        printf("%s: %s", buffer, ERR_DUP_RES);
        return ERROR;
    }

    if((n_pass + flight_db[i].n_pass) > flight_db[i].cap) {
        printf("%s", ERR_TOO_MANY_RES);
        return ERROR;
    }
    
    if(verifyDate(d, m, y) == ERROR) {
        return ERROR;
    }

    if(n_pass <= 0) {
        printf("%s", ERR_INV_PASS);
        return ERROR;
    }

    return SUCCESS;
}

/* verifies if program as ran out of memory */
int verifyMemory(reserv *r) {
    if(r == NULL || r->cod_r == NULL) {
        printf("%s", ERR_NO_MEM);
        destroy();
        return ERROR;
    }
    return SUCCESS;
}

/* verifies if input is digit */
int isdig(char input) {
    if('0' <= input && input <= '9')
        return SUCCESS;    
    return ERROR;
}

/* verifies if input is caracther */
int isup(char input) {
    if('A' <= input && input <= 'Z')
        return SUCCESS;
    return ERROR;
}

/*----------------------------print functions---------------------------------*/

/* print airport id, country, city and number of flights */
void printAirport(int i) {
    printf("%s %s %s %d\n",
        airport_db[i].id,
        airport_db[i].city,
        airport_db[i].country,
        airport_db[i].n_flights);
}

/* print flight details according to a specific command */
void printFlight(int i) {
    printf("%s ", flight_db[i].code);
    if (state == 'v') { 
        printf("%s %s %02d-%02d-%d %02d:%02d\n",
            flight_db[i].id_dep,
            flight_db[i].id_arr,
            flight_db[i].date_dep.d,
            flight_db[i].date_dep.m,
            flight_db[i].date_dep.y,
            flight_db[i].time_dep.h,
            flight_db[i].time_dep.m);
    }
    else if (state == 'p') {
        printf("%s %02d-%02d-%d %02d:%02d\n",
            flight_db[i].id_arr,
            flight_db[i].date_dep.d,
            flight_db[i].date_dep.m,
            flight_db[i].date_dep.y,
            flight_db[i].time_dep.h,
            flight_db[i].time_dep.m);
    }
    else { /* state == 'c' */
        printf("%s %02d-%02d-%d %02d:%02d\n",
            flight_db[i].id_dep,
            flight_db[i].date_arr.d,
            flight_db[i].date_arr.m,
            flight_db[i].date_arr.y,
            flight_db[i].time_arr.h,
            flight_db[i].time_arr.m);
    }
    
}

/*----------------------------general functions-------------------------------*/

/* selects the command from the user input */
void commandSelec(char input[]) {
    state = input[0];
    switch(state) {
        case 'q':
            break;
        case 'a':
            addAirport(input);
            break;
        case 'l':
            listAirports(input);
            break;
        case 'v':
            addFlight(input);
            break;
        case 'p':
            listFlights(input);
            break;
        case 'c':
            listFlights(input);
            break;
        case 't':
            changeDate(input);
            break;
        case 'r':
            addReservation(input);
            break;
        case 'e':
            deletion(input);
            break;
    }
}

/* returns power of 10 */
int powerTen(int i) {
    int res = 1;
    while(i > 0) {
        res *= 10;
        i--;
    }
    return res;
}

/* calculates minutes since 01-01-2022 00:00 to specific date and time after */
int calcMinutes(int day, int mon, int year, int h, int m) {
    int months[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int minutes = 0;
    int i;

    if (year > YEAR) {
        minutes += (year - YEAR) * (MIN_DAY * 365);
    }
    if (mon > MON) {
        for (i = 0; i < mon - 1; i++) {
            minutes += months[i]*MIN_DAY;
        }
    }

    if (day > DAY) {
        minutes += (day - DAY) * MIN_DAY;
    }

    if (h != 0 || m != 0) {
        minutes += m;
        minutes += h * 60;
    }
    return minutes;
}

/* calculates the arrival date and time of flight */
void arrivalTime(int i) {
    int days_of_mon[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int h_res = flight_db[i].time_dep.h + flight_db[i].duration.h; 
    int m_res = flight_db[i].time_dep.m + flight_db[i].duration.m;
    if (m_res >= 60) {
        h_res++;
        flight_db[i].time_arr.m = m_res - 60;
    }
    else {
        flight_db[i].time_arr.m = m_res;
    }
    if (h_res >= 24) {
        flight_db[i].date_arr.d = flight_db[i].date_dep.d + 1;
        flight_db[i].time_arr.h = h_res - 24;
        
        if (flight_db[i].date_arr.d > days_of_mon[flight_db[i].date_dep.m - 1]) {
            flight_db[i].date_arr.d = 1;
            flight_db[i].date_arr.m = flight_db[i].date_dep.m + 1;
        
            if (flight_db[i].date_arr.m == 13) {
                flight_db[i].date_arr.m = 1;
                flight_db[i].date_arr.y = flight_db[i].date_dep.y + 1;
            }
            else{
                flight_db[i].date_arr.y = flight_db[i].date_dep.y;
            }
        }
        else {
            flight_db[i].date_arr.m = flight_db[i].date_dep.m;
            flight_db[i].date_arr.y = flight_db[i].date_dep.y;
        }
    }
    else {
        flight_db[i].time_arr.h = h_res;
        flight_db[i].date_arr.d = flight_db[i].date_dep.d;
        flight_db[i].date_arr.m = flight_db[i].date_dep.m;
        flight_db[i].date_arr.y = flight_db[i].date_dep.y;
    }
}

/* sorting algorithm */
void bubbleSort(int right) {
    int i, j, done, aux;

    for (i = 0; i < right; i++) {
        done = 1;
        for (j = right; j > i; j--) {
            if (state == 'l') {
                if (strcmp(airport_db[j].id, airport_db[j-1].id) < 0) {
                    strcpy(aux_air.id, airport_db[j-1].id);
                    strcpy(aux_air.country, airport_db[j-1].country);
                    strcpy(aux_air.city, airport_db[j-1].city); 

                    strcpy(airport_db[j-1].id, airport_db[j].id);
                    strcpy(airport_db[j-1].country, airport_db[j].country);
                    strcpy(airport_db[j-1].city, airport_db[j].city);
                    
                    strcpy(airport_db[j].id, aux_air.id);
                    strcpy(airport_db[j].country, aux_air.country);
                    strcpy(airport_db[j].city, aux_air.city);
                    done = 0;
                }
            }
            else if (state == 'p') {
                if (flight_db[v_aux[j]].min_dep < flight_db[v_aux[j-1]].min_dep) {
                    aux = v_aux[j-1];
                    v_aux[j-1] = v_aux[j];
                    v_aux[j] = aux;
                    done = 0;
                }
            }
            else {
                if (flight_db[v_aux[j]].min_arr < flight_db[v_aux[j-1]].min_arr) {
                    aux = v_aux[j-1];
                    v_aux[j-1] = v_aux[j];
                    v_aux[j] = aux;
                    done = 0;
                }
            }
        }
        if (done) break;
    }
}
