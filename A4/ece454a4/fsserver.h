#include <stdbool.h>

/* returnType */
typedef struct {
	char serverIP[500];
    int	srvPort;
	char localFolderName[500];
	int validOrNot;
	int correspondenceID;
	struct fsMountInstance * nextMount;
} fsMountInstance;