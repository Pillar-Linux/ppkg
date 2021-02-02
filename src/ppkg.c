/* Pillar package-script(ppkg) */
/* Tony Martinez 2021 */

/* Libraries */
#include <dirent.h>
#include <getopt.h>
#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <toml.h>

/* Headers */
#include "ppkg.h"

/* default package struct follows config file documentation/examples*/
typedef struct package_struct{
	char	*author, 
		*descr,
	     	*license,
	     	*home,
	     	*url,
	     	*vcs,
	     	*version,
		compile[PACKAGE_CONF_ARRAY_SIZE][PACKAGE_CONF_ARRAY_WIDTH],		/* stores PACKAGE_CONF_ARRAY_SIZE lines of PACKAGE_CONF_ARRAY_WIDTH chars */
		install[PACKAGE_CONF_ARRAY_SIZE][PACKAGE_CONF_ARRAY_WIDTH],		/* stores PACKAGE_CONF_ARRAY_SIZE lines of PACKAGE_CONF_ARRAY_WIDTH chars */
		uninstall[PACKAGE_CONF_ARRAY_SIZE][PACKAGE_CONF_ARRAY_WIDTH];		/* stores PACKAGE_CONF_ARRAY_SIZE lines of PACKAGE_CONF_ARRAY_WIDTH chars */
}package_struct;

