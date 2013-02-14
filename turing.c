/*
 * Turing.c
 * Simulate a turing machine.
 * This code is written dirty and quickly, any improvement ist welcome.
 * 
 *  Created on: 06.12.2012
 *      Author: Stefan Schuster
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MaxNumberOfStates 100
#define MaxNumberOfSymbols 50
#define MaxLengthOfTape 100

// Encode one entry in the transition table
typedef struct {
  int newstate;
  char mvwr; // Either a <symbol> to be written or a movement, 'L' or 'R'
} transition;

int number_of_states;
int number_of_symbols;
char symbols[MaxNumberOfSymbols];
transition transition_table[MaxNumberOfSymbols][MaxNumberOfStates];
char tape[MaxLengthOfTape+1];
int current_head_pos;
int current_state = 0;

void error(char *err_msg) {
	fprintf(stderr, "%s", err_msg);
	exit(1);
}

int getSymbolIndex(char symbol) {
	int i=0;
	while (symbols[i] != '\0' && symbols[i] != symbol) i++;
	if (symbols[i] == '\0') {
		fprintf(stderr, "getSymbolIndex: Request for"
			" undefined symbol: %c\n", symbol);
		exit(1);
	}
	return i;
}

void printTape() {
	if (strlen(tape) < current_head_pos+1){
		printf("%s", tape);
		for (int i=strlen(tape); i<= current_head_pos; i++)
			printf("#");
	} else {
		printf("%s#", tape);
	}
	printf("... (%i)\n", current_state);
	for (int i=0; i<current_head_pos; i++)
		printf(" ");
	printf("^\n");
}

void printTransitionTable() {
	for (int i=0; i<number_of_symbols; i++) {
		printf("%c ", symbols[i]);
		for (int j=0; j<number_of_states; j++) {
			printf("%i %c ", transition_table[i][j].newstate, transition_table[i][j].mvwr);
		}
		printf("\n");
	}
}

void initTM(){
	/* Read number of states */
	scanf("%i", &number_of_states);
	if (number_of_states > MaxNumberOfStates) {
		fprintf(stderr, "Maximal %i "
			"states are allowed, got %i", MaxNumberOfStates,
			number_of_states);
		exit(1);
	}

	/* Read symbols */
	scanf("%s\n", symbols);
	printf("Read symbols: %s\n", symbols);
	number_of_symbols = strlen(symbols);
	printf("Number of symbols: %i\n", number_of_symbols);

	/* Read transition table */
	for (int i=0; i<number_of_symbols; i++) {
		char t_sym;
		scanf("%c", &t_sym);
		printf("Reading line for %c\n", t_sym);
		if (t_sym == '!') { /* Comment */
			while (fgetc(stdin) != '\n') ;
			i--;
			continue;
		}
		int t_row = getSymbolIndex(t_sym);
		for (int j=0; j<number_of_states; j++) {
			int t_nstate;
			scanf(" %d", &t_nstate);
			printf("Reading for(%i,%c) -> state: %i\n", j, t_sym, t_nstate);
			char t_mvwr;
			scanf(" %c", &t_mvwr);
			printf("Reading for(%i,%c) -> movement/write: %c\n", j, t_sym, t_mvwr);
			transition newTrans = (transition) {.newstate = t_nstate,
				.mvwr = t_mvwr};
			transition_table[t_row][j] = newTrans;
		}
		char blah;
		scanf("%c", &blah);
	}

	/* Read initial state of the band */
	printf("Reading initial tape\n");
	scanf("%s", tape);
	printf("Tape: %s\n", tape);
	scanf("\n");
	scanf("%i", &current_head_pos);
	printf("Initial head position: %i\n", current_head_pos);
	printTape();
	printTransitionTable();
}

void executeTM() {
	int steps = 0;
	while (current_state < number_of_states) {
		transition curr_trans = transition_table[getSymbolIndex(tape[current_head_pos])][current_state];
		if (curr_trans.mvwr == 'L'  ) {
			if ( current_head_pos > 0)
				current_head_pos--;
		} else if (curr_trans.mvwr == 'R') {
			if (current_head_pos < MaxLengthOfTape) {
				current_head_pos++;
				if (tape[current_head_pos] == '\0')
						tape[current_head_pos] = '#';
			}
		} else {
			tape[current_head_pos] = curr_trans.mvwr;
		}
		current_state = curr_trans.newstate;
		printf("Step %i\n", ++steps);
		printTape();
		char tmp;
		scanf("%c", &tmp);
	}
	printf("Execution stopped after %i steps.\n", steps);
}

int main(void) {
	/* Read Turing Machine */
	/* Read Initial Configuration, band and position of head */
	initTM();

	/* Execute */
	executeTM();
}
