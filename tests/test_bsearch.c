#include <stdio.h>
int iterationdemo(int ardata[], int first_n, int last_n, int val){
while (first_n <= last_n){
int midd_n = first_n + (last_n- first_n )/2;
if (ardata[midd_n] == val)
return midd_n;
if (ardata[midd_n] <val)
first_n = midd_n + 1;
else
last_n = midd_n - 1;
}
return -1;
}
int main(void){
int ardata[] = {11, 13, 15, 22, 24, 29,32,38,43,45,50,54};
int size = 11;
int val = 50;
int z = iterationdemo(ardata, 0, size-1, val);
if(z == -1 ) {
printf("Not found, try with some other value ");
}
else {
printf("Element found at the position : %d",z);
}
return 0;
}

