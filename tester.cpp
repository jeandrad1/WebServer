#include <iostream>
#include <cstring>      // memset
#include <unistd.h>     // close
#include <sys/socket.h> // socket, bind, listen, accept
#include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h>  // inet_addr

#define PORT 8080

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    // 1. Crear socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "❌ Error creando el socket\n";
        return 1;
    }

    // 2. Configurar dirección (IP y puerto)
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
    address.sin_port = htons(PORT);       // puerto 8080

    // 3. Hacer bind del socket a la IP:puerto
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "❌ Error en bind()\n";
        close(server_fd);
        return 1;
    }

    // 4. Poner el socket en modo escucha
    if (listen(server_fd, 10) < 0) {
        std::cerr << "❌ Error en listen()\n";
        close(server_fd);
        return 1;
    }

    std::cout << "✅ Servidor escuchando en el puerto " << PORT << "...\n";

    // 5. Bucle principal de aceptación
    while (true) {
        client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (client_fd < 0) {
            std::cerr << "❌ Error en accept()\n";
            continue;
        }

        std::cout << "🔌 Cliente conectado desde " << inet_ntoa(address.sin_addr) << ":" << ntohs(address.sin_port) << "\n";

        // Enviar mensaje al cliente (opcional)
        const char *msg = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, world!";
        send(client_fd, msg, strlen(msg), 0);

        close(client_fd); // cerrar cliente después de enviar (modo simple)
    }

    close(server_fd);
    return 0;
}