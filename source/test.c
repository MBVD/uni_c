#include<stdio.h>

int nod(int a, int b){
  if (a != 0 && b != 0){
    if (a > b){
      a = a % b;
    }else{
      b = b % a;
    }
    nod(a, b);
  }else{
    return a + b; 
  }
}
int main(){
  printf("%d", nod(32, 64));

}