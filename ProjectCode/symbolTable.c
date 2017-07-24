/* Group - 20
* Yash Sinha 2012C6PS365P
* Krishna Garg 2012A7PS033P
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 15

int scope_line_info[200];
int varUsageCount[100];
char* varUsage[100][100];
extern int lineNo;
int tableSz = 100, totElements = 0, subHash = 0, subRec = 0, totRecords = 0, totProcList = 0, subProcList = 0, totRecList = 0, subRecList = 0;
struct SymbolTable *hashBucket = NULL;
struct SymbolTableRecord *recordBucket = NULL;
struct SymbolTableProcList *procListBucket = NULL;
struct SymbolTableRecList *recListBucket = NULL;

struct SymbolTableProcList {
    int lineNum, key;
    char name[100];
    int marker;
};
struct SymbolTableRecList {
    int lineNum, key;
    char name[100];
    int marker;
};
struct SymbolTableRecord {
    int lineNum, key;
    char recName[100];
    char name[100];
    char datatype[20];
    int marker;
    void *pointer;
};

struct SymbolTable {
    int lineNum, key;
    char name[100];
    char datatype[20];
    int marker;
    int scope;
    int block;
    void *pointer;
};

struct node {
    char *name;
    char *lexeme;
    int lineNum;
    int childCount;
    int ruleNum;
    struct node* children[MAX_SIZE]; // 15 children pointers
};

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
extern struct ASTNode* procArray[10];
extern int procArrayCount;
extern struct ASTNode* recordArray[10];
extern struct ASTNode* executeArray[1024];
extern int recordArrayCount;
extern int executeArrayCount;
char *idArray[30];
int no_of_id = 0;
int scope = 0;
int block = 0;
void populateRecArray(struct ASTNode *executeArray[], int *executeArrayCount, char *recName){
    int i;
    // printf("Rec: %s\n", executeArray[0]->type);
    for (i = 0; i < *executeArrayCount; i++){
    if(strcmp(executeArray[i]->type,"decl_stmt")==0){
            // printf("Decl Stmt, StmtCount: %d\n",executeArray[i]->stmtArrayCount);
            int j;
            for (j = 0; j < executeArray[i]->stmtArrayCount; j++)
            {
                if(strcmp(executeArray[i]->stmtArray[j]->type,"basic_type"))
                {
                    idArray[no_of_id++] = executeArray[i]->stmtArray[j]->children[0]->name;
                    // printf("%s\n", executeArray[i]->stmtArray[j]->children[0]->name);
                    insertIntoRecordTable(idArray[no_of_id-1],executeArray[i]->stmtArray[j]->type,executeArray[i]->stmtArray[j]->pointerToParseTree->lineNum,NULL,recName);
                }
            }
        }
    }
}
void populateIDArray(struct ASTNode *executeArray[], int *executeArrayCount){
    int i;
    // printf("Exe: %s\n", executeArray[0]->type);
    for (i = 0; i < *executeArrayCount; i++){
        if(strcmp(executeArray[i]->type,"decl_stmt")==0){
            int j;
            for (j = 0; j < executeArray[i]->stmtArrayCount; j++)
            {
                if(strcmp(executeArray[i]->stmtArray[j]->type,"basic_type"))
                {
                    idArray[no_of_id++] = executeArray[i]->stmtArray[j]->children[0]->name;
                    // printf("%s\n", executeArray[i]->stmtArray[j]->children[0]->name);
                    insertIntoHashTable(idArray[no_of_id-1],executeArray[i]->stmtArray[j]->type,executeArray[i]->stmtArray[j]->pointerToParseTree->lineNum,NULL,scope,block);
                }
            }
        }
        else if(strcmp(executeArray[i]->type,"cond_stmt")==0){
            int j;

            for(j = 0; j < executeArray[i]->stmtArray[0]->stmtArrayCount; j++){
                /*loop iterates over if, elif, elif ..., else*/

                if(strcmp(executeArray[i]->stmtArray[0]->stmtArray[j]->stmtArray[0]->type,"cond_stmt") == 0){
                    block++;
                    int count = executeArray[i]->stmtArray[0]->stmtArray[j]->stmtArray[0]->stmtArrayCount;
                    // printf("%d\n", count);
                    /*Search for Decl Stmts*/
                    if(strcmp(executeArray[i]->stmtArray[0]->stmtArray[j]->stmtArray[0]->stmtArray[0]->type,"decl_stmt") == 0){
                    // printf("Decl stmt in IF BLOCK\n");
                    scope ++;
                    populateIDArray(executeArray[i]->stmtArray[0]->stmtArray[j]->stmtArray[0]->stmtArray,&count);
                    scope--;
                    }
                }
                else if(strcmp(executeArray[i]->stmtArray[0]->stmtArray[j]->stmtArray[0]->type,"else_if") == 0){
                    block++;
                    int count = executeArray[i]->stmtArray[0]->stmtArray[j]->stmtArray[0]->stmtArrayCount;
                    if(strcmp(executeArray[i]->stmtArray[0]->stmtArray[j]->stmtArray[0]->stmtArray[0]->type,"decl_stmt") == 0){
                    scope ++;
                    populateIDArray(executeArray[i]->stmtArray[0]->stmtArray[j]->stmtArray[0]->stmtArray,&count);
                    scope--;
                    }

                } 
                else if(strcmp(executeArray[i]->stmtArray[0]->stmtArray[j]->stmtArray[0]->type,"_cond_stmt") == 0){
                    block++;
                    // printf("ELSE Block\n");
                    int count = executeArray[i]->stmtArray[0]->stmtArray[j]->stmtArray[0]->stmtArrayCount;
                    // printf("%d\n", count);
                    /*Search for Decl Stmts*/
                    if(strcmp(executeArray[i]->stmtArray[0]->stmtArray[j]->stmtArray[0]->stmtArray[0]->type,"decl_stmt") == 0){
                    // printf("Decl stmt in ELSE BLOCK\n");
                    scope ++;
                    populateIDArray(executeArray[i]->stmtArray[0]->stmtArray[j]->stmtArray[0]->stmtArray,&count);
                    scope--;
                    }

                }
            }  
        }
        else if(strcmp(executeArray[i]->type,"loop_stmt")==0){
            block++;
            // printf("Loop Stmt, StmtCount: %d, Scope: %d, Block: %d\n",executeArray[i]->stmtArray[0]->stmtArrayCount,scope,block);
            if(strcmp(executeArray[i]->stmtArray[0]->stmtArray[0]->type,"decl_stmt") == 0){
                // printf("Decl stmt in loop stmt\n");
                int count = executeArray[i]->stmtArray[0]->stmtArrayCount;
                scope ++;
                populateIDArray(executeArray[i]->stmtArray[0]->stmtArray,&count);
                scope--;
            }
        }
        else if(strcmp(executeArray[i]->type,"proc_call_stmt")==0){
            // printf("%d\n", executeArray[i]->stmtArray[0]->stmtArrayCount);
            if(!(searchProcList(executeArray[i]->stmtArray[0]->children[1]->name,0))){
               printf("\n\nError: Calling function %s but it is undeclared\n",executeArray[i]->stmtArray[0]->children[1]->name); 
            }
        }
    }
}

