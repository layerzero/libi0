#ifndef _BIGMSG_H_
#define _BIGMSG_H_

#include "stdlib.h"

#define msg_option_t char*

option_block_t msg_option_to_option(msg_option_t option);

long systemMsg(char *command, msg_option_t option);

msg_option_t create_single_msg_option(char *addr, uint64_t len);

#endif // _BIGMSG_H_
