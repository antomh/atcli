#ifndef ATCLI_H_
#define ATCLI_H_

//=============================================================================

// Includes.
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "tc.h"

//=============================================================================

// Definitions.
#define UNIVERSE_ANSWER     42  // The Answer.
#define ATCLI_IN            stdin  // Input command stream.
#define ATCLI_OUT           stdout  // Output stream.
#define SOCKET_PATH         "/tmp/9Lq7BNBnBycd6nxy.socket"  // Path to socket.
#define CMDFILE_PATH        "./atcmds.txt"  // Path to valid command file.
#define CMD_LENGTH_MAX      64  // Max AT cmd length possible.
#define CMD_NUMBER_MAX      64  // Max AT cmds number possible.
#define HIST_MAX_CAPACITY   100  // Max AT cmd history capacity.
#define HIST_PATHNAME       "./athist.txt"  // Path to history file.

//=============================================================================

// Enums.

//=============================================================================

// Types and structures.
struct atcmds {
    int  length_min;  // Max command length.
    int  length_max;  // Min command length. // For now do not use anywhere.
    int  number;      // Number of command.
    char **arr;       // Pointer at string commands array.
    FILE *fp;         // Pointer at cmds file.
};

struct athist {
    FILE *fd;             // History file descriptor.
    int  cmd_index_curr;  // Chosen (current) command index.
    int  cmd_index_last;  // Index of the last cmd in history ring buffer.
    char **arr;           // Pointer at commands history array.
};

struct atsock {
    char *path; // Path to socket file.
    int  fd;    // File descriptor to opened socket file.
};

//=============================================================================

// Global variables.

//=============================================================================

// Global functions.

// Client run.
bool atcli_cli_run(void);

//-----------------------------------------------------------------------------

// Print program welcome header.
void atcli_cmdintr_print_header(void);

// Read command from stdin into pcmd.
void atcli_cmdintr_input(char ** pcmd, char * cmd);

//-----------------------------------------------------------------------------

// Check if entered command is valid.
bool atcli_cmdchk_is_valid(char * cmd, const struct atcmds * atcmds);

// Check if entered command is exit command ('0').
bool atcli_cmdchk_no_exit(const char * cmd);

// Create all dynamically allocated variables, init structure and open
//  valid commands file.
bool atcli_cmdchk_init(struct atcmds * atcmds);

// Get information from command file.
bool atcli_cmdchk_get_info(struct atcmds * atcmds);

// Delete all dynamically allocated variables.
bool atcli_cmdchk_deinit(struct atcmds * atcmds);

//-----------------------------------------------------------------------------

// Initialize command history and open history file.
bool atcli_hist_init(struct athist * history);

// Deinitialize command history and close history file.
bool atcli_hist_deinit(struct athist * history);

// Add input cmd to the end of history.
bool atcli_hist_add_cmd(struct athist * history, const char * cmd);

// Read line from history file start at the current position. Writed data to
//  line buffer.
bool atcli_hist_readline(struct athist * history,
    const char * line, const size_t linesize);

//-----------------------------------------------------------------------------

// Initialize socket structure.
bool atcli_sock_init(struct atsock * sock);

// Open UNIX socket. Return socket file descriptor.
int  atcli_sock_open(const struct atsock * sock);

// Send cmd to opened UNIX socket.
bool atcli_sock_send(const struct atsock * sock, const char * cmd);

// Check if socket available to write or not.
bool atcli_sock_check_available(struct atsock * sock);

// Deinitialize socket structure and free all alocated memory.
bool atcli_sock_deinit(struct atsock * sock);

//=============================================================================

#endif  // ATCLI_H_
