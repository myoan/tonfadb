#include <tonfa.h>
#include <stdbool.h>

#define matchToken(t, c) (!strncmp(t, c, strlen(t)))

int isSpace(char c)
{
	if (c == ' ' || c == '\t')
		return 1;
	return 0;
}

char* addString(char** tmp, size_t size)
{
	char* ret = malloc(sizeof(char) * (size + 1));
	strncpy(ret, *tmp, size);
	ret[size] = '\0';
	*tmp += size;
	return ret;
}

int isBlank(char c)
{
	if (c == ' ' || c == '\t'){
		return 1;
	}
	return 0;
}

char* getToken(char** line)
{
	char c = *line[0];
	char* tmp = *line;
	int size = 0;

	while (c != '\0'){
		c = tmp[0];
		if (c == ' '){
			char* ret = addString(line, size);
			(*line)++;
			return ret;
		}
		else if (isBlank(c)){
			return addString(line, size);
		}
		else if (c == '(' || c == ')'){
			if (*line[0] != c){
				return addString(line, size);
			}
			if (isBlank(tmp[1])){
				char* ret = addString(line, size+1);
				(*line)++;
				return ret;
			}
			return addString(line, size+1);
		}
		tmp++; size++;
	}
	return '\0';
}

/*-------------------------------------------------*/
//                 parse functions 
/*-------------------------------------------------*/

TNF_Inst_t getInst(char* token){
	TNF_Inst_t ret = INST_NULL;
	if (!strncmp(token, "UPDATE", 1)){
		ret = INST_UPDATE;
	}
	else if (!strncmp(token, "SELECT", 1)){
		ret = INST_SELECT;
	}
	else if (!strncmp(token, "INSERT", 1)){
		ret = INST_INSERT;
	}
	else {
		ret = INST_NULL;
	}
	return ret;
}

//bool isDigit(char* token){
//	int tmp = atoi(token);
//	if (tmp != 0) return TRUE;
//	else {
//		if (strncmp(token, "0", 1)){
//			return FALSE;
//		}
//		return TRUE;
//	}
//}

bool isEnd(char* token) {
	return true;
}

void parse(TNF_Index* index, char** line)
{
	char* token;
	TNF_Inst_t inst;
	while ((token = getToken(line)) != '\0') {
		fprintf(stderr, "token(%s): %s\n", __FUNCTION__, token);
		if (matchToken(token, ";")) break;
		if ((inst = getInst(token)) == INST_NULL) {
			break;
		}
		TNF_CmdLibs[inst](index, line);
	}
}

