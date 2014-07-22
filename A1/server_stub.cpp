#include <stdio.h>
#include <iostream>
#include <netinet/in.h>
#include <string.h>
//#include"unp.h"
#include <netdb.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
//#include <unordered_map>
#include "ece454rpc_types.h"
#include <arpa/inet.h>

#include <sys/types.h>
#include <ifaddrs.h>

#include <netinet/in.h>
#include <netinet/ip.h>
#include <errno.h>

#define	PORT_RANGE_LO	10000
#define PORT_RANGE_HI	10100

using namespace std;

const int SERVER_BUFF_SIZE = 1000;

struct function_entry {
    char * fn_name;
    int nparams;
    fp_type fn_pointer;
    struct function_entry * next;
};

int mybind(int sockfd, struct sockaddr_in *addr) {
  if(sockfd < 1) {
	fprintf(stderr, "mybind(): sockfd has invalid value %d\n", sockfd);
	return -1;
    }

    if(addr == NULL) {
	fprintf(stderr, "mybind(): addr is NULL\n");
	return -1;
    }

    if(addr->sin_port != 0) {
	fprintf(stderr, "mybind(): addr->sin_port is non-zero. Perhaps you want bind() instead?\n");
	return -1;
    }

    unsigned short p;
    for(p = PORT_RANGE_LO; p <= PORT_RANGE_HI; p++) {
	addr->sin_port = htons(p);
	int b = bind(sockfd, (const struct sockaddr *)addr, sizeof(struct sockaddr_in));
	if(b < 0) {
	    continue;
	}
	else {
	    break;
	}
    }

    if(p > PORT_RANGE_HI) {
	fprintf(stderr, "mybind(): all bind() attempts failed. No port available...?\n");
	return -1;
    }

    /* Note: upon successful return, addr->sin_port contains, in network byte order, the
     * port to which we successfully bound. */
    return 0;
}

// Forward Declaration
char * getCurrentIPAddress(char * finalHost);
void clearList();





// Global Variables
struct function_entry * head;
struct function_entry * tail;
int server_sock;

void kill_server(int s){
           close(server_sock);
           clearList();
           exit(1);
}

void serverDeSerialize(void* buffer,
						char* procName,
						int* nArgs,
						arg_type* argList){

	uint procNameLen;
	uint offset = 0;
	uint nparams;
	char* internalBuf = (char*) buffer;  // Casted pointer to buffer, to be able to use [] dereference operator.

	memcpy((void*)&procNameLen, (void*)(&internalBuf[0]), sizeof(procNameLen));  // Get length of string.
	offset += sizeof(procNameLen);


	memcpy((void*)procName, (void*)(&internalBuf[offset]), procNameLen);
	offset += procNameLen;


	memcpy((void*)&nparams, (void*)(&internalBuf[offset]), sizeof(nparams));  // Insert the number of parameters
	offset += sizeof(nparams);
  //cout << "Got the number of params: " << nparams << endl;

	// Start reading the rest of the arguments.
	int argSize = 0;
  arg_type* curArg;
  arg_type* rootArg;
  arg_type* prevArg;
  *nArgs = nparams;  // Pass # of function arguments out.

  if(nparams!=0)
  {
  	for(int j=0; j<nparams; j++)
  	{
  		if(j==0)
  		{
  			memcpy((void*)&argSize, (void*)(&internalBuf[offset]), sizeof(argSize));  // Get argument size
  			offset += sizeof(argSize);
  			argList->arg_val = malloc(argSize);

  			memcpy(argList->arg_val, (void*)(&internalBuf[offset]), argSize);  // Copy data
  			offset += argSize;

  			argList->arg_size = argSize;
  			argList->next = NULL;
  		  //cout << "ArgumentSize: " << argList->arg_size << endl;
  			//cout << "Argument val: " << *((int*)(argList->arg_val)) << endl;
  			prevArg = argList;
  			//cout << "finished root" << endl;
  		}
  		else
  		{
  			curArg = (arg_type*)malloc(sizeof(arg_type));
  			memcpy((void*)&argSize, (void*)(&internalBuf[offset]), sizeof(argSize));  // Get argument size
  			offset += sizeof(argSize);
  			curArg->arg_val = malloc(argSize);

  			memcpy(curArg->arg_val, (void*)(&internalBuf[offset]), argSize);  // Copy data
  			offset += argSize;

  			curArg->arg_size = argSize;
  			curArg->next = NULL;
  			//cout <<  "ArgumentSize: " << curArg->arg_size << endl;
  			//cout << "Argument val: " << *((int*)(curArg->arg_val)) << endl;
  			prevArg->next = curArg;
        prevArg = curArg;
  			//cout <<  "finished node" << endl;
  		}

  	}
  }
  else
  {
    //cout << "In else" << endl;
    argList->arg_val = NULL;
    argList->arg_size = 0;
    argList->next = NULL;
  }
}