/* reads input parameters and outputs corresponding field */
package_struct toml_read(char *package, int debug_flag)
{
	FILE 	*fp;
	char 	errbuff[1024],
		filepath[PATH_MAX];
	int 	i;			/* loop variable */
	package_struct package_conf;	/* package conf struct */

	/* debug */
	DEBUG_FUNCTION_PRINT

	/* build package file path */
	strcpy(filepath, PACKAGE_PATH_DEFAULT);
	strcat(filepath, package);
	strcat(filepath, "/");
	strcat(filepath, ".conf");
	
	/* debug */
	if (debug_flag == 1)
	{
		printf("debug> package: %s\n", package);
		printf("debug> filepath: %s\n", filepath);
	}

	fp = fopen(filepath, "r");
	if (!fp)
	{
		printf("cannot open %s .conf file\n", package);
		exit(1);
	}

	/* file */
	toml_table_t* conf = toml_parse_file(fp, errbuff, sizeof(errbuff));	/* conf variables refers to .conf being loaded */
	fclose(fp);
	if (!conf)
	{
		printf("cannot parse %s\n", errbuff);
		exit(1);
	}

	/* table */
	toml_table_t* conf_package = toml_table_in(conf, package);	/* loads [program] from .conf file */
	if (!conf_package)
	{
		printf("missing [%s] in .conf", package);
		exit(1);
	}

	/* debug */
	if (debug_flag == 1)
	{
		printf("debug> [%s]\n", package);
		for (i = 0; ; i++)
		{
			if (!toml_key_in(conf_package, i))
				break;
			printf("debug>    %s\n", toml_key_in(conf_package, i));
		}
	}

	/* extracting values from [program] and storing to package conf struct */
	/* author */
	toml_datum_t conf_package_author = toml_string_in(conf_package, "author");
	if (!conf_package_author.ok)
		package_conf.author = NULL;
	else
		package_conf.author = conf_package_author.u.s;
	/* descr */
	toml_datum_t conf_package_descr = toml_string_in(conf_package, "descr");
	if (!conf_package_descr.ok)
		package_conf.descr = NULL;
	else
		package_conf.descr = conf_package_descr.u.s;
	/* license */
	toml_datum_t conf_package_license = toml_string_in(conf_package, "license");
	if (!conf_package_license.ok)
		package_conf.license = NULL;
	else
		package_conf.license = conf_package_license.u.s;
	/* home */
	toml_datum_t conf_package_home = toml_string_in(conf_package, "home");
	if (!conf_package_home.ok)
		package_conf.home = NULL;
	else
		package_conf.home = conf_package_home.u.s;
	/* url */
	toml_datum_t conf_package_url = toml_string_in(conf_package, "url");
	if (!conf_package_url.ok)
	{
		printf("missing url %s in .conf\n", package);
		exit(1);
	}
	else
		package_conf.url = conf_package_url.u.s;
	/* vcs */
	toml_datum_t conf_package_vcs = toml_string_in(conf_package, "vcs");
	if (!conf_package_vcs.ok)
	{
		printf("missing url in %s.conf\n", package);
		exit(1);
	}
	else
		package_conf.vcs = conf_package_vcs.u.s;
	/* version */
	toml_datum_t conf_package_version = toml_string_in(conf_package, "version");
	if (!conf_package_version.ok)
	{
		printf("missing version in %s.conf\n", package);
            	exit(1);
	}
	else
		package_conf.version = conf_package_version.u.s;
	/* compile */
	toml_array_t* conf_package_compile = toml_array_in(conf_package, "compile");
	if (!conf_package_compile)
	{
		printf("missing compile options in %s.conf\n", package);
		exit(1);
	}
	/* install */
	toml_array_t* conf_package_install = toml_array_in(conf_package, "install");
	if (!conf_package_install)
	{
		printf("missing install options in %s.conf\n", package);
		exit(1);
	}
	/* uninstall */
	toml_array_t* conf_package_uninstall = toml_array_in(conf_package, "uninstall");
	if (!conf_package_uninstall)
	{
		printf("missing uninstall options in %s.conf\n", package);
		exit(1);
	}
	/* compile */
	for (i = 0; i < PACKAGE_CONF_ARRAY_SIZE; i++) 
	{
		toml_datum_t package_conf_compile = toml_string_at(conf_package_compile, i);
		if (!package_conf_compile.ok || strlen(package_conf_compile.u.s) == 0)
			break;
		strcpy(package_conf.compile[i], package_conf_compile.u.s);
	}
	/* install */ 
	for (i = 0; i < PACKAGE_CONF_ARRAY_SIZE; i++) 
	{
		toml_datum_t package_conf_install = toml_string_at(conf_package_install, i);
		if (!package_conf_install.ok || strlen(package_conf_install.u.s) == 0)
			break;
		strcpy(package_conf.install[i], package_conf_install.u.s);
	}
	/* uninstall */
	for (i = 0; i < PACKAGE_CONF_ARRAY_SIZE; i++) 
	{
		toml_datum_t package_conf_uninstall = toml_string_at(conf_package_uninstall, i);
		if (!package_conf_uninstall.ok || strlen(package_conf_uninstall.u.s) == 0)
			break;
		strcpy(package_conf.uninstall[i], package_conf_uninstall.u.s);
	}

	/* debug */
	if (debug_flag == 1)
	{
		printf("debug> package_conf.author: %s\n", package_conf.author);
		printf("debug> package_conf.descr: %s\n", package_conf.descr);
		printf("debug> package_conf.license: %s\n", package_conf.license);
		printf("debug> package_conf.home: %s\n", package_conf.home);
		printf("debug> package_conf.url: %s\n", package_conf.url);
		printf("debug> package_conf.vcs: %s\n", package_conf.vcs);
		printf("debug> package_conf.version: %s\n", package_conf.version);
		printf("debug> package_conf.compile:\n");
		for (i = 0; i < PACKAGE_CONF_ARRAY_SIZE; i++) 
		{
			if (strlen(package_conf.compile[i]) == 0)
				break;
			printf("debug>    %s\n", package_conf.compile[i]);
		}
		printf("debug> package_conf.install:\n");
		for (i = 0; i < PACKAGE_CONF_ARRAY_SIZE; i++) 
		{
			if (strlen(package_conf.install[i]) == 0)
				break;
			printf("debug>    %s\n", package_conf.install[i]);
		}
		printf("debug> package_conf.uninstall:\n");
		for (i = 0; i < PACKAGE_CONF_ARRAY_SIZE - 1; i++) 
		{
			if (strlen(package_conf.uninstall[i]) == 0)
				break;
			printf("debug>    %s\n", package_conf.uninstall[i]);
		}
	}

	return package_conf;

	/* free memory allocations */
	free(conf_package_author.u.s);
	free(conf_package_descr.u.s);
	free(conf_package_license.u.s);
	free(conf_package_home.u.s);
	free(conf_package_url.u.s);
	free(conf_package_vcs.u.s);
	free(conf_package_version.u.s);
	
	toml_free(conf);
}

