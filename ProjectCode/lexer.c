/* Group - 20
* Yash Sinha 2012C6PS365P
* Krishna Garg 2012A7PS033P
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 10 * 1024
#define MAX_LINE_SIZE 10 * 1024

typedef int BuffSize;
typedef char* Buff;
Buff currentBuff; 
typedef struct _tokeninfo{
	int tk_lineNo;
	char lexeme[100];
	char *token_name;
}tokeninfo;

tokeninfo instance[2000];

int state;
char *tokenList[2000];
int tokenIndex = 0;
int lineNo = 0;

char* varUsage[100][100];
int varUsageCount[100];

char* opUsage[100][100];
int opUsageCount[100];

char* opIDUsage[100][100];
int opIDUsageCount[100];

char* values[100][100];
int valuesCount[100];

FILE* readToBuff(FILE *fp, Buff B, BuffSize k) {
	int n;
	if (fp == NULL) return fp;
	n = fread(B, 1, BUFFER_SIZE, fp);
	B[n] = EOF;
	return fp;
}

int isComingFromL(int state ) {
	switch(state) {
		case 1:
		case 44:
		case 85:
		case 116:
		case 86:
		case 117:
		case 42:
		case 83:
		case 111:
		case 136:
		case 153:
		case 165:
		case 43:
		case 84:
		case 112:
		case 137:
		case 154:
		case 166:
		case 175:
		case 181:
		case 186:
		case 113:
		case 138:
		case 155:
		case 167:
		case 176:
		case 182:
		case 187:
		case 189:
		case 190:
		case 191:
		case 192: //PROCS
		case 193: //ENDPROCS
		case 114:
		case 139:
		case 115:
		case 140:
		case 156:
		case 168:
		case 2:
		case 47:
		case 89:
		case 120:
		case 45:
		case 87:
		case 118:
		case 141:
		case 157:
		case 169:
		case 177:
		case 46:
		case 88:
		case 119:
		case 3:
		case 48:
		case 90:
		case 4:
		case 49:
		case 91:
		case 121:
		case 142:
		case 158:
		case 170:
		case 92:
		case 122:
		case 5:
		case 50:
		case 93:
		case 123:
		case 143:
		case 159:
		case 124:
		case 160:
		case 171:
		case 178:
		case 183:
		case 144:
		case 6:
		case 52:
		case 95:
		case 125:
		case 145:
		case 161:
		case 172:
		case 179:
		case 184:
		case 188:
		case 51:
		case 94:
		case 7:
		case 54:
		case 53:
		case 96:
		case 8:
		case 55:
		case 97:
		case 126:
		case 146:
		case 162:
		case 9:
		case 56:
		case 98:
		case 127:
		case 10:
		case 58:
		case 100:
		case 129:
		case 57:
		case 99:
		case 128:
		case 147:
		case 11:
		case 59:
		case 101:
		case 12:
		case 60:
		case 102:
		case 13:
		case 61:
		case 14:
		case 62:
		case 103:
		case 130:
		case 15:
		case 63:
		case 104:
		case 131:
		case 148:
		case 16:
		case 64:
		case 105:
		case 18:
		case 66:
		case 106:
		case 132:
		case 149:
		case 163:
		case 173:
		case 180:
		case 185:
		case 19:
		case 69:
		case 109:
		case 135:
		case 152:
		case 164:
		case 174:
		case 67:
		case 107:
		case 133:
		case 150:
		case 68:
		case 108:
		case 134:
		case 151:
			return 1;
		default:
			return 0;
	}
}

int stateIsFinal(int finalState) {
	if(finalState <= 18 && finalState >= 1)
		return 1;
	switch(finalState) {
		case -2:
		case 101:
		case 102:
		case 105:
		case 106:
		case 110:
		case 115:
		case 116:
		case 117:
		case 119:
		case 120:
		case 122:
		case 125:
		case 127:
		case 129:
		case 130:
		case 139:
		case 147:
		case 148:
		case 150:
		case 151:
		case 154:
		case 159:
		case 162:
		case 165:
		case 167:
		case 168:
		case 170:
		case 174:
		case 177:
		case 17:
		case 183:
		case 185:
		case 186:
		case 188:
		case 191:
		case 192: //PROCS
		case 193: //ENDPROCS
		case 20:
		case 21:
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		case 29:
		case 30:
		case 31:
		case 32:
		case 34:
		case 35:
		case 36:
		case 37:
		case 38:
		case 39:
		case 40:
		case 41:
		case 54:
		case 61:
		case 70:
		case 71:
		case 72:
		case 74:
		case 76:
		case 77:
		case 78:
		case 80:
		case 81:
		case 82:
		case 84:
		case 90:
		case 91:
		case 94:
		case 96:
			return 1;
		default:
			return 0;
			break;
	}
}

void acceptToken(int finalState, int initPos, int lastpos, Buff B) {
	if((finalState <= 16 || finalState == 18)&& finalState >= 1)
			tokenList[tokenIndex++] = "ID";
	switch(finalState) {
		case -2:
			// tokenList[tokenIndex++] = "SPACE";
			break;
		case 17:
			tokenList[tokenIndex++] = "INT_LIT";
			break;
		case 20:
			tokenList[tokenIndex++] = "COLON";
			break;
		case 21:
			tokenList[tokenIndex++] = "LPAREN";
			break;
		case 22:
			tokenList[tokenIndex++] = "RPAREN";
			break;
		case 23:
			tokenList[tokenIndex++] = "MINUS";
			break;
		case 24:
			tokenList[tokenIndex++] = "GT";
			break;
		case 25:
			tokenList[tokenIndex++] = "COMMA";
			break;
		case 26:
			tokenList[tokenIndex++] = "SEMICOLON";
			break;
		case 27:
			tokenList[tokenIndex++] = "LT";
			break;
		case 28:
			tokenList[tokenIndex++] = "LSQ";
			break;
		case 29:
			tokenList[tokenIndex++] = "RSQ";
			break;
		case 30:
			tokenList[tokenIndex++] = "DQUOTE";
			break;			
		case 31:
			tokenList[tokenIndex++] = "DOT";
			break;
		case 32:
			tokenList[tokenIndex++] = "ASSIGN_OP";
			break;
		case 34:
			tokenList[tokenIndex++] = "SQUOTE";
			break;
		case 35:
			tokenList[tokenIndex++] = "PLUS";
			break;
		case 36:
			tokenList[tokenIndex++] = "MUL";
			break;
		case 37:
			tokenList[tokenIndex++] = "DIV";
			break;
		case 38:
			tokenList[tokenIndex++] = "MOD";
			break;
		case 39:
			tokenList[tokenIndex++] = "LCURL";
			break;
		case 40:
			tokenList[tokenIndex++] = "RCURL";
			break;
		case 41:
			tokenList[tokenIndex++] = "ID";
			break;
		case 70:
			tokenList[tokenIndex++] = "ARROW";
			break;
		case 71:
			tokenList[tokenIndex++] = "GE";
			break;
		case 72:
			tokenList[tokenIndex++] = "LE";
			break;
		case 74:
			tokenList[tokenIndex++] = "STR_LIT";
			break;
		case 76:
			tokenList[tokenIndex++] = "REAL_LIT";
			break;
		case 77:
			tokenList[tokenIndex++] = "EQ";
			break;
		case 78:
			tokenList[tokenIndex++] = "NEQ";
			break;
		case 80:
			tokenList[tokenIndex++] = "POW";
			break;
		case 81:
			tokenList[tokenIndex++] = "ID";
			break;
		case 82:
			tokenList[tokenIndex++] = "ID";
			break;
		case 84:
			tokenList[tokenIndex++] = "END";
			break;
		case 110:
			tokenList[tokenIndex++] = "CHAR_LIT";
			break;
		case 116:
			tokenList[tokenIndex++] = "ELSE";
			break;
		case 117:
			tokenList[tokenIndex++] = "ELIF";
			break;
		case 167:
			tokenList[tokenIndex++] = "ENDPROC";
			break;
			//jhbcfkjbfkhbsdfkcbskfjbdsf
		case 115:
			tokenList[tokenIndex++] = "ENDL";
			break;
		case 139:
			tokenList[tokenIndex++] = "ENDIF";
			break;
		case 154:
			tokenList[tokenIndex++] = "ENDREC";
			break;
		case 165:
			tokenList[tokenIndex++] = "EXECUTE";
			break;
		case 168:
			tokenList[tokenIndex++] = "ENDLOOP";
			break;
		case 120:
			tokenList[tokenIndex++] = "CHAR";
			break;
		case 177:
			tokenList[tokenIndex++] = "CONTINUE";
			break;
		case 119:
			tokenList[tokenIndex++] = "CALL";
			break;
		case 90:
			tokenList[tokenIndex++] = "TAB";
			break;
		case 91:
			tokenList[tokenIndex++] = "REC";
			break;
		case 170:
			tokenList[tokenIndex++] = "RECORDS";
			break;
		case 122:
			tokenList[tokenIndex++] = "REAL";
			break;
		case 159:
			tokenList[tokenIndex++] = "STRING";
			break;
		case 183:
			tokenList[tokenIndex++] = "STRASSIGN";
			break;
		case 188:
			tokenList[tokenIndex++] = "PROCEDURES";
			break;
		case 125:
			tokenList[tokenIndex++] = "PROC";
			break;
		case 94:
			tokenList[tokenIndex++] = "PUT";
			break;
		case 54:
			tokenList[tokenIndex++] = "IF";
			break;
		case 96:
			tokenList[tokenIndex++] = "INT";
			break;
		case 162:
			tokenList[tokenIndex++] = "ASSIGN";
			break;
		case 127:
			tokenList[tokenIndex++] = "LOOP";
			break;
		case 129:
			tokenList[tokenIndex++] = "BOOL";
			break;
		case 147:
			tokenList[tokenIndex++] = "BREAK";
			break;
		case 101:
			tokenList[tokenIndex++] = "GET";
			break;
		case 102:
			tokenList[tokenIndex++] = "NOT";
			break;
		case 61:
			tokenList[tokenIndex++] = "OR";
			break;
		case 130:
			tokenList[tokenIndex++] = "BOOL_LIT";
			break;
		case 148:
			tokenList[tokenIndex++] = "BOOL_LIT";
			break;
		case 105:
			tokenList[tokenIndex++] = "AND";
			break;
		case 106:
			tokenList[tokenIndex++] = "MAT";
			break;
		case 185:
			tokenList[tokenIndex++] = "MATASSIGN";
			break;
		case 174:
			tokenList[tokenIndex++] = "STR_LEN";
			break;
		case 150:
			tokenList[tokenIndex++] = "MAT_COLS";
			break;
		case 151:
			tokenList[tokenIndex++] = "MAT_ROWS";
			break;
		case 186:
			tokenList[tokenIndex++] = "ENDRECORDS";
			break;
		case 191:
			tokenList[tokenIndex++] = "ENDPROCEDURES";
			break;
		case 192:
			tokenList[tokenIndex++] = "PROCS";
			break; //PROCS
		case 193:
			tokenList[tokenIndex++] = "ENDPROCS";
			break; //ENDPROCS
		default:
			break;
	}
	if(finalState!=-2){

		instance[tokenIndex - 1].token_name = tokenList[tokenIndex-1];
		instance[tokenIndex - 1].tk_lineNo = lineNo+1;
		// strcpy(instance[tokenIndex - 1].token_name,tokenList[tokenIndex-1]);
		// printf("%s ", tokenList[tokenIndex-1]);
		printf("%s ", instance[tokenIndex - 1].token_name);
		// printf("LineNum = %d\n", instance[tokenIndex - 1].tk_lineNo);
	// sleep(1);
		if(strcmp(instance[tokenIndex - 1].token_name, "ID") == 0) {
			int i, k = 0;
			char id[100];
			for(i = initPos-1; i < lastpos-1; i++) {
				instance[tokenIndex - 1].lexeme[k++] = B[i];
				printf("%c", B[i]);
			}
			varUsage[lineNo][varUsageCount[lineNo]++] = instance[tokenIndex - 1].lexeme;
			// strcpy(instance[tokenIndex - 1].lexeme, id);

			printf(" ");
			// printf(" %c %c ", B[initPos-1], B[lastpos-2]);
		}
		else if(strcmp(instance[tokenIndex - 1].token_name, "LPAREN") == 0
			|| strcmp(instance[tokenIndex - 1].token_name, "RPAREN") == 0
			|| strcmp(instance[tokenIndex - 1].token_name, "MINUS") == 0
			|| strcmp(instance[tokenIndex - 1].token_name, "GT") == 0
			|| strcmp(instance[tokenIndex - 1].token_name, "GE") == 0
			|| strcmp(instance[tokenIndex - 1].token_name, "LT") == 0
			|| strcmp(instance[tokenIndex - 1].token_name, "LE") == 0
			|| strcmp(instance[tokenIndex - 1].token_name, "EQ") == 0
			|| strcmp(instance[tokenIndex - 1].token_name, "NEQ") == 0
			|| strcmp(instance[tokenIndex - 1].token_name, "PLUS") == 0
			|| strcmp(instance[tokenIndex - 1].token_name, "MUL") == 0
			|| strcmp(instance[tokenIndex - 1].token_name, "POW") == 0
			|| strcmp(instance[tokenIndex - 1].token_name, "DIV") == 0
			|| strcmp(instance[tokenIndex - 1].token_name, "MOD") == 0
			|| strcmp(instance[tokenIndex - 1].token_name, "ASSIGN_OP") == 0
			) 
		{
			int i, k = 0;
			char id[100];
			for(i = initPos-1; i < lastpos-1; i++) {
				instance[tokenIndex - 1].lexeme[k++] = B[i];
				// printf("%c", B[i]);
			}
			opUsage[lineNo][opUsageCount[lineNo]++] = instance[tokenIndex - 1].lexeme;
			// strcpy(instance[tokenIndex - 1].lexeme, id);

			// printf(" ");
			// printf(" %c %c ", B[initPos-1], B[lastpos-2]);
		}
		if(strcmp(instance[tokenIndex - 1].token_name, "ID") == 0
			|| strcmp(instance[tokenIndex - 1].token_name, "BOOL_LIT") == 0
			|| strcmp(instance[tokenIndex - 1].token_name, "CHAR_LIT") == 0
			|| strcmp(instance[tokenIndex - 1].token_name, "REAL_LIT") == 0
			|| strcmp(instance[tokenIndex - 1].token_name, "INT_LIT") == 0
			|| strcmp(instance[tokenIndex - 1].token_name, "STR_LIT") == 0
			) 
		{
			int i, k = 0;
			char id[100];
			for(i = initPos-1; i < lastpos-1; i++) {
				instance[tokenIndex - 1].lexeme[k++] = B[i];
				// printf("%c", B[i]);
			}
			if(strcmp(instance[tokenIndex - 1].token_name, "ID") == 0)
				opIDUsage[lineNo][opIDUsageCount[lineNo]++] = instance[tokenIndex - 1].lexeme;
			else {
				opIDUsage[lineNo][opIDUsageCount[lineNo]++] = instance[tokenIndex - 1].token_name;
				values[lineNo][valuesCount[lineNo]++] = instance[tokenIndex - 1].lexeme;
			}
			// strcpy(instance[tokenIndex - 1].lexeme, id);

			// printf(" ");
			// printf(" %c %c ", B[initPos-1], B[lastpos-2]);
		}
	}

}

int getTokens(Buff B, int position) {
	char c;
	int lastpos = position, initPos = position, acceptState = 0, isAccepted = 0, start;
	while(1){
		c = B[position++];
		// printf("%c", c);
		if(stateIsFinal(state)) {
			lastpos = position-1;
			acceptState = state;
			isAccepted = 1;
			if(c == '\n') {
				acceptToken(acceptState, initPos, lastpos, B);

				lineNo++;
				// printf("%dPosition \n", position);
				// printf("\n%c",B[lastpos]);
				// printf("%c",B[position]);
				isAccepted = 0;
				position = lastpos + 1;
				return position;
			}
			// state = 0;
		}
		if(c == '\n') lineNo++;
		switch(c) {
			case 'a':
				if(state == 0) {
					state = 8;
					lastpos = position;
					start = position;
				}
				else if(state == 106)
					state = 132;
				else if(state == 18)
					state = 66;
				else if(state == 15)
					state = 63;
				else if(state == 99)
					state = 128;
				else if(state == 93)
					state = 124;
				else if(state == 49)
					state = 92;
				else if(state == 2)
					state = 46;
				else if(state == 47)
					state = 89;
				else if(state == 3)
					state = 48;
				else if(state == 30 || state == 75)
					state = 75;
				else if(state == 73)
					state = 75;
				else if(state == 41 || state == 82 || state == 81)
					state == 82;
				else if(state == 34)
					state = 79;
				else if(isComingFromL(state))
					state = 41;
				else state = -1;
				break;
			case 'b':
				if(state == 0) {
					state = 10;
					lastpos = position;
					start = position;
				}
				else if(state == 48)
					state = 90;
				else if(state == 30 || state == 75)
					state = 75;
				else if(state == 73)
					state = 75;
				else if(state == 41 || state == 82 || state == 81)
					state == 82;
				else if(state == 34)
					state = 79;
				else if(isComingFromL(state))
					state = 41;
				else state = -1;
				break;
			case 'c':
				if(state == 0) {
					state = 2;
					lastpos = position;
					start = position;
				}
				else if(state == 19)
					state = 67;
				else if(state == 95)
					state = 125;
				else if(state == 49)
					state = 91;
				else if(state == 137)
					state = 154;
				else if(state == 83)
					state = 111;
				else if(state == 30 || state == 75)
					state = 75;
				else if(state == 73)
					state = 75;
				else if(state == 41 || state == 82 || state == 81)
					state == 82;
				else if(state == 34)
					state = 79;
				else if(state == 155)
				state = 167;
				else if(isComingFromL(state))
					state = 41;
				else state = -1;
				break;
			case 'd':
				if(state == 0) {
					state = 41;
					lastpos = position;
					start = position;
				}
				else if(state == 145)
					state = 161;
				else if(state == 142)
					state = 158;
				else if(state == 176)
					state = 182;
				else if(state == 175)
					state = 181;
				else if(state == 30 || state == 75)
					state = 75;
				else if(state == 73)
					state = 75;
				else if(state == 41 || state == 82 || state == 81)
					state == 82;
				else if(state == 34)
					state = 79;
				else if(state == 43)
				state = 84;
				else if(isComingFromL(state))
					state = 41;
				else state = -1;
				break;
			case 'e':
				if(state == 0) {
					state = 1;
					lastpos = position;
					start = position;
				}
				else if(state == 69)
					state = 109;
				else if(state == 131)
					state = 148;
				else if(state == 103)
					state = 130;
				else if(state == 11)
					state = 59;
				else if(state == 57)
					state = 99;
				else if(state == 179)
					state = 184;
				else if(state == 125)
					state = 145;
				else if(state == 4)
					state = 49;
				else if(state == 169)
					state = 177;
				else if(state == 189)
					state = 190;
				else if(state == 167)
					state = 176;
				else if(state == 112)
					state = 137;
				else if(state == 153)
					state = 165;
				else if(state == 42)
					state = 83;
				else if(state == 30 || state == 75)
					state = 75;
				else if(state == 73)
					state = 75;
				else if(state == 41 || state == 82 || state == 81)
					state == 82;
				else if(state == 34)
					state = 79;
				else if(state == 85)
				state = 116;
				else if(isComingFromL(state))
					state = 41;
				else state = -1;
				break;
			case 'f':
				if(state == 0) {
					state = 41;
					lastpos = position;
					start = position;
				}
				else if(state == 7)
					state = 54;
				else if(state == 114)
					state = 139;
				else if(state == 30 || state == 75)
					state = 75;
				else if(state == 73)
					state = 75;
				else if(state == 41 || state == 82 || state == 81)
					state == 82;
				else if(state == 34)
					state = 79;
				else if(state == 86)
				state = 117;
				else if(isComingFromL(state))
					state = 41;
				else state = -1;
				break;
			case 'g':
				if(state == 0) {
					state = 11;
					lastpos = position;
					start = position;
				}
				else if(state == 135)
					state = 152;
				else if(state == 173)
					state = 180;
				else if(state == 126)
					state = 146;
				else if(state == 171)
					state = 178;
				else if(state == 143)
					state = 159;
				else if(state == 30 || state == 75)
					state = 75;
				else if(state == 73)
					state = 75;
				else if(state == 41 || state == 82 || state == 81)
					state == 82;
				else if(state == 34)
					state = 79;
				else if(isComingFromL(state))
					state = 41;
				else state = -1;
				break;
			case 'h':
				if(state == 0) {
					state = 41;
					lastpos = position;
					start = position;
				}
				else if(state == 164)
					state = 174;
				else if(state == 2)
					state = 47;
				else if(state == 30 || state == 75)
					state = 75;
				else if(state == 73)
					state = 75;
				else if(state == 41 || state == 82 || state == 81)
					state == 82;
				else if(state == 34)
					state = 79;
				else if(isComingFromL(state))
					state = 41;
				else state = -1;
				break;
			case 'i':
				if(state == 0) {
					state = 7;
					lastpos = position;
					start = position;
				}
				else if(state == 163)
					state = 173;
				else if(state == 97)
					state = 126;
				else if(state == 160)
					state = 171;
				else if(state == 93)
					state = 123;
				else if(state == 118)
					state = 141;
				else if(state == 84)
					state = 114;
				else if(state == 30 || state == 75)
					state = 75;
				else if(state == 73)
					state = 75;
				else if(state == 41 || state == 82 || state == 81)
					state == 82;
				else if(state == 34)
					state = 79;
				else if(state == 44)
				state = 86;
				else if(isComingFromL(state))
					state = 41;
				else state = -1;
				break;
			case 'j':
				if(state == 0) {
					state = 41;
					lastpos = position;
					start = position;
				}
				else if(state == 30 || state == 75)
					state = 75;
				else if(state == 73)
					state = 75;
				else if(state == 41 || state == 82 || state == 81)
					state == 82;
				else if(state == 34)
					state = 79;
				else if(isComingFromL(state))
					state = 41;
				else state = -1;
				break;
			case 'k':
				if(state == 0) {
					state = 41;
					lastpos = position;
					start = position;
				}
				else if(state == 128)
					state = 147;
				else if(state == 30 || state == 75)
					state = 75;
				else if(state == 73)
					state = 75;
				else if(state == 41 || state == 82 || state == 81)
					state == 82;
				else if(state == 34)
					state = 79;
				else if(isComingFromL(state))
					state = 41;
				else state = -1;
				break;
			case 'l':
				if(state == 0) {
					state = 9;
					lastpos = position;
					start = position;
				}
				else if(state == 107)
					state = 133;
				else if(state == 19)
					state = 69;
				else if(state == 63)
					state = 104;
				else if(state == 100)
					state = 129;
				else if(state == 92)
					state = 122;
				else if(state == 46)
					state = 88;
				else if(state == 88)
					state = 119;
				else if(state == 84)
					state = 115;
				else if(state == 30 || state == 75)
					state = 75;
				else if(state == 73)
					state = 75;
				else if(state == 41 || state == 82 || state == 81)
					state == 82;
				else if(state == 34)
					state = 79;
				else if(state == 1)
				state = 44;
				else if(isComingFromL(state))
					state = 41;
				else state = -1;
				break;
			case 'm':
				if(state == 0) {
					state = 18;
					lastpos = position;
					start = position;
				}
				else if(state == 30 || state == 75)
					state = 75;
				else if(state == 73)
					state = 75;
				else if(state == 41 || state == 82 || state == 81)
					state == 82;
				else if(state == 34)
					state = 79;
				else if(isComingFromL(state))
					state = 41;
				else state = -1;
				break;
			case 'n':
				if(state == 0) {
					state = 41;
					lastpos = position;
					start = position;
				}
				else if(state == 109)
					state = 135;
				else if(state == 180)
					state = 185;
				else if(state == 146)
					state = 162;
				else if(state == 7)
					state = 53;
				else if(state == 178)
					state = 183;
				else if(state == 123)
					state = 143;
				else if(state == 141)
					state = 157;
				else if(state == 45)
					state = 87;
				else if(state == 30 || state == 75)
					state = 75;
				else if(state == 73)
					state = 75;
				else if(state == 41 || state == 82 || state == 81)
					state == 82;
				else if(state == 34)
					state = 79;
				else if(state == 1)
				state = 43;
				else if(isComingFromL(state))
					state = 41;
				else state = -1;
				break;
			case 'o':
				if(state == 0) {
					state = 41;
					lastpos = position;
					start = position;
				}
				else if(state == 68)
					state = 108;
				else if(state == 67)
					state = 107;
				else if(state == 58)
					state = 100;
				else if(state == 10)
					state = 58;
				else if(state == 56)
					state = 98;
				else if(state == 9)
					state = 56;
				else if(state == 52)
					state = 95;
				else if(state == 91)
					state = 121;
				else if(state == 2)
					state = 45;
				else if(state == 140)
					state = 156;
				else if(state == 115)
					state = 140;
				else if(state == 154)
					state = 166;
				else if(state == 30 || state == 75)
					state = 75;
				else if(state == 73)
					state = 75;
				else if(state == 41 || state == 82 || state == 81)
					state == 82;
				else if(state == 34)
					state = 79;
				else if(state == 138)
				state = 155;
				else if(isComingFromL(state))
					state = 41;
				else state = -1;
				break;
			case 'p':
				if(state == 0) {
					state = 6;
					lastpos = position;
					start = position;
				}
				else if(state == 98)
					state = 127;
				else if(state == 156)
					state = 168;
				else if(state == 30 || state == 75)
					state = 75;
				else if(state == 73)
					state = 75;
				else if(state == 41 || state == 82 || state == 81)
					state == 82;
				else if(state == 34)
					state = 79;
				else if(state == 84)
					state = 113;
				else if(isComingFromL(state))
					state = 41;
				else state = -1;
				break;
			case 'q':
				if(state == 0) {
					state = 41;
					lastpos = position;
					start = position;
				}
				else if(state == 30 || state == 75)
					state = 75;
				else if(state == 73)
					state = 75;
				else if(state == 41 || state == 82 || state == 81)
					state == 82;
				else if(state == 34)
					state = 79;
				else if(isComingFromL(state))
					state = 41;
				else state = -1;
				break;
			case 'r':
				if(state == 0) {
					state = 4;
					lastpos = position;
					start = position;
				}
				else if(state == 19)
					state = 68;
				else if(state == 14)
					state = 62;
				else if(state == 10)
					state = 57;
				else if(state == 172)
					state = 179;
				else if(state == 6)
					state = 52;
				else if(state == 93)
					state = 123;
				else if(state == 50)
					state = 93;
				else if(state == 121)
					state = 142;
				else if(state == 89)
					state = 120;
				else if(state == 187)
					state = 189;
				else if(state == 166)
					state = 175;
				else if(state == 84)
					state = 112;
				else if(state == 30 || state == 75)
					state = 75;
				else if(state == 73)
					state = 75;
				else if(state == 41 || state == 82 || state == 81)
					state == 82;
				else if(state == 34)
					state = 79;
				else if(state == 113)
					state = 138;
				else if(isComingFromL(state))
					state = 41;
				else state = -1;
				break;
			case 's':
				if(state == 0) {
					state = 5;
					lastpos = position;
					start = position;
				}
				else if(state == 167)
					state = 193;
				else if(state == 125)
					state = 192;
				else if(state == 134)
					state = 151;
				else if(state == 133)
					state = 150;
				else if(state == 149)
					state = 163;
				else if(state == 132)
					state = 149;
				else if(state == 104)
					state = 131;
				else if(state == 55)
					state = 97;
				else if(state == 8)
					state = 55;
				else if(state == 184)
					state = 188;
				else if(state == 144)
					state = 160;
				else if(state == 124)
					state = 144;
				else if(state == 158)
					state = 170;
				else if(state == 190)
					state = 191;
				else if(state == 181)
					state = 186;
				else if(state == 30 || state == 75)
					state = 75;
				else if(state == 73)
					state = 75;
				else if(state == 41 || state == 82 || state == 81)
					state == 82;
				else if(state == 34)
					state = 79;
				else if(state == 44)
					state = 85;
				else if(isComingFromL(state))
					state = 41;
				else state = -1;
					break;
			case 't':
				if(state == 0) {
					state = 3;
					lastpos = position;
					start = position;
				}
				else if(state == 152)
					state = 164;
				else if(state == 66)
					state = 106;
				else if(state == 59)
					state = 101;
				else if(state == 53)
					state = 96;
				else if(state == 51)
					state = 94;
				else if(state == 5)
					state = 50;
				else if(state == 87)
					state = 118;
				else if(state == 136)
					state = 153;
				else if(state == 30 || state == 75)
					state = 75;
				else if(state == 73)
					state = 75;
				else if(state == 41 || state == 82 || state == 81)
					state == 82;
				else if(state == 34)
					state = 79;
				else if(isComingFromL(state))
					state = 41;
				else state = -1;
				break;
			case 'u':
				if(state == 0) {
					state = 41;
					lastpos = position;
					start = position;
				}
				else if(state == 62)
					state = 103;
				else if(state == 6)
					state = 51;
				else if(state == 161)
					state = 172;
				else if(state == 157)
					state = 169;
				else if(state == 182)
					state = 187;
				else if(state == 111)
					state = 136;
				else if(state == 30 || state == 75)
					state = 75;
				else if(state == 73)
					state = 75;
				else if(state == 41 || state == 82 || state == 81)
					state == 82;
				else if(state == 34)
					state = 79;
				else if(isComingFromL(state))
					state = 41;
				else state = -1;
				break;
			case 'v':
				if(state == 0) {
					state = 41;
					lastpos = position;
					start = position;
				}
				else if(state == 30 || state == 75)
					state = 75;
				else if(state == 73)
					state = 75;
				else if(state == 41 || state == 82 || state == 81)
					state == 82;
				else if(state == 34)
					state = 79;
				else if(isComingFromL(state))
					state = 41;
				else state = -1;
				break;
			case 'w':
				if(state == 0) {
					state = 41;
					lastpos = position;
					start = position;
				}
				else if(state == 108)
					state = 134;
				else if(state == 30 || state == 75)
					state = 75;
				else if(state == 73)
					state = 75;
				else if(state == 41 || state == 82 || state == 81)
					state == 82;
				else if(state == 34)
					state = 79;
				else if(isComingFromL(state))
					state = 41;
				else state = -1;
				break;
			case 'x':
				if(state == 0) {
					state = 41;
					lastpos = position;
					start = position;
				}
				else if(state == 1)
					state = 42;
				else if(state == 30 || state == 75)
					state = 75;
				else if(state == 73)
					state = 75;
				else if(state == 41 || state == 82 || state == 81)
					state == 82;
				else if(state == 34)
					state = 79;
				else if(isComingFromL(state))
					state = 41;
				else state = -1;
				break;
			case 'y':
				if(state == 0) {
					state = 41;
					lastpos = position;
					start = position;
				}
				else if(state == 30 || state == 75)
					state = 75;
				else if(state == 73)
					state = 75;
				else if(state == 41 || state == 82 || state == 81)
					state == 82;
				else if(state == 34)
					state = 79;
				else if(isComingFromL(state))
					state = 41;
				else state = -1;
				break;
			case 'z':
				if(state == 0) {
					state = 41;
					lastpos = position;
					start = position;
				}
				else if(state == 30 || state == 75)
					state = 75;
				else if(state == 73)
					state = 75;
				else if(state == 41 || state == 82 || state == 81)
					state == 82;
				else if(state == 34)
					state = 79;
				else if(isComingFromL(state))
					state = 41;
				else state = -1;
				break;
			// case '\n':
				// if(lastpos != initPos && lastpos != position) 
					// printf("Error ");
				// state = 0;
				// return position;
			case ' ':
			case '\t':
				// if(lastpos != -1 && lastpos != position) 
				// 	printf("Error ");
				// position++;
				if(state == 0){

					state = -2;//Indentify SPACE
				// printf("position = %d\n",position);

				}
				else if(state == 30 || state == 73 || state == 75)
					state = 75;
				else state = -1;
				// continue;
				break;
			case EOF: 
				return position - 1;
			case '.':
				if(state == 0) {
					state = 31;
					lastpos = position;
					start = position;
				}
				else if(state == 17)
					state = 65;
				else state = -1;
				break;
			case 'O':
				if(state == 0){
					state = 13;
					lastpos = position;
					start = position;
				}
				else if(state == 12)
					state = 60;
				else if(isComingFromL(state))
					state = 41;
				else if(state == 0) {
					state = 41;
					lastpos = position;
					start = position;
				}
				else if(state == 30 || state == 75)
					state = 75;
				else if(state == 73)
					state = 75;
				else if(state == 41 || state == 82 || state == 81)
					state == 82;
				else if(state == 34)
					state = 79;
				else if(isComingFromL(state))
					state = 41;
				else state = -1;
				break;
			case 'T':
				if(state == 0){
					state = 14;
					lastpos = position;
					start = position;
				}
				else if(state == 60)
					state = 102;
				else if(isComingFromL(state))
					state = 41;
				else if(state == 0) {
					state = 41;
					lastpos = position;
					start = position;
				}
				else if(state == 30 || state == 75)
					state = 75;
				else if(state == 73)
					state = 75;
				else if(state == 41 || state == 82 || state == 81)
					state == 82;
				else if(state == 34)
					state = 79;
				else if(isComingFromL(state))
					state = 41;
				else state = -1;
				break;
			case 'R':
				if(state == 13)
					state = 61;
				else if(isComingFromL(state))
					state = 41;
				else if(state == 0) {
					state = 41;
					lastpos = position;
					start = position;
				}
				else if(state == 30 || state == 75)
					state = 75;
				else if(state == 73)
					state = 75;
				else if(state == 41 || state == 82 || state == 81)
					state == 82;
				else if(state == 34)
					state = 79;
				else if(isComingFromL(state))
					state = 41;
				else state = -1;
				break;
			case 'N':
				if(state == 0){
					state = 12;
					lastpos = position;
					start = position;
				}
				else if(state == 16)
					state = 64;
				else if(isComingFromL(state))
					state = 41;
				else if(state == 0) {
					state = 41;
					lastpos = position;
					start = position;
				}
				else if(state == 30 || state == 75)
					state = 75;
				else if(state == 73)
					state = 75;
				else if(state == 41 || state == 82 || state == 81)
					state == 82;
				else if(state == 34)
					state = 79;
				else if(isComingFromL(state))
					state = 41;
				else state = -1;
				break;
			case 'D':
				if(state == 64)
					state = 105;
				else if(isComingFromL(state))
					state = 41;
				else if(state == 0) {
					state = 41;
					lastpos = position;
					start = position;
				}
				else if(state == 30 || state == 75)
					state = 75;
				else if(state == 73)
					state = 75;
				else if(state == 41 || state == 82 || state == 81)
					state == 82;
				else if(state == 34)
					state = 79;
				else if(isComingFromL(state))
					state = 41;
				else state = -1;
				break;
			case 'A':
				if(state == 0){
					state = 16;
					lastpos = position;
					start = position;
				}
				else if(isComingFromL(state))
					state = 41;
				else if(state == 0) {
					state = 41;
					lastpos = position;
					start = position;
				}
				else if(state == 30 || state == 75)
					state = 75;
				else if(state == 73)
					state = 75;
				else if(state == 41 || state == 82 || state == 81)
					state == 82;
				else if(state == 34)
					state = 79;
				else if(isComingFromL(state))
					state = 41;
				else state = -1;
				break;
			case 'F':
				if(state == 0){
					state = 15;
					lastpos = position;
					start = position;
				}
				else if(state == 30 || state == 75)
					state = 75;
				else if(state == 73)
					state = 75;
				else if(state == 41 || state == 82 || state == 81)
					state == 82;
				else if(state == 34)
					state = 79;
				else if(isComingFromL(state))
					state = 41;
				else state = -1;
				break;
			case 'B':
			case 'C':
			case 'E':
			case 'G':
			case 'H':
			case 'I':
			case 'J':
			case 'K':
			case 'L':
			case 'M':
			case 'P':
			case 'Q':
			case 'S':
			case 'U':
			case 'V':
			case 'W':
			case 'X':
			case 'Y':
			case 'Z':
				if(state == 0) {
					state = 41;
					lastpos = position;
					start = position;
				}
				else if(state == 30 || state == 75)
					state = 75;
				else if(state == 73)
					state = 75;
				else if(state == 41 || state == 82 || state == 81)
					state == 82;
				else if(state == 34)
					state = 79;
				else if(isComingFromL(state))
					state = 41;
				else state = -1;
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if(state == 0) {
					state = 17;
					lastpos = position;
					start = position;
				}
				else if(state == 41 || state == 81 || state == 82)
					state = 81;
				else if(state == 30 || state == 73 || state == 75)
					state = 73;
				else if(state == 17)
					state = 17;
				else if(state == 65 || state == 76 || state == 31)
					state = 76;
				else if(isComingFromL(state))
					state = 81;
				else state = -1;
				break;
			case '@':
				if(state == 0) {
					state = 19;
					lastpos = position;
					start = position;
				}
				else state = -1;
				break;
			case '"':
				if(state == 0) {
					state = 30;
					lastpos = position;
					start = position;
				}
				else if(state == 30 || state == 73 || state == 75)
					state = 74;
				else state = -1;
				break;
			case '\'':
				if(state == 0){
					state = 34;
					lastpos = position;
					start = position;
				}
				else if(state == 79)
					state = 110;
				else state = -1;
				break;
			case ':':
				if(state == 0){
					state = 20;
					lastpos = position;
					start = position;
				}
				else state = -1;
				break;
			case '(':
				if(state == 0){
					state = 21;
					lastpos = position;
					start = position;
				}
				else state = -1;
				break;
			case ')':
				if(state == 0){
					state = 22;
					lastpos = position;
					start = position;
				}
				else state = -1;
				break;
			case '-':
				if(state == 0){
					state = 23;
					lastpos = position;
					start = position;
				}
				else state = -1;
				break;
			case '>':
				if(state == 0){
					state = 24;
					lastpos = position;
					start = position;
				}
				else if(state == 23)
					state = 70;
				else state = -1;
				break;
			case '=':
				if(state == 0){
					state = 32;
					lastpos = position;
					start = position;
				}
				else if(state == 24)
					state = 71;
				else if(state == 27)
					state = 72;
				else if(state == 32)
					state = 77;
				else if(state == 33)
					state = 78;
				else state = -1;
				break;
			case ',':
				if(state == 0){
					state = 25;
					lastpos = position;
					start = position;
				}
				else state = -1;
				break;
			case ';':
				if(state == 0){
					state = 26;
					lastpos = position;
					start = position;
				}
				else state = -1;
				break;
			case '<':
				if(state == 0){
					state = 27;
					lastpos = position;
					start = position;
				}
				else state = -1;
				break;
			case '[':
				if(state == 0){
					state = 28;
					lastpos = position;
					start = position;
				}
				else state = -1;
				break;
			case ']':
				if(state == 0){
					state = 29;
					lastpos = position;
					start = position;
				}
				else state = -1;
				break;
			case '!':
				if(state == 0){
					state = 33;
					lastpos = position;
					start = position;
				}
				else state = -1;
				break;
			case '+':
				if(state == 0){
					state = 35;
					lastpos = position;
					start = position;
				}
				else state = -1;
				break;
			case '*':
				if(state == 0){
					state = 36;
					lastpos = position;
					start = position;
				}
				else if(state == 36)
					state = 80;
				else state = -1;
				break;
			case '/':
				if(state == 0){
					state = 37;
					lastpos = position;
					start = position;
				}
				else state = -1;
				break;
			case '%':
				if(state == 0){
					state = 38;
					lastpos = position;
					start = position;
				}
				else state = -1;
				break;
			case '{':
				if(state == 0){
					state = 39;
					lastpos = position;
					start = position;
				}
				else state = -1;
				break;
			case '}':
				if(state == 0){
					state = 40;
					lastpos = position;
					start = position;
				}
				else state = -1;
				break;
			default:
				// printf("Error: %c ", c);
				state = 0;
				break;
		}
		if(state == -1) {
			// printf("Error: %c ", c);
			if(lastpos != initPos) {
				if(isAccepted) {
					if(stateIsFinal(acceptState)) {
						// printf("%dPosition\n", position);
						// printf("\n%c",B[lastpos]);
						// printf("%c",B[position]);
						acceptToken(acceptState, start, position, B);

						isAccepted = 0;
					}
				}
				else {
					printf("Error1 ");
					sleep(1);
				}
				position = lastpos;
			}
			// position = lastpos + 1;
			state = 0;
		}
	}
}

void lexAnalysis(FILE *src) {

	Buff upperBuff = (Buff) malloc(BUFFER_SIZE * sizeof(char));
	Buff lowerBuff = (Buff) malloc(BUFFER_SIZE * sizeof(char));
	Buff lexemeBuff = (Buff) malloc(BUFFER_SIZE * sizeof(char));
	currentBuff = upperBuff;
	printf("\n____________RUNNING LEXER ... ________________\n");

	if (readToBuff(src, upperBuff, BUFFER_SIZE) != NULL)
	{
		int errors = 0, tokens = 0, retval = 0;
		setvbuf (stdout, NULL, _IONBF, 0);
		// state = 0;
		// retval = getTokens(currentBuff, 0);
		// printf("\n");
		do{
			printf("%d ", lineNo+1);
			state = 0;
			retval = getTokens(currentBuff, retval);
	printf("\n");
			// printf("%d\n", lineNo);
			// sleep(1);
		}while(currentBuff[retval] != EOF);
	}
	printf("\n____________LEX_SUCCESSFUL____________________\n");
	// printf("\n____________LIST_OF_IDs_______________________\n");
	// int i, j;
	// for (i = 0; i < lineNo; ++i) {
	// 	if(varUsageCount [i]>0)
	// 	printf("%d ", i);
	// 	for (j = 0; j < varUsageCount[i]; ++j) {
	// 		printf("%s ", varUsage[i][j]);
	// 	}
	// 	if(varUsageCount [i]>0)
	// 	printf("\n");
	// }
	// printf("\n______________________________________________\n");
	// printf("\n____________LIST_OF_OPs_______________________\n");
	// for (i = 0; i < lineNo; ++i) {
	// 	if(opUsageCount [i]>0)
	// 	printf("%d ", i);
	// 	for (j = 0; j < opUsageCount[i]; ++j) {
	// 		printf("%s ", opUsage[i][j]);
	// 	}
	// 	if(opUsageCount [i]>0)
	// 	printf("\n");
	// }
	// printf("\n______________________________________________\n");
	// printf("\n____________LIST_OF_OP_IDs____________________\n");
	// for (i = 0; i < lineNo; ++i) {
	// 	if(opIDUsageCount [i]>0)
	// 	printf("%d ", i);
	// 	for (j = 0; j < opIDUsageCount[i]; ++j) {
	// 		printf("%s ", opIDUsage[i][j]);
	// 	}
	// 	if(opIDUsageCount [i]>0)
	// 	printf("\n");
	// }
	printf("\n______________________________________________\n");
	// int i = 0;
	// printf("\n");
	// for (i = 0; i < tokenIndex; ++i)
	// {
	// 	printf("%s\n", tokenList[i]);
	// }
}
