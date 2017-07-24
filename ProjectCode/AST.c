/* Group - 20
* Yash Sinha 2012C6PS365P
* Krishna Garg 2012A7PS033P
*/
#include <stdio.h>
#include <string.h>
#define BUFFER_SIZE 10 * 1024
#define MAX_LINE_SIZE 10 * 1024
#define MAX_SIZE 15
#define DEBUG 0

struct node {
	char *name;
	char *lexeme;
	int lineNum;
	int childCount;
	int ruleNum;
	struct node* children[MAX_SIZE]; // 15 children pointers
};

struct tree {
	struct node* root;
	int nodeCount;
};

extern struct tree* parseTree;

struct AST {
	struct ASTNode* root;
	int nodeCount;
};

struct AST* ASTtree;

struct ASTNode {
	char *type;
	char *name;
	int childCount;
	int lineNum;
	struct node* pointerToParseTree;
	struct ASTNode* stmtArray[10];
	int stmtArrayCount;
	struct ASTNode* children[MAX_SIZE]; // 15 children pointers
};

struct ASTNode* createASTNode(char* name, char *type) {
	struct ASTNode* n = (struct ASTNode*) malloc(sizeof(struct ASTNode));
	n->type = type;
	n->name = name;
	n->childCount = 0;
	n->lineNum = 0;
	return n;
}

struct AST* createNewAST() {
	struct AST* t = (struct AST*) malloc(sizeof(struct AST));
	t->nodeCount = 1;
	t->root = createASTNode("start","START");
	return t;
}

void printAST(struct ASTNode* n) {
	int i;
	printf(" %s\n", n->name);
	for (i = 0; i < n->childCount; ++i) {
	  printAST(n->children[i]);
	}
	printf("END ____________%s, %d\n",n->name, n->lineNum );
}

struct node *findNTNode(struct node *n, char *str) {
	int i;
	if(strcmp(n->name,str) == 0){
		// printf("%s\n", n->name);
		return n;
	}
	for (i = 0; i < n->childCount; ++i)
	{
		struct node *node = findNTNode(n->children[i],str);
		if(node != NULL)
			return node;
	}
	return NULL; 
}

struct ASTNode* recordArray[10];
int recordArrayCount = 0;

void printrecordArray(struct ASTNode* recordArray[]) {
	printf("RECORDS AST\n======================================\n");
	printf("%d\n", recordArrayCount);
	int i;
	for (i = 0; i < recordArrayCount; ++i){
		printf("%s %d\n", recordArray[i]->name,recordArray[i]->childCount);
	}
}

struct ASTNode* executeArray[1024];
int executeArrayCount = 0;

void printexecuteArray(struct ASTNode* executeArray[]) {
	printf("EXECUTE AST\n======================================\n");
	printf("%d\n", executeArrayCount);
	int i, j, k, l;
	for (i = 0; i < executeArrayCount; ++i){
		printf("    %s %d\n",
		executeArray[i]->name,
		executeArray[i]->childCount);
		for (j = 0; j < executeArray[i]->stmtArrayCount; ++j){
			printf("        %s %d\n",
			executeArray[i]->stmtArray[j]->type,
			executeArray[i]->stmtArray[j]->stmtArrayCount);
			for (k = 0; k < executeArray[i]->stmtArray[j]->stmtArrayCount; ++k){
				printf("            %s %d\n",
				executeArray[i]->stmtArray[j]->stmtArray[k]->type,
				executeArray[i]->stmtArray[j]->stmtArray[k]->stmtArrayCount);
				for (l = 0; l < executeArray[i]->stmtArray[j]->stmtArray[k]->stmtArrayCount; ++l){
					printf("                %s %d\n",
					executeArray[i]->stmtArray[j]->stmtArray[k]->stmtArray[l]->type,
					executeArray[i]->stmtArray[j]->stmtArray[k]->stmtArray[l]->stmtArrayCount);
				}
			}
		}
		printf("\n");
	}
}

struct ASTNode* procArray[10];
int procArrayCount = 0;