int exec_command(char *command)
{
	FILE *fpcommand;

	fpcommand = popen(command, "r");
	if (fpcommand == NULL)
	{
		printf("ERROR: command: %s failed run\n", command);
		exit(1);
	}
	pclose(fpcommand);
	
	return 0;
}

int create_directory(char filepath[PATH_MAX])
{
	DIR *dir;
	dir = opendir(filepath);
	if (dir)
	{
		closedir(dir);
		return 0;
	}
	else if (mkdir(filepath, 0750) == -1)
	{
		printf("failed to create directory: %s\n", filepath);
		return 1;
	}
	return 0;
}

int list_packages(char *package, int debug_flag)
{
	/* directory variables */
        DIR 	*dir;				/* directory pointer */
	FILE 	*fp;				/* file pointer */
        struct 	dirent *ent;			/* dirent object */
        char 	package_path[PATH_MAX];		/* stores package path */

	/* debug */
	DEBUG_FUNCTION_PRINT
	if (debug_flag == 1)
		printf("debug> package: %s\n", package);

	if (strcmp(package, "all") == 0)
	{
		dir = opendir(PACKAGE_PATH_DEFAULT);
		if (!dir)
			printf("unable to open directory: %s", PACKAGE_PATH_DEFAULT);

		while ((ent = readdir(dir)) != NULL)
		{
			if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
			{
				/* builds package path */
				strcpy(package_path, PACKAGE_PATH_DEFAULT);
				strcat(package_path, ent->d_name);
				strcat(package_path, "/");
				strcat(package_path, ".conf");
				
				/* debug */
				if (debug_flag == 1)
				{
					printf("debug> folder: %s\n", ent->d_name);
					printf("debug> package_path: %s\n", package_path);
				}

				if ((fp = fopen(package_path, "r")))
				{
					fclose(fp);
					printf("%s\n", ent->d_name);
				}
				strcpy(package_path, "");
			}
		}
	}
	else 
	{
		/* finishes building package path */
		package_struct package_conf = toml_read(package, debug_flag);
		printf("[%s] \n  version: %s \n  author: %s \n  license: %s \n  description: %s \n  url: %s \n", package, package_conf.version, package_conf.author, package_conf.license, package_conf.descr, package_conf.url);
	}

	return 0;
}

