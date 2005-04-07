#include <sys/param.h>
#include <sys/mount.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mntopts.h"

const struct mntopt opts[] = {
	MOPT_STDOPTS,
	{ NULL }
};

__dead void usage(void);

int
main(int argc, char *argv[])
{
	struct hfs_args args;
	char dir[PATH_MAX];
	int ch, flags = 0;

	while ((ch = getopt(argc, argv, "")) != -1) {
		switch (ch) {
		case 'o':
			getmntopts(optarg, opts, &flags);
			break;
		default:
			usage();
			/* NOTREACHED */
		}
	}
	argc -= optind;
	argv += optind;

	args.fspec = argv[0];
	if (realpath(argv[1], dir) == -1)
		err(1, "%s", argv[1]);
	if (mount(MOUNT_HFS, dir, flags, &args) == -1)
		err(1, "mount");
	exit(0);
}

void
usage(void)
{
	extern char *__progname;

	fprintf(stderr, "usage: %s [-o options] special node\n",
	    __progname);
	exit(1);
}
