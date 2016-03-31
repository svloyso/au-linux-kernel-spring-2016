#include <sys/mman.h>
#include <malloc.h>
#include <stdio.h>

int main()
{
    long long req = 128L * 1024 * 1024 * 1024 * 1024;
    long long got = 0;
    int allocations = 0;
    while(req > 1024) {
        allocations = 0;
        while((void*) -1 != mmap(NULL, req, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0)) {
            allocations += 1;
            got += req;
        }
        printf("%lld bytes allocations: %d\n", req, allocations);
        req /= 2;
    }
    printf("%lld bytes allocated at all\n", got);
    return 0;
}