int build_package(char *package, int debug_flag)
{
	char	*bname,
		*filename, 				/* stores downloaded filename */
		*ext,					/* stores downloaded file extension */
		command[ARG_MAX],			/* command string */	
		package_path[PATH_MAX],			/* file path */
		*package_download,			/* curl output file */
		buff[PATH_MAX];				/* package filepath */
	int	i;					/* loop variable */

	package_struct package_conf = toml_read(package, debug_flag);	/* stores package toml .conf information */

	/* build package path */
	strcpy(package_path, PACKAGE_PATH_DEFAULT);
	strcat(package_path, package);

	/* file name from url */
	bname = strdup(package_conf.url);
	filename = basename(bname);
	if (!filename)
	{
		printf("failed to get filename from url\n");
		exit(1);
	}

	/* file extension from url */
	ext = strrchr(package_conf.url, '.');
	if (!ext)
	{
		printf("downloaded file has no extension");
		exit(1);
	}

	/* build package_download name */
	package_download = package;
	strcat(package_download, "_");
	strcat(package_download, package_conf.version);

	/* debug */
	DEBUG_FUNCTION_PRINT
	if (debug_flag == 1)
	{
		printf("debug> working directory: %s\n", package_path);
		printf("debug> url: %s\n", package_conf.url);
		printf("debug> file: %s\n", filename);
		printf("debug> file extension: %s\n", ext);
	}

	printf("building package: %s\n", package);

	/* enter package_path */
	chdir(package_path);

	/* supported vcs git, svn, none */
	/* git */
	if (strcmp(package_conf.vcs, "git") == 0)
	{
		/* git pull package_url src */
		strcpy(command, "git clone --single-branch -b ");
		strcat(command, package_conf.version);
		strcat(command, " ");
		strcat(command, package_conf.url);
		strcat(command, " ");
		strcat(command, PACKAGE_BUILD_SRC);
		exec_command(command);
	}

	/* svn */
	else if (strcmp(package_conf.vcs, "svn") == 0)
	{
		/* svn checkout package_url src */
		strcpy(command, "svn checkout ");
		strcat(command, package_conf.url);
		strcat(command, " ");
		strcat(command, PACKAGE_BUILD_SRC);
		exec_command(command);
	}
	
	/* none */
	else if (strcmp(package_conf.vcs, "none") == 0)
	{
		/* clean up src dir */
		strcpy(command, "rm -rf ");
		strcat(command, PACKAGE_BUILD_SRC);
		exec_command(command);
		
		/* debug */
		if (debug_flag == 1)
			strcpy(command, "curl ");
		else
			strcpy(command, "curl -s ");
		strcat(command, package_conf.url);
		strcat(command, " ");
		strcat(command, "-o ");
		strcat(command, package_download);

		/* debug */
		if (debug_flag == 1)
			printf("debug> command: %s\n", command);
		exec_command(command);

		/* .tar, .tar.gz */
		if ((strcmp(ext, "tar")) == 0 || (strstr(filename, "tar.gz")) != NULL)
		{
			create_directory(PACKAGE_BUILD_SRC);
			/* debug */
			if (debug_flag == 1)
				strcpy(command, "tar -xf ");
			else
				strcpy(command, "tar -xf ");
			strcat(command, package_download);
			strcat(command, " -C ");
			strcat(command, PACKAGE_BUILD_SRC);
			strcat(command, " --strip-components=1");
			/* debug */
			if (debug_flag == 1)
				printf("debug> command: %s\n", command);
			exec_command(command);
		}

		/* .zip */
		else if (strcmp(ext, "zip") == 0 || strstr(filename, ".zip") != NULL)
		{
			create_directory(PACKAGE_BUILD_SRC);
			/* debug */
			if (debug_flag == 1)
				strcpy(command, "bsdtar --strip-components=1 -xvf ");
			else
				strcpy(command, "bsdtar --strip-components=1 -xf ");
			strcat(command, package_download);
			strcat(command, " -C ");
			strcat(command, PACKAGE_BUILD_SRC);
			/* debug */
			if (debug_flag == 1)
				printf("debug> command: %s\n", command);
			exec_command(command);
		}
	}

	chdir(PACKAGE_BUILD_SRC);
	
	/* debug */
	if (debug_flag == 1)
	{
		printf("debug> working directory: %s\n", getcwd(buff, sizeof(buff)));
		printf("debug> command compile:\n");
	}

	for (i = 0; i < PACKAGE_CONF_ARRAY_SIZE; i++)
	{
		if (strlen(package_conf.compile[i]) == 0)
			break;

		strcpy(command, package_conf.compile[i]);

		/* debug */
		if (debug_flag == 1)
			printf("debug>    %s\n", command);
		
		/* compile directives directly use shell commands */
		/* debug */
		if (debug_flag == 1)
			system(command);
		else
		{
			strcat(command, " > /dev/null");
			system(command);
		}
	}

	printf("built package: %s\n", package);

	return 0;	
}

int install_package(char *package, int debug_flag)
{
	char	buff[PATH_MAX],		/* file path buffer */
		command[ARG_MAX];	/* command buffer */
	int 	i;			/* loop variable */
	package_struct package_conf = toml_read(package, debug_flag);	/* initialize struct containing toml data */

	/* build package */
	build_package(package, debug_flag);

	/* debug */
	DEBUG_FUNCTION_PRINT
	if (debug_flag == 1)
	{
		printf("debug> working directory: %s\n", getcwd(buff, sizeof(buff)));
		printf("debug> command install:\n");
	}

	for (i = 0; i < PACKAGE_CONF_ARRAY_SIZE; i++)
	{
		if (strlen(package_conf.install[i]) == 0)
			break;
		
		strcpy(command, package_conf.install[i]);

		/* debug */
		if (debug_flag == 1)
			printf("debug>    %s\n", command);
		
		/* install directives directly use shell commands */
		/* debug */
		if (debug_flag == 1)
			system(command);
		else
		{
			strcat(command, " > /dev/null");
			system(command);
		}
	}

	return 0;
}