void printprocArray(struct ASTNode* procArray[]) {
	printf("PROCEDURES AST\n======================================\n");
	printf("%d\n", procArrayCount);
	int i, j;
	for (i = 0; i < procArrayCount; ++i){
		printf("%s %d\n", procArray[i]->name,procArray[i]->childCount);
		for (j = 0; j < procArray[i]->stmtArrayCount; ++j){
			printf("    %s %d\n", procArray[i]->stmtArray[j]->name,procArray[i]->stmtArray[j]->childCount);
		}
	}
}

struct node* copyNode(struct node *sourceRec, char* type){
	struct ASTNode *destRec = (struct ASTNode*) malloc(sizeof(struct ASTNode));
	(destRec)->name = (sourceRec)->name;
	(destRec)->childCount = (sourceRec)->childCount;
	(destRec)->pointerToParseTree = (sourceRec);
	(destRec)->type = type;
	(destRec)->lineNum = sourceRec->lineNum - 1;

	int i;
	for(i = 0;i < sourceRec->childCount; i++){
		destRec->children[i] = sourceRec->children[i];
	} 
	return destRec;
}

struct node *reduceRecord(struct node *n) {
	if(n == NULL)
		return NULL;
	struct node* recs = findNTNode(n,"recs");
	if(recs == NULL)
		return NULL;
	recordArray[recordArrayCount] = copyNode(recs->children[0], "rec");
	recordArrayCount++;

	while(recs->children[1]->childCount == 1) {
		recs = findNTNode(recs->children[1],"recs");
		recordArray[recordArrayCount] = copyNode(recs->children[0], "rec");
		// printf("%s %d\n", recordArray[recordArrayCount]->name,recordArray[recordArrayCount]->childCount);
		recordArrayCount++;
	}
	return NULL;
}

void getDeclStmt(struct node* rec, struct ASTNode* decl_stmtArray[], int* decl_stmtArrayCount) {
	if(rec == NULL)
		return NULL;
	struct node* decl_stmts = findNTNode(rec,"decl_stmts");
	decl_stmtArray[*decl_stmtArrayCount] = copyNode(decl_stmts->children[0], "decl_stmt");
	// printf("%s %d\n", decl_stmtArray[*decl_stmtArrayCount]->name,decl_stmtArray[*decl_stmtArrayCount]->childCount);
	*decl_stmtArrayCount += 1;

	while(decl_stmts->children[1]->childCount == 1) {
		decl_stmts = findNTNode(decl_stmts->children[1],"decl_stmts");
		decl_stmtArray[*decl_stmtArrayCount] = copyNode(decl_stmts->children[0], "decl_stmt");
		// printf("%s %d\n", decl_stmtArray[*decl_stmtArrayCount]->name,decl_stmtArray[*decl_stmtArrayCount]->childCount);
		*decl_stmtArrayCount += 1;
	}
	return NULL;
}

void processDeclStmt(struct node* decl_stmt, struct ASTNode* stmtArray[], int* stmtArrayCount, int flag) {
	//stmt
	if(decl_stmt == NULL)
		return NULL;
	if(flag)
		decl_stmt = findNTNode(decl_stmt,"decl_stmt");

	struct node* a = decl_stmt->children[0]->children[0];
	char* datatype;
	if(strcmp(a->name, "basic_type") == 0) {
		datatype = a->children[0]->name;
	}
	else {
		datatype = a->name;
	}
	struct node* id_list = findNTNode(decl_stmt,"id_list");
	stmtArray[*stmtArrayCount] = copyNode(id_list, datatype);
	if(DEBUG) printf("%s %d\n", stmtArray[*stmtArrayCount]->name,stmtArray[*stmtArrayCount]->childCount);
	*stmtArrayCount += 1;

	while(id_list->children[1]->childCount == 2) {
		id_list = findNTNode(id_list->children[1],"id_list");
		stmtArray[*stmtArrayCount] = copyNode(id_list, datatype);
		if(DEBUG) printf("%s %d\n", stmtArray[*stmtArrayCount]->name,stmtArray[*stmtArrayCount]->childCount);
		*stmtArrayCount += 1;
	}
	return NULL;
}

