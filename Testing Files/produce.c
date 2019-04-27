# include <stdio.h>

int main(){
	// initialization
	FILE * read;
	FILE * write;
	char word[100];
	int i = 0;
	int file_num = 1;

	// opening 2 files for reading and writing
	read = fopen("random" , "r");
	if (!read){
		perror("fopen");
		return -1;
	}
	write = fopen("outfile", "w");
	if (!write){
		perror("fopen");
		return -1;
	}
	// take the words and write it to outfile in the insert syntax
	while (fscanf(read, "%99s", word) == 1){
		if ((i % 3) == 0){
			fprintf(write, "i %s ", word);
		}
		else if ((i % 3) == 1){
			fprintf(write, "%s ", word);
		}
		else{
			fprintf(write, "%s file%d\n", word, file_num);
			file_num++;
		}
		i++;
	}
	// PRINT THE ENTIRE TREE
	fprintf(write, "p\n");
	fclose(read);
	fclose(write);

	// open another for the queries that is modified from the outfile
	read = fopen("outfile", "r");
	if (!read){
		perror("fopen");
		return -1;
	}
	write = fopen("queries", "w");
	if (!write){
		perror("fopen");
		return -1;
	}
	i = 0;
	while (fscanf(read, "%99s", word) == 1){
		if ((i % 5) == 0){
			fprintf(write, "q ");
		}
		else if ((i % 5) <= 3){
			fprintf(write, "%s ", word);
		}
		else{
			fprintf(write, "\n");
		}
		i++;
	}
	fclose(read);
	fclose(write);
	return 0;
}