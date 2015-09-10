/* Declaration of BigMessage functions in Puc
   Copyright 2014-15, Hututa Technologies */

#ifndef _BIGMSG_H_
#define _BIGMSG_H_

#include "stdlib.h"

#define msg_option_t char*

/* Apparently, convert a BigMessage option to be an option block.
   Returns the option block.     20150910/gl */ 
option_block_t msg_option_to_option(msg_option_t option);

/* Apparently, invoke command with option and wait till the command exits.
   Implemented with systemEx().

   Returns the return value of systemEx().   20150910/gl  */
long systemMsg(char *command, msg_option_t option);

/* Apparently, create the option for a single message.
   Returns the option.              20150910/gl */
msg_option_t create_single_msg_option(char *addr, uint64_t len);

#endif // _BIGMSG_H_