void processLoopStmt(struct node* loop_stmt, struct ASTNode* stmtArray[], int* stmtArrayCount) {
	//stmt
	if(loop_stmt == NULL)
		return NULL;

	struct node* id_list = findNTNode(loop_stmt,"loop_stmt");
	stmtArray[*stmtArrayCount] = copyNode(id_list, "loop_stmt");
	if(DEBUG) printf("%s %d\n", stmtArray[*stmtArrayCount]->name,stmtArray[*stmtArrayCount]->childCount);
	*stmtArrayCount += 1;

	struct node* iloop_stmts = loop_stmt->children[0]->children[11];//iloop_stmts
	struct node* stmt = iloop_stmts->children[0];//iloop_stmt
	char* a = stmt->children[0]->children[0]->name;
	processStmts(stmt, a, stmtArray[*stmtArrayCount - 1]->stmtArray, &(stmtArray[*stmtArrayCount - 1]->stmtArrayCount));
	stmtArray[*stmtArrayCount - 1]->stmtArrayCount += 1;

	while(iloop_stmts->children[1]->childCount == 2) {
		iloop_stmts = iloop_stmts->children[1];
		stmt = iloop_stmts->children[0];
		char* a = stmt->children[0]->children[0]->name;
		processStmts(stmt, a, stmtArray[*stmtArrayCount - 1]->stmtArray, &(stmtArray[*stmtArrayCount - 1]->stmtArrayCount));
		stmtArray[*stmtArrayCount - 1]->stmtArrayCount += 1;
	}

	return NULL;
}

void processCondStmt(struct node* cond_stmt, struct ASTNode* stmtArray[], int* stmtArrayCount) {
	//stmt
	if(cond_stmt == NULL)
		return NULL;

	struct node* id_list = findNTNode(cond_stmt,"cond_stmt");
	stmtArray[*stmtArrayCount] = copyNode(id_list, "cond_stmt");
	if(DEBUG) printf("%s %d\n", stmtArray[*stmtArrayCount]->name,stmtArray[*stmtArrayCount]->childCount);
	*stmtArrayCount += 1;

	// stmtArray[*stmtArrayCount - 1] = (struct ASTNode*) malloc(sizeof(struct ASTNode));

	//Add if
	processStmts(
		cond_stmt,
		"if_stmt",
		stmtArray[*stmtArrayCount - 1]->stmtArray,
		&(stmtArray[*stmtArrayCount - 1]->stmtArrayCount));
	stmtArray[*stmtArrayCount - 1]->stmtArrayCount += 1;

	// Add elif
	struct node* elif_stmt = cond_stmt->children[0]->children[6];//elif_stmt
	// printf("SENDING TO ELIF %s\n", elif_stmt->name);
	while(elif_stmt->childCount != 0) {
		processStmts(elif_stmt, "elif_stmt", stmtArray[*stmtArrayCount - 1]->stmtArray, &(stmtArray[*stmtArrayCount - 1]->stmtArrayCount));
		stmtArray[*stmtArrayCount - 1]->stmtArrayCount += 1;
		elif_stmt = elif_stmt->children[6];//elif_stmt
	}

	// Add else
	if(cond_stmt->children[0]->children[7]->childCount == 4) {
		processStmts(cond_stmt->children[0]->children[7], "else_stmt", stmtArray[*stmtArrayCount - 1]->stmtArray, &(stmtArray[*stmtArrayCount - 1]->stmtArrayCount));
		stmtArray[*stmtArrayCount - 1]->stmtArrayCount += 1;
	}

	return NULL;
}

