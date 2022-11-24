/* Program to assist in the challenge of solving sudoku puzzles.

   Skeleton program written by Alistair Moffat, ammoffat@unimelb.edu.au,
   August 2021, with the intention that it be modified by students
   to add functionality, as required by the assignment specification.

   Student Authorship Declaration:

   (1) I certify that except for the code provided in the initial skeleton
   file, the  program contained in this submission is completely my own
   individual work, except where explicitly noted by further comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students, or by
   non-students as a result of request, solicitation, or payment, may not be
   submitted for assessment in this subject.  I understand that submitting for
   assessment work developed by or in collaboration with other students or
   non-students constitutes Academic Misconduct, and may be penalized by mark
   deductions, or by other penalties determined via the University of
   Melbourne Academic Honesty Policy, as described at
   https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will I
   do so until after the marks are released. I understand that providing my
   work to other students, regardless of my intention or any undertakings made
   to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring service,
   or drawing the attention of others to such services and code that may have
   been made available via such a service, may be regarded as Student General
   Misconduct (interfering with the teaching activities of the University
   and/or inciting others to commit Academic Misconduct).  I understand that
   an allegation of Student General Misconduct may arise regardless of whether
   or not I personally make use of such solutions or sought benefit from such
   actions.

   Signed by: [Angeline Cassie Ganily]
   Dated:     [2 September 2021]

*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* these #defines provided as part of the initial skeleton */

#define NDIM 3		    /* sudoku dimension, size of each inner square */
#define NDIG (NDIM*NDIM)
			            /* total number of values in each row */
#define NGRP 3		    /* number of sets each cell is a member of */
#define NSET (NGRP*NDIG)
			            /* total number of sets in the sudoku */
#define NCLL (NDIG*NDIG)
			            /* total number of cells in the sudoku */

#define ERROR	(-1)	/* error return value from some functions */

/* these global constant arrays provided as part of the initial skeleton,
   you may use them in your code but must not alter them in any way,
   regard them as being completely fixed. They describe the relationships
   between the cells in the sudoku and provide a basis for all of the
   sudoku processing loops */

/* there are 27 different different sets of elements that need to be
   checked against each other, this array converts set numbers to cells,
   that's why its called s2c */
int s2c[NSET][NDIM*NDIM] = {
	/* the first group of nine sets describe the sudoku's rows */
	{  0,  1,  2,  3,  4,  5,  6,  7,  8 },
	{  9, 10, 11, 12, 13, 14, 15, 16, 17 },
	{ 18, 19, 20, 21, 22, 23, 24, 25, 26 },
	{ 27, 28, 29, 30, 31, 32, 33, 34, 35 },
	{ 36, 37, 38, 39, 40, 41, 42, 43, 44 },
	{ 45, 46, 47, 48, 49, 50, 51, 52, 53 },
	{ 54, 55, 56, 57, 58, 59, 60, 61, 62 },
	{ 63, 64, 65, 66, 67, 68, 69, 70, 71 },
	{ 72, 73, 74, 75, 76, 77, 78, 79, 80 },
	/* the second group of nine sets describes the sudoku's columns */
	{  0,  9, 18, 27, 36, 45, 54, 63, 72 },
	{  1, 10, 19, 28, 37, 46, 55, 64, 73 },
	{  2, 11, 20, 29, 38, 47, 56, 65, 74 },
	{  3, 12, 21, 30, 39, 48, 57, 66, 75 },
	{  4, 13, 22, 31, 40, 49, 58, 67, 76 },
	{  5, 14, 23, 32, 41, 50, 59, 68, 77 },
	{  6, 15, 24, 33, 42, 51, 60, 69, 78 },
	{  7, 16, 25, 34, 43, 52, 61, 70, 79 },
	{  8, 17, 26, 35, 44, 53, 62, 71, 80 },
	/* the last group of nine sets describes the inner squares */
	{  0,  1,  2,  9, 10, 11, 18, 19, 20 },
	{  3,  4,  5, 12, 13, 14, 21, 22, 23 },
	{  6,  7,  8, 15, 16, 17, 24, 25, 26 },
	{ 27, 28, 29, 36, 37, 38, 45, 46, 47 },
	{ 30, 31, 32, 39, 40, 41, 48, 49, 50 },
	{ 33, 34, 35, 42, 43, 44, 51, 52, 53 },
	{ 54, 55, 56, 63, 64, 65, 72, 73, 74 },
	{ 57, 58, 59, 66, 67, 68, 75, 76, 77 },
	{ 60, 61, 62, 69, 70, 71, 78, 79, 80 },
};