int uninstall_package(char *package, int debug_flag)
{
	char	buff[PATH_MAX],		/* file path buffer */
		command[ARG_MAX],	/* command buffer */
		package_path[PATH_MAX];	/* file path */
	int 	i;			/* loop variable */
	package_struct package_conf = toml_read(package, debug_flag);

	/* build package path */
	strcpy(package_path, PACKAGE_PATH_DEFAULT);
	strcat(package_path, package);
	strcat(package_path, "/");
	strcat(package_path, PACKAGE_BUILD_SRC);

	chdir(package_path);

	/* debug */
	DEBUG_FUNCTION_PRINT
	if (debug_flag == 1)
	{
		printf("debug> working directory: %s\n", getcwd(buff, sizeof(buff)));
		printf("debug> command uninstall:\n");
	}
	if (strlen(package_conf.uninstall[0]) == 0)
	{
			printf("no uninstall directives for package: %s\n", package);
			exit(1);
	}
	else
	{
		for (i = 0; i < PACKAGE_CONF_ARRAY_SIZE; i++)
		{
			if (strlen(package_conf.uninstall[i]) == 0)
				break;
			
			strcpy(command, package_conf.uninstall[i]);

			/* debug */
			if (debug_flag == 1)
				printf("debug>    %s\n", command);
			
			/* uninstall directives directly use shell commands */
			/* debug */
			if (debug_flag == 1)
				system(command);
			else
			{
				strcat(command, " > /dev/null");
				system(command);
			}
		}

		printf("uninstalled package: %s\n", package);
		return 0;
	}
}

int show_package(char *package, int debug_flag)
{
	DIR 	*dir;			/* directory pointer */
	FILE 	*fp;			/* file pointer */
	char 	package_path[PATH_MAX],	/* file path */
		buff;			/* temp buffer */

	/* build package path */
	strcpy(package_path, PACKAGE_PATH_DEFAULT);
	strcat(package_path, package);

	/* debug */
	DEBUG_FUNCTION_PRINT

	dir = opendir(package_path);
	if (!dir)
	{
		printf("package %s does not exists\n", package);
		exit(1);
	}
	closedir(dir);

	strcat(package_path, "/.conf");

	/* reads file into buffer and prints to stdout*/
	fp = fopen(package_path, "r");
	if (!fp)
	{
		printf("unable to open %s .conf\n", package);
		exit(1);
	}
	
	buff = fgetc(fp);
	while (buff != EOF)
	{
		printf("%c", buff);
		buff = fgetc(fp);
	}
	fclose(fp);

	return 0;
}

/* usage output */
void usage()
{
	printf(
	"Pillar package-script(ppkg):"
	"\nUsage: ppkg [options] ..."
	"\n    -h (package)      Show usage output"			/* usage output */
	"\n    -b (package)      Build package"				/* builds package only */
	"\n    -i (package)      Install package"			/* builds and installs package */
	"\n    -l (package)/all  List packages"				/* lists all packages */
	"\n    -r (package)      Remove package" 			/* removes specified package that is installed */
	"\n    -s (package)      Show package conf file"		/* shows config file for packages */
	"\n    -d                Debug output"				/* debug mode */
	"\n    -v                Version output"			/* version output */
	"\n");
}

int main (int argc, char **argv)
{
	int 		opt;			/* cli input */
	static int 	debug_flag;		/* verbose output flag */
	char 		*package;		/* stores package input */

	while ((opt = getopt(argc, argv, "hdvb:i:l:r:s:")) != -1)
		switch (opt)
		{
			case 'h':
				usage();
				break;
			case 'b':
				package = optarg;
				build_package(package, debug_flag);
				break;
			case 'i':
				package = optarg;
				install_package(package, debug_flag);
				break;
			case 'l':
				package = optarg;
				list_packages(package, debug_flag);
				break;
			case 'r':
				package = optarg;
				uninstall_package(package, debug_flag);
				break;
			case 's':
				package = optarg;
				show_package(package, debug_flag);
				break;
			case 'd':
				debug_flag = 1;
				break;
			case 'v':
				PACKAGE_VERSION_OUTPUT
				break;
			case '?':
				break;
			default:
				return 1;
		}

	return 0;
}
