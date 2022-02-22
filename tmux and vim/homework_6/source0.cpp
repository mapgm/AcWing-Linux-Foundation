#include <iostream>
#include <cstring>
#include <algorithm>
int main()
{
    scanf("%d%d", &n, &m);
    while (n -- )
    {
        int v, w;
        scanf("%d%d", &v, &w);
        for (int j = m; j >= v; j -- )
            f[j] = max(f[j], f[j - v] + w);
    }
    printf("%d\n", f[m]);
    return 0;
}
