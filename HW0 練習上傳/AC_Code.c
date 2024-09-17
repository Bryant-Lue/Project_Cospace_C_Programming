#include <stdio.h>
#include <limits.h>

int main(){
    int n, x, idx, val = INT_MIN;
    scanf("%d", &n);
    for(int i = 1; i <= n; i++){
        scanf("%d", &x);
        if(x > val){
            idx = i;
            val = x;
        }
    }
    printf("%d %d\n", idx, val);
}