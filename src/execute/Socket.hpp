#ifndef SOCKET_HPP
#define SOCKET_HPP

class Socket
{
	private:
		int socket;
		Socket();
	public:
		Socket(int int_socket);
		~Socket();
		
		void getSocket();
};

#endif