void check_declared_before_use(struct ASTNode *executeArray[], int *executeArrayCount){
    printf("\n--------Checking Semantic: Declaration before use -----------\n\n");
    int i, j;
    for (i = 0; i < lineNo; ++i) {
        if(varUsageCount [i]>0)
        for (j = executeArray[0]->lineNum; j < varUsageCount[i]; ++j) {
            int h = getHashIndex(varUsage[i][j]);
            if(h==-1){
                int p = searchProcList(varUsage[i][j],1);
                int r = searchRecList(varUsage[i][j]);
                int rc = getHashIndexRecordBlock(varUsage[i][j]);
                if(p == 1){
                    printf("%s found in Proc Block\n",varUsage[i][j]);
                }
                else if(r == 1){
                    printf("%s found in Rec Block\n",varUsage[i][j]);
                }
                else if(rc != -1){
                    printf("%s found in rec\n", varUsage[i][j]);
                }
 
                else{
                    printf("Error: %s not declared\n",varUsage[i][j]);
                }
            }
            else{
                if(i < hashBucket[h].lineNum - 1)
                    printf("Error: %s not declared\n",varUsage[i][j]);
            }
        }
    }
    printf("\n--------Checking Semantic: Declaration before use: Over-----------\n\n");

}
void insertIntoHashTable(char *name, char *datatype, int lineNum, void *pointer, int scope, int block) {
    /* 1st hashing - finding hash index */
    if (searchData(name,scope,block) == 1){
        printf("Error: variable with same name exists in this scope \n", name);
        return;
    }
    int key = 0;
    int i = 0;
    for(i = 0; i < strlen(name); i++){
        key += name[i++]%100;
    }
    int hashInd = key % tableSz, hashVal;
    /* 2nd hashing - no of buckets to skip while probing */
    hashVal = subHash - (key % subHash);
    if (tableSz == totElements) {
            printf("Can't perform Insertion..Hash Table is full!!");
            return;
    }
    /* linear probing */
    while (hashBucket[hashInd].marker == 1) {
            hashInd = (hashInd + hashVal)%tableSz;
    }
    hashBucket[hashInd].key = key;
    hashBucket[hashInd].lineNum = lineNum;
    hashBucket[hashInd].pointer = pointer;
    hashBucket[hashInd].scope = scope;
    hashBucket[hashInd].block = block;
    strcpy(hashBucket[hashInd].name, name);
    strcpy(hashBucket[hashInd].datatype, datatype);
    hashBucket[hashInd].marker = 1;
    totElements++;
    return;
}
void insertIntoRecordTable(char *name, char *datatype, int lineNum, void *pointer, char *recName) {
    /* 1st hashing - finding hash index */
    if (searchRecord(name) == 1){
        printf("Error: variable with same name exists in this scope \n\n", name);
        return;
    }
    int key = 0;
    int i = 0;
    for(i = 0; i < strlen(name); i++){
        key += name[i++]%100;
    }
    int hashInd = key % tableSz, hashVal;
    /* 2nd hashing - no of buckets to skip while probing */
    hashVal = subRec - (key % subRec);
    if (tableSz == totRecords) {
            printf("Can't perform Insertion..Hash Table is full!!");
            return;
    }
    /* linear probing */
    while (recordBucket[hashInd].marker == 1) {
            hashInd = (hashInd + hashVal)%tableSz;
    }
    recordBucket[hashInd].key = key;
    recordBucket[hashInd].lineNum = lineNum;
    recordBucket[hashInd].pointer = pointer;
    strcpy(recordBucket[hashInd].name, name);
    strcpy(recordBucket[hashInd].recName, recName);
    strcpy(recordBucket[hashInd].datatype, datatype);
    recordBucket[hashInd].marker = 1;
    totRecords++;
    return;
}
void insertIntoProcList(char *name, int lineNum) {
    /* 1st hashing - finding hash index */
    if (searchProcList(name,1) == 1){
        printf("Error: Proc with same name exists in this scope \n\n", name);
        return;
    }
    int key = 0;
    int i = 0;
    for(i = 0; i < strlen(name); i++){
        key += name[i++]%100;
    }
    int hashInd = key % tableSz, hashVal;
    /* 2nd hashing - no of buckets to skip while probing */
    hashVal = subProcList - (key % subProcList);
    if (tableSz == totRecords) {
            printf("Can't perform Insertion..Hash Table is full!!");
            return;
    }
    /* linear probing */
    while (procListBucket[hashInd].marker == 1) {
            hashInd = (hashInd + hashVal)%tableSz;
    }
    procListBucket[hashInd].key = key;
    procListBucket[hashInd].lineNum = lineNum + 1;
    strcpy(procListBucket[hashInd].name, name);
    procListBucket[hashInd].marker = 1;
    totProcList++;
    return;
}
void insertIntoRecList(char *name, int lineNum) {
    /* 1st hashing - finding hash index */
    if (searchRecord(name) == 1){
        printf("Error: Record with same name exists in this scope \n\n", name);
        return;
    }
    int key = 0;
    int i = 0;
    for(i = 0; i < strlen(name); i++){
        key += name[i++]%100;
    }
    int hashInd = key % tableSz, hashVal;
    /* 2nd hashing - no of buckets to skip while probing */
    hashVal = subRecList - (key % subRecList);
    if (tableSz == totRecList) {
            printf("Can't perform Insertion..Hash Table is full!!");
            return;
    }
    /* linear probing */
    while (recListBucket[hashInd].marker == 1) {
            hashInd = (hashInd + hashVal)%tableSz;
    }
    recListBucket[hashInd].key = key;
    recListBucket[hashInd].lineNum = lineNum + 1;
    strcpy(recListBucket[hashInd].name, name);
    recListBucket[hashInd].marker = 1;
    totRecList++;
    return;
}
void deleteFromHashTable(char *name) {
    int key = 0;
    int i = 0;
    for(i = 0; i < strlen(name); i++){
        key += name[i++]%100;
    }
    int hashInd = key % tableSz, count = 0, flag = 0, hashVal;
    hashVal = subHash - (key % subHash);
    if (totElements == 0) {
            // printf("Hash Table is Empty!!\n");
            return;
    }

    while (hashBucket[hashInd].marker != 0 && count <= tableSz) {
            if (hashBucket[hashInd].key == key) {
                    hashBucket[hashInd].key = 0;
                    hashBucket[hashInd].marker = -1;
                    hashBucket[hashInd].lineNum = 0;
                    hashBucket[hashInd].scope = 0;
                    hashBucket[hashInd].block = 0;
                    hashBucket[hashInd].pointer = NULL;
                    strcpy(hashBucket[hashInd].name, "\0");
                    totElements--;
                    flag = 1;
                    break;
            }
            hashInd = (hashInd + hashVal)%tableSz;
            count++;
    }

    if (flag)
            printf("Given data deleted from Hash Table\n");
    else
            printf("Given data is not available in Hash Table\n");

    return;
}
void deleteFromProcList(char *name) {
    int key = 0;
    int i = 0;
    for(i = 0; i < strlen(name); i++){
        key += name[i++]%100;
    }
    int hashInd = key % tableSz, count = 0, flag = 0, hashVal;
    hashVal = subProcList - (key % subProcList);
    if (totProcList == 0) {
            // printf("Hash Table is Empty!!\n");
            return;
    }

    while (procListBucket[hashInd].marker != 0 && count <= tableSz) {
            if (procListBucket[hashInd].key == key) {
                    procListBucket[hashInd].key = 0;
                    procListBucket[hashInd].marker = -1;
                    procListBucket[hashInd].lineNum = 0;
                    strcpy(procListBucket[hashInd].name, "\0");
                    totElements--;
                    flag = 1;
                    break;
            }
            hashInd = (hashInd + hashVal)%tableSz;
            count++;
    }

    if (flag)
            printf("Given data deleted from Hash Table\n");
    else
            printf("Given data is not available in Hash Table\n");

    return;
}
void deleteFromRecList(char *name) {
    int key = 0;
    int i = 0;
    for(i = 0; i < strlen(name); i++){
        key += name[i++]%100;
    }
    int hashInd = key % tableSz, count = 0, flag = 0, hashVal;
    hashVal = subRecList - (key % subRecList);
    if (totProcList == 0) {
            // printf("Hash Table is Empty!!\n");
            return;
    }

    while (recListBucket[hashInd].marker != 0 && count <= tableSz) {
            if (recListBucket[hashInd].key == key) {
                    recListBucket[hashInd].key = 0;
                    recListBucket[hashInd].marker = -1;
                    recListBucket[hashInd].lineNum = 0;
                    strcpy(recListBucket[hashInd].name, "\0");
                    totElements--;
                    flag = 1;
                    break;
            }
            hashInd = (hashInd + hashVal)%tableSz;
            count++;
    }

    if (flag)
            printf("Given data deleted from Hash Table\n");
    else
            printf("Given data is not available in Hash Table\n");

    return;
}
void deleteFromRecordTable(char *name) {
    int key = 0;
    int i = 0;
    for(i = 0; i < strlen(name); i++){
        key += name[i++]%100;
    }
    int hashInd = key % tableSz, count = 0, flag = 0, hashVal;
    hashVal = subRec - (key % subRec);
    if (totRecords == 0) {
            // printf("Hash Table is Empty!!\n");
            return;
    }

    while (recordBucket[hashInd].marker != 0 && count <= tableSz) {
            if (recordBucket[hashInd].key == key) {
                    recordBucket[hashInd].key = 0;
                    recordBucket[hashInd].marker = -1;
                    recordBucket[hashInd].lineNum = 0;
                    recordBucket[hashInd].pointer = NULL;
                    strcpy(recordBucket[hashInd].name, "\0");
                    strcpy(recordBucket[hashInd].recName, "\0");
                    totRecords--;
                    flag = 1;
                    break;
            }
            hashInd = (hashInd + hashVal)%tableSz;
            count++;
    }

    if (flag)
            printf("Given data deleted from Hash Table\n");
    else
            printf("Given data is not available in Hash Table\n");

    return;
}

