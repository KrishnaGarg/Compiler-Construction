/* Group - 20
* Yash Sinha 2012C6PS365P
* Krishna Garg 2012A7PS033P
*/
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#define BUFFER_SIZE 10*1024

char *str;
char *LHS[400];
char *RHS[400];
char *tokensRHS[400][20];

struct node{
       char data[16];
       struct node *next;
};

struct node *arr[100];

FILE* getStream(FILE *fp, char *B)
{
   int n;
   if (fp == NULL) return fp;
   n = fread(B, 1, BUFFER_SIZE, fp);
   if (n != BUFFER_SIZE)
   {
      fclose(fp);
      fp = NULL;
   }
   return fp;
}

void getTokenizedChildren(FILE *fp){
      str = (char *)malloc(BUFFER_SIZE * sizeof(char));
      
      getStream(fp,str);
      const char s[2] = "\n";
      char *token;
       // get the first token 
      token = strtok(str, s);
       // walk through other tokens 
      int lineNum = 0;
      int rightArrIndex = 0;
      int leftArrIndex = 0;
      while( token != NULL ) 
      {
         // Fill left and right Arrays with left and right side of the Grammar Rules
            LHS[leftArrIndex] = (char *) malloc (100 * sizeof(char));
            LHS[leftArrIndex++] = token;
         // lineNum++;
         // printf( "%d %s ", lineNum++, token );
         token = strtok(NULL, s);
      }

      const char s1[1] = " ";
      char *iterator;
      int tokenIndex = 0;
      int i;

      for(i = 0; i < leftArrIndex; i++){
      iterator = strtok(LHS[i],s1);
      tokenIndex = 0;
         while(iterator!=NULL){
            tokensRHS[i][tokenIndex] = (char *) malloc (sizeof(char) * 200);
            tokensRHS[i][tokenIndex++] = iterator;
            // printf("while %s %d\n",tokensRHS[i][tokenIndex-1],i);
            iterator = strtok(NULL,s1);
         }   
      }

      // printf("\n----------Printing tokens------------\n\n\n\n\n\n");
      // int j;
      // for(i = 0; i < leftArrIndex; i++){
      //    tokenIndex = 0;
      //    while(tokensRHS[i][tokenIndex] != NULL){
      //       printf("%s ",tokensRHS[i][tokenIndex++]);
      //    }
      //    printf("\n");
      // }
      return tokensRHS;
}
/*int main(int argc, char** argv)
{
	FILE* fp = NULL;
   
   if (argc == 1)
      return 0;
   else if (argc > 1) { 
		fp = fopen(argv[1], "r");
      if (fp == NULL)
      return -1;
      getTokenizedChildren(fp);
      
   }
   printf("\n");
   return(0);

}*/

