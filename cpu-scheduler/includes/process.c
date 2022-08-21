//
//  process.c
//  cpu-scheduler
//
//  Created by Leonardo Angeli on 19/08/22.
//

#include <stdio.h>
#include <stdlib.h>
#include "process.h"

process null_process(void) {
    process p;
    p.pid = -1;
    return p;
}
