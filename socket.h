int setup_socket_server(unsigned short port);
ssize_t xread(int fd, void *buf, size_t len);
ssize_t read_full(int fd, void *buf, size_t count);
ssize_t xwrite(int fd, void *buf, size_t len);
ssize_t write_full(int fd, void *buf, size_t count);