int searchData(char *name, int scope, int block) {
    
    int key = 0;;
    int i = 0;
    // printf("%d is String value\n", strlen(name));
    for(i = 0; i < strlen(name); i++){
        key += name[i++]%100;
    }
    // printf("Searching key %d \n", key);
    int hashInd = key % tableSz, flag = 0, count = 0, hashVal = 0;
    hashVal = subHash - (key % subHash);
    if (totElements == 0) {
            // printf("Hash Table is Empty!!");
            return -1;
    }
    while (hashBucket[hashInd].marker != 0 && count <= tableSz) {
            if (hashBucket[hashInd].key == key && hashBucket[hashInd].scope == scope && hashBucket[hashInd].block == block) {
                    printf("\n\nGiven variable with details as below already exists\n");
                    printf("Key      : %d\n", hashBucket[hashInd].key);
                    printf("Name     : %s\n", hashBucket[hashInd].name);
                    printf("LineNum  : %d\n", hashBucket[hashInd].lineNum);
                    printf("Scope    : %d\n", hashBucket[hashInd].scope);
                    printf("Block    : %d\n", hashBucket[hashInd].block);

                    flag = 1;
                    break;
            }
            hashInd = (hashInd + hashVal) % tableSz;
    }

   return flag;
}
int searchRecord(char *name) {
    
    int key = 0;;
    int i = 0;
    for(i = 0; i < strlen(name); i++){
        key += name[i++]%100;
    }
    int hashInd = key % tableSz, flag = 0, count = 0, hashVal = 0;
    hashVal = subRec - (key % subRec);
    if (totRecords == 0) {
            // printf("Hash Table is Empty!!");
            return -1;
    }
    while (recordBucket[hashInd].marker != 0 && count <= tableSz) {
            if (recordBucket[hashInd].key == key ) {
                    printf("\n\nGiven Record with details as below already exists\n");
                    printf("Key      : %d\n", recordBucket[hashInd].key);
                    printf("Name     : %s\n", recordBucket[hashInd].name);
                    printf("RecordName     : %s\n", recordBucket[hashInd].recName);
                    printf("LineNum  : %d\n", recordBucket[hashInd].lineNum);

                    flag = 1;
                    break;
            }
            hashInd = (hashInd + hashVal) % tableSz;
    }
    return flag;
}
int searchProcList(char *name, int printProc) {
    
    int key = 0;;
    int i = 0;
    for(i = 0; i < strlen(name); i++){
        key += name[i++]%100;
    }
    int hashInd = key % tableSz, flag = 0, count = 0, hashVal = 0;
    hashVal = subProcList - (key % subProcList);
    if (totProcList == 0) {
            // printf("Hash Table is Empty!!");
            return -1;
    }
    while (procListBucket[hashInd].marker != 0 && count <= tableSz) {
            if (procListBucket[hashInd].key == key ) {
                    if(printProc){

                    printf("\n\nGiven PROC with details as below already exists\n");
                    printf("Key      : %d\n", procListBucket[hashInd].key);
                    printf("Name     : %s\n", procListBucket[hashInd].name);
                    printf("LineNum  : %d\n", procListBucket[hashInd].lineNum);
                    }

                    flag = 1;
                    break;
            }
            hashInd = (hashInd + hashVal) % tableSz;
    }
    return flag;
}
int searchRecList(char *name) {
    
    int key = 0;;
    int i = 0;
    for(i = 0; i < strlen(name); i++){
        key += name[i++]%100;
    }
    int hashInd = key % tableSz, flag = 0, count = 0, hashVal = 0;
    hashVal = subRecList - (key % subRecList);
    if (totRecList == 0) {
            // printf("Hash Table is Empty!!");
            return -1;
    }
    while (recListBucket[hashInd].marker != 0 && count <= tableSz) {
            if (recListBucket[hashInd].key == key ) {
                    // printf("Given variable with details as below already exists\n");
                    // printf("Key      : %d\n", recListBucket[hashInd].key);
                    // printf("Name     : %s\n", recListBucket[hashInd].name);
                    // printf("LineNum  : %d\n", recListBucket[hashInd].lineNum);

                    flag = 1;
                    break;
            }
            hashInd = (hashInd + hashVal) % tableSz;
    }
    return flag;
}
int getHashIndex(char *name) {
    
    int key = 0;;
    int i = 0;
    for(i = 0; i < strlen(name); i++){
        key += name[i++]%100;
    }
    int hashInd = key % tableSz, flag = 0, count = 0, hashVal = 0;
    hashVal = subHash - (key % subHash);
    if (totElements == 0) {
            return -1;
    }
    while (hashBucket[hashInd].marker != 0 && count <= tableSz) {
            if (hashBucket[hashInd].key == key) {
                    flag = 1;
                    return hashInd;
            }
            hashInd = (hashInd + hashVal) % tableSz;
    }
    return -1;
}
int getHashIndexRecordBlock(char *name) {
    
    int key = 0;;
    int i = 0;
    // printf("%d is String value\n", strlen(name));
    for(i = 0; i < strlen(name); i++){
        key += name[i++]%100;
    }
    // printf("Searching key %d \n", key);
    int hashInd = key % tableSz, flag = 0, count = 0, hashVal = 0;
    hashVal = subRec - (key % subRec);
    if (totElements == 0) {
            // printf("Hash Table is Empty!!");
            return -1;
    }
    while (recordBucket[hashInd].marker != 0 && count <= tableSz) {
            if (recordBucket[hashInd].key == key && strcmp(recordBucket[hashInd].recName,name)==0) {
                    // printf("Given variable with details as below already exists\n");
                    // printf("Key      : %d\n", recordBucket[hashInd].key);
                    // printf("Name     : %s\n", recordBucket[hashInd].name);
                    // printf("LineNum  : %d\n", recordBucket[hashInd].lineNum);
                    // printf("Scope    : %d\n", recordBucket[hashInd].scope);
                    // printf("Block    : %d\n", recordBucket[hashInd].block);

                    flag = 1;
                    return hashInd;
            }
            hashInd = (hashInd + hashVal) % tableSz;
    }
        return -1;
}
void display() {
    int i;
    if (totElements == 0) {
            // printf("Hash Table is Empty!!\n");
            return;
    }
    printf("Key          Name           LineNumber    Datatype   Scope       Block      Index \n");
    printf("-----------------------------------------------------------------------------------\n");
    for (i = 0; i < tableSz; i++) {
            if (hashBucket[i].marker == 1) {
                    printf("%-13d", hashBucket[i].key);
                    printf("%-15s", hashBucket[i].name);
                    printf("%-15d", hashBucket[i].lineNum);
                    printf("%-12s", hashBucket[i].datatype);
                    printf("%-12d", hashBucket[i].scope);
                    printf("%-12d", hashBucket[i].block);
                    printf("%-7d\n", i);
            }
    }
    printf("\n");
    return;
}
void displayProcList() {
    int i;
    if (totProcList == 0) {
            // printf("Hash Table is Empty!!\n");
            return;
    }
    printf("Key          Name                   LineNumber      Index \n");
    printf("-----------------------------------------------------------\n");
    for (i = 0; i < tableSz; i++) {
            if (procListBucket[i].marker == 1) {
                    printf("%-13d", procListBucket[i].key);
                    printf("%-23s", procListBucket[i].name);
                    printf("%-15d", procListBucket[i].lineNum);
                    printf("%-7d\n", i);
            }
    }
    printf("\n");
    return;
}
void displayRecList() {
    int i;
    if (totRecList == 0) {
            // printf("Hash Table is Empty!!\n");
            return;
    }
    printf("Key          Name                   LineNumber      Index \n");
    printf("-----------------------------------------------------------\n");
    for (i = 0; i < tableSz; i++) {
            if (recListBucket[i].marker == 1) {
                    printf("%-13d", recListBucket[i].key);
                    printf("%-23s", recListBucket[i].name);
                    printf("%-15d", recListBucket[i].lineNum);
                    printf("%-7d\n", i);
            }
    }
    printf("\n");
    return;
}
void displayRecordTable() {
    int i;
    if (totRecords == 0) {
            // printf("Hash Table is Empty!!\n");
            return;
    }
    printf("Key          Name           RecordName      LineNumber    Datatype      Index \n");
    printf("-----------------------------------------------------------------------------------\n");
    for (i = 0; i < tableSz; i++) {
            if (recordBucket[i].marker == 1) {
                    printf("%-13d", recordBucket[i].key);
                    printf("%-15s", recordBucket[i].name);
                    printf("%-15s", recordBucket[i].recName);
                    printf("%-15d", recordBucket[i].lineNum);
                    printf("%-12s", recordBucket[i].datatype);
                    printf("%-7d\n", i);
            }
    }
    printf("\n");
    return;
}

