/*
 * Turing.c
 * Simulate a turing machine.
 *  Created on: 06.12.2012
 *      Author: Stefan Schuster
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MaxNumberOfStates 100
#define MaxNumberOfSymbols 50
#define MaxLengthOfTape 100

typedef struct transition {
  int newstate;
  char mvwr;
} transition;

typedef struct turing_machine {
  int number_of_states;
  int number_of_symbols;
  char symbols[MaxNumberOfSymbols];
  transition transition_table[MaxNumberOfSymbols][MaxNumberOfStates];
  char tape[MaxLengthOfTape+1];
  int current_tape_pos;
  int current_state;
} turing_machine;

turing_machine g_tm;

int g_linenumber = 0;

int getSymbolIndex(char symbol) {
	int i=0;
	char *symbols = g_tm.symbols;
	while (symbols[i] != '\0' && symbols[i] != symbol) i++;
	if (symbols[i] == '\0') {
		return -1;
	}
	return i;
}

void printTape() {
	if (strlen(tape) < current_tape_pos+1){
		printf("%s", tape);
		for (int i=strlen(tape); i<= current_tape_pos; i++)
			printf("#");
	} else {
		printf("%s#", tape);
	}
	printf("... (%i)\n", current_state);
	for (int i=0; i<current_tape_pos; i++)
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

/* Skip any whitespace character and any commentary. */
void advance(FILE *stream)
{
	int c;
	while (c=lookahead(stream) == '!')
		skipline(stream);
}

/* Skip any whitespace character in the stream and return the next
 * character in stream. */
int lookahead(FILE *stream)
{
	int c;
	while ((c=fgetc(stream)) != EOF && isspace(c))
		if (c=='\n') g_linenumber++;
	if (c==EOF && ferror(stream)) {
		 perror("Error!");
		exit(1);
	}
	int e = ungetc(c, stream);
	if (e == EOF) {
		perror("Error ungetc.");
		exit(1);
	}
	return c;
}

int skipline(FILE *stream)
{
	while ((c=fgetc(stream)) != EOF && c != '\n')
		;
	return c;
}

int getString(FILE stream, char *buf)
{
	advance(stream);
	int c, maxnoc = 100;
	char t_buf[maxnoc+1];
	int i=0;
	while ((c=fgetc()) != EOF && isalpha(c) && i<maxnoc+1) {
		buf[i] = c;
		i++;
	}
	/* Error handling */
	if (i>=maxnod) {
		printf("Line %d: integer too large\n", g_linenumber);
		exit(1);
	}
	int e = ungetc(c, stream);
	if (e == EOF) {
		printf("Unable to put %c back into stream.\n", c);
		exit(1);
	}
	strncpy(buf, t_buf, 30);
	return r;
}

int getInteger(FILE *stream, int *n)
{
	advance(stream);
	int c;
	int r = 0;
	int i=0;
	int maxnod = 5;
	while ((c=fgetc()) != EOF && isalpha(c) && i<maxnod+1) {
		r = 10*r+ctoi(c);
		i++;
	}
	/* Error handling */
	if (i>=maxnod) {
		printf("Line %d: integer too large\n", g_linenumber);
		exit(1);
	}
	int e = ungetc(c, stream);
	if (e == EOF) {
		printf("Unable to put %c back into stream.\n", c);
		exit(1);
	}
	*n = r;
	return e;
}


int isIn(char c, char *s) {
	for (char *p=s; *p != '\0'; p++)
		if (*p = c)
			return 1;
	return 0;
}

void expect(char c, FILE *stream) {
	int t_c;
	advance(stream);
	if ((t_c = getc(stream)) != c) {
		printf("Expected %c, got %c.\n", c, t_c);
		exit(1);
	}
}

int getIndexOfSymbol(char c) {
	for (int i=0; i<g_tm.number_of_symbols; i++) {
		if (g_tm.symbols[i] = c)
			return i;
	}
	return -1;
}

void processTable(FILE *stream)
{
	/* Check prerequisites. */
	if (g_tm.number_of_symbols == 0) {
		printf("Line %d: symbols not defined.\n", g_linenumber);
		exit(1);
	}
	if (g_tm.number_of_states == 0) {
		printf("Line %d: states not defined.\n", g_linenumber);
		exit(1);
	}

	for (int t_line = 0; t_line < g_tm.number_of_symbols; t_line++) {
		advance(stream);
		/* Read symbol */
		int t_sym = getc(stream);
		if (! isIn(t_sym,  g_tm.symbols)) {
			printf("Symbol %c not defined.\n", t_sym);
			exit(1);
		}
		int t_curr_state = getIndexOfSymbol(t_sym);	
		/* Read g_tm.number_of_states pairs (nextState, nextAction) */
		for (int i=0; i<g_tm.number_of_states; i++) {
			advance(stream);
			expect('(', stream);
			int t_state;
			scanf("%d", &t_state);
			expect(',', stream);
			advance(stream);
			int t_nextAction = getc(stream);
			/* TODO: Check if we have a valid symbol. */
			expect(')', stream);
			transition t_transition = {t_state, t_nextAction};
			g_tm.transition[t_curr_state][i] = t_transition;
		}
	}
	char t_buf[35];
	getString(stream, t_buf);
	if (!strcmp(t_buf, "tableend")) {
		printf("Line %d: Expected tag tableend.\n", );
		exit(1);
	}
}

void initTM(FILE *stream)
{
	g_tm.tape[0] = '\0';
	g_tm.symbols[0] = '\0';
	g_tm.current_tape_pos = 0;
	g_tm.number_of_states = 0;
	g_tm.number_of_symbols = 0;
	char kwbuf[35];
	getString(stream, kwbuff);
	if (strcmp(kwbuff, "states")) {
		g_tm.number_of_states = getInteger(stream);
	} else if (strcmp(kwbuff, "alphabet")) {
		getString(stream, g_tm.symbols);
		g_tm.number_of_symbols = strlen(g_tm.symbols);
	} else if (strcmp(kwbuff, "tape")) {
		getString(stream, g_tm.tape);
	} else if (strcmp(kwbuff, "headpos")) {
		g_tm.current_tape_pos = getInteger(stream);
	} else if (strcmp(kwbuff, "tablestart")) {
		processTable(stream);
	}
}

void initTM2(){
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
	scanf("%i", &current_tape_pos);
	printf("%i\n", current_tape_pos);
	printTape();
	printTransitionTable();
}

void executeTM() {
	int steps = 0;
	fflush(stdin);
	while (current_state < number_of_states) {
		transition curr_trans = transition_table[getSymbolIndex(tape[current_tape_pos])][current_state];
		if (curr_trans.mvwr == 'L'  ) {
			if ( current_tape_pos > 0)
				current_tape_pos--;
		} else if (curr_trans.mvwr == 'R') {
			if (current_tape_pos < MaxLengthOfTape) {
				current_tape_pos++;
				if (tape[current_tape_pos] == '\0')
						tape[current_tape_pos] = '#';
			}
		} else {
			tape[current_tape_pos] = curr_trans.mvwr;
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
	initTM();

	/* Read Initial Configuration, band and position of head */
	executeTM();
	/* Execute */
}
