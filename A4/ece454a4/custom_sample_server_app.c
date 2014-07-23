/* Mahesh V. Tripunitara
   University of Waterloo
   Part of a simplified RPC implementation
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "ece454rpc_types.h"
#include "khash.h"
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <assert.h>
//#include "ece454_fs.h"

#if 0
#define _DEBUG_1_
#endif





// Below variables define which folders the client can mount on
// Anything on or below these folders are ok
int NumOfMasterFolders;
char ** MasterFolders;

int next_available_client_id;

char * noPathAvailable = "**";

// Mount Hash Table
KHASH_MAP_INIT_INT(32, char)


int nparam_error = -5;
int arg_size_error = -6;
int illegal_folder_access = -7;



/* We allocate a global variable for returns. However, 
 * r.return_val is allocated dynamically. We look to
 * the server_stub to deallocate after it returns the
 * response to the client. */

return_type r;

extern printRegisteredProcedures();
char * checkFolderEqualOrBelow(char * folderNameToBeChecked);
int checkdir(char *dir, char * dirToFind, char * builtStr);

return_type fsMount(const int nparams, arg_type*a) {
    
    if(nparams != 0) {
		/* Error! */
		printf("\nError at Line: %d\n", __LINE__);
		r.return_val = nparam_error;
		r.return_size = sizeof(int);
		return r;
    }
	
	// if(a->arg_size <= 2) {
		/* Error - We expected a folder name! */
		/*printf("\nError at Line: %d\n", __LINE__);
		r.return_val = NULL;
		r.return_size = 0;
		return r;
    } */
	
	
	
	/*char * folderName = (char *)(a->arg_val);

	// We have the folder name to mount on so check if it is exactly the root folder we support
	// or anything below it.
	
	if ((checkFolderEqualOrBelow(folderName)) != 0) { */
		/* Error - This server doesn't support that folder */
		/* Or atleast it doesn't have it in or equal to it's mountable folders list */
		/*printf("\nError at Line: %d\n", __LINE__);
		r.return_val = NULL;
		r.return_size = 0;
		return r;
	}*/
	
	// Passed all checks so add to the array that has client IP and folder mounted.
	
	// TODO: We have to ensure that this client did not previously mount ON CLIENT SIDE?
	
	// Returning Client-Server Unique ID
	int *ret_int = (int *)malloc(sizeof(int));

    *ret_int = next_available_client_id;
	next_available_client_id++;
	printf("The new client mounted with the ID: %d \n\n", (*ret_int));
    r.return_val = (void *)(ret_int);
    r.return_size = sizeof(int);

    return r;	
    
}

char resolved_path[5000];

char * checkFolderEqualOrBelow(char * folderNameToBeChecked) {
	
	int i;
	char * currentRootFolder;
	
	realpath(folderNameToBeChecked, resolved_path); 
	
	
	for(i = 0; i < NumOfMasterFolders; i++) {
		
		// Most likely going to finish on first iteration
		
		currentRootFolder = MasterFolders[i];
		
		int comparison_result = strcmp(resolved_path, currentRootFolder);
		
		if (comparison_result == 0) {
			// Found as root, return the absolute root directory
			return MasterFolders[i];
		} else {
			
			//printf("The char is %c and it should be %c so comparison yields: %d\n",resolved_path[((strlen(currentRootFolder)))],'/',);
			
			if (((strncmp(currentRootFolder,resolved_path,((strlen(currentRootFolder))))) == 0)&&(resolved_path[((strlen(currentRootFolder)))] == '/')) {
				
				// Attempting to access below root folder
				// Next Test - does folder exist?
				
				DIR *dp;
	
				if((dp = opendir(resolved_path)) != NULL) {
					// Directory Exists and can be Opened!!!
					closedir(dp);
					return resolved_path;
				}
				
				closedir(dp);			
			}
			
			/*
			// Need to recursively check - Experimental - verified to have some flaws
			if ((checkdir(currentRootFolder, resolved_path, currentRootFolder)) == 1) {
				return 0;
			}*/
		}
		
	}

	// Nope - the directory requested doesn't exist in any of the available directories
	return noPathAvailable;
}

int checkdir(char *dir, char * dirToFind, char * builtStr)
{
	
	// OBSOLETE METHOD
	
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
			
			// Lower Folders
			
			if ((checkdir(entry->d_name,dirToFind,subDir)) == 1)
			{
				return 1;
			}
        }
        else continue;
    }
    chdir("..");
    closedir(dp);
}

