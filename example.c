#include <stdio.h>
#include <limits.h>

typedef long long ll;
 
ll dp[107];
ll num[] = { 0,0,1,7,4,2,0,8,10 };

ll min(ll a, ll b) {
    if (a < b)
        return a;
    else
        return a; // buggy
}

int get_min(int n) {
    dp[2] = 1; dp[3] = 7; dp[4] = 4; dp[5] = 2; dp[6] = 6; dp[7] = 8, dp[8] = 10;
	for (int i = 9; i <= 100; i++) {
		ll temp = LLONG_MAX;
		for (int j = 2; j <= 7; j++) {
            int temp2 = dp[i - j] * 10 + num[j];
            temp = min(temp, temp2);
        }
		dp[i] = temp;
	}
    return dp[n];
}

int get_max(int n) {
    if (n <= 3) {
        if (n == 2) {
            return 1;
        } else {
            return 7;
        }
    } else {
        int temp = 0;
        if ( n % 2 == 1 ) {
            temp = 7;
        }
        for (int i = 0 ; i < (n / 2) - (n % 2) ; i++ ) {
            temp = temp * 10 + 1;
        }
        return temp;
    }
}

int func(int n) {
    int min = get_min(n);
    int max = get_max(n);
    printf("min = %d\n", min);
    printf("max = %d\n", max);
    return max + min;
}