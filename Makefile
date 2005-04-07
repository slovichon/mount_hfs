# $Id$

PROG=		mount_hfs
SRCS=		mount_hfs.c getmntopts.c
MAN=		mount_hfs.8

MOUNT=		/usr/src/sbin/mount
CFLAGS+=	-I${MOUNT}
.PATH:		${MOUNT}

.include <bsd.prog.mk>
