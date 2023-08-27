/**
 * @file debug.h
 * @brief Manages displaying messages in the debug window
 * @author Laurence Scotford
 */

void set_debug_mode(bool mode);
bool get_debug_mode(void);
void debug_message(char *message, ...);