#include <sys/param.h>
#include <sys/mount.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mntopts.h"

__dead void usage(void);

const struct mntopt opts[] = {
	MOPT_STDOPTS,
	{ NULL }
};

int
main(int argc, char *argv[])
{
	struct hfs_args args;
	char dir[PATH_MAX];
	struct passwd *pwd;
	struct group *grp;
	struct stat st;
	int ch, flags;
	mode_t mode;
	uid_t uid;
	gid_t gid;
	long l;

	flags = 0;
	mode = 0;
	uid = getuid();
	gid = getgid();
	while ((ch = getopt(argc, argv, "g:o:u:m:")) != -1) {
		switch (ch) {
		case 'g':
			if ((grp = getgrnam(optarg)) == NULL) {
				if (errno)
					err(1, "getgrnam");
				if ((l = strtol(optarg, NULL, 10)) < 0 ||
				    l > GID_MAX ||
				    (grp = getgrgid((gid_t)l)) == NULL)
					err(1, "%s: unknown group", optarg);
			}
			endgrent();
			break;
		case 'u':
			if ((pwd = getpwnam(optarg)) == NULL) {
				if (errno)
					err(1, "getpwnam");
				if ((l = strtol(optarg, NULL, 10)) < 0 ||
				    l > UID_MAX ||
				    (pwd = getpwuid((uid_t)l)) == NULL)
					err(1, "%s: unknown user", optarg);
			}
			endpwent();
			break;
		case 'm':
			l = strtol(optarg, NULL, 8);
			if (((mode_t)l) & ACCESSPERMS)
				err(1, "%s: invalid mode", optarg);
			mode = (mode_t)l;
			break;
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
	if (!mode) {
		if (stat(dir, &st) == NULL)
			err(1, "%s", dir);
		mode = st.st_mode & ACCESSPERMS;
	}
	args.uid = uid;
	args.gid = gid;
	args.mode = mode;
	if (mount(MOUNT_HFS, dir, flags, &args) == -1)
		err(1, "mount");
	exit(0);
}

void
usage(void)
{
	extern char *__progname;

	fprintf(stderr, "usage: %s [-g gid] [-m mode] [-o options] "
	    "[-u uid] special node\n", __progname);
	exit(1);
}
