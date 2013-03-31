#include <tonfa.h>

int getParenth(char* line)
{
	int ret = 0, i = 0;
	char c = line[i++];

	while (c != '\0'){
		if (c == '('){
			ret++;
		}
		else if (c == ')'){
			ret--;
		}
		else {}
		c = line[i++];
	}
	return ret;
}

char* expand_line(char* line, size_t size)
{
	char* ret;
	//printf("line: %s\n", line);
	char* tmp = (char*)malloc(size);
	strncpy(tmp, line, size);
	if ((ret = (char*)realloc(line, size * 2)) == NULL) {
		return NULL;
	}
	memset(ret, '\0', size * 2);
	strncpy(ret, tmp, size);
	//printf("ret: %s\n", ret);
	free(tmp);
	return ret;
}

#define SSIZE 64
#define DEFAULT_LINESIZE 256

void readFile(TNF_Index* index, char* str)
{
	FILE *fp;
	char s[SSIZE];
	char* line = (char*)malloc(DEFAULT_LINESIZE);
	char* linehead = line;
	size_t linesize = 0, linemaxsize = DEFAULT_LINESIZE;
	int ans;
	memset(s, 0, SSIZE);
	memset(line, 0, DEFAULT_LINESIZE);
	int parenth = 0;
	if ((fp = fopen(str, "r")) == NULL){
		fprintf(stderr, "ERROR: cannot open file %s\n", str);
		exit(0);
	}

	while (fgets(s, SSIZE, fp) != NULL){
		printf("s: %s\n", s);

		linesize += SSIZE;
		if (linesize > linemaxsize) {
			printf("line(%p): %p\n", line, linehead);
			linehead = expand_line(linehead, linemaxsize);
			printf("line(%p): %p\n", line, linehead);
			if (linehead == NULL) {
				return;
			}
			linemaxsize *= 2;
			line = linehead;
		}
		line = line + 2; // ';', '\n'
		//printf("line: %s, s: %s\n", line, s);
		strcat(line, s);
		//printf("line(%s): %s\n", __FUNCTION__, line);
		parse(index, &line);
		//free(linehead);
	}
	if (parenth != 0){
		fprintf(stderr, "ERROR: sytax error\n");
		exit(0);
	}
	fclose(fp);
}

/*
int main(int argc, char** argv)
{
	if (argc == 1){
		// interactive mode
	}
	else if (argc == 2){
		// file mode
		readFile(argv[1]);
	}
	else {
		fprintf(stderr, "ERROR: lispy need more arguments\n");
		exit(0);
	}
	return 0;
}
*/