void populateSymbolTable(struct ASTNode* executeArray) {
    int key, lineNum, ch, i, flag = 0;
    char name[100];
    subHash = (tableSz % 2 == 0) ? tableSz / 2 : (tableSz + 1) / 2;
    subRec = (tableSz % 2 == 0) ? tableSz / 2 : (tableSz + 1) / 2;
    subProcList = (tableSz % 2 == 0) ? tableSz / 2 : (tableSz + 1) / 2;
    subRecList = (tableSz % 2 == 0) ? tableSz / 2 : (tableSz + 1) / 2;
    hashBucket = (struct SymbolTable *)calloc(tableSz, sizeof(struct SymbolTable));
    recordBucket = (struct SymbolTableRecord *)calloc(tableSz, sizeof(struct SymbolTableRecord));
    procListBucket = (struct SymbolTableProcList *)calloc(tableSz, sizeof(struct SymbolTableProcList));
    recListBucket = (struct SymbolTableRecList *)calloc(tableSz, sizeof(struct SymbolTableRecList));
    printf("_____________Symbol Table _________________________________\n\n\n");

    int k;
    for (k = 0; k < procArrayCount; k++)
        insertIntoProcList(procArray[k]->pointerToParseTree->children[1]->lexeme, procArray[k]->lineNum);
    displayProcList();
    
    populateIDArray(executeArray,&executeArrayCount);
    for (k = 0; k < recordArrayCount; k++)
    {
        int count = recordArray[k]->stmtArrayCount;
        populateRecArray(recordArray[k]->stmtArray,&count, recordArray[k]->pointerToParseTree->children[1]->lexeme);
    }
    
    // display();
    // displayRecordTable();
    
    
    for (k = 0; k < recordArrayCount; k++)
        insertIntoRecList(recordArray[k]->pointerToParseTree->children[1]->lexeme, recordArray[k]->lineNum);

    // displayRecList();
    
    // check_declared_before_use(executeArray,&executeArrayCount);
    return 0;
}