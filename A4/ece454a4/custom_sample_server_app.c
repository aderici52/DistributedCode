/* Mahesh V. Tripunitara
   University of Waterloo
   Part of a simplified RPC implementation
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ece454rpc_types.h"
#include "khash.h"
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
//#include "ece454_fs.h"

#if 0
#define _DEBUG_1_
#endif





// Below variables define which folders the client can mount on
// Anything on or below these folders are ok
int NumOfMasterFolders;
char ** MasterFolders;




// Mount Hash Table
KHASH_MAP_INIT_INT(32, char)




/* We allocate a global variable for returns. However, 
 * r.return_val is allocated dynamically. We look to
 * the server_stub to deallocate after it returns the
 * response to the client. */

return_type r;

extern printRegisteredProcedures();
int checkFolderEqualOrBelow(char * folderNameToBeChecked);
int checkdir(char *dir, char * dirToFind, char * builtStr);

return_type fsMount(const int nparams, arg_type*a) {
    
    if(nparams != 1) {
		/* Error! */
		printf("\nError at Line: %d\n", __LINE__);
		r.return_val = NULL;
		r.return_size = 0;
		return r;
    }
	
	if(a->arg_size <= 2) {
		/* Error - We expected a folder name! */
		printf("\nError at Line: %d\n", __LINE__);
		r.return_val = NULL;
		r.return_size = 0;
		return r;
    }
	
	char * folderName = (char *)(a->arg_val);

	// We have the folder name to mount on so check if it is exactly the root folder we support
	// or anything below it.
	
	if ((checkFolderEqualOrBelow(folderName)) != 0) {
		/* Error - This server doesn't support that folder */
		/* Or atleast it doesn't have it in or equal to it's mountable folders list */
		printf("\nError at Line: %d\n", __LINE__);
		r.return_val = NULL;
		r.return_size = 0;
		return r;
	}
	
	// Passed all checks so add to the array that has client IP and folder mounted.
    
}

int checkFolderEqualOrBelow(char * folderNameToBeChecked) {
	
	int i;
	char * currentRootFolder;
	char resolved_path[5000]; 
	realpath(folderNameToBeChecked, resolved_path); 
	
	
	for(i = 0; i < NumOfMasterFolders; i++) {
		currentRootFolder = MasterFolders[i];
		int comparison_result = strcmp(resolved_path, currentRootFolder);
		
		if (comparison_result == 0) {
			// Found as root
			return 0;
		} else {
			// Need to recursively check - Experimental - verified to have some flaws
			
			/*if ((checkdir(currentRootFolder, resolved_path, currentRootFolder)) == 1) {
				return 0;
			}*/
		}
		
	}

	// Nope - the directory requested doesn't exist in any of the available directories
	return -1;
}

int checkdir(char *dir, char * dirToFind, char * builtStr)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
	
    if((dp = opendir(dir)) == NULL) {
		// Cannot open directory
        return -1;
    }
	
    chdir(dir);
    while((entry = readdir(dp)) != NULL) {
        lstat(entry->d_name,&statbuf);
        if(S_ISDIR(statbuf.st_mode)) {
			char subDir[5000];
			
			
            /* Found a directory, but ignore . and .. */
			if(strcmp(".",entry->d_name) == 0 ||
			strcmp("..",entry->d_name) == 0)
                continue;
            /* printf("%s%s/\n","",entry->d_name); */
            /* Recurse at a new indent level */
            //printdir(entry->d_name);
			memcpy(subDir,builtStr, strlen(builtStr));
			strcat(subDir,"/");
			strcat(subDir,(entry->d_name));
			
			
			if(strcmp(dirToFind,subDir) == 0) {
				return 1;
			}
			
			checkdir(entry->d_name,dirToFind,subDir);
        }
        else continue;
    }
    chdir("..");
    closedir(dp);
}

return_type fsReadDir(const int nparams, arg_type*a) {
    
    // In this call, we have to check what is the <next file in the
    // directory listing that the client didn't yet see
    
    if(nparams != 1) {
	/* Error! */
	r.return_val = NULL;
	r.return_size = 0;
	return r;
    }

    if(a->arg_size != sizeof(int)) {
	/* Error! */
	r.return_val = NULL;
	r.return_size = 0;
	return r;
    }


    // TODO: Continue from here
    int client_folder_identifier = *(int *)(a->arg_val);
    int file_name_size;

    
    char *ret_char = (char *)malloc(sizeof(int));

    //*ret_int = i+j;

    //r.return_val = (void *)(ret_int);
    //r.return_size = ;

    return r;
}

return_type fsOpen(const int nparams, arg_type*a) {
    /*int flags = -1;

    if(mode == 0) {
	flags = O_RDONLY;
    }
    else if(mode == 1) {
	flags = O_WRONLY | O_CREAT;
    }

    return(open(fname, flags, S_IRWXU));*/
}

return_type fsClose(const int nparams, arg_type*a) {
    //return(close(fd));
}

return_type fsRead(const int nparams, arg_type*a) {
    //return(read(fd, buf, (size_t)count));
}