void processIfStmt(struct node* if_elif_stmt, struct ASTNode* stmtArray[], int* stmtArrayCount) {
	//cond_stmt
	if(if_elif_stmt == NULL)
		return NULL;
	stmtArray[*stmtArrayCount] = copyNode(if_elif_stmt, if_elif_stmt->name);
	if(DEBUG) printf("%s %d\n", stmtArray[*stmtArrayCount]->name,stmtArray[*stmtArrayCount]->childCount);
	*stmtArrayCount += 1;

	struct node* stmts = if_elif_stmt->children[5];//stmts
	char* a = stmts->children[0]->children[0]->name;
	// printf("........................ %s\n", stmts->name);
	processStmts(stmts, a, stmtArray[*stmtArrayCount - 1]->stmtArray, &(stmtArray[*stmtArrayCount - 1]->stmtArrayCount));
	stmtArray[*stmtArrayCount - 1]->stmtArrayCount += 1;

	while(stmts->children[1]->childCount == 1) {
		stmts = stmts->children[1]->children[0];
		char* a = stmts->children[0]->children[0]->name;
		// printf("........................ %s\n", stmts->name);
		processStmts(stmts, a, stmtArray[*stmtArrayCount - 1]->stmtArray, &(stmtArray[*stmtArrayCount - 1]->stmtArrayCount));
		stmtArray[*stmtArrayCount - 1]->stmtArrayCount += 1;
	}

	return NULL;
}

void processElifStmt(struct node* if_elif_stmt, struct ASTNode* stmtArray[], int* stmtArrayCount) {
	//else_if
	if(if_elif_stmt == NULL)
		return NULL;
	stmtArray[*stmtArrayCount] = copyNode(if_elif_stmt, if_elif_stmt->name);
	if(DEBUG) printf("%s %d\n", stmtArray[*stmtArrayCount]->name,stmtArray[*stmtArrayCount]->childCount);
	*stmtArrayCount += 1;

	struct node* stmts = if_elif_stmt->children[5];//stmts
	char* a = stmts->children[0]->children[0]->name;
	// printf("........................ %s\n", stmts->name);
	processStmts(stmts, a, stmtArray[*stmtArrayCount - 1]->stmtArray, &(stmtArray[*stmtArrayCount - 1]->stmtArrayCount));
	stmtArray[*stmtArrayCount - 1]->stmtArrayCount += 1;

	while(stmts->children[1]->childCount == 1) {
		stmts = stmts->children[1]->children[0];
		char* a = stmts->children[0]->children[0]->name;
		// printf("........................ %s\n", stmts->name);
		processStmts(stmts, a, stmtArray[*stmtArrayCount - 1]->stmtArray, &(stmtArray[*stmtArrayCount - 1]->stmtArrayCount));
		stmtArray[*stmtArrayCount - 1]->stmtArrayCount += 1;
	}

	return NULL;
}

void processElseStmt(struct node* if_elif_stmt, struct ASTNode* stmtArray[], int* stmtArrayCount) {
	//_cond_stmt
	if(if_elif_stmt == NULL)
		return NULL;
	stmtArray[*stmtArrayCount] = copyNode(if_elif_stmt, if_elif_stmt->name);
	if(DEBUG) printf("%s %d\n", stmtArray[*stmtArrayCount]->name,stmtArray[*stmtArrayCount]->childCount);
	*stmtArrayCount += 1;

	struct node* stmts = if_elif_stmt->children[2];//stmts
	char* a = stmts->children[0]->children[0]->name;
	// printf("........................ %s\n", stmts->name);
	processStmts(stmts, a, stmtArray[*stmtArrayCount - 1]->stmtArray, &(stmtArray[*stmtArrayCount - 1]->stmtArrayCount));
	stmtArray[*stmtArrayCount - 1]->stmtArrayCount += 1;

	while(stmts->children[1]->childCount == 1) {
		stmts = stmts->children[1]->children[0];
		char* a = stmts->children[0]->children[0]->name;
		// printf("........................ %s\n", stmts->name);
		processStmts(stmts, a, stmtArray[*stmtArrayCount - 1]->stmtArray, &(stmtArray[*stmtArrayCount - 1]->stmtArrayCount));
		stmtArray[*stmtArrayCount - 1]->stmtArrayCount += 1;
	}

	return NULL;
}

void processProcCallStmt(struct node* proc_call_stmt, struct ASTNode* stmtArray[], int* stmtArrayCount) {
	//stmt
	if(proc_call_stmt == NULL)
		return NULL;

	struct node* id_list = findNTNode(proc_call_stmt,"proc_call_stmt");
	stmtArray[*stmtArrayCount] = copyNode(id_list, "proc_call_stmt");
	if(DEBUG) printf("%s %d\n", stmtArray[*stmtArrayCount]->name,stmtArray[*stmtArrayCount]->childCount);
	*stmtArrayCount += 1;

	return NULL;
}