void serverSerialize(void * returnBuf, void* return_val, int return_size)
{
	int baseoffset = 0;
	cout << "Value in the input arg " << *((int*)(return_val)+2) << endl;
	bzero((void*)returnBuf,SERVER_BUFF_SIZE);
	//memcpy(&( ((char *)returnBuf)[baseoffset] ), &(value_tortn->return_size), sizeof(value_tortn->return_size));
	memcpy(&( ((char *)returnBuf)[baseoffset] ), &return_size, sizeof(return_size));
	baseoffset += sizeof(return_size);
	memcpy(&( ((char *)returnBuf)[baseoffset] ),return_val, return_size);
	cout << "Size value: " << ((int*)returnBuf)[0] << endl;
	//cout << "Value in the input arg " << *(int*)(return_val) << endl;
	//cout << "Value in the output arg" << (int)((char*)returnBuf)[4+return_size-1] << endl;
}

void addFunctionToList(struct function_entry* p)
{
  p->next = 0;
  // cout << "head ACCESWE" << endl;
  if(head == 0)
  {
    // cout << "added 1" << endl;
    head = p;
    tail = p;
    // cout << "Fn_Name in addfinc" << p->fn_name << endl;
  }else
  {
    // cout << "added 2" << endl;
    tail->next = p;
    tail = p;
  }
}

struct function_entry* getFunctionInList(char * name_of_function)
{
  
  struct function_entry * iterator_FN = head;

  // cout << "Pointer head : in getfunc " << head << endl;
  // cout << "Pointer head->fn_name : " << head->fn_name << endl;
  // cout << "Looking for function: " << name_of_function << " with length : " << strlen(name_of_function) << endl;

  while(iterator_FN != 0)
  {
    // cout << "Iterating " << endl;
    //// cout << "Comparing " << iterator_FN->fn_name << " with " << name_of_function << " respective sizes: " << strlen(iterator_FN->fn_name) << " & " << strlen(name_of_function) << " output of strncmp = " << strcmp((iterator_FN->fn_name), name_of_function) << endl;
    if (strcmp((iterator_FN->fn_name), name_of_function) == 0)
    {
    	return iterator_FN;
    }
    // cout << "comparison failed" << endl;
    iterator_FN = iterator_FN->next;
  }

  // cout << "Found function" << endl;

  return iterator_FN;
}

