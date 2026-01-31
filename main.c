#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

void cleanup() {
}

typedef struct {
    char *buffer;
    size_t buffer_length;
    ssize_t input_length;
} InputBuffer;

InputBuffer * new_input_buffer() {
    InputBuffer *input_buffer = (InputBuffer *)malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;
    return input_buffer;
}
void close_input_buffer(InputBuffer *input_buffer) {
    free(input_buffer->buffer);
    free(input_buffer);
} 

void print_prompt() {
    printf("db > ");
}
void read_input(InputBuffer *input_buffer) {
    ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);
    if(bytes_read <= 0) {
        printf("Error reading input!");
        exit(EXIT_FAILURE);
    }
    // ignoring trailing newline character
    input_buffer->input_length = bytes_read - 1;
    input_buffer->buffer[bytes_read-1] = 0; 
}

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

MetaCommandResult do_meta_command(InputBuffer *input_buffer) {
    if(strcmp(input_buffer->buffer, ".exit") == 0) {
        exit(EXIT_SUCCESS);
    } else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT
} StatmentType;

typedef struct {
    StatmentType type;
} Statement;

typedef enum {
    PREPARE_SUCESS,
    PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

PrepareResult prepare_statment(InputBuffer *input_buffer, Statement *statement) {
    if(strncmp(input_buffer->buffer, "insert", 6) == 0) {
        statement->type = STATEMENT_INSERT;
        return PREPARE_SUCESS;
    }
    if(strncmp(input_buffer->buffer, "select", 6) == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(Statement *statement) {
    switch (statement->type) {
        case STATEMENT_INSERT:
            printf("Inserting here\n");
            break;
        case STATEMENT_SELECT:
            printf("Select here\n");
            break;
    }
}

int main(int argc, char *argv[]) {
    atexit(cleanup);
    InputBuffer *input_buffer = new_input_buffer();
    while(1) {
        print_prompt();
        read_input(input_buffer);
        if(input_buffer->buffer[0] == '.') {
            switch (do_meta_command(input_buffer)) {
                case META_COMMAND_SUCCESS:
                    continue;
                case META_COMMAND_UNRECOGNIZED_COMMAND:
                    printf("Unrecognized command: %s\n", input_buffer->buffer);
                    continue;
            }
        }
        Statement statement;
        switch (prepare_statment(input_buffer, &statement)) {
            case PREPARE_SUCESS:
                break;
            case PREPARE_UNRECOGNIZED_STATEMENT: 
                printf("Unrecognized keyword at start of %s.\n", input_buffer->buffer);
                continue;
        }
        execute_statement(&statement);
        printf("Executed\n");
    }
    return 0;
}