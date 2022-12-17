#include "filesys/filesys.h"
#include <debug.h>
#include <stdio.h>
#include <string.h>
#include "filesys/file.h"
#include "filesys/free-map.h"
#include "filesys/inode.h"
#include "filesys/directory.h"
#include "devices/disk.h"
#include "filesys/fat.h"
#include "threads/thread.h"


/* The disk that contains the file system. */
struct disk *filesys_disk;

static void do_format(void);

/* Initializes the file system module.
 * If FORMAT is true, reformats the file system. */
void filesys_init(bool format)
{
	filesys_disk = disk_get(0, 1);
	if (filesys_disk == NULL)
		PANIC("hd0:1 (hdb) not present, file system initialization failed");

	inode_init();

#ifdef EFILESYS
	fat_init();

	if (format)
		do_format();

	fat_open();
	struct thread *initial_thread = thread_current ();
	initial_thread->cur_dir = dir_open_root();
#else
	/* Original FS */
	free_map_init();

	if (format)
		do_format();

	free_map_open();
#endif
}

/* Shuts down the file system module, writing any unwritten data
 * to disk. */
void filesys_done(void)
{
	/* Original FS */
#ifdef EFILESYS
	fat_close();
#else
	free_map_close();
#endif
}


struct dir* parse_path (char *path_name, char *file_name) {
	struct dir *dir;
	if (path_name == NULL || file_name == NULL)
		goto fail;
	if (strlen(path_name) == 0)
		return NULL;
	/* PATH_NAME의 절대/상대경로에 따른 디렉터리 정보 저장 (구현)*/
	if (path_name[0] == '/') {
		//절대
		dir->inode;}
	char *token, *nextToken, *savePtr;
	token = strtok_r (path_name, "/", &savePtr);
	nextToken = strtok_r (NULL, "/", &savePtr);
	while (token != NULL && nextToken != NULL){
	/* dir에서 token이름의 파일을 검색하여 inode의 정보를 저장*/
	/* inode가 파일일 경우 NULL 반환 */
	/* dir의 디렉터리 정보를 메모리에서 해지 */
	/* inode의 디렉터리 정보를 dir에 저장 */
	/* token에 검색할 경로 이름 저장 */
	}
	/* token의 파일 이름을 file_name에 저장
	   char s[] = "  String to  tokenize. ";
   char *token, *save_ptr;

   for (token = strtok_r (s, " ", &save_ptr); token != NULL;
   token = strtok_r (NULL, " ", &save_ptr))
   printf ("'%s'\n", token);

outputs:

'String'
'to'
'tokenize.'
	/* dir 정보 반환 */
	return dir;
	fail :
		return -1;
}

/* Creates a file named NAME with the given INITIAL_SIZE.
 * Returns true if successful, false otherwise.
 * Fails if a file named NAME already exists,
 * or if internal memory allocation fails. */
/* 지정된 INITIAL_SIZE를 사용하여 NAME이라는 이름의 파일을 만든다.
   성공하면 true를 반환하고, 그렇지 않으면 false를 반환
   이름이 NAME인 파일이 이미 있거나 내부 메모리 할당이 실패한 경우 실패 */
bool filesys_create(const char *name, off_t initial_size)
{

	cluster_t inode_cluster = fat_create_chain(0);
	disk_sector_t inode_sector = cluster_to_sector(inode_cluster);

	struct dir *dir = dir_open_root();
	bool success = (dir != NULL && inode_create(inode_sector, initial_size, INODE_FILE) && dir_add(dir, name, inode_sector));
					// && free_map_allocate (1, &inode_sector)
					// && fat_create_chain(0)
	if (!success && inode_sector != 0)
		// free_map_release (inode_sector, 1);
		fat_remove_chain(inode_cluster, 0);
	dir_close(dir);
	return success;
}

/* Opens the file with the given NAME.
 * Returns the new file if successful or a null pointer
 * otherwise.
 * Fails if no file named NAME exists,
 * or if an internal memory allocation fails. */
/* 성공하면 새 파일을 반환하고 그렇지 않으면 null 포인터를 반환
   이름이 NAME인 파일이 없거나 내부 메모리 할당이 실패할 경우 실패*/
struct file *
filesys_open(const char *name)
{
	struct dir *dir = dir_open_root();
	struct inode *inode = NULL;
	// char cp_name[15] = *name;
	printf("===========> file name :: %s \n", name);
	// printf("===========> dir :: %d \n", dir.inode.sector);
	// parse_path(cp_name)

	if (dir != NULL)
		dir_lookup(dir, name, &inode);
	dir_close(dir);
	printf("===========>  :: %p \n", dir);

	return file_open(inode);
}

/* Deletes the file named NAME.
 * Returns true if successful, false on failure.
 * Fails if no file named NAME exists,
 * or if an internal memory allocation fails. */
bool filesys_remove(const char *name)
{
	struct dir *dir = dir_open_root();
	bool success = dir != NULL && dir_remove(dir, name);
	dir_close(dir);

	return success;
}

/* Formats the file system. */
static void
do_format(void)
{
	printf("Formatting file system...");

#ifdef EFILESYS
	/* Create FAT and save it to the disk. */
	fat_create();
	fat_close();
#else
	free_map_create();
	if (!dir_create(ROOT_DIR_SECTOR, 16))
		PANIC("root directory creation failed");
	free_map_close();
#endif

	printf("done.\n");
}
