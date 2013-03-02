#define _GNU_SOURCE
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

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

void run(const char *cmd[])
{
  pid_t child;

  if ((child = vfork()) == 0) {
    execvp(cmd[0], (char * const *)cmd);
  }
  else {
    waitpid(child, NULL, 0);
  }
}

void SIGCHLD_handler(int sig, siginfo_t *siginfo, void *ignore)
{
  printf("child(%d) exited, code %d\n", siginfo->si_pid, siginfo->si_status);
  
  if (siginfo->si_pid == dwm_pid) {
    if (siginfo->si_status == 42) {
      ASSERT(dwm_pid = start_subprocess(CMD{"dwm", NULL}));
      should_restart_dwm = 0;
    }
    else {
      exit(EXIT_SUCCESS);
    }
  }
}


int main(int argc, char **argv)
{
  //绑定SIGCHLD
  struct sigaction act_sigchld;
  act_sigchld.sa_flags = SA_SIGINFO|SA_NOCLDSTOP|SA_NOCLDWAIT;
  act_sigchld.sa_sigaction = SIGCHLD_handler;
  sigaction(SIGCHLD, &act_sigchld, NULL);

  //设定X
  run(CMD{"xset", "+fp", "/usr/share/fonts/bitmap", NULL});
  run(CMD{"xset", "fp", "rehash", NULL});

  //配置MPC
  setenv("MPD_HOST", "Yayoi@localhost", 1);
  setenv("MPD_PORT", "15903", 1);

  start_subprocess(CMD{"urxvtd", NULL});
  start_subprocess(CMD{"start-pulseaudio-x11", NULL});

  //启动DWM
  ASSERT(dwm_pid = start_subprocess(CMD{"dwm", NULL}));

  //信号循环
  while (pause()){};
  
}
