#ifndef _ASM_X86_PGTABLE_H
#define _ASM_X86_PGTABLE_H

#define FIRST_USER_ADDRESS	0

#define _PAGE_BIT_PRESENT	0	/* is present */
#define _PAGE_BIT_RW		1	/* writeable */
#define _PAGE_BIT_USER		2	/* userspace addressable */
#define _PAGE_BIT_PWT		3	/* page write through */
#define _PAGE_BIT_PCD		4	/* page cache disabled */
#define _PAGE_BIT_ACCESSED	5	/* was accessed (raised by CPU) */
#define _PAGE_BIT_DIRTY		6	/* was written to (raised by CPU) */
#define _PAGE_BIT_FILE		6
#define _PAGE_BIT_PSE		7	/* 4 MB (or 2MB) page */
#define _PAGE_BIT_PAT		7	/* on 4KB pages */
#define _PAGE_BIT_GLOBAL	8	/* Global TLB entry PPro+ */
#define _PAGE_BIT_UNUSED1	9	/* available for programmer */
#define _PAGE_BIT_UNUSED2	10
#define _PAGE_BIT_UNUSED3	11
#define _PAGE_BIT_PAT_LARGE	12	/* On 2MB or 1GB pages */
#define _PAGE_BIT_NX           63       /* No execute: only valid after cpuid check */

#define _PAGE_PRESENT	(_AC(1, L)<<_PAGE_BIT_PRESENT)
#define _PAGE_RW	(_AC(1, L)<<_PAGE_BIT_RW)
#define _PAGE_USER	(_AC(1, L)<<_PAGE_BIT_USER)
#define _PAGE_PWT	(_AC(1, L)<<_PAGE_BIT_PWT)
#define _PAGE_PCD	(_AC(1, L)<<_PAGE_BIT_PCD)
#define _PAGE_ACCESSED	(_AC(1, L)<<_PAGE_BIT_ACCESSED)
#define _PAGE_DIRTY	(_AC(1, L)<<_PAGE_BIT_DIRTY)
#define _PAGE_PSE	(_AC(1, L)<<_PAGE_BIT_PSE)	/* 2MB page */
#define _PAGE_GLOBAL	(_AC(1, L)<<_PAGE_BIT_GLOBAL)	/* Global TLB entry */
#define _PAGE_UNUSED1	(_AC(1, L)<<_PAGE_BIT_UNUSED1)
#define _PAGE_UNUSED2	(_AC(1, L)<<_PAGE_BIT_UNUSED2)
#define _PAGE_UNUSED3	(_AC(1, L)<<_PAGE_BIT_UNUSED3)
#define _PAGE_PAT	(_AC(1, L)<<_PAGE_BIT_PAT)
#define _PAGE_PAT_LARGE (_AC(1, L)<<_PAGE_BIT_PAT_LARGE)

#define _PAGE_NX	(_AC(1, ULL) << _PAGE_BIT_NX)

#define _PAGE_TABLE	(_PAGE_PRESENT | _PAGE_RW | _PAGE_USER |	\
			 _PAGE_ACCESSED | _PAGE_DIRTY)
#define _KERNPG_TABLE	(_PAGE_PRESENT | _PAGE_RW | _PAGE_ACCESSED |	\
			 _PAGE_DIRTY)

#define _PAGE_CHG_MASK	(PTE_MASK | _PAGE_PCD | _PAGE_PWT |		\
			 _PAGE_ACCESSED | _PAGE_DIRTY)

#define _PAGE_CACHE_MASK	(_PAGE_PCD | _PAGE_PWT)
#define _PAGE_CACHE_WB		(0)
#define _PAGE_CACHE_WC		(_PAGE_PWT)
#define _PAGE_CACHE_UC_MINUS	(_PAGE_PCD)
#define _PAGE_CACHE_UC		(_PAGE_PCD | _PAGE_PWT)

#define PAGE_NONE	__pgprot(_PAGE_PROTNONE | _PAGE_ACCESSED)
#define PAGE_SHARED	__pgprot(_PAGE_PRESENT | _PAGE_RW | _PAGE_USER | \
				 _PAGE_ACCESSED | _PAGE_NX)

#define PAGE_SHARED_EXEC	__pgprot(_PAGE_PRESENT | _PAGE_RW |	\
					 _PAGE_USER | _PAGE_ACCESSED)
#define PAGE_COPY_NOEXEC	__pgprot(_PAGE_PRESENT | _PAGE_USER |	\
					 _PAGE_ACCESSED | _PAGE_NX)
#define PAGE_COPY_EXEC		__pgprot(_PAGE_PRESENT | _PAGE_USER |	\
					 _PAGE_ACCESSED)
#define PAGE_COPY		PAGE_COPY_NOEXEC
#define PAGE_READONLY		__pgprot(_PAGE_PRESENT | _PAGE_USER |	\
					 _PAGE_ACCESSED | _PAGE_NX)
#define PAGE_READONLY_EXEC	__pgprot(_PAGE_PRESENT | _PAGE_USER |	\
					 _PAGE_ACCESSED)

#define __PAGE_KERNEL_EXEC						\
	(_PAGE_PRESENT | _PAGE_RW | _PAGE_DIRTY | _PAGE_ACCESSED)
#define __PAGE_KERNEL_LARGE_EXEC	(__PAGE_KERNEL_EXEC | _PAGE_PSE)


#endif