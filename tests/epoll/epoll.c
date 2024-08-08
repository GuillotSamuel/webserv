#include <stdio.h>     // for fprintf()
#include <unistd.h>    // for close(), read()
#include <sys/epoll.h> // for epoll_create1(), epoll_ctl(), struct epoll_event
#include <string.h> 
#define MAX_EVENTS 5
#define READ_SIZE 10

int	main(void)
{
	int					epoll_fd;
	struct epoll_event	event, events[MAX_EVENTS];
	size_t				bytes_read;
	char				read_buffer[READ_SIZE + 1];
	int					event_count;
	int					i;

	epoll_fd = epoll_create(1);
	if (epoll_fd == -1)
	{
		fprintf(stderr, "epoll creat failed\n");
		return (1);
	}
	printf("epoll_fd creat = %d\n", epoll_fd);
	event.events = EPOLLIN;
	event.data.fd = 0;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, 0, &event))
	{
		fprintf(stderr, "Failed to add fd to epoll\n");
	}
	while (1)
	{
		printf("\nPolling for input...\n");
		event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, 30000);
		printf("%d ready events\n", event_count);
		for (i = 0; i < event_count; i++) {
			printf("Reading file descriptor '%d' -- ", events[i].data.fd);
			bytes_read = read(events[i].data.fd, read_buffer, READ_SIZE);
			printf("%zd bytes read.\n", bytes_read);
			read_buffer[bytes_read] = '\0';
			printf("Read '%s'\n", read_buffer);
			if (strncmp(read_buffer, "stop\n", 5))
				break ;
		}
	}

	if (close(epoll_fd) == -1)
	{
		fprintf(stderr, "failed to close\n");
		return (1);
	}
	return (0);
}