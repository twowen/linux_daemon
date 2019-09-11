/*
 * daemon.c
 *
 * Copyright 2019 Owen Wu <twowen@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

int main(int argc, char **argv)
{
	/* Process ID and Session ID */
	pid_t pid, sid;
	/* File descriptor */
	FILE *fp;

	/* Fork off the parent process */
	pid = fork();
	if (pid < 0)
	{
		printf("fork failed!\n");
		exit(EXIT_FAILURE);
	}

	/* Parent process */
	if (pid > 0)
	{
		printf("Child process ID: %d\n", pid);
		printf("Exit the parent process\n");
		exit(EXIT_SUCCESS);
	}

	/* Child process */
	/* Change the file mode mask */
	umask(0);

	/* (Optional) Open any logs here */

	/* Create a new session ID */
	sid = setsid();
	if (sid < 0)
	{
		printf("setsid failed!\n");
		exit(EXIT_FAILURE);
	}

	/* Change current working directory */
	if ((chdir("/")) < 0)
	{
		printf("Change path to / failed!\n");
		exit(EXIT_FAILURE);
	}

	/* Close stdin stdout stderr */
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	/* Daemon specific initialization goes here */
	/* Open a log file in write mode */
	fp = fopen("/dev/shm/Log.txt", "w+");
	if (NULL == fp)
	{
		printf("fopen failed!\n");
		exit(EXIT_FAILURE);
	}

	/* The big loop */
	while (1)
	{
		/* Do some task here ... */
		fprintf(fp, "My pid is %d\n", getpid());
		fflush(fp);
		/* Wait 3 seconds */
		sleep(3);
	}

	fclose(fp);

	return 0;
}