char * buildFNameArray(char * dir){
	
	char * finalDirArraySTR;
	int sizeofArray;
	int firstPass;
	
	DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
	
	firstPass = 0;
	sizeofArray = 0;
	finalDirArraySTR = NULL;
	
    if((dp = opendir(dir)) == NULL) {
		// Cannot open directory
		printf("\nError at Line: %d\n", __LINE__);
        return NULL;
    }
	
    chdir(dir);
    while((entry = readdir(dp)) != NULL) {
        lstat(entry->d_name,&statbuf);
		
		char * entType; 
		
        if(S_ISDIR(statbuf.st_mode)) {
			
			entType = "+";
			
            /* Found a directory, but ignore . and .. */
			if(strcmp(".",entry->d_name) == 0 ||
			strcmp("..",entry->d_name) == 0)
                continue;
            /* printf("%s%s/\n","",entry->d_name); */
            /* Recurse at a new indent level */
            printf("FolderName: %s; Name Length: %d\n",entry->d_name,strlen(entry->d_name));
			
        }
        else if (S_ISREG(statbuf.st_mode)) {
			entType = "0";
			printf("FileName: %s; Name Length: %d\n",entry->d_name,strlen(entry->d_name));
		} else {
			entType = "-";
			printf("Unknown File type: %s; Name Length: %d\n",entry->d_name,strlen(entry->d_name));
		}
		
		if (firstPass == 0) {
			firstPass = 1;
			sizeofArray = (((sizeof(char))*(strlen(entry->d_name)))+2);  // +1 for null terminator  +1 for file type
			
			finalDirArraySTR = (char *)(malloc( sizeofArray ));
			strcat(finalDirArraySTR,entType);
			strcat(finalDirArraySTR,entry->d_name);
		} else {
			sizeofArray += (((sizeof(char))*(strlen(entry->d_name)))+2); // +1 for seperator Slash +1 for file type
			finalDirArraySTR = (char *)(realloc(finalDirArraySTR,sizeofArray));
			strcat(finalDirArraySTR,"/");
			strcat(finalDirArraySTR,entType);
			strcat(finalDirArraySTR,(entry->d_name));			
		}
		
    }
	
    chdir("..");
    closedir(dp);
	
	return finalDirArraySTR;
	
}

return_type fsOpenDir(const int nparams, arg_type*a) {
	
	if(nparams != 1) {
		/* Error! */
		printf("\nError at Line: %d\n", __LINE__);
		r.return_val = nparam_error;
		r.return_size = sizeof(int);
		return r;
    }
	
	 if(a->arg_size <= 1) {
		/* Error - We expected a folder name! */
		printf("\nError at Line: %d\n", __LINE__);
		r.return_val = arg_size_error;
		r.return_size = sizeof(int);
		return r;
    } 
	
	
	
	char * folderName = (char *)(a->arg_val);
	char * absolutePathName;
	
	absolutePathName = noPathAvailable;

	if (strcmp(".",folderName) == 0) {
		// It's the root folder he wants - Acceptable so return all folder/files beneath Except "." and ".."
	
		
		absolutePathName = (MasterFolders[0]);
	
	} else {

		// We have the folder name to mount on so check if it is exactly the root folder we support
		// or anything below it.
		absolutePathName = checkFolderEqualOrBelow(folderName);		
		
	}
	
	
	
	if (strcmp(absolutePathName,noPathAvailable) == 0) { 
		/* Error - This server doesn't support that folder */
		/* Or atleast it doesn't have it in or equal to it's mountable folders list */
		printf("\nError at Line: %d\n", __LINE__);
		r.return_val = illegal_folder_access;
		r.return_size = sizeof(int);
	} else {
		
		// TODO: Create file/folder array data structure (Unix only allows 256 character names so it's ok)
		char * completedFNameArray;
		int currentFNameSize;
		
		
		completedFNameArray = buildFNameArray(absolutePathName);
		
		if (completedFNameArray != NULL){
			r.return_val = (void *)completedFNameArray;
			r.return_size = ((strlen(completedFNameArray)) + 1);
		} else {
			// No Sub Folders or files
			r.return_val = (void *)"";
			r.return_size = 0;
		}
		
	}
	
	
	
	return r;
}

return_type fsCloseDir(const int nparams, arg_type*a) {
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

char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;
	
	char * dupString;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

	dupString = malloc( ((sizeof(char*) * (strlen(a_str)))+1)  );
    result = malloc(sizeof(char*) * count);
	
	memcpy(dupString, a_str, ((strlen(a_str))+1));
	//dupString = strdupa(a_str);

    if (result)
    {
        size_t idx  = 0;
        char* token = strsep(&dupString, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strsep(&dupString, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
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
	
	
	register_procedure("fsMount", 0, fsMount);
	register_procedure("fsOpenDir", 1, fsOpenDir);
	
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

	
	next_available_client_id = 0;



	// TESTING "fsOpenDir" on 

	char * myCH = "";
	
	char** splitArr;


    //printf("files/folders=[%s]\n\n", myCH);

    splitArr = str_split(myCH, '/');

    if (splitArr)
    {
        int i;
        for (i = 0; *(splitArr + i); i++)
        {
            printf("file/folder=[%s]  ", *(splitArr + i));
			
			
			// Create new FSDIR Linked List from Output
			if (((*(splitArr + i))[0]) == '0') {
				//  0
				
			} else if (((*(splitArr + i))[0]) == '+') {
				//  1
				
			} else {
				// -1
				
			}
			
            free(*(splitArr + i));
        }
        //printf("\n");
        free(splitArr);
    }
	
	while(1){}
    //TODO: UNCOMMENTlaunch_server();
    return 0;
}
