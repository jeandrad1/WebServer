# Webserv

Custom HTTP server implemented in C++ following HTTP/1.1 semantics.

This README provides usage and configuration information, lists bonus/demo features present in the `assets/` folder, and includes a short design review (SOLID-style) describing how the codebase maps to common software design principles.

## Description

Webserv is an educational HTTP server that accepts connections, parses requests, executes CGI scripts, and serves static files. It supports multiple simultaneous connections and the basic HTTP methods used in teaching exercises.

## Features

- HTTP/1.1 request parsing and response generation
- Non-blocking I/O with an Epoll-based event loop
- Supported methods: `GET`, `POST`, `DELETE`
- Configurable server and location blocks via a `.conf` file
- Static file serving and directory index handling (autoindex)
- CGI execution (multiple interpreters supported by configuration)
- Redirections and custom error pages
- Request size limits and basic input validation
- Mime type detection

## Bonuses / Demo assets (included)

Under `assets/www/` you will find example pages and scripts used for testing and demos:

- `index/`, `login/`, `register/` — demo pages and simple forms
- `upload/` and `portal/` — upload/management demo pages and server-side handlers
- `cgi-bin/` — example CGI scripts (`.php`, `.py`, compiled test programs)
- `download/` — example downloadable files
- `error_page/` — custom error page assets
- `config_files/` — example configuration files used in tests (`should_work.conf`, `test_server.conf`, etc.)

## Compilation

Run from the `webserv/` directory:

```bash
make        # Build the server
make clean  # Remove object files
make fclean # Remove all build artifacts
make re     # Rebuild from scratch
```

## Usage

```bash
./webserv <config_file.conf>
```

If no config path is provided the program currently expects exactly one argument (a config file path). See `assets/config_files/` for example configs.

## Example configuration

```
server {
  listen 989;
  server_name example.com;
  location /
  {
    root        /var/www/html;#hola
    index index.html;     
  }
  listen pepe;
  location /api{
    proxy_pass http://backend;
  }
  server {
    listen test_error;
  }
  listen 8080;
}}
```

## HTTP methods & status codes

Supported methods:

| Method | Description |
|---|---|
| GET | Retrieve a resource |
| POST | Send data to the server (e.g., file upload) |
| DELETE | Remove a resource |

Common response codes returned by the server:

- `200 OK` — Success
- `301/302` — Redirect
- `400 Bad Request` — Malformed request
- `403 Forbidden` — Permission denied
- `404 Not Found` — Resource not found
- `405 Method Not Allowed` — Unsupported method
- `413 Payload Too Large` — Request exceeds size limits
- `500 Internal Server Error` — Server-side error

## Requirements

- C++ compiler (g++ / clang++)
- C++98 or later (codebase uses classic C++ style; incremental modernization recommended)
- `make`
- POSIX-compatible OS (Linux / macOS)

## Advanced behavior implemented

- Non-blocking sockets with `fcntl` and Epoll
- Event loop dispatching accept/read/write using `EpollManager` and `EventLoop`
- Socket lifecycle handled by `SocketsManager`
- Request parsing and header handling in `HttpRequestManager`
- Response construction in `ResponseFactory` (including custom error pages)
- CGI handling with environment setup (`CgiEnvBuilder`, `CgiHandler`)
- Config parsing/building using a Builder/Composite pattern (`ServerBuilder`, `LocationBuilder`, `AConfigBlock`)

## Testing examples

Use `curl` to exercise endpoints (examples):

```bash
curl http://localhost:8080/
curl -X POST -d "hello" http://localhost:8080/upload
curl -X DELETE http://localhost:8080/somefile.txt
curl -v http://localhost:8080/index.html
```

## Design notes

This project uses several well-known patterns (Factory, Strategy, Builder, Composite) and modular directories (`builder/`, `factory/`, `strategy/`, `execute/`, `composite/`) which map well to separation of concerns.

- Single Responsibility (S): Generally respected — classes like `ServerBuilder`, `SocketsManager`, `HttpRequestManager`, and `ResponseFactory` have focused responsibilities.
- Open/Closed (O): `StrategyFactory` and registration functions allow adding new validation/strategy implementations without modifying existing consumers.
- Liskov Substitution (L): Base interfaces (e.g., `IConfigBuilder`, `IValidationStrategy`, `IHttpRequestHandle`) are used; derived classes appear to be substitutable in current usage.
- Interface Segregation (I): The code defines multiple small interfaces (e.g., `IValidationStrategy`, `IConfigBuilder`), which keeps consumers from depending on unused methods.
- Dependency Inversion (D): High-level modules use abstract interfaces and factories, but there is still frequent use of raw `new`/`delete` and concrete type construction. Consider injecting dependencies (or factories) where possible instead of constructing concrete dependencies deep in the call stack.

## Functionality coverage (what the code implements)

From inspection the codebase implements:

- Configuration parsing into `ServerConfig` / `LocationConfig` structures (Builder/Composite)
- Strategy-based validation and directive handling (StrategyFactory)
- Socket creation and non-blocking accept/listen (SocketsManager)
- Event-driven loop with epoll integration (EventLoop / EpollManager)
- HTTP request parsing, header handling and basic validation (HttpRequestManager)
- Response creation including custom error pages and basic mime detection (ResponseFactory / MimeTypeDetector)
- CGI environment building and execution (CgiEnvBuilder / CgiHandler)
- Static file serving, downloads, uploads and directory indexing (implemented in response logic)
---

Last updated: December 2025
