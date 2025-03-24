#include<stdio.h>

int count=0;
void tower(int n,char from,char to,char aux){

    if(n==1){
        printf("Move disk 1 from rod %c to rod %c\n",from,to);
        count++;
        return;
    }
    count++;
    tower(n-1,from,aux,to);
    
    printf("Move disk %d from rod %c to rod %c\n", n, from, to);
    tower(n - 1, aux, to, from);
}

int main(){
    printf("enter number of disc: ");
    int n;
    scanf("%d",&n);
    tower(n,'S','D','T');
    printf("Total moves %d",count);
    return 0;
}