/* there are 81 cells in a dimension-3 sudoku, and each cell is a
   member of three sets, this array gets filled by the function 
   fill_c2s(), based on the defined contents of the array s2c[][] */
int c2s[NCLL][NGRP];

void
fill_c2s() {
	int s=0, d=0, c;
	for ( ; s<NSET; s++) {
		/* record the first set number each cell is part of */
		for (c=0; c<NDIM*NDIM; c++) {
			c2s[s2c[s][c]][d] = s;
		}
		if ((s+1)%(NGRP*NDIM) == 0) {
			d++;
		}
	}
#if 0
	/* this code available here if you want to see the array
	   cs2[][] that gets created, just change that 0 two lines back
	   to a 1 and recompile */
	for (c=0; c<NCLL; c++) {
		printf("cell %2d: sets ", c);
		for (s=0; s<NGRP; s++) {
			printf("%3d", c2s[c][s]);
		}
		printf("\n");
	}
	printf("\n");
#endif
	return;
}

/* find the row number a cell is in, counting from 1
*/
int
rownum(int c) {
	return 1 + (c/(NDIM*NDIM));
}

/* find the column number a cell is in, counting from 1
*/
int
colnum(int c) {
	return 1 + (c%(NDIM*NDIM));
}

/* find the minor square number a cell is in, counting from 1
*/
int
sqrnum(int c) {
	return 1 + 3*(c/NSET) + (c/NDIM)%NDIM;
}

/* If you wish to add further #defines, put them below this comment,
   then prototypes for the functions that you add

   The only thing you should alter above this line is to complete the
   Authorship Declaration 
*/


/****************************************************************/
#define N 81 /* total number of cells */

void readvals(int array[]);
void pformat(int array[]);

int gridcheck(int array[]);
int setcheck(int array[], int set);
void set_to_words(int set);

int strategyone(int array[]);
int fillcell(int array[], int cell);
void cell_to_name(int array[], int cell);
int completesudoku(int array[]);

void vertical_break(void);
void horizontal_break(void);
void blank_line(void);
void ta_daa(void);
void spacing(void);
/****************************************************************/


/************************* MAIN PROGRAM *************************/
/* main program controls all the action */
int
main(int argc, char *argv[]) {
    int array[N], strategyok=1;
    /* STAGE ONE */
    blank_line();
    readvals(array);
    pformat(array);

    /* STAGE TWO */
    fill_c2s();
    if (gridcheck(array)==ERROR) {
        EXIT_FAILURE;

    /* STAGE THREE */
    } else {
        while (completesudoku(array)==0 && strategyok!=0) { 
            strategyok = strategyone(array); /* output of strategy one */
            blank_line();
        }
        if (completesudoku(array)==1) { 
            pformat(array);
            ta_daa();
            return 0;
        } else {
            pformat(array);
            return 0;
        }
    }
	/* all done, so pack up bat and ball and head home! YAY!! :> */
}   
/****************************************************************/


/************************** STAGE ONE ***************************/
/* read values from input sudoku and store it in an array */
void 
readvals(int array[]) {
    int cell=0, loop, value;
    for (loop=0; loop<N; loop++) {
        scanf("%d ", &value);
        array[cell] = value; /* stores in my array */
        cell++;
    }
}

/* print values from the erray created earlier into a sudoku 
format that sir alistair wants :> */
void 
pformat(int array[]) {
    int cell, unknown=0;

    for (cell=0; cell<N; cell++) {
        /* formatting data */
        if (cell%NDIM==0 && !(cell%NDIG==0)) { 
            vertical_break();
        } else if (cell!=0 && cell%(NDIM*NDIG)==0) {
            horizontal_break();
        } else if (cell!=0 && cell%NDIG==0) {
            blank_line();
        } else {
            if (cell!=0) {
                spacing();
            }
        }

        /* printing numbers*/
        if (array[cell]==0) {
            printf(".");
            unknown++;
        } else {
            printf("%d", array[cell]);
        }
    }
    blank_line();
    printf("\n%2d cells are unknown\n", unknown);
    blank_line();
}
/****************************************************************/


/************************** STAGE TWO ***************************/
/* checks if the input sudoku is valid or not */
int
gridcheck(int array[]) {
    int set, setviolation=0, violatedset=0, violations=0;

    for (set=0; set<NSET; set++) {
        /* check if sets have violations or not */
        setviolation = setcheck(array, set);
        if (setviolation!=0) {
            violatedset++; 
            violations+=setviolation;
        }
    }

    if (violations>0) {
        blank_line();
        printf("%d different sets have violations\n", violatedset);
        printf("%d violations in total\n", violations);
        return ERROR;
    }
    return 1;
}

