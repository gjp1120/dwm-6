#define _GNU_SOURCE
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

volatile int should_restart_dwm = 0;
pid_t dwm_pid = 0;

#define CMD (const char*[])
#define ASSERT(val) do{if ((val) == 0) abort();}while(0)

pid_t start_subprocess(const char *cmd[])
{
  pid_t child;

  if ((child = vfork()) == 0) {
    execvp(cmd[0], (char * const *)cmd);
  }
  else {
    return child;
  }
  return 0;
}

void SIGCHLD_handler(int sig, siginfo_t *siginfo, void *ignore)
{
  printf("child(%d) exited, code %d\n", siginfo->si_pid, siginfo->si_status);
  
  if (siginfo->si_pid == dwm_pid) {
    if (should_restart_dwm) {
      start_subprocess(CMD{"dwm", NULL});
      should_restart_dwm = 0;
    }
    else {
      exit(EXIT_SUCCESS);
    }
  }
}


void SIGUSR1_handler(int sig, siginfo_t *siginfo, void *ignore)
{
  should_restart_dwm = 1;
}

int main(int argc, char **argv)
{
  //绑定SIGCHLD
  struct sigaction act_sigchld;
  act_sigchld.sa_flags = SA_SIGINFO|SA_NOCLDSTOP|SA_NOCLDWAIT;
  act_sigchld.sa_sigaction = SIGCHLD_handler;
  sigaction(SIGCHLD, &act_sigchld, NULL);

  //绑定SIGUSR1
  struct sigaction act_sigusr1;
  act_sigusr1.sa_sigaction = SIGUSR1_handler;
  sigaction(SIGUSR1, &act_sigusr1, NULL);

  //启动DWM
  ASSERT(dwm_pid = start_subprocess(CMD{"dwm", NULL}));

  //信号循环
  while (pause()){};
  
}