void processAssignStmt(struct node* assign_stmt, struct ASTNode* stmtArray[], int* stmtArrayCount) {
	//stmt
	if(assign_stmt == NULL)
		return NULL;
	struct node* id_list = findNTNode(assign_stmt,"assign_stmt");

	stmtArray[*stmtArrayCount] = copyNode(id_list, "assign_stmt");
	if(DEBUG) printf("%s %d\n", stmtArray[*stmtArrayCount]->name,stmtArray[*stmtArrayCount]->childCount);
	*stmtArrayCount += 1;

	return NULL;
}

void processIoStmt(struct node* io_stmt, struct ASTNode* stmtArray[], int* stmtArrayCount) {
	//stmt
	if(io_stmt == NULL)
		return NULL;
	struct node* id_list = findNTNode(io_stmt,"io_stmt");
	stmtArray[*stmtArrayCount] = copyNode(id_list, "io_stmt");
	if(DEBUG) printf("%s %d\n", stmtArray[*stmtArrayCount]->name,stmtArray[*stmtArrayCount]->childCount);
	*stmtArrayCount += 1;

	return NULL;
}

void processStrAssignStmt(struct node* str_assign_stmt, struct ASTNode* stmtArray[], int* stmtArrayCount) {
	//stmt
	if(str_assign_stmt == NULL)
		return NULL;
	struct node* id_list = findNTNode(str_assign_stmt,"str_assign_stmt");
	stmtArray[*stmtArrayCount] = copyNode(id_list, "str_assign_stmt");
	if(DEBUG) printf("%s %d\n", stmtArray[*stmtArrayCount]->name,stmtArray[*stmtArrayCount]->childCount);
	*stmtArrayCount += 1;

	// while(id_list->children[1]->childCount == 2) {
	// 	id_list = findNTNode(id_list->children[1],"id_list");
	// 	stmtArray[*stmtArrayCount] = copyNode(id_list, datatype);
	// 	if(DEBUG) printf("%s %d\n", stmtArray[*stmtArrayCount]->name,stmtArray[*stmtArrayCount]->childCount);
	// 	*stmtArrayCount += 1;
	// }
	return NULL;
}

void processMatAssignStmt(struct node* mat_assign_stmt, struct ASTNode* stmtArray[], int* stmtArrayCount) {
	//stmt
	if(mat_assign_stmt == NULL)
		return NULL;
	struct node* id_list = findNTNode(mat_assign_stmt,"mat_assign_stmt");
	stmtArray[*stmtArrayCount] = copyNode(id_list, "mat_assign_stmt");
	if(DEBUG) printf("%s %d\n", stmtArray[*stmtArrayCount]->name,stmtArray[*stmtArrayCount]->childCount);
	*stmtArrayCount += 1;

	return NULL;
}

void processProcStmt(struct node* proc, struct ASTNode* stmtArray[], int* stmtArrayCount) {
	//stmt
	if(proc == NULL)
		return NULL;
	// struct node* id_list = findNTNode(proc,"proc");
	// stmtArray[*stmtArrayCount] = copyNode(id_list, "proc");
	// if(DEBUG) printf("%s %d\n", stmtArray[*stmtArrayCount]->name,stmtArray[*stmtArrayCount]->childCount);
	// *stmtArrayCount += 1;

	struct node* stmts = proc->children[10];//stmts
	char* a = stmts->children[0]->children[0]->name;
	// printf("........................ %s\n", stmts->name);
	processStmts(stmts, a, stmtArray, (stmtArrayCount));
	*stmtArrayCount += 1;

	while(stmts->children[1]->childCount == 1) {
		stmts = stmts->children[1]->children[0];
		char* a = stmts->children[0]->children[0]->name;
		// printf("........................ %s\n", stmts->name);
		processStmts(stmts, a, stmtArray, (stmtArrayCount));
		*stmtArrayCount += 1;
	}

	return NULL;
}

