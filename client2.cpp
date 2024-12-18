#include<iostream>
#include<winsock2.h>
#include<ws2tcpip.h>
using namespace std;
#pragma comment(lib, " ws2_32.lib")
bool Initialise()
{
    WSADATA data;
    return WSAStartup(MAKEWORD(2,2), &data)==0;
}
void SendMsg(SOCKET s)
{
    cout<<"enter your chat name:\n";
    string name,message;
    getline(cin,name);
    while(1)
    {
        getline(cin,message);
        string msg=name + " : " + message;
        int bytesent=send(s,msg.c_str(),msg.length(),0);
        if(bytesent==SOCKET_ERROR)
        {
            cout<<"error sending message\n";
            break;
        }
        if(message=="quit")
        {
            cout<<"stopping the application\n";
        }

    }
    closesocket(s);
    WSACleanup();
}
void Recievemsg(SOCKET s)
{
     char buffer[4096];
     int recvlength;
     string msg="";
     while(1)
     {
        recvlength= recv(s,buffer,sizeof(buffer),0);
        if(recvlength<=0)
        {
            cout<<"disconnectd from the server\n";
            break;
        }
        else
        {
            msg= string(buffer, recvlength);
            cout<<msg;
        }
     }
     closesocket(s);
    WSACleanup();
}
int main()

{
    if(!Initialise())
    {
        cout<<"Initailise failed\n";
    }
    SOCKET s;
    s=socket(AF_INET, SOCK_STREAM, 0);
    if(s==INVALID_SOCKET)
    {
        cout<<"INVALID SOCKET CREATED\n";
        return 1;
    }
    int port=12345;
    string serveraddress="127.0.0.1";
    sockaddr_in serveraddr;
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(port);
    inet_pton(AF_INET,serveraddress.c_str(), &(serveraddr.sin_addr));
    if(connect(s,reinterpret_cast<sockaddr*>(&serveraddr),sizeof(serveraddr))==SOCKET_ERROR)
    {
        cout<<"Not able to connet\n";
        closesocket(s);
        WSACleanup();
        return 1;

    }
    cout<<"successfully conneted to server\n";
     thread senderthread(SendMsg,s);
     thread receiver(Recievemsg,s);
     senderthread.join();
     receiver.join();
    //send 
    //receive
    
    return 0;
}