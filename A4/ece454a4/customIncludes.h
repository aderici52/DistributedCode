#include <sys/types.h>
#include <dirent.h>
#include "./simplified_rpc/ece454rpc_types.h"

extern return_type fsMount(const int nparams, arg_type*a);
extern return_type fsReadDir(const int nparams, arg_type*a);
extern return_type fsOpen(const int nparams, arg_type*a);
extern return_type fsClose(const int nparams, arg_type*a);
extern return_type fsRead(const int nparams, arg_type*a);
extern return_type fsWrite(const int nparams, arg_type*a);
extern return_type fsRemove(const int nparams, arg_type*a);