void processStmts(struct node* stmt, char* a, struct ASTNode* stmtArray[], int* stmtArrayCount) {
	//stmt
	// printf("Inside processStmts %s, %s\n", stmt->name, a);
	if(strcmp(a, "decl_stmt") == 0) {
		stmtArray[*stmtArrayCount] = copyNode(stmt->children[0], "decl_stmt");
		processDeclStmt(stmtArray[*stmtArrayCount]->pointerToParseTree, stmtArray[*stmtArrayCount]->stmtArray,
				&stmtArray[*stmtArrayCount]->stmtArrayCount, 1);
	}
	else if(strcmp(a, "cond_stmt") == 0) {
		stmtArray[*stmtArrayCount] = copyNode(stmt->children[0], "cond_stmt");
		processCondStmt(stmtArray[*stmtArrayCount]->pointerToParseTree, stmtArray[*stmtArrayCount]->stmtArray,
				&stmtArray[*stmtArrayCount]->stmtArrayCount);
	}
	else if(strcmp(a, "loop_stmt") == 0) {
		stmtArray[*stmtArrayCount] = copyNode(stmt->children[0], "loop_stmt");
		processLoopStmt(stmtArray[*stmtArrayCount]->pointerToParseTree, stmtArray[*stmtArrayCount]->stmtArray,
				&stmtArray[*stmtArrayCount]->stmtArrayCount);
	}
	else if(strcmp(a, "if_stmt") == 0) {
		stmtArray[*stmtArrayCount] = copyNode(stmt->children[0], "if_elif_stmt");
		processIfStmt(stmtArray[*stmtArrayCount]->pointerToParseTree, stmtArray[*stmtArrayCount]->stmtArray,
				&stmtArray[*stmtArrayCount]->stmtArrayCount);
	}
	else if(strcmp(a, "elif_stmt") == 0) {
		stmtArray[*stmtArrayCount] = copyNode(stmt, "if_elif_stmt");
		processElifStmt(stmtArray[*stmtArrayCount]->pointerToParseTree, stmtArray[*stmtArrayCount]->stmtArray,
				&stmtArray[*stmtArrayCount]->stmtArrayCount);
	}
	else if(strcmp(a, "else_stmt") == 0) {
		stmtArray[*stmtArrayCount] = copyNode(stmt, "if_elif_stmt");
		processElseStmt(stmtArray[*stmtArrayCount]->pointerToParseTree, stmtArray[*stmtArrayCount]->stmtArray,
				&stmtArray[*stmtArrayCount]->stmtArrayCount);
	}
	else if(strcmp(a, "proc_call_stmt") == 0) {
		stmtArray[*stmtArrayCount] = copyNode(stmt->children[0], "proc_call_stmt");
		processProcCallStmt(stmtArray[*stmtArrayCount]->pointerToParseTree, stmtArray[*stmtArrayCount]->stmtArray,
				&stmtArray[*stmtArrayCount]->stmtArrayCount);
	}
	else if(strcmp(a, "assign_stmt") == 0) {
		stmtArray[*stmtArrayCount] = copyNode(stmt->children[0], "assign_stmt");
		processAssignStmt(stmtArray[*stmtArrayCount]->pointerToParseTree, stmtArray[*stmtArrayCount]->stmtArray,
				&stmtArray[*stmtArrayCount]->stmtArrayCount);
	}
	else if(strcmp(a, "io_stmt") == 0) {
		stmtArray[*stmtArrayCount] = copyNode(stmt->children[0], "io_stmt");
		processIoStmt(stmtArray[*stmtArrayCount]->pointerToParseTree, stmtArray[*stmtArrayCount]->stmtArray,
				&stmtArray[*stmtArrayCount]->stmtArrayCount);
	}
	else if(strcmp(a, "mat_assign_stmt") == 0) {
		stmtArray[*stmtArrayCount] = copyNode(stmt->children[0], "mat_assign_stmt");
		processMatAssignStmt(stmtArray[*stmtArrayCount]->pointerToParseTree, stmtArray[*stmtArrayCount]->stmtArray,
				&stmtArray[*stmtArrayCount]->stmtArrayCount);
	}
	else if(strcmp(a, "str_assign_stmt") == 0) {
		stmtArray[*stmtArrayCount] = copyNode(stmt->children[0], "str_assign_stmt");
		processStrAssignStmt(stmtArray[*stmtArrayCount]->pointerToParseTree, stmtArray[*stmtArrayCount]->stmtArray,
				&stmtArray[*stmtArrayCount]->stmtArrayCount);
	}
	else {
		printf("No such <stmt> ERROR\n====================\n");
		exit(1);
	}
}

