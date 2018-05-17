#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int MEMORY = 30000;

/*
	`die` function writes first argument to stderr
	and terminates the program.
	If it ends in ':' perror will be called.
*/
void die(const char *msg) {
	fputs(msg, stderr);
	if (msg[0] && msg[strlen(msg)-1] == ':') {
		fputc(' ', stderr);
		perror(NULL);
	} else {
		fputc('\n', stderr);
	}
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
	
	if (argc != 2) {
		fprintf(stderr, "usage: %s file", argv[0]);
		exit(EXIT_FAILURE);
	}

	FILE *file = fopen(argv[1], "r");
	if (file == NULL)
		die("fopen:");

	/* Source file size */
	if (fseek(file, 0, SEEK_END) != 0) die("fseek:");
	size_t source_length = ftell(file);
	if (fseek(file, 0, SEEK_SET) != 0) die("fseek:");

	if (source_length == 0) {
		/* Empty source file */
		return 0;
	}

	char *source = malloc(source_length + 1);
	if (source == NULL)
		die("malloc:");

	if (fread(source, source_length, 1, file) != 1)
		die("fread:");

	fclose(file);

	size_t mem = 0;
	unsigned char *memory = calloc(MEMORY, 1);
	if (memory == NULL)
		die("calloc:");

	/* Check for shebang in source file */
	if (source_length >= 2 && source[0] == '#' && source[1] == '!') {
		/*
			If the source starts with "#!" it will remove first line from it
			Note: original 'malloc'ed adress is lost, it will be freed on exit
		*/
		while (*source != '\n' && source_length) {
			source++;
			source_length--;
		}
	}

	if (source_length == 0) {
		/* This file contains only shebang, exiting */
		return 0;
	}

	int ch; /* for getchar() below */
	int brackets;
	
	for (size_t pos = 0; pos < source_length; pos++) {

		char code = source[pos];
		
		switch (code) {

		case '>':
			if (mem == MEMORY)
				die("Upper memory bounds reached");
			mem++;
			break;

		case '<':
			if (mem == 0)
				die("Lower memory bounds reached");
			mem--;
			break;

		case '+':
			memory[mem]++;
			break;
			
		case '-':
			memory[mem]--;
			break;

		case '.':
			if (putchar(memory[mem]) == EOF)
				die("putchar:");
			break;

		case ',':
			/* EOF yields 0 in current memory cell */
			ch = getchar();
			if (ch == EOF)
				ch = 0;
			memory[mem] = ch;
			break;

		case '[':
			if (memory[mem] != 0)
				break;
			brackets = 1;
			for (pos++; pos < source_length; pos++) {
				if (source[pos] == '[' || source[pos] == ']') {
					brackets += source[pos] == code ? 1 : -1;
					if (brackets == 0) {
						break;
					}
				}
			}
			if (brackets != 0)
				die("Unmatched bracket");
			break;

		case ']':
			if (memory[mem] == 0)
				break;
			brackets = 1;
			for (pos--; pos >= 0; pos--) {
				if (source[pos] == '[' || source[pos] == ']') {
					brackets += source[pos] == code ? 1 : -1;
					if (brackets == 0) {
						break;
					}
				}
			}
			if (brackets != 0)
				die("Unmatched bracket");
			break;

		case '$':
			/* Print memory */
			fputc('\n', stderr);
			for (int i = 0; i < 32; i++) {
				if (i == mem) fputc('[', stderr);
				else if (i == mem + 1) fputc(']', stderr);
				else fputc(' ', stderr);
				fprintf(stderr, "%02X", memory[i]);
			}
			if (mem == 32) fputc(']', stderr);
			fputc('\n', stderr);
			break;

		default:
			/* Ignoring other chars */
			break;
		
		}
		
	}

	return 0;
}
