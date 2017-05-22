#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

int main(int argc, char **argv)
{
    struct stat fStat;
    if(argc != 2 || stat(argv[1],&fStat) < 0)    
        return 1;
    printInfo(fStat);	 
    return 0;
}
/* Return a character indicating the type of file described by
   file mode BITS:
   '-' regular file
   'b' block special file
   'c' character special file
   'C' high performance ("contiguous data") file
   'd' directory
   'D' door
   'l' symbolic link
   'm' multiplexed file (7th edition Unix; obsolete)
   'n' network special file (HP-UX)
   'p' fifo (named pipe)
   'P' port
   's' socket
   'w' whiteout (4.4BSD)
   '?' some other file type  */

static char
ftypelet (mode_t bits)
{
  /* These are the most common, so test for them first.  */
  if (S_ISREG (bits))
    return '-';
  if (S_ISDIR (bits))
    return 'd';

  /* Other letters standardized by POSIX 1003.1-2004.  */
  if (S_ISBLK (bits))
    return 'b';
  if (S_ISCHR (bits))
    return 'c';
  if (S_ISLNK (bits))
    return 'l';
  if (S_ISFIFO (bits))
    return 'p';

  /* Other file types (though not letters) standardized by POSIX.  */
  if (S_ISSOCK (bits))
    return 's';

  /* Nonstandard file types.  */
  /*if (S_ISCTG (bits))
    return 'C';
  if (S_ISDOOR (bits))
    return 'D';
  if (S_ISMPB (bits) || S_ISMPC (bits) || S_ISMPX (bits))
    return 'm';
  if (S_ISNWK (bits))
    return 'n';
  if (S_ISPORT (bits))
    return 'P';
  if (S_ISWHT (bits))
    return 'w';*/

  return '?';
}

/* Like filemodestring, but rely only on MODE.  */
void
strmode (mode_t mode, char *str)
{
  str[0] = ftypelet (mode);
  str[1] = mode & S_IRUSR ? 'r' : '-';
  str[2] = mode & S_IWUSR ? 'w' : '-';
  str[3] = (mode & S_ISUID
            ? (mode & S_IXUSR ? 's' : 'S')
            : (mode & S_IXUSR ? 'x' : '-'));
  str[4] = mode & S_IRGRP ? 'r' : '-';
  str[5] = mode & S_IWGRP ? 'w' : '-';
  str[6] = (mode & S_ISGID
            ? (mode & S_IXGRP ? 's' : 'S')
            : (mode & S_IXGRP ? 'x' : '-'));
  str[7] = mode & S_IROTH ? 'r' : '-';
  str[8] = mode & S_IWOTH ? 'w' : '-';
  str[9] = (mode & S_ISVTX
            ? (mode & S_IXOTH ? 't' : 'T')
            : (mode & S_IXOTH ? 'x' : '-'));
  str[10] = ' ';
  str[11] = '\0';
}

void printFileType(struct stat fStat)
{ 
    switch (fStat.st_mode & S_IFMT) {
    	case S_IFBLK:
		printf("block device\n");
		break;
    	case S_IFCHR:
		printf("char device\n");
		break;
    	case S_IFDIR:
		printf("directory\n");
		break;
    	case S_IFIFO:
		printf("FIFO/pipe\n");
		break;
    	case S_IFLNK:
		printf("symbolic link\n");
		break;
    	case S_IFREG:
		printf("regular file\n");
		break;
    	case S_IFSOCK:
		printf("socket\n");
		break;
    	default:
		printf("?\n");
		break;
    }
}

void printInfo(struct stat fStat)
{
    printf("File size: \n\t%d byte\n",fStat.st_size);
    printf("Number of hard links: \n\t%d\n",fStat.st_nlink);
    printf("inode of file: \n\t%d\n",fStat.st_ino);
    printf("ID device сontaining the file: \n\t%d\n",fStat.st_dev);
    printf("ID owner user: \n\t%d\n",fStat.st_uid);
    printf("ID owner group: \n\t%d\n",fStat.st_gid);
    printf("ID of device (if special file): \t%d\n",fStat.st_rdev);
    printf("Size of block for I/O: \n\t%d\n",fStat.st_blksize);
    printf("Count of blocks (size 512 byte):\n\t%d\n",fStat.st_blocks);
    char perms[12];
    strmode (fStat.st_mode, perms);
    printf("Access to file:\n\t%s\n", perms);
    printf("Type of file:\n\t");
    printFileType(fStat);
    struct tm * timeinfo;
    printf("Last status change:\n\t%s", ctime(&fStat.st_ctime));
    printf("Last access to file:\n\t%s", ctime(&fStat.st_atime));
    printf("Last modification to file:\n\t%s", ctime(&fStat.st_mtime));
}
