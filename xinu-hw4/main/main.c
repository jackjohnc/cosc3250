/**
 * @file     main.c
 */
/* Embedded Xinu, Copyright (C) 2009, 2022.  All rights reserved. */

#include <xinu.h>

#define MAX 128

char decode(int c, int key, int prev);
int convert(int in);

/**
 * Main process.  You can modify this routine to customize what Embedded Xinu
 * does when it starts up.
 */
process main(void)
{
	//initialize all needed variables
	int c, numE, j, k, prev;
	int i = 0 , topE = 0, secE = 0, maxKey = 0, probKey = 0;
	char in[MAX] = ""; //input array
	char save[MAX] = ""; //decoded array, updates for each new key
	char maxE[MAX] = ""; //decoded message with max number of Es
	char twoE[MAX] = ""; //decoded message with second most Es

	kprintf("Hello Xinu World!\n");	
	kprintf("Enter your codebreaker text: \n");

	//reads input from data register using kgetc()
	while(((c = kgetc()) != EOF) && (i < MAX)) {
		in[i] = c;
		i++;
	}

	for (i = 0; i < 27; i++) { //outer loop runs once for each key value
		numE = 0; //resets the count of Es for each key
		prev = 0; //resets the previous saved character
		for (j = 0; (in[j] != 0) && (j < MAX); j++) { //decodes in[]
			save[j] = decode(in[j], i, prev);
			if (save[j] == 'E') numE++; //counts number of Es in decoded array
			if (in[j] != '\n') prev = in[j]; //saves proper value of previous
		}
		//saves array if it decodes to have max number of Es
		if (numE > topE) {
			secE = topE;
			topE = numE;
			probKey = maxKey;
			maxKey = i;
			for (k = 0; k <= j; k++) {
				twoE[k] = maxE[k];
				maxE[k] = save[k];
			}
		}
		//saves array if it decodes to have second most Es
		else if (numE > secE) {
			secE = numE;
			probKey = i;
			for (k = 0; k <= j; k++) twoE[k] = save[k];
		}
	}
	//prints proper output
	kprintf("Most probable key is %d\n", probKey);
	for (i = 0; (twoE[i] != 0) && (i < MAX); i++) kprintf("%c", twoE[i]);
	kprintf("\n");
    
	kprintf("===TEST END===");

    while (1)
        ;

    return 0;
}

char decode(int c, int key, int prev) {
	if (c == '\n') return '\n'; //if newline is seen, return newline
	//decodes and returns the encoded input
	int cypher = (convert(c) - convert(prev) - key) % 27;
	if (cypher < 0) cypher += 27;
	if (cypher == 0) return ' ';
	else return cypher + 64;

	return 0;
}

int convert(int in) {
	//converts from ASCII to 0-26
	if ((in >= 'A') && (in <= 'Z')) return (in - 64);
	else if ((in >= 'a') && (in <= 'z')) return (in - 96);
	else return 0;
}
