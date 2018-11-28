#include <cstdio>
#include <cstdlib>
#include <cstring>

const static size_t max_length = 1024;

typedef struct Message {
	char msg[max_length];
	size_t length;
}Message;