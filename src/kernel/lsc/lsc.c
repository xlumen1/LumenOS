#include "lsc.h"

struct Lsc_Info* gbl_environment;

struct Lsc_Info* lsc_getenv() {
    return gbl_environment;
}

void lsc_init() {
    struct Lsc_Info* info = malloc(sizeof (struct Lsc_Info));
    info->env = malloc(ENV_SIZE);
    info->line = malloc(LINE_SIZE);
    gbl_environment = info;
}

void lsc_load(const char* statement) {
    *gbl_environment->line = statement;
}

char* lsc_run_command(const char* cmd) {
    if (strcmp(cmd, "DBG") == 0) {
        char* output = malloc(6);
        strcpy(output, "DEBUG");
        return output;
    } else if (strcmp(cmd, "ZERO"))
    {
        char* output = malloc(2);
        strcpy(output, "0");
        return output;
    }
    
    char* output = malloc(1);
    output[0] = '\0';
    return output;
}

void lsc_resolve() {
    // Step 1: Prune Comments
    for (uint8_t i = 0; i < LINE_SIZE; i++) {
        if (gbl_environment->line[i] == ';') {
            gbl_environment->line[i] = '\0';
            break;
        }
    }

    // Step 2: Shift left to fill nulls
    size_t write_idx = 0;
    for (size_t read_idx = 0; read_idx < LINE_SIZE; ++read_idx) {
        if (gbl_environment->line[read_idx] != '\0') {
            gbl_environment->line[write_idx++] = gbl_environment->line[read_idx];
        }
    }
    while (write_idx < LINE_SIZE) {
        gbl_environment->line[write_idx++] = '\0';
    }

    // Step 3: Recursively resolve parentheses
    char* line = gbl_environment->line;
    while (1) {
        char* open = NULL;
        char* close = NULL;
        // Find innermost '('
        for (char* p = line; *p; ++p) {
            if (*p == '(') open = p;
            if (*p == ')' && open) {
                close = p;
                break;
            }
        }
        if (!open || !close) break; // No more parentheses

        // Extract command inside parentheses
        size_t cmd_len = close - open - 1;
        char* cmd = malloc(cmd_len + 1);
        strncpy(cmd, open + 1, cmd_len);
        cmd[cmd_len] = '\0';

        // Run the command and get result
        char* result = lsc_run_command(cmd);

        // Replace (command) with result in line
        size_t result_len = strlen(result);
        size_t tail_len = strlen(close + 1);
        memmove(open + result_len, close + 1, tail_len + 1); // +1 to move null
        memcpy(open, result, result_len);

        free(cmd);
        free(result);
    }
}