struct node *reduceExecute(struct node *n) {
	if(n == NULL)
		return NULL;
	struct node* stmt = findNTNode(n,"stmts");
	char* a = stmt->children[0]->children[0]->name;
	processStmts(stmt, a, executeArray, &executeArrayCount);
	// printf("%s %d\n", executeArray[executeArrayCount]->name,executeArray[executeArrayCount]->childCount);
	executeArrayCount++;

	while(stmt->children[1]->childCount == 1) {
		stmt = findNTNode(stmt->children[1],"stmts");
		char* a = stmt->children[0]->children[0]->name;
		processStmts(stmt, a, executeArray, &executeArrayCount);
		// printf("%s %d\n", executeArray[executeArrayCount]->name,executeArray[executeArrayCount]->childCount);
		executeArrayCount++;
	}
	return NULL;
}

struct node *reduceProc(struct node *n) {
	if(n == NULL)
		return NULL;
	struct node* procs = findNTNode(n,"procs");
	if(procs == NULL)
		return NULL;
	procArray[procArrayCount] = copyNode(procs->children[0], "proc");
	procArrayCount++;

	while(procs->children[1]->childCount == 1) {
		procs = findNTNode(procs->children[1],"procs");
		procArray[procArrayCount] = copyNode(procs->children[0], "proc");
		// printf("%s %d\n", procArray[procArrayCount]->name,procArray[procArrayCount]->childCount);
		procArrayCount++;
	}
	return NULL;
}

void makeAST(struct node *parseTreeRoot){
	printf("\n____________START_AST_________________________\n");
	struct node *executeNode = findNTNode(parseTreeRoot,"execute");
	// printf("%s\n", executeNode->name);

	struct node *procsNode = findNTNode(parseTreeRoot,"_procedures");
	// printf("%s\n", procsNode->name);
	
	struct node *recordsNode = findNTNode(parseTreeRoot,"_records");
	// printf("%s\n", recordsNode->name);

	reduceRecord(recordsNode);
	// printrecordArray(&recordArray);
	int i;
	for(i = 0;i < recordArrayCount; i++){
		recordArray[i]->stmtArrayCount = 0;
		getDeclStmt(recordArray[i]->pointerToParseTree, recordArray[i]->stmtArray, &recordArray[i]->stmtArrayCount);
		// printf("%d\n", recordArray[i]->stmtArrayCount);
		int j;
		for(j = 0;j < recordArray[i]->stmtArrayCount; j++){
			processDeclStmt(recordArray[i]->stmtArray[j]->pointerToParseTree, recordArray[i]->stmtArray[j]->stmtArray,
				&recordArray[i]->stmtArray[j]->stmtArrayCount, 0);
			// printf("\n");
		}
	}

	reduceProc(procsNode);
	for(i = 0;i < procArrayCount; i++){
		procArray[i]->stmtArrayCount = 0;
		processProcStmt(procArray[i]->pointerToParseTree, procArray[i]->stmtArray, &procArray[i]->stmtArrayCount);
		// printf("%d\n", procArray[i]->stmtArrayCount);
		// int j;
		// for(j = 0;j < procArray[i]->stmtArrayCount; j++){
		// 	processDeclStmt(procArray[i]->stmtArray[j]->pointerToParseTree, procArray[i]->stmtArray[j]->stmtArray,
		// 		&procArray[i]->stmtArray[j]->stmtArrayCount, 0);
		// 	// printf("\n");
		// }
	}

	reduceExecute(executeNode);
	printprocArray(&procArray);
	printrecordArray(&recordArray);
	printexecuteArray(&executeArray);
	populateSymbolTable(executeArray);
	typeCheck(recordsNode, executeNode, procsNode);
}