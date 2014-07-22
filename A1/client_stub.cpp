#include <stdio.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include "ece454rpc_types.h"
#include <stdarg.h>
#include <cstring>

using namespace std;

// Golbal Variables.
const int SIZE_OF_DATA_ARRAY = 1000;
char dataArray[SIZE_OF_DATA_ARRAY];
int sockfd;

void kill_sender(int s){
           // cout<<"Caught signal "<<s<<endl;
           close(sockfd);
           exit(1);
}


void clientSerialize( const char *procedure_name,
					const int nparams,
					va_list args){

	// Zero out the buffer
	// cout << "We are clearing the data Array in client Serialize" << endl;
	bzero((void*)dataArray, SIZE_OF_DATA_ARRAY);
	// cout << "We cleared the data in client Serialize" << endl;
	uint paramLen = strlen(procedure_name);  // # chars before null
	uint baseOffset = 0;
	char nullChar = '\0';

	//memcpy((void*)&dataArray[baseOffset], (void*)("ab"), 3);
	//baseOffset += 3;
	memcpy((void*)&(dataArray[baseOffset]), (void*)&paramLen, sizeof(paramLen));  // Copy string length
	baseOffset += sizeof(paramLen);

	//Serialize the procedure name
	memcpy((void*)&(dataArray[baseOffset]), (void*)procedure_name, paramLen);
	// cout << "We cleared the data in client Serialize" << endl;
	baseOffset += paramLen;

	// cout << "Step 1" << endl;
	//memcpy((void*)(&dataArray[baseOffset]), (const void*)(&nullChar), sizeof(char));  // Insert the null character
	//baseOffset += sizeof(char);
	// cout << "Step 2" << endl;
	memcpy((void*)(&dataArray[baseOffset]), (const void*)(&nparams), sizeof(nparams));  // Insert the number of parameters
	baseOffset += sizeof(nparams);
	// cout << "Step 3" << endl;
	/* Deal with variably sized arguments. The protocol is to list the size
	* of variably sized arguments in bytes, followed by the bytes of the
	* argument itself
	* [size_t sizeOf(ArgN)] [k chars(bytes) of ArgN] [size_t sizeOf(ArgN+1)] [k chars(bytes) of ArgN+1] ...
	*/
	void* tmpArg;
	int argSize = 0;
    // cout << "We are starting to create the argument array in client Serialize" << endl;
	for(int j=0; j < nparams; j++)
	{
		argSize = va_arg(args, int);  // Cast to unsigned int
		memcpy((void*)(&dataArray[baseOffset]), &argSize, sizeof(argSize));  // Copy arg size
		baseOffset += sizeof(argSize);

        tmpArg = va_arg(args, void*);  // Get next argument of the size, data pair
		memcpy((void*)&(dataArray[baseOffset]), tmpArg, argSize);  // Copy data
		baseOffset += argSize;  // Plus one for next free slot
	}
	// cout << "Reached the last step of client Serialize" << endl;
	// cout << dataArray[1] << endl;
}

void clientDeSerialize(void * return_buf, return_type * resultant_val)
{

	int baseoffset = 0;
	memcpy((void *)&(resultant_val->return_size), (void*)&( ((char *)return_buf)[baseoffset] ), sizeof(resultant_val->return_size));
	baseoffset += sizeof(resultant_val->return_size);
	resultant_val->return_val = malloc(resultant_val->return_size);
	cout << "TOTAL SIZE: " << resultant_val->return_size << endl;
	memcpy(resultant_val->return_val, (void*)((char*)return_buf + baseoffset), resultant_val->return_size);
	cout << "Retrurecve : " << *((int*)(resultant_val->return_val)+2) << endl;

}


return_type makeServerCall(const char * ip_address, int port_num, void * send_message)
{
    return_type resultant_tp;
    struct sigaction sigIntHandler;
    struct sockaddr_in serv,client;
    int recv_code;
    int send_rtn;

    // cout << "Accessing " << ip_address << ":" << port_num << endl;

    sigIntHandler.sa_handler = kill_sender;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);


    sockfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);

    bzero(&serv,sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(port_num);
    int inet_mine = inet_aton(ip_address, (struct in_addr *)(&serv.sin_addr.s_addr));
    // cout << "Output from inet: " << inet_mine << endl; 


    char buffer[1000];
    socklen_t l = sizeof(client);
    socklen_t m = sizeof(serv);


    // cout << "\nSending UDP Packet\n";
    // cout << "Contents: " << ((char*)(send_message))[0] << endl;
    send_rtn = sendto(sockfd,send_message,1000,0,(struct sockaddr *)&serv,m);
    if (send_rtn < 0)
    {
    	printf("%s","The send failed");
    	return resultant_tp;
    }

    // cout << "Message Successfully sent - Receiving...\n";
    //sleep(5);
    recv_code = recvfrom(sockfd,buffer,1000,0,(struct sockaddr *)&client,&l);
    if(recv_code==-1)
    {
	printf("%s","Error - Receiving Failed");
    	return resultant_tp;
    }
    else{
    	
        //cout<<"RCV successful"<<endl;
        
        clientDeSerialize(buffer, &resultant_tp);
	        
	// cout << "Client SAYS SMOETHING" << endl;
        // cout << "Client Says: " << *((int *)(resultant_tp.return_val)) << endl;
        return resultant_tp;
    }
    //close(sockfd);
}

return_type make_remote_call(const char *servernameorip, const int serverportnumber, const char *procedure_name, const int nparams, ...)
{
    //struct rpc_structure current_call;

    va_list ap;
    va_start(ap,nparams);

    void * builtFnArgs[100];  // Limit arguments to 100
    int intermediate_Sizes[100];
    int size_idx = 0;

    return_type FinalValue;

    int j;
    int kkj;

    int as = 5;
    int bs = 4;
    arg_type * currentArgs;
    char* serializedData = dataArray;
    clientSerialize(procedure_name, nparams, ap);
    va_end(ap);


    arg argument_a;
    arg argument_b;
    argument_a.arg_val = &as;
    argument_a.arg_size = sizeof(as);
    argument_b.arg_val = &bs;
    argument_b.arg_size = sizeof(as);
    argument_a.next = &argument_b;
    argument_b.next = 0;
    currentArgs = &argument_a;

    return (makeServerCall(servernameorip, serverportnumber, (void *)serializedData ));
    //printf("\n-- DONE --\n");
}


