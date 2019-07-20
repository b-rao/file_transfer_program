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

#define PORTNUM 5555
#define INIT_BUFSIZE 256 // max filename length

int main(int argc, const char **argv)
{
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    int port;
    if (argc == 2)
    {
        address.sin_port = htons(port = atoi(argv[1]));
    }
    else
    {
        address.sin_port = htons(port = PORTNUM);
    }

    cout << "Listening on port: " << port << endl;

    int sock = socket(AF_INET, SOCK_STREAM, 0); //create
    if (socket < 0)
    {
        cerr << "Server Socket creation failed" << endl;
        return -1;
    }
    //bind
    if (bind(sock, (struct sockaddr *)&address, sizeof(struct sockaddr_in)) < 0)
    {
        cerr << "Server Socket bind failed" << endl;
        return -1;
    }
    //listen
    if (listen(sock, 5) < 0)
    {
        cerr << "Server Socket Listen failed" << endl;
        return -1;
    }

    cout << "Server listening " << endl;

    int fd;

    while (true)
    {
        if ((fd = accept(sock, NULL, NULL)) < 0)
        {
            cerr << "Server accept() failed" << endl;
            return -1;
        }
        int pid = fork();

        if (pid < 0)
        {
            cerr << "fork failed" << endl;
            return -1;
        }
        else if (pid == 0)
        {
            //child
            close(sock);
            vector<char> receiveBuf(INIT_BUFSIZE);
            size_t count = recv(fd, receiveBuf.data(), INIT_BUFSIZE, 0);

            //parce filename from buffer
            receiveBuf[count] = 0;
            string filename(receiveBuf.data());
            cout << "Received Filename: " << filename << endl;
            //get filesize
            count = recv(fd, receiveBuf.data(), INIT_BUFSIZE, 0);
            receiveBuf[count] = 0;
            count = stoull(receiveBuf.data());

            receiveBuf.clear();
            receiveBuf.resize(count);

            cout << "# of bytes for file: " << count << endl;

            //write a file with filename into a file
            ofstream outFile;
            size_t packet;
            size_t total = 0;

            outFile.open(filename, ios::binary | ios::app);

            if (outFile.is_open())
            {
                 cout << "Writing to " << filename << endl;

                do
                {
                    packet = recv(fd, receiveBuf.data(), count, 0);
                    outFile.write(receiveBuf.data(), packet);
                    total += packet ;
                    cout << "Packet contains " << packet <<" bytes" <<endl;
                } while (total < count);
                
                    cout << "Done. # of bytes received: " << total << endl;

                    outFile.close();
                    receiveBuf.clear();
            }else{
                    cerr << "Could not write to " << filename << " File" << endl;
                    return -1;
                }
            
                //successful, child exits
                return 0;
              
          }  else
                {
                    //parent
                    signal(SIGCHLD, SIG_IGN);
                }
        }//while
                    return 0;
            }