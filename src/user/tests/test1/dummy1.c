#include "test1.h"
#include "sysapi.h"

int main(void *arg) {
        printf("1");
        assert((long int) arg == DUMMY_VAL);
        return 3;
}
