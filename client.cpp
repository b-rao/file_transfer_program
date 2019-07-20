#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string>
#include <cstring>
#include <vector>

using namespace std;

// Server details
#define PORTNUM 5555
#define HOSTNAME "localhost"

int main(int argc, const char **argv)
{
    if(argc != 2){
        cout << "usage: " << argv[0] << " <filename>" << endl;
        return 0;
    }
    //create connection to HOSTNAME through port PORTNUM
    struct sockaddr_in address;
    memset(&address,0,sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(PORTNUM); // internet port
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    struct hostent *hostweb = gethostbyname(HOSTNAME);

    if(hostweb == NULL){
        cout << "Error in getting host" <<endl;
	    return -1;
    }
    //copy the ip into address
    bcopy((char *)hostweb->h_addr, (char *)&address.sin_addr.s_addr,  hostweb->h_length);

    int sock = socket(AF_INET, SOCK_STREAM, 0); //create
	if (socket < 0) {
		cerr << "Client Socket creation failed" << endl;
			return -1;
		}

    if (connect(sock, (struct sockaddr*) &address, sizeof(struct sockaddr_in)) < 0){
	    cerr << "client socket connect failed" << endl;
			return -1;
    }

    //Read a file into buffer, then send it

    ifstream inFile;
    inFile.open(argv[1], ios::binary);

    if(inFile.is_open()){

        //vector is initialized
        vector<char> sendBuf(istreambuf_iterator<char>{inFile} , istreambuf_iterator<char>{});

        size_t count = sendBuf.size();

        //send filename
        send(sock,argv[1], strlen(argv[1])  , 0 );
        //send filesize
        usleep(500);
        string filesize = to_string(count);
        send(sock,filesize.c_str(),filesize.length() , 0 );
        //without a sleep, server receives both filename and file in same buffer 
        usleep(1000);
        //send file
        size_t about = send(sock, sendBuf.data(), count, 0 );
        close(sock);

        cout << "Sent file: " << argv[1] <<"\n# of bytes sent: " <<about <<endl;
 
    }else{
        cerr << "Could not open file: " << argv[1] << endl;
            close(sock);
			return -1;
    }

    
}
