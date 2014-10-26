#include <errno.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int setup_socket_server(unsigned short port)
{
	struct sockaddr_in6 addr;
	socklen_t no = 0, yes = 1;
	int fd, r;

	fd = socket(AF_INET6, SOCK_STREAM, 0);
	if (fd < 0)
		return -1;

	r = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
	if (r != 0) {
		close(fd);
		return -1;
	}

	r = setsockopt(fd, IPPROTO_IPV6, IPV6_V6ONLY, &no, sizeof(no));
	if (r != 0) {
		close(fd);
		return -1;
	}

	bzero((char *)&addr, sizeof(addr));
	addr.sin6_family = AF_INET6;
	addr.sin6_addr = in6addr_any;
	addr.sin6_port = htons(port);

	r = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
	if (r != 0) {
		close(fd);
		return -1;
	}

	r = listen(fd, 1024);
	if (r != 0) {
		close(fd);
		return -1;
	}

	return fd;
}

ssize_t xread(int fd, void *buf, size_t len)
{
	ssize_t bytes;
	while (1) {
		bytes = read(fd, buf, len);
		if ((bytes < 0) && (errno == EINTR || errno == EAGAIN))
			continue;
		return bytes;
	}
}

ssize_t read_full(int fd, void *buf, size_t count)
{
        ssize_t total = 0;
        char *b = buf;

        while (count > 0) {
                ssize_t bytes = xread(fd, b, count);
                if (bytes < 0)
                        return -1;
                if (bytes == 0)
                        return total;
                count -= bytes;
                b += bytes;
                total += bytes;
        }

        return total;
}

ssize_t xwrite(int fd, const void *buf, size_t len)
{
	ssize_t bytes;
	while (1) {
		bytes = write(fd, buf, len);
		if ((bytes < 0) && (errno == EINTR || errno == EAGAIN))
			continue;
		return bytes;
	}
}

ssize_t write_full(int fd, const void *buf, size_t count)
{
	ssize_t total = 0;
	const char *b = buf;

	while (count > 0) {
		ssize_t bytes = xwrite(fd, b, count);
		if (bytes < 0)
			return -1;
		if (!bytes) {
			errno = ENOSPC;
			return -1;
		}
		count -= bytes;
		b += bytes;
		total += bytes;
	}

	return total;
}
