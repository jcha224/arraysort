/**
 * Course:  TCSS 333 C for System Programming
 * Author:  John Chang
 * Date: 10/26/2016
 *
 * This program pulls out the top 100 most popular names from 10 files and
 * creates 2 dimensional arrays that are parallel. It also sorts the names
 * and ranks and prints into the out file: summary.csv
 */
#include <stdio.h>
#include <string.h>

/**
 * Prototypes of all fuctions used in this program.
 */
void changeList(char name[][20], int rank[][15], int old, int new);
int checkString(char string[][20], char name[]);
int quickSortHelp(char name[][20], int rank[][15], int bottom, int top);
void quickSort(char name[][20], int rank[][15], int bottom, int top);
int getNames(char names[][20], int ranks[][15]);
void printFile(char name[][20], int rank[][15], FILE *file);

/**
 * Main method instantiates 2D arrays for names and ranks.
 */
int main () {
	FILE *summary = fopen("summary.csv", "w");
	char nameList[400][20] = {'\0'};
	int rankList[400][15] = {0};
	int totalNames = getNames(nameList, rankList);
	quickSort(nameList, rankList, 0, totalNames-1);
	printFile(nameList, rankList, summary);
	fclose(summary);
	return 0;

}



/**
 * Pulls names and ranks from the files and fills the 2D arrays.
 */
int getNames(char names[][20], int rank[][15]) {

	char name[20] = {'\0'};
	char temp[20] = {'\0'};
	int nlcount = 0;
	int year = 1920;

	for(int j = 0; j < 10; year = year+10, j++) {
		sprintf(temp, "yob%d.txt", year);
		FILE *tempFile = fopen(temp, "r");
		if(tempFile == NULL) {
			perror("Null");
		}

		for(int k = 1; k < 101; k++) {
			fscanf(tempFile, " %[^,],%*s,%*d", name);
			int check = checkString(names, name);
			if(check < 0) {
				strcpy(names[nlcount], name);
				rank[nlcount][j] = k;
				nlcount++;
			} else {
				rank[check][j] = k;
			}
		}
		fclose(tempFile);
	}
	return nlcount;
}

/**
 * Helper function to change contents of name array and rank array.
 */
void changeList(char name[][20], int rank[][15], int old, int new) {
	int temprank = 0;
	char tempname[20] = {'\0'};
	strcpy(tempname, name[old]);
	strcpy(name[old], name[new]);
	strcpy(name[new], tempname);

	for(int i = 0; i < 10; i++) {
		temprank = rank[old][i];
		rank[old][i] = rank[new][i];
		rank[new][i] = temprank;
	}
}

/**
 * Compares the new name with all previous names and returns index
 */
int checkString(char string[][20], char name[]) {
	int i =0, result =-1;
	while(string[i][0] != '\0') {
		if(strcmp(string[i], name) == 0) {
			result = i;
			break;
		}
		i++;
	}
	return result;
}

/**
 * Helper function for quicksort. Iterates through the array of names and switches
 * the names through quicksort.
 */
int quickSortHelp(char name[][20], int rank[][15], int bottom, int top) {
	int tempBottom = bottom, tempTop = top+1;
	char pivot[20] = {'\0'};
	strcpy(pivot, name[bottom]);

	while(1) {
		do tempBottom++;
		while(strcmp(name[tempBottom], pivot) <= 0 && tempBottom < top);
		do tempTop--;
		while(strcmp(name[tempTop], pivot) > 0);
		if(tempBottom >= tempTop) {
			break;
		}
		changeList(name, rank, tempBottom, tempTop);
	}
	changeList(name, rank, bottom, tempTop);
	return tempTop;

}

/**
 * Initializes the sort and divides the array through recursion.
 */
void quickSort(char name[][20], int rank[][15], int bottom, int top) {
	int mid = 0;
	if (bottom < top) {
		mid = quickSortHelp(name, rank, bottom, top);
		quickSort(name, rank, bottom, mid-1);
		quickSort(name, rank, mid+1, top);
	}
}

/**
 * Prints the contents of the arrays into the output file "summary.csv"
 */
void printFile(char name[][20], int rank[][15], FILE *file) {
	fprintf(file, "Name, 1920, 1930, 1940, 1950, 1960, "
			"1970, 1980, 1990, 2000, 2010");
	int index = 0;
	while(name[index][0] != '\0') {
		fprintf(file, "\n%s,", name[index]);
		for(int i = 0; i < 10; i++) {
			if(rank[index][i] < 1 || rank[index][i] > 100) {
				fprintf(file, ",");
			}else {
			fprintf(file, "%d,", rank[index][i]);
			}
		}
		index++;
	}
}

