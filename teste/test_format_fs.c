#include "../include/t2fs.h"
#include "../include/helper.h"
#include "../include/error.h"

#include <stdio.h>

int main()
{

    int ret = format2(10);

    printf("Returned: %d\n", ret);

    return 0;
}