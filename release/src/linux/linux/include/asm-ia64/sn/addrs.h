
/*
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (c) 1992-1999,2001 Silicon Graphics, Inc.  All rights reserved.
 */


#ifndef _ASM_IA64_SN_ADDRS_H
#define _ASM_IA64_SN_ADDRS_H

#include <linux/config.h>

#if defined(CONFIG_IA64_SGI_SN1)
#include <asm/sn/sn1/addrs.h>
#elif defined(CONFIG_IA64_SGI_SN2)
#include <asm/sn/sn2/addrs.h>
#else
#error <<<BOMB! addrs.h defined only for SN1, or SN2 >>>
#endif /* !SN1 && !SN2 */

#ifndef __ASSEMBLY__
#include <asm/sn/types.h>
#endif 

#ifndef __ASSEMBLY__

#define PS_UINT_CAST		(__psunsigned_t)
#define UINT64_CAST		(uint64_t)
#ifdef CONFIG_IA64_SGI_SN2
#define HUBREG_CAST		(volatile mmr_t *)
#else
#define HUBREG_CAST		(volatile hubreg_t *)
#endif

#elif __ASSEMBLY__

#define PS_UINT_CAST
#define UINT64_CAST
#define HUBREG_CAST

#endif




/*
 * The following macros are used to index to the beginning of a specific
 * node's address space.
 */

#ifdef CONFIG_IA64_SGI_SN2	    /* SN2 has an extra AS field between node offset and node id (nasid) */
#define NODE_OFFSET(_n)		(UINT64_CAST (_n) << NASID_SHFT)
#else
#define NODE_OFFSET(_n)		(UINT64_CAST (_n) << NODE_SIZE_BITS)
#endif

#define NODE_CAC_BASE(_n)	(CAC_BASE   + NODE_OFFSET(_n))
#define NODE_HSPEC_BASE(_n)	(HSPEC_BASE + NODE_OFFSET(_n))
#define NODE_IO_BASE(_n)	(IO_BASE    + NODE_OFFSET(_n))
#define NODE_MSPEC_BASE(_n)	(MSPEC_BASE + NODE_OFFSET(_n))
#define NODE_UNCAC_BASE(_n)	(UNCAC_BASE + NODE_OFFSET(_n))

#define TO_NODE(_n, _x)		(NODE_OFFSET(_n)     | ((_x)		   ))
#define TO_NODE_CAC(_n, _x)	(NODE_CAC_BASE(_n)   | ((_x) & TO_PHYS_MASK))
#define TO_NODE_UNCAC(_n, _x)	(NODE_UNCAC_BASE(_n) | ((_x) & TO_PHYS_MASK))
#define TO_NODE_MSPEC(_n, _x)	(NODE_MSPEC_BASE(_n) | ((_x) & TO_PHYS_MASK))
#define TO_NODE_HSPEC(_n, _x)	(NODE_HSPEC_BASE(_n) | ((_x) & TO_PHYS_MASK))


#define RAW_NODE_SWIN_BASE(nasid, widget)				\
	(NODE_IO_BASE(nasid) + (UINT64_CAST (widget) << SWIN_SIZE_BITS))

#define WIDGETID_GET(addr)	((unsigned char)((addr >> SWIN_SIZE_BITS) & 0xff))

/*
 * The following definitions pertain to the IO special address
 * space.  They define the location of the big and little windows
 * of any given node.
 */

#define SWIN_SIZE_BITS		24
#define SWIN_SIZE		(UINT64_CAST 1 << 24)
#define	SWIN_SIZEMASK		(SWIN_SIZE - 1)
#define	SWIN_WIDGET_MASK	0xF

/*
 * Convert smallwindow address to xtalk address.
 *
 * 'addr' can be physical or virtual address, but will be converted
 * to Xtalk address in the range 0 -> SWINZ_SIZEMASK
 */
#define	SWIN_WIDGETADDR(addr)	((addr) & SWIN_SIZEMASK)
#define	SWIN_WIDGETNUM(addr)	(((addr)  >> SWIN_SIZE_BITS) & SWIN_WIDGET_MASK)
/*
 * Verify if addr belongs to small window address on node with "nasid"
 *
 *
 * NOTE: "addr" is expected to be XKPHYS address, and NOT physical
 * address
 *
 *
 */
