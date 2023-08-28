#include "atcli.h"

//-----------------------------------------------------------------------------

int main(void) {
    if (atcli_cli_run())
        return (EXIT_FAILURE);
    else
        return (EXIT_SUCCESS);
}

//-----------------------------------------------------------------------------

bool atcli_cli_run(void) {

    // Open Unix socket. Get socket file descriptor.
    // int sfd = atcli_sock_open(SOCKET_PATH);
    // if (sfd == -1) {
    //     fprintf(ATCLI_OUT, "ERROR : Couldn't connect to socket.\n");
    //     return 0;
    // }

    //*************************************************************************

    struct atcmds atcmds;

    if (!atcli_cmdchk_init(&atcmds)) {
        fputs("ERROR : atcli_cmdchk_init.\n", ATCLI_OUT);
        return false;
    }

    if (!atcli_cmdchk_get_info(&atcmds)) {
        fputs("ERROR : atcli_cmdchk_get_info", ATCLI_OUT);
        return false;
    }

    //*************************************************************************

    struct athist athist;

    if (!atcli_hist_init(&athist)) {
        fputs("ERROR : atcli_hist_init", ATCLI_OUT);
        return false;
    }

    //*************************************************************************

    struct atsock atsock;

    if (!atcli_sock_init(&atsock)) {
        fputs("ERROR : atcli_sock_init", ATCLI_OUT);
        return false;
    }

    //*************************************************************************

    atcli_cmdintr_print_header();

    char cmdarr[CMD_LENGTH_MAX];  // It is array for input command string.
    char *cmd = cmdarr;  // Cmd is the pointer at final command
                         // string (one word).
    while (true) {

        // Print input welcome symbols
        fputs("> ", ATCLI_OUT);

        // Input command and process it
        atcli_cmdintr_input(&cmd, cmdarr);

        puts(cmd);

        // Check command
        if (!atcli_cmdchk_no_exit(cmd)) {
            fprintf(ATCLI_OUT, "  Goodbye! %d\n", UNIVERSE_ANSWER);
            break;
        }
        // if (!atcli_cmd_check_is_valid(cmd, &atcmds)) {
        //     fputs("  ERROR : Command is not correct.\n", ATCLI_OUT);
        //     continue;
        // }

        // // Send valid cmd to socket
        // atcli_sock_send(sfd, cmd);
    }

    //*************************************************************************

    // Deinit history.
    if (!atcli_hist_deinit(&athist)) {
        fputs("ERROR : atcli_hist_deinit.\n", ATCLI_OUT);
        return false;
    }

    // Deinit AT commands check.
    if (!atcli_cmdchk_deinit(&atcmds)) {
        fputs("ERROR : atcli_cmdchk_deinit.\n", ATCLI_OUT);
        return false;
    }

    // Deinit socket.
    if (!atcli_sock_deinit(&atsock)) {
        fputs("ERROR : atcli_sock_deinit.\n", ATCLI_OUT);
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------

void atcli_cmdintr_print_header(void) {
    fputs("=============================================\n", ATCLI_OUT);
    fputs("| Welcome to Linux modem AT command client! |\n", ATCLI_OUT);
    fputs("|-------------------------------------------|\n", ATCLI_OUT);
    fputs("| Write AT command or \"0\" to exit.          |\n", ATCLI_OUT);
    fputs("=============================================\n", ATCLI_OUT);
}

//-----------------------------------------------------------------------------

// void atcli_cmdintr_input(char ** pcmd, char * cmd) {

    // // Get stdin input, less (or equal) CMD_LENGTH_MAX
    // fgets(cmd, CMD_LENGTH_MAX, stdin);

    // // Pass first spaces before command
    // *pcmd = cmd;
    // int i = 0;
    // for ( ; i < strlen(cmd); i++) {
    //     if (cmd[i] != ' ') break;
    //     *pcmd += 1;
    // }

    // // Take only one first word
    // for ( ; i < strlen(cmd); i++) {
    //     bool cmd_str_end = (
    //         cmd[i] == '\n' || cmd[i] == '\0' ||
    //         cmd[i] == ' '  || cmd[i] == EOF
    //     );
    //     if (cmd_str_end) {
    //         cmd[i] = '\0'; break;
    //     }
    // }

    // // Clean remain stdin symbols
    // rewind(stdin);
// }

void atcli_cmdintr_input(char ** pcmd, char * cmd) {

    // Set terminal input settings.
    tc_echo_off();
    tc_icanon_off();

    // Input loop.
    memset(cmd, 0x00, CMD_LENGTH_MAX);
    char inpsym;  // Input symbol.
    int  cpos = 0;  // Cursor position.
    while (true) {
        inpsym = getc(stdin);

        // Check arrow key pressed button.
        if (inpsym == 27) {
            if (getc(stdin) == 91) {
                switch (getc(stdin)) {
                    case 65:
                    // Arrow up.
                        // print history previous cmd
                        break;
                    case 66:
                    // Arrow down.
                        // print history next cmd
                        break;
                    case 67:
                    // Arrow right.
                        if (cpos >= CMD_LENGTH_MAX - 1) break;
                        tc_move_cursor_right(1);
                        cpos++;
                        break;
                    case 68:
                    // Arrow left.
                        if (cpos <= 0) break;
                        tc_move_cursor_left(1);
                        cpos--;
                        break;
                    default:
                        break;
                }
            }
            continue; // End loop cycle.
        }

        cmd[cpos] = inpsym;
        cpos++;

        putc(inpsym, stdout);

        // Check end cmd input symbol.
        if (inpsym == '\n') {
            break;
        }
    }

    // Set terminal input settings back.
    tc_echo_on();
    tc_icanon_on();

    // Pass first spaces before command
    *pcmd = cmd;
    int i = 0;
    for ( ; i < strlen(cmd); i++) {
        if (cmd[i] != ' ') break;
        *pcmd += 1;
    }

    // Take only one first word
    for ( ; i < strlen(cmd); i++) {
        bool cmd_str_end = (
            cmd[i] == '\n' || cmd[i] == '\0' ||
            cmd[i] == ' '  || cmd[i] == EOF
        );
        if (cmd_str_end) {
            cmd[i] = '\0'; break;
        }
    }

    // Clean remain stdin symbols
    rewind(stdin);
}

//-----------------------------------------------------------------------------

bool atcli_cmdchk_no_exit(const char * cmd) {
    char *exit_cmd = "0";
    return (strcmp(cmd, exit_cmd)) ? true : false;
}

//-----------------------------------------------------------------------------

bool atcli_cmdchk_is_valid(char * cmd, const struct atcmds * atcmds) {

    // Check min and max command length.
    if (strlen(cmd) < atcmds->length_min) return false;
    if (strlen(cmd) > CMD_LENGTH_MAX)     return false;

    // Make string uppercase
    for (int sym = 0; sym < strlen(cmd); sym++)
        cmd[sym] = toupper(cmd[sym]);

    // Check first 3 symbols of command.
    int sym = 0;
    if (cmd[sym++] != 'A') return false;
    if (cmd[sym++] != 'T') return false;
    if (cmd[sym++] != '+') return false;

    // Compare cmd string with every command in commands list.
    for (int icmd = 0; icmd < atcmds->number; icmd++) {
        if (!strncmp(&cmd[sym], atcmds->arr[icmd],
                strlen(atcmds->arr[icmd]))) {
            fprintf(ATCLI_OUT, "  COMMAND : \"%s\"\n", cmd);
            return true;
        }
    }

    return false;
}

//-----------------------------------------------------------------------------

bool atcli_cmdchk_init(struct atcmds * atcmds) {

    // Init command structure.
    atcmds->arr = NULL;
    atcmds->length_max = 0;
    atcmds->length_min = __INT_MAX__;
    atcmds->number = 0;
    atcmds->fp = NULL;

    // Open cmds file.
    atcmds->fp = fopen(CMDFILE_PATH, "r");
    if (atcmds->fp == NULL)
        return false;

    return true;
}

//-----------------------------------------------------------------------------

bool atcli_cmdchk_get_info(struct atcmds * atcmds) {

    // Array of commands.
    atcmds->arr = calloc(CMD_NUMBER_MAX, sizeof(char*));

    for (int icmd = 0; icmd < CMD_NUMBER_MAX; icmd++) {
        // Array for command strings.
        static char line[CMD_LENGTH_MAX];

        // Get command string.
        char *pline = fgets(line, CMD_LENGTH_MAX, atcmds->fp);
        if (pline == NULL) break;

        // Get commands number + 1.
        atcmds->number++;
        // Calculate command length.
        int cmd_length = strlen(line) - 1;
        // Get max commands length.
        atcmds->length_max = (atcmds->length_max < cmd_length) ?
            cmd_length : atcmds->length_max;
        // Get min commands length.
        atcmds->length_min = (atcmds->length_min > cmd_length) ?
            cmd_length : atcmds->length_min;
        
        // Allocate memory for cmd string.
        atcmds->arr[icmd] = malloc(strlen(line));
        // Getting rid of '\n'.
        line[strlen(line) - 1] = '\0';
        // Copy string to allocated memory.
        strcpy(atcmds->arr[icmd], line);
    }

    return true;
}

//-----------------------------------------------------------------------------

bool atcli_cmdchk_deinit(struct atcmds * atcmds) {

    // Deinit cmds string array.
    for (int icmd = 0; icmd < atcmds->number; icmd++) {
        free(atcmds->arr[icmd]);
    }

    free(atcmds->arr);

    // Close cmds file.
    if (fclose(atcmds->fp))
        return false;

    return true;
}

//-----------------------------------------------------------------------------

bool atcli_hist_init(struct athist * history) {

    // Init history structure.
    history->arr = NULL;
    history->cmd_index_curr = 0;
    history->cmd_index_last = 0;
    history->fd = NULL;

    // Open history file.
    history->fd = fopen(HIST_PATHNAME, "r+");
    if (history->fd == NULL) return false;

    // Create array of command history.
    history->arr = calloc(HIST_MAX_CAPACITY, sizeof(char*));

    // Read history file and get commands history.
    for (int icmd = 0; icmd < HIST_MAX_CAPACITY; icmd++) {
        // // Array for command strings.
        // static char line[CMD_LENGTH_MAX];

        // // Get command string.
        // char *pline = fgets(line, CMD_LENGTH_MAX, atcmds->fp);
        // if (pline == NULL) break;
        history->cmd_index_last = icmd;
        
        // Create read cmd buffer.
        char tmpcmd[CMD_LENGTH_MAX];
        memset(tmpcmd, 0x00, CMD_LENGTH_MAX);
        
        // Read line from history file.
        if (!atcli_hist_readline(history, tmpcmd, CMD_LENGTH_MAX))
            break;

        // Add read cmd to history array.
        history->arr[icmd] = malloc(strlen(tmpcmd));
    }    

    // Read and get index number.
    history->cmd_index_curr = 0;

    return true;
}

//-----------------------------------------------------------------------------

bool atcli_hist_deinit(struct athist * history) {

    // free history array mem //

    free(history->arr);

    close(history->fd);

    return true;
}

//-----------------------------------------------------------------------------

bool atcli_hist_add_cmd(struct athist * history, const char * cmd) {
    ;
}

//-----------------------------------------------------------------------------

bool atcli_hist_readline(struct athist * history,
    const char * line, const size_t linesize) {
    
    // Check history file descriptor.
    if (history->fd <= STDERR_FILENO)
        return false;

    while 
}

//-----------------------------------------------------------------------------

bool atcli_sock_init(struct atsock * sock) {
    
    sock->path = malloc(sizeof(SOCKET_PATH));
    if (sock->path == NULL)
        return false;
    strcpy(sock->path, SOCKET_PATH);

    sock->fd = 0;

    return true;
}

//-----------------------------------------------------------------------------

int atcli_sock_open(const struct atsock * sock) {

    struct sockaddr_un name;

    // Create local socket.
    int sfd = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if (sfd == -1) return -1;

    // Clear socket name structure.
    memset(&name, 0, sizeof(name));

    // Set socket name.
    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, sock->path, sizeof(name.sun_path) - 1);

    // Connect to socket.
    int res = connect(sfd, (const struct sockaddr *) &name, sizeof(name));
    if (res == -1) return -1;

    return sfd;
}

//-----------------------------------------------------------------------------

bool atcli_sock_send(const struct atsock * sock, const char * cmd) {

    int res = write(sock->fd, cmd, strlen(cmd) + 1);
    if (res == -1) return false;

    return true;
}

bool atcli_sock_deinit(struct atsock * sock) {
    free(sock->path);

    return true;
}