void clearList()
{
  struct function_entry* prev = head;
  struct function_entry* cur = head;

  while(cur->next !=  0)
  {
    prev = cur;
    cur = cur->next;
    delete prev;
  }
  delete cur;

  head = 0;
  tail = 0;
}


 void launch_server(){
    struct sigaction sigIntHandler;
    server_sock=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    char host_ip_rpc[NI_MAXHOST];

    getCurrentIPAddress(host_ip_rpc);
    cout << "host_ip_rpc: " << host_ip_rpc << "; length: " << strlen(host_ip_rpc) << endl;

    memset((char *) &server_addr, 0, sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=inet_addr(host_ip_rpc);  //((in_addr_t)
    

    struct hostent tempSettings;
    char * host_addr;
    int i_addr = 0;

    sigIntHandler.sa_handler = kill_server;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);

    int ret = mybind(server_sock, &server_addr);

    if(ret == -1){
		cout << "BIND ERROR!" << endl;
		return;
    }

    char buffer[SERVER_BUFF_SIZE];
    

/////////////////////////////////////////////////////////////////////////////

    struct sockaddr_storage my_addr;
    socklen_t my_addr_len = sizeof(my_addr);
    if (getsockname(server_sock, (struct sockaddr *) &my_addr, &my_addr_len) != -1)
    {
        #ifndef NI_MAXHOST
        # define NI_MAXHOST 1025
        #endif
        #ifndef NI_MAXSERV
        # define NI_MAXSERV 32
        #endif

        char host[NI_MAXHOST];
        char serv[NI_MAXSERV];

        if (getnameinfo((const struct sockaddr *) &my_addr, sizeof(my_addr),
                    host, sizeof(host),
                    serv, sizeof(serv), 0) == 0)
        {
            cout << host_ip_rpc << " " << htons(server_addr.sin_port) << endl;
	    //cout << "Port Num may also just be: " << htons(server_addr.sin_port) << endl;
        }
    }

///////////////////////////////////////////////////////////////////////////////


    socklen_t client_addr_size;
    
    client_addr_size = sizeof(client_addr);

    while(1){
        int count=recvfrom(server_sock,&buffer,1000,0,(struct sockaddr *)(&client_addr),((socklen_t *)(&client_addr_size)));
        arg_type all_args;
        char function_name[500];
        int num_of_args;
        function_entry *requested_fn;
        return_type current_rt;

        bzero(((void *)function_name), 500);
        

        if(count==-1) cout<<"RECV ERROR\n";else {

            serverDeSerialize(buffer, function_name, &num_of_args, &all_args);
            cout << "deserialized successfully" << endl;
            requested_fn = getFunctionInList(function_name);
            if(requested_fn != NULL)
            {
              //cout << "Function retrieved from list" << endl;
            
	          //cout << "received Something - 1" << endl;
            //cout << "Current return value size: " << current_rt.return_size << endl;
            //cout<< "Num Args: " << num_of_args << endl;

            //arg_type* cur_arg = &all_args;

            //while(cur_arg != NULL)
            //{
              //cout << "Current argument data size: " << cur_arg->arg_size << endl;
              //cur_arg = cur_arg->next;
            //}
            
            //cout << "Function pointer not null" << endl;
            
            current_rt = requested_fn->fn_pointer( num_of_args, &all_args );

            //cout << "Current return value size: " << current_rt.return_size << endl;
            void * new_val = malloc(current_rt.return_size);
	          //cout << "Function Exited giving " << *((int *)(current_rt.return_val)+2) << endl;
            memcpy(new_val, (void*)current_rt.return_val, current_rt.return_size);

            serverSerialize(buffer, new_val, current_rt.return_size);

            sleep(1);
	       
            int count2=sendto(server_sock,&buffer,1000,0,(const struct sockaddr *)&client_addr,(client_addr_size));
           
            free(new_val);

            if(count2==-1) cout << "SND ERROR\n" << endl;
          }
            //sleep(1);

        }
    }

    close(server_sock);
}

char * getCurrentIPAddress(char * finalHost)
{
           // Borrowed from linux man page

    	   struct ifaddrs *ifaddr, *ifa;
           int family, s, n;
           char host[NI_MAXHOST];

           if (getifaddrs(&ifaddr) == -1) {
               printf("Error with getifaddrs");
               exit(EXIT_FAILURE);
           }

           

           for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) {
			#ifdef __APPLE__
        	   if ((ifa->ifa_addr == NULL) || ((strcmp(ifa->ifa_name,"en0")) != 0))
                   continue;
			#else
        	   if ((ifa->ifa_addr == NULL) || ((strcmp(ifa->ifa_name,"eth0")) != 0))
        	                      continue;
			#endif
               family = ifa->ifa_addr->sa_family;
	       //printf("Entered Loop");
	       if (family != AF_INET)
         	   continue;
	       
               /* For an AF_INET* interface address, display the address */

               if (family == AF_INET || family == AF_INET6) {
                   s = getnameinfo(ifa->ifa_addr,
                           (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                                 sizeof(struct sockaddr_in6),
                           host, NI_MAXHOST,
                           NULL, 0, NI_NUMERICHOST);
                   if (s != 0) {
                       printf("getnameinfo() failed: %s",gai_strerror(s));
                       exit(EXIT_FAILURE);
                   }

                   strncpy(finalHost, host, NI_MAXHOST);
		   
               }
           }

           freeifaddrs(ifaddr);
}

bool register_procedure(const char * procedure_name, const int nparams, fp_type fnpointer){
    try
    {
        struct function_entry * newEntry;
        newEntry = (struct function_entry *)(malloc((sizeof(struct function_entry))));
    		int fn_name_size = (strlen(procedure_name));
    		// cout << "\n\n HARUN adding proc " << procedure_name << " with size " <<  fn_name_size << endl;
    		newEntry->fn_name = (char *)(malloc(fn_name_size));
    		strncpy(newEntry->fn_name, procedure_name, (fn_name_size));
        newEntry->nparams = nparams;
        newEntry->fn_pointer = fnpointer;

        addFunctionToList(newEntry);
        //// cout << "fawgaw" << endl;
        // cout << "Head fn name (in register proc) " << head->fn_name << endl;
        //FunctionPointerTable.insert(std::make_pair(procedure_name, newEntry));
        return 1;
    }
    catch (int e)
    {
        printf("Procedure Registration failed - %d \n",e);
        return 0;
    }
}