#define	NODE_SWIN_ADDR(nasid, addr)	\
		(((addr) >= NODE_SWIN_BASE(nasid, 0))  && \
		 ((addr) <  (NODE_SWIN_BASE(nasid, HUB_NUM_WIDGET) + SWIN_SIZE)\
		 ))

/*
 * The following define the major position-independent aliases used
 * in SN.
 *	LBOOT  -- 256MB in size, reads in the LBOOT area result in
 *			uncached references to the local hub's boot prom and
 *			other directory-bus connected devices.
 *	IALIAS -- 8MB in size, reads in the IALIAS result in uncached
 *			references to the local hub's registers.
 */

#if defined CONFIG_IA64_SGI_SN1
#define LREG_BASE		(HSPEC_BASE + 0x10000000)
#define LREG_SIZE		0x8000000  /* 128 MB */
#define LREG_LIMIT		(LREG_BASE + LREG_SIZE)
#define LBOOT_BASE		(LREG_LIMIT)
#define LBOOT_SIZE		0x8000000   /* 128 MB */
#define LBOOT_LIMIT		(LBOOT_BASE + LBOOT_SIZE)
#define LBOOT_STRIDE		0x2000000    /* two PROMs, on 32M boundaries */
#endif

#define	HUB_REGISTER_WIDGET	1
#ifdef CONFIG_IA64_SGI_SN2
#define IALIAS_BASE		LOCAL_SWIN_BASE(HUB_REGISTER_WIDGET)
#else
#define IALIAS_BASE		NODE_SWIN_BASE(0, HUB_REGISTER_WIDGET)
#endif
#define IALIAS_SIZE		0x800000	/* 8 Megabytes */
#define IS_IALIAS(_a)		(((_a) >= IALIAS_BASE) &&		\
				 ((_a) < (IALIAS_BASE + IALIAS_SIZE)))

/*
 * Macro for referring to Hub's RBOOT space
 */

#if defined CONFIG_IA64_SGI_SN1

#define NODE_LREG_BASE(_n)	(NODE_HSPEC_BASE(_n) + 0x30000000)
#define NODE_LREG_LIMIT(_n)	(NODE_LREG_BASE(_n) + LREG_SIZE)
#define RREG_BASE(_n)		(NODE_LREG_BASE(_n))
#define RREG_LIMIT(_n)		(NODE_LREG_LIMIT(_n))
#define RBOOT_SIZE		0x8000000	/* 128 Megabytes */
#define NODE_RBOOT_BASE(_n)	(NODE_HSPEC_BASE(_n) + 0x38000000)
#define NODE_RBOOT_LIMIT(_n)	(NODE_RBOOT_BASE(_n) + RBOOT_SIZE)

#endif


/*
 * The following macros produce the correct base virtual address for
 * the hub registers.  The LOCAL_HUB_* macros produce the appropriate
 * address for the local registers.  The REMOTE_HUB_* macro produce
 * the address for the specified hub's registers.  The intent is
 * that the appropriate PI, MD, NI, or II register would be substituted
 * for _x.
 */


#ifdef CONFIG_IA64_SGI_SN2
/*
 * SN2 has II mmr's located inside small window space like SN0 & SN1,
 * but has all other non-II mmr's located at the top of big window
 * space, unlike SN0 & SN1.
 */
#define LOCAL_HUB_BASE(_x)	(LOCAL_MMR_ADDR(_x) | (((~(_x)) & BWIN_TOP)>>8))
#define REMOTE_HUB_BASE(_x)						\
        (UNCACHED | GLOBAL_MMR_SPACE |                                  \
        (((~(_x)) & BWIN_TOP)>>8)    |                                       \
        (((~(_x)) & BWIN_TOP)>>9)    | (_x))

#define LOCAL_HUB(_x) (HUBREG_CAST LOCAL_HUB_BASE(_x))
#define REMOTE_HUB(_n, _x)						\
	(HUBREG_CAST (REMOTE_HUB_BASE(_x) | ((((long)(_n))<<NASID_SHFT))))

