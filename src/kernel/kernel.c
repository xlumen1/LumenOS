void kmain() {
    char* terminal_buffer = (char*) 0xB8000;
    terminal_buffer[0] = 'A';
    terminal_buffer[1] = 15;
}