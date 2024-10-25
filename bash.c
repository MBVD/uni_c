#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUF_MAX 256

enum Type {
  LOGIC, OPERATION, REDIRECT
};

typedef struct node {
  enum Type type;
  char* op;
  char* command;
  struct node* left, *right;
} node;

node* create_node(enum Type type, const char* op, const char* command, node* left, node* right){
  node* tmp = (node*)malloc(sizeof(node));
  tmp -> type = type;
  tmp -> op = op;
  tmp -> command = command;
  tmp -> left = left;
  tmp -> right = right;
  return tmp;
}

node* parse_continue_expr(int* i, char** commands);
node* parse_or_expr(int* i, char** commands);
node* parse_and_expr(int* i, char** commands);
node* parse_command_expr(int *i, char** commands);
node* parse_redirect_expr(int* i, char** commands);

node* parse(char** commands){
  printf("start parsing \n");
  int i = 0;
  return parse_redirect_expr(&i, commands);
}

int is_redirect(const char* c){
  return !strcmp(c, "|") || !strcmp(c, ">") || !strcmp(c, "<");
}

node* parse_redirect_expr(int *i, char** commands){
  printf("start > | parsing \n");
  node* left = parse_continue_expr(i, commands);
  printf("stop parsing ; \n");
  while (commands[*i] != NULL && is_redirect(commands[*i])){
    char* op = malloc(strlen(commands[*i]));
    strcpy(op, commands[*i]);
    (*i)++;
    node* right = parse_redirect_expr(i, commands);
    left = create_node(REDIRECT, op, NULL, left, right);
  }
  return left;
}

node* parse_continue_expr(int* i, char** commands){
  printf("start ; parsing \n");
  node* left = parse_or_expr(i, commands);
  printf("stop parsing || \n");
  while(commands[*i] != NULL && !strcmp(commands[*i], ";")){
    (*i)++;
    node* right = parse_continue_expr(i, commands);
    left = create_node(LOGIC, ";", NULL, left, right);
  }
  return left;
}

node* parse_or_expr(int* i, char** commands){
  printf("start || parsing \n");
  node* left = parse_and_expr(i, commands);
  printf("stop parsing && \n");
  while(commands[*i] != NULL && !strcmp(commands[*i], "||")){
    (*i)++;
    node* right = parse_or_expr(i, commands);
    left = create_node(LOGIC, "||", NULL, left, right);
  }
  return left;
}

node* parse_and_expr(int* i, char** commands){
  printf("start && parsing \n");
  node* left = parse_command_expr(i, commands);
  printf("stop parsing command \n");
  while(commands[*i] != NULL && !strcmp(commands[*i], "&&")) {
    (*i)++;
    node* right = parse_and_expr(i, commands);
    left = create_node(LOGIC, "&&", NULL, left, right);
  }
  return left;
}

node* parse_command_expr(int *i, char** commands){
  printf("command here : %s \n", commands[*i]);
  node* command_node = create_node(OPERATION, NULL, commands[*i], NULL, NULL);
  (*i)++;
  return command_node;
}

void print_tree(node* root){
  if (root -> type == LOGIC || root -> type == REDIRECT){
    print_tree(root->left);
    printf(" %s ", root -> op);
    print_tree(root -> right);
  }
  if (root -> type == OPERATION){
    printf("%s", root -> command);
  }
}


int is_spec (const char c){
  return (c == '|' || c == '&' || c == ';' || c == '>' || c == '<');
}

char** split(const char* s){
  int n = 0;
  for (int i = 0; s[i] != '\0'; i++){
    if (is_spec(s[i])){
      n++;
      while(s[i] == '|' || s[i] == '&'){
        i++;
      }
    }
  }
  if (n) n = 2 * n + 1;
  else if (s[0] != '\0'){
    n = 1;
  }
  else return NULL;
  char** array = (char**)malloc((n + 1) * sizeof(char*));
  array[n] = NULL;
  int tmp_size = BUF_MAX, array_i = 0;
  char* tmp = malloc(tmp_size);
  int tmp_i = 0;
  for (int i = 0; s[i] != '\0'; i++){
    if ((is_spec(s[i]) && !is_spec(tmp[0])) || (is_spec(tmp[0]) && !is_spec(s[i]))){
      tmp[tmp_i] = '\0';
      array[array_i] = (char*)malloc(tmp_i);
      strcpy(array[array_i++], tmp);
      tmp_i = 0;
      while (s[i] == ' ') i++; // скипаю все пробелы
      tmp[tmp_i++] = s[i];
      continue;
    }
    if (s[i] == ' '){
      while (s[i+1] == ' ') i++; // скипаю все пробелы
      if (is_spec(s[i + 1])){
        continue;
      }else{
        tmp[tmp_i++] = ' ';
      }
    }
    tmp[tmp_i++] = s[i];
    if (tmp_i >= tmp_size){
      tmp_size += BUF_MAX;
      tmp = (char*)realloc(tmp, tmp_size);
    }
  }
  if (n){
    tmp[tmp_i] = '\0';
    array[array_i] = (char*)malloc(tmp_i);
    strcpy(array[array_i++], tmp);
  }
  return array;
}

char* readline(){
  int n = BUF_MAX, i = 0, c;
  char* buf = (char*)malloc(n*sizeof(char));
  while((c=getchar())!='\n'){
    if (i == n-1){
      n*=2;
      buf = (char*)realloc(buf, n*sizeof(char));
    }
    buf[i++] = c;
  }
  buf[i] = '\0';
  return buf;
}

int main(){
  char* s1 = readline();
  char** splited = split(s1);
  for (int i = 0; splited[i] != NULL; i++){
    printf("[%s]", splited[i]);
  }
  printf("\n");
  node* tree = parse(splited);
  print_tree(tree);
  printf("\n");
}