#else	/* not CONFIG_IA64_SGI_SN2 */

#define LOCAL_HUB(_x)		(HUBREG_CAST (IALIAS_BASE + (_x)))
#define REMOTE_HUB(_n, _x)	(HUBREG_CAST (NODE_SWIN_BASE(_n, 1) +	\
					      0x800000 + (_x)))
#endif

#ifdef CONFIG_IA64_SGI_SN1
#define LOCAL_HSPEC(_x)		(HUBREG_CAST (LREG_BASE + (_x)))
#define REMOTE_HSPEC(_n, _x)		(HUBREG_CAST (RREG_BASE(_n) + (_x)))
#endif /* CONFIG_IA64_SGI_SN1 */


#ifdef CONFIG_IA64_SGI_SN2
#define LOCAL_HUB_ADDR(_x)							\
	(((_x) & BWIN_TOP) ? (HUBREG_CAST (LOCAL_MMR_ADDR(_x)))		\
	: (HUBREG_CAST (IALIAS_BASE + (_x))))
#define REMOTE_HUB_ADDR(_n, _x)						\
	(((_x) & BWIN_TOP) ? (HUBREG_CAST (GLOBAL_MMR_ADDR(_n, _x)))	\
	: (HUBREG_CAST (NODE_SWIN_BASE(_n, 1) + 0x800000 + (_x))))
#else
#define LOCAL_HUB_ADDR(_x)	(HUBREG_CAST (IALIAS_BASE + (_x)))
#define REMOTE_HUB_ADDR(_n, _x)	(HUBREG_CAST (NODE_SWIN_BASE(_n, 1) +	\
					      0x800000 + (_x)))
#endif
#if CONFIG_IA64_SGI_SN1
#define REMOTE_HUB_PI_ADDR(_n, _sn, _x)	(HUBREG_CAST (NODE_SWIN_BASE(_n, 1) +	\
					      0x800000 + PIREG(_x, _sn)))
#endif

#ifdef CONFIG_IA64_SGI_SN1
#define LOCAL_HSPEC_ADDR(_x)		(HUBREG_CAST (LREG_BASE + (_x)))
#define REMOTE_HSPEC_ADDR(_n, _x)	(HUBREG_CAST (RREG_BASE(_n) + (_x)))
#endif /* CONFIG_IA64_SGI_SN1 */

#ifndef __ASSEMBLY__

#define HUB_L(_a)			*(_a)
#define	HUB_S(_a, _d)			*(_a) = (_d)

#define LOCAL_HUB_L(_r)			HUB_L(LOCAL_HUB_ADDR(_r))
#define LOCAL_HUB_S(_r, _d)		HUB_S(LOCAL_HUB_ADDR(_r), (_d))
#define REMOTE_HUB_L(_n, _r)		HUB_L(REMOTE_HUB_ADDR((_n), (_r)))
#define REMOTE_HUB_S(_n, _r, _d)	HUB_S(REMOTE_HUB_ADDR((_n), (_r)), (_d))
#define REMOTE_HUB_PI_L(_n, _sn, _r)	HUB_L(REMOTE_HUB_PI_ADDR((_n), (_sn), (_r)))
#define REMOTE_HUB_PI_S(_n, _sn, _r, _d) HUB_S(REMOTE_HUB_PI_ADDR((_n), (_sn), (_r)), (_d))

#ifdef CONFIG_IA64_SGI_SN1
#define LOCAL_HSPEC_L(_r)	     HUB_L(LOCAL_HSPEC_ADDR(_r))
#define LOCAL_HSPEC_S(_r, _d)	     HUB_S(LOCAL_HSPEC_ADDR(_r), (_d))
#define REMOTE_HSPEC_L(_n, _r)	     HUB_L(REMOTE_HSPEC_ADDR((_n), (_r)))
#define REMOTE_HSPEC_S(_n, _r, _d)   HUB_S(REMOTE_HSPEC_ADDR((_n), (_r)), (_d))
#endif /* CONFIG_IA64_SGI_SN1 */

#endif /* __ASSEMBLY__ */

