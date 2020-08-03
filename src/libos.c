#include "libos.h"
#include <signal.h>
#include <assert.h>
#include <types.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define UNUSED(x) (void)(x)

// Crash related functions
void dump_crash_to_file(exit_type_t exit_type, raw_input_t *data) {

  UNUSED(exit_type);

  char *filename = ck_alloc(100);

  /* This filename will be replaced by "crashes-SHA_OF_BYTES" later */
  snprintf(filename, 100, "crashes-%d", rand());

  FILE *f = fopen(filename, "w+");
  fwrite(data->bytes, 1, data->len, f);

  fclose(f);

}

// Process related functions

static process_t *current_process;

void _afl_process_init_(process_t *process) {

  process->current = return_current_default;
  process->fork = do_fork_default;

  process->resume = resume_default;
  process->wait = wait_default;
  process->suspend = suspend_default;

}

process_t *return_current_default(process_t *process) {

  UNUSED(process);

  if (current_process) return current_process;

  process_t *p = afl_process_init(NULL, getpid());

  current_process = p;
  return p;

}

fork_result_t do_fork_default(process_t *process) {

  pid_t child = fork();

  if (child == 0)
    return CHILD;
  else if (child < 0)
    return FORK_FAILED;

  process->handler_process = child;
  return PARENT;

}

void suspend_default(process_t *process) {

  kill(process->handler_process, SIGSTOP);

}

void resume_default(process_t *process) {

  kill(process->handler_process, SIGCONT);

}

exit_type_t wait_default(process_t *process, bool untraced) {

  int status = 0;
  if (waitpid((process->handler_process), &status, untraced ? WUNTRACED : 0) <
      0)
    return -1;  // Waitpid fails here, how should we handle this?

  if (WIFEXITED(status)) return NORMAL;

  // If the process was simply stopped , we return STOP
  if (WIFSTOPPED(status)) return STOP;

  // If the process exited with a signal, we check the corresponsing signum of
  // the process and return values correspondingly
  if (WIFSIGNALED(status)) {

    int signal_num = WTERMSIG(status);  // signal number
    switch (signal_num) {

      case SIGKILL:
        return TIMEOUT;
      case SIGSEGV:
        return SEGV;
      case SIGABRT:
        return ABRT;
      case SIGBUS:
        return BUS;
      case SIGILL:
        return ILL;
      default:
        /* Any other SIGNAL we need to take care of? */
        return CRASH;

    }

  }

  else {

    FATAL("Currently Unhandled");

  }

}
