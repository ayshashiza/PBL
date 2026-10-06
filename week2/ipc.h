#ifndef IPC_H
#define IPC_H

#include <mqueue.h>
#include <sys/types.h>

/* POSIX message queue names */
#define Q_UI_TO_CORE   "/ipc_ui_core"
#define Q_CORE_TO_UI   "/ipc_core_ui"
#define Q_CORE_TO_LOG  "/ipc_core_log"

/* Message sizes */
#define MAX_ARG    128
#define MAX_REPLY  256
#define MAX_LOG    256

/* Commands sent from UI to Core */
typedef enum
{
    CMD_LOAD,
    CMD_RUN,
    CMD_STEP,
    CMD_REG,
    CMD_MEM,
    CMD_STACK,
    CMD_QUEUE,
    CMD_EXIT,
    CMD_UNKNOWN
} CmdType;

/* Command message */
typedef struct
{
    CmdType type;
    char arg[MAX_ARG];
} Message;

/* Reply from Core to UI */
typedef struct
{
    int status;
    char text[MAX_REPLY];
} Reply;

/* Log message types */
typedef enum
{
    LOG_INFO,
    LOG_ERROR,
    LOG_DEBUG,
    LOG_QUIT

} LogType;

/* Message sent from Core to Logger */
typedef struct
{
    LogType type;
    char message[MAX_LOG];
} LogMessage;

#endif

