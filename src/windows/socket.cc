#include "include/windows/socket.hh"

int createSocketServer(const std::string& address, const std::string& port) {
    /*
        WSADATA provides information about the Windows Sockets implementation, such as:

        - The Winsock version supported by the system.
        - Descriptive information about the implementation.
        - System limitations (e.g., the maximum number of sockets).
    */
    WSADATA wsaData;

    // iResult is often used to store the return values of various functions, primary used to check whether a Winsock function call has succeeded or failed.
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;

    /*
        addrinfo is a structure used in Winsock and POSIX systems to represent address information for network connection setup. I
    */
    struct addrinfo* result = nullptr;
    struct addrinfo hints;

    int iSendResult;
    
    // Buffer to hold the incoming data recieved from a connected socket.
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    /*
        MAKEWORD is a macro used to combine two bytes into a single WORD (16-bit value).
    */
    
    // Initializes Winsock.
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Checks if it is valid.
    if (iResult != 0) 
    {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    /*
        ZeroMemory is a macro that clears a block of memory by filling it with zeroes.
    */
    ZeroMemory(&hints, sizeof(hints));

    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM; // Stream-oriented (TCP)
    hints.ai_flags = AI_PASSIVE; // Uses local address
    hints.ai_protocol = IPPROTO_TCP; // Sets explicitly to TCP


    // Resolves the server address and port.
    iResult = getaddrinfo(address.c_str(), port.c_str(), &hints, &result);

    // Checks if it is valid.
    if (iResult != 0) 
    {
        printf("getaddrinfo failed with error: %d\n", iResult);
       
        /*
            WSACleanup is a Winsock function that terminates the use of Winsock by the current process. It releases resources allocated by WSAStartup and prepares the system for Winsock to be uninitialized.
        */
        WSACleanup();

        return 1;
    }

    // Creates a SOCKET for the server to listen for client connections.
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (ListenSocket == INVALID_SOCKET) 
    {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        
        freeaddrinfo(result);
        WSACleanup();

        return 1;
    }

    // Setups the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);

    if (iResult == SOCKET_ERROR) 
    {
        printf("bind failed with error: %d\n", WSAGetLastError());

        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();

        return 1;
    }

    freeaddrinfo(result);

    /*
        SOMAXCONN = 0x7fffffff
    */
    iResult = listen(ListenSocket, SOMAXCONN);
    
    if (iResult == SOCKET_ERROR) 
    {
        printf("listen failed with error: %d\n", WSAGetLastError());
        
        closesocket(ListenSocket);
        WSACleanup();

        return 1;
    }

    return ListenSocket;
}