typedef unsigned int uint32_t;
typedef unsigned int size_t;

#define SYS_READ 3
#define SYS_WRITE 4

static inline int sys_write(int fd, const char* buf, size_t count) {
    int ret;
    asm volatile (
        "int $0x80"
        : "=a"(ret)
        : "a"(SYS_WRITE), "b"(fd), "c"(buf), "d"(count)
        : "memory"
    );
    return ret;
}

static inline int sys_read(int fd, char* buf, size_t count) {
    int ret;
    asm volatile (
        "int $0x80"
        : "=a"(ret)
        : "a"(SYS_READ), "b"(fd), "c"(buf), "d"(count)
        : "memory"
    );
    return ret;
}

int main(int argc, char** argv) {
    const char* msg = "Hello, world!\n";
    sys_write(1, msg, 14);
    return 0;
}