return_type fsWrite(const int nparams, arg_type*a) {
    //return(write(fd, buf, (size_t)count)); 
}

return_type fsRemove(const int nparams, arg_type*a) {
    //return(remove(name));
}



return_type add(const int nparams, arg_type* a)
{
    if(nparams != 2) {
	/* Error! */
	r.return_val = NULL;
	r.return_size = 0;
	return r;
    }

    if(a->arg_size != sizeof(int) ||
       a->next->arg_size != sizeof(int)) {
	/* Error! */
	r.return_val = NULL;
	r.return_size = 0;
	return r;
    }

    int i = *(int *)(a->arg_val);
    int j = *(int *)(a->next->arg_val);

    int *ret_int = (int *)malloc(sizeof(int));

    *ret_int = i+j;
	printf("The result of add is: %d \n\n", (*ret_int));
    r.return_val = (void *)(ret_int);
    r.return_size = sizeof(int);

    return r;
}

return_type pickFirst(const int nparams, arg_type *a) {
    /* Pick the first of a list of stuff & return it */
    if(nparams <= 0) {
	r.return_val = NULL;
	r.return_size = 0;
    }
    else {
	r.return_val = (void *)malloc(a->arg_size);
	memcpy(r.return_val, a->arg_val, a->arg_size);
	r.return_size = a->arg_size;
    }

    return r;
}

/* compar() below for use by qsort() */
int compar(const void *a, const void *b) {
    int i = *(int *)a;
    int j = *(int *)b;

    return i - j;
}

return_type max(const int nparams, arg_type *a) {
    /* Interprets the one arg as an array of integers.
     * Returns them sorted.
     * Note: we sort the arg a "in place". So no
     * new memory is allocated. */

    int *ia = (int *)malloc(a->arg_size);
    memcpy(ia, a->arg_val, a->arg_size);
    int n = (a->arg_size)/(sizeof(int));

    if(n < 0) {
	r.return_val = NULL;
	r.return_size = 0;
    }
    else {
	qsort(ia, n, sizeof(int), compar);
	r.return_size = (a->arg_size);
	r.return_val = (void *)ia;
    }

    return r;
}

return_type concatStr(const int nparams, arg_type *a) {
    /* Concatenates 5 strings and returns them.
     * We assume a is 5 null-terminated strings. */

    if(nparams != 5) {
	r.return_val = NULL;
	r.return_size = 0;
	return r;
    }

    /* else */
    char *s[5];
    int i;
    int totallen = 0;

    for(i = 0; i < 5; i++) {
	s[i] = a->arg_val;
	a = a->next;
	totallen += strlen(s[i]);
    }

    r.return_val = (void *)calloc(totallen + 1, sizeof(char));
    ((char *)(r.return_val))[totallen] = (char) 0;

    for(i = totallen = 0; i < 5; totallen += strlen(s[i++])) {
	memcpy(((char *)(r.return_val)) + totallen, s[i], strlen(s[i]));
    }

    r.return_size = strlen((char *)(r.return_val)) + 1;
    return r;
}

int main(int argc, char **argv) {
    /*register_procedure("addtwo", 2, add);
    register_procedure("pickFirst", 2, pickFirst);
    register_procedure("max_of_integer_array", 1, max);
    register_procedure("concatenate_five_strings", 5, concatStr);*/
    /*"fsReadDir"
    "fsOpen"
    "fsClose"
    "fsRead"
    "fsWrite"
    "fsRemove"  */
	
	
	register_procedure("fsMount", 1, fsMount);
	
	NumOfMasterFolders = (argc-1);
	//MasterFolders = argv;
	
	// Only allow mounting to provided folders (and their subfolders)
	int i;
	// First allocate overall array with each element of char*                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
    MasterFolders = (char **)(malloc(sizeof(char*) * (argc-1)));        
	
	for(i = 1; i < argc; i++){
		
		char resolved_path[5000]; 
		realpath(argv[i], resolved_path); 
		
		int length = strlen(resolved_path);
		MasterFolders[(i-1)] = malloc((length + 1) * sizeof(char));
		int j;
		// Cycle through all the chars and copy them in one by one
		for(j = 0; j <= length; j++){
			MasterFolders[(i-1)][j] = resolved_path[j];
		}
	}
	
	
	printf("\n\nSupporting the following folders:\n");
	for(i = 0; i < NumOfMasterFolders; i++) {
		printf("%s \n",(MasterFolders[i]));
	}


	/*int ret, is_missing;
    khiter_t k;
    khash_t(32) *h = kh_init(32);
    k = kh_put(32, h, 5, &ret);
    if (!ret) kh_del(32, h, k);
    kh_value(h, k) = 10;
    k = kh_get(32, h, 10);
    is_missing = (k == kh_end(h));
    k = kh_get(32, h, 5);
    kh_del(32, h, k);
    for (k = kh_begin(h); k != kh_end(h); ++k)
        if (kh_exist(h, k)) kh_value(h, k) = 1;
    kh_destroy(32, h);*/
    //return 0;
	
	
	//printf("\n%s\n",resolved_path); 

    launch_server();
    return 0;
}
