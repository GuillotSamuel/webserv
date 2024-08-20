#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <list>

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/epoll.h>
#include <sys/stat.h>

#define MAX_EVENTS 10
#define BUFFER_SIZE 4096

#define ERROR_400_PAGE "../www/html/errors/400.html"
#define HTML_FILES "../www/html"
#define CSS_FILES "../www/styles"

#include "server.hpp"
