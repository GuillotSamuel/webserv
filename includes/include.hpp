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

/* Parsing */
#define HTML_FILES "../www/html"
#define CSS_FILES "../www/styles"
#define JS_FILES "../www/js"
#define JSON_FILES "../www/data"
#define IMAGE_FILES "../www/images"
#define VIDEO_FILES "../www/videos"
#define AUDIO_FILES "../www/audio"
#define FONT_FILES "../www/fonts"
#define PDF_FILES "../www/docs"
#define XML_FILES "../www/xml"
#define ICON_FILES "../www/icons"
#define CSV_FILES "../www/data"
#define ERROR_400_PAGE "../www/html/errors/400.html"

#include "server.hpp"