/* checks if the set has no duplicate elements; 
return the number of violations if set has duplicate elements */
int 
setcheck(int array[], int set) {
    int cell, cellvalue, value, repeated, violations=0;
    int temporary[NDIG] = {0};

    /* store cell values of the set in temporary array */
    for (cell=0; cell<NDIG; cell++) {
        cellvalue = array[s2c[set][cell]];
        temporary[cell] = cellvalue; 
    }

    /* check for any repeated numbers (1-9) of the set */
    for (value=1; value<=NDIG; value++) {
        repeated = 0;
        for (cell=0; cell<NDIG; cell++) {
            if (temporary[cell]==value) {
                repeated++;
            }
        }

        /* print violation wordings if there are duplicate elements */
        if (repeated>1) {
            violations++;
            printf("set %2d ", set);
            set_to_words(set);
            printf(": %d instances of %d\n", repeated, value);
        }
    }
    
    return violations;
}

/* to print out the representative set name */
void
set_to_words (int set) {
    if (set<NDIG) {
        printf("(row %d)", set+1);
    } else if (set<NDIG*2) {
        printf("(col %d)", (set-NDIG)+1);
    } else if (set<NDIG*3) {
        printf("(sqr %d)", (set-NDIG*2)+1);
    }
}
/****************************************************************/


/************************* STAGE THREE **************************/
/* implementing strategy one; find empty cells and check whether
they can be filled with their must be value */
int 
strategyone(int array[]) {
    int cell, newvalue, apply=0, i;
    int tmparray[N];

    /* fills in temporary array -- to avoid unidentifiable 
    cell to fill in before its pass */
    for (i=0; i<N; i++) {
        tmparray[i] = array[i];
    }

    /* CHECK if it is able to be filled */
    for (cell=0; cell<N; cell++) {
        if (array[cell]==0) {
            newvalue = fillcell(array, cell);
            /* found a cell that can be filled thus print header*/
            if (newvalue!=0) {
                apply = 1;
                printf("strategy one\n");
                break;
            }
        }
    }

    /* fill and update each cell that can be filled */
    for (cell=0; cell<N; cell++) {
        if (array[cell]==0) {
            newvalue = fillcell(array, cell);
            if (newvalue!=0) {
                tmparray[cell] = newvalue; 
                cell_to_name(array, cell);
                printf("%d\n", newvalue);
            }
        }
    }

    /* fills array from temporary array */ 
    for (i=0; i<N; i++) {
        array[i] = tmparray[i];
    }

    return apply;
}

/* find the value of cell that can be filled */
int
fillcell(int array[], int cell) {
    int set, setnum, othercell, cellmate, i, mustbe_value, possiblevals=0;
    int complete[NDIG] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (set=0; set<NGRP; set++) { 
	    setnum = c2s[cell][set];
        
        /* find cellmates */
        for (othercell=0; othercell<NDIG; othercell++) {
            cellmate = array[s2c[setnum][othercell]];
            
            if (cellmate!=0) {
                /* eliminate numbers that could not be the cell number */
                for (i=0; i<NDIG; i++) {
                    if (complete[i]==cellmate) {
                        complete[i] = 0; 
                    }
                }
            }
        }
    }
    
    /* find the possible values for the cells */
    for (i=0; i<NDIG; i++) {
        if (complete[i]!=0) {
            mustbe_value = complete[i];
            possiblevals++;
        }
    }
    
    if (possiblevals==1) {
        return mustbe_value;
    } else {
        return 0;
    }
}

/* change cell number to their row and col */
void
cell_to_name(int array[], int cell) {
    int col=0, row;
    for (row=1; row<=NDIG; row++)
        if (cell<NDIG*row) {
            col = (cell-NDIG*(row-1))+1;
            printf("row %d col %d must be ", row, col);
            break;
        }
}

/* returns 1 if soduku is completed */
int
completesudoku(int array[]) {
    int cell;
    for (cell=0; cell<N; cell++) {
        if (array[cell]==0) {
            return 0;
        }
    }

    return 1;
}
/****************************************************************/


/******************* OTHER HELPER FUNCTIONS *********************/
void
vertical_break(void) {
	printf(" | ");
}

void
horizontal_break(void) {
	printf("\n------+-------+------\n");
}

void
blank_line(void) {
	printf("\n");
}

void
ta_daa(void) {
	printf("ta daa!!!\n");
}
void
spacing(void) {
	printf(" ");
}
/****************************************************************/

/* ALGORITHMS ARE FUNN!! <3 */