/**
 * Seccomp Library test program
 *
 * Copyright (c) 2012 Red Hat <pmoore@redhat.com>
 * Author: Paul Moore <pmoore@redhat.com>
 */

/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>

#include <seccomp.h>

int main(int argc, char *argv[])
{
	int rc;

	rc = seccomp_init(SCMP_ACT_DENY);
	if (rc != 0)
		return rc;

	rc = seccomp_add_syscall(SCMP_ACT_ALLOW, SCMP_SYS(read), 2,
				 0, SCMP_CMP_EQ, 0, /* stdin */
				 1, SCMP_CMP_NE, NULL);
	if (rc != 0)
		return rc;
	rc = seccomp_add_syscall(SCMP_ACT_ALLOW, SCMP_SYS(read), 2,
				 0, SCMP_CMP_EQ, 0, /* stdin */
				 2, SCMP_CMP_GT, SSIZE_MAX);
	if (rc != 0)
		return rc;

	rc = seccomp_add_syscall(SCMP_ACT_ALLOW, SCMP_SYS(write), 1,
				 0, SCMP_CMP_EQ, 1 /* stdout */);
	if (rc != 0)
		return rc;
	rc = seccomp_add_syscall(SCMP_ACT_ALLOW, SCMP_SYS(write), 1,
				 0, SCMP_CMP_EQ, 2 /* stderr */);
	if (rc != 0)
		return rc;

	rc = seccomp_gen_bpf(STDOUT_FILENO);
	if (rc != 0)
		return rc;

	seccomp_release();
	return rc;
}
