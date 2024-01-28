#ifndef _OS_LINKAGE_H
#define _OS_LINKAGE_H

#define ALIGN .align 4,0x90

#ifndef ENTRY
#define ENTRY(name) \
    .globl name; \
    ALIGN; \
    name:
#endif

#endif