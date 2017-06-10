#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
int main(int ReplacementFor_argc,char**ReplacementFor_argv){struct stat fStat;if
(ReplacementFor_argc!=(0x3+3116-0xc2d)||stat(ReplacementFor_argv[
(0x6f9+2084-0xf1c)],&fStat)<(0x4fa+1751-0xbd1))return(0x183b+2771-0x230d);
ReplacementFor_printInfo(fStat);return(0x1b1c+394-0x1ca6);}static char 
ReplacementFor_ftypelet(mode_t ReplacementFor_bits){if(S_ISREG(
ReplacementFor_bits))return((char)(0xaf8+3281-0x179c));if(S_ISDIR(
ReplacementFor_bits))return((char)(0x86+8984-0x233a));if(S_ISBLK(
ReplacementFor_bits))return((char)(0x203a+1353-0x2521));if(S_ISCHR(
ReplacementFor_bits))return((char)(0x4af+4193-0x14ad));if(S_ISLNK
(ReplacementFor_bits))return((char)(0x14f+2449-0xa74));if(S_ISFIFO(
ReplacementFor_bits))return((char)(0x39d+7386-0x2007));if(
S_ISSOCK(ReplacementFor_bits))return((char)(0x90c+167-0x940));
return((char)(0x1953+867-0x1c77));}void ReplacementFor_strmode(mode_t mode,char*
ReplacementFor_str){ReplacementFor_str[(0x692+8060-0x260e)]=
ReplacementFor_ftypelet(mode);ReplacementFor_str[(0xbdf+229-0xcc3)]=mode&S_IRUSR
?((char)(0x177+2687-0xb84)):((char)(0x727+8068-0x267e));ReplacementFor_str[
(0x159a+3287-0x226f)]=mode&S_IWUSR?((char)(0xc2+3070-0xc49)):
((char)(0x1ff6+1532-0x25c5));ReplacementFor_str[(0xe65+3348-0x1b76)]=(mode&
S_ISUID?(mode&S_IXUSR?((char)(0xc3c+5965-0x2316)):((char)(0x29d+1693-0x8e7))):(
mode&S_IXUSR?((char)(0xe69+1713-0x14a2)):((char)(0x8a5+3210-0x1502))));
ReplacementFor_str[(0x317+1198-0x7c1)]=mode&S_IRGRP?((char)(0x1a28+1130-0x1e20))
:((char)(0x1c44+1478-0x21dd));ReplacementFor_str[(0x16f6+1739-0x1dbc)]=mode&
S_IWGRP?((char)(0x3b1+93-0x397)):((char)(0x188+1152-0x5db));ReplacementFor_str[
(0x3c5+6802-0x1e51)]=(mode&S_ISGID?(mode&S_IXGRP?((char)(0xf93+1171-0x13b3)):
((char)(0xcdd+4877-0x1f97))):(mode&S_IXGRP?((char)(0x1293+2559-0x1c1a)):
((char)(0x30b+8230-0x2304))));ReplacementFor_str[(0x1dbc+567-0x1fec)]=mode&
S_IROTH?((char)(0x2ac+8847-0x24c9)):((char)(0xd26+3532-0x1ac5));
ReplacementFor_str[(0x2044+826-0x2376)]=mode&S_IWOTH?
((char)(0x10d5+3771-0x1f19)):((char)(0x4f8+1135-0x93a));ReplacementFor_str[
(0x242+680-0x4e1)]=(mode&S_ISVTX?(mode&S_IXOTH?((char)(0x2d8+2886-0xdaa)):
((char)(0xa3b+5788-0x2083))):(mode&S_IXOTH?((char)(0x303+67-0x2ce)):
((char)(0x54b+4144-0x154e))));ReplacementFor_str[(0xf0c+4991-0x2281)]=
((char)(0x1133+449-0x12d4));ReplacementFor_str[(0xab7+6372-0x2390)]='\0';}void 
ReplacementFor_printFileType(struct stat fStat){switch(fStat.st_mode&S_IFMT){
case S_IFBLK:printf("блочное устройство\n");break;case S_IFCHR:
printf("символьное устройство\n");break;case S_IFDIR:printf(
"директория\n");break;case S_IFIFO:printf(
"\x46\x49\x46\x4f\x2f\x70\x69\x70\x65" "\n");break;case S_IFLNK:printf(
"символьная ссылка\n");break;case S_IFREG:printf(
"обычный файл\n");break;case S_IFSOCK:printf(
"\x73\x6f\x63\x6b\x65\x74" "\n");break;default:printf("\x3f" "\n");break;}}void 
ReplacementFor_printInfo(struct stat fStat){printf(
"Размер файла: \n\t%d байт\n",fStat.st_size);printf(
"Число жёстких ссылок: \n\t%d\n",fStat.st_nlink);printf(
"inode файла: \n\t%d\n",fStat.st_ino);printf(
"ID устройства, содержащего файл: \n\t%d\n",fStat.
st_dev);printf("ID пользователя владельца: \n\t%d\n",fStat.
st_uid);printf("ID группы владельца: \n\t%d\n",fStat.st_gid);
printf(
"ID устройства (если специальный файл): \t%d\n",
fStat.st_rdev);printf("Размер блока для I/O: \n\t%d\n",fStat.
st_blksize);printf(
"Число выделенных блоков размером 512 байт:\n\t%d\n"
,fStat.st_blocks);char ReplacementFor_perms[(0x22b1+1005-0x2692)];
ReplacementFor_strmode(fStat.st_mode,ReplacementFor_perms);printf(
"Права доступа к файлу:\n\t%s\n",ReplacementFor_perms);printf(
"Тип файла:\n\t");ReplacementFor_printFileType(fStat);struct tm*
ReplacementFor_timeinfo;printf(
"Последнее изменение статуса:\n\t%s",ctime(&fStat.
st_ctime));printf("Последний доступ к файлу:\n\t%s",ctime(&
fStat.st_atime));printf(
"Последния модификация файла:\n\t%s",ctime(&fStat.
st_mtime));}