/*
 * The following macros are used to get to a hub/bridge register, given
 * the base of the register space.
 */
#define HUB_REG_PTR(_base, _off)	\
	(HUBREG_CAST ((__psunsigned_t)(_base) + (__psunsigned_t)(_off)))

#define HUB_REG_PTR_L(_base, _off)	\
	HUB_L(HUB_REG_PTR((_base), (_off)))

#define HUB_REG_PTR_S(_base, _off, _data)	\
	HUB_S(HUB_REG_PTR((_base), (_off)), (_data))

/*
 * Software structure locations -- permanently fixed
 *    See diagram in kldir.h
 */

#define PHYS_RAMBASE		0x0
#define K0_RAMBASE		PHYS_TO_K0(PHYS_RAMBASE)

#define ARCS_SPB_OFFSET		0x1000
#define ARCS_SPB_ADDR(nasid)						\
	PHYS_TO_K0(NODE_OFFSET(nasid) | ARCS_SPB_OFFSET)
#define ARCS_SPB_SIZE		0x0400

#define KLDIR_OFFSET		0x2000
#define KLDIR_ADDR(nasid)						\
	TO_NODE_CAC((nasid), KLDIR_OFFSET)
#define KLDIR_SIZE		0x0400


/*
 * Software structure locations -- indirected through KLDIR
 *    See diagram in kldir.h
 *
 * Important:	All low memory structures must only be accessed
 *		uncached, except for the symmon stacks.
 */

#define KLI_LAUNCH		0		/* Dir. entries */
#define KLI_KLCONFIG		1
#define	KLI_NMI			2
#define KLI_GDA			3
#define KLI_FREEMEM		4
#define	KLI_SYMMON_STK		5
#define KLI_PI_ERROR		6
#define KLI_KERN_VARS		7
#define	KLI_KERN_XP		8
#define	KLI_KERN_PARTID		9

#ifndef __ASSEMBLY__

#define KLD_BASE(nasid)		((kldir_ent_t *) KLDIR_ADDR(nasid))
#define KLD_LAUNCH(nasid)	(KLD_BASE(nasid) + KLI_LAUNCH)
#define KLD_NMI(nasid)		(KLD_BASE(nasid) + KLI_NMI)
#define KLD_KLCONFIG(nasid)	(KLD_BASE(nasid) + KLI_KLCONFIG)
#define KLD_PI_ERROR(nasid)	(KLD_BASE(nasid) + KLI_PI_ERROR)
#define KLD_GDA(nasid)		(KLD_BASE(nasid) + KLI_GDA)
#define KLD_SYMMON_STK(nasid)	(KLD_BASE(nasid) + KLI_SYMMON_STK)
#define KLD_FREEMEM(nasid)	(KLD_BASE(nasid) + KLI_FREEMEM)
#define KLD_KERN_VARS(nasid)	(KLD_BASE(nasid) + KLI_KERN_VARS)
#define	KLD_KERN_XP(nasid)	(KLD_BASE(nasid) + KLI_KERN_XP)
#define	KLD_KERN_PARTID(nasid)	(KLD_BASE(nasid) + KLI_KERN_PARTID)

#ifndef CONFIG_IA64_SGI_SN2
#define KLCONFIG_OFFSET(nasid)	KLD_KLCONFIG(nasid)->offset
#else
#define KLCONFIG_OFFSET(nasid) \
	ia64_sn_get_klconfig_addr(nasid)
#endif /* CONFIG_IA64_SGI_SN2 */

#define KLCONFIG_ADDR(nasid)						\
	TO_NODE_CAC((nasid), KLCONFIG_OFFSET(nasid))
#define KLCONFIG_SIZE(nasid)	KLD_KLCONFIG(nasid)->size

#define GDA_ADDR(nasid)		KLD_GDA(nasid)->pointer
#define GDA_SIZE(nasid)		KLD_GDA(nasid)->size

#define NODE_OFFSET_TO_K0(_nasid, _off)					\
	(PAGE_OFFSET | NODE_OFFSET(_nasid) | (_off))

#endif /* __ASSEMBLY__ */

#endif /* _ASM_IA64_SN_ADDRS_H */
