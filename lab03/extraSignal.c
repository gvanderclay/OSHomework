#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
void shutdownHandler(int, siginfo_t *, void *);
void sigUSRHandler(int, siginfo_t *, void *);

// array of children pids
pid_t pids[2];

int main() {
  // struct to handle SIGUSR 1 and 2 signals
  struct sigaction usract;
  memset(&usract, '\0', sizeof(usract));
  // assign signal handler for usract
  usract.sa_sigaction = &sigUSRHandler;
  // tell sigaction to use the sa_sigaction field
  usract.sa_flags = SA_SIGINFO;

  if (sigaction(SIGUSR1, &usract, NULL) < 0) {
    perror("sigaction");
    return 1;
  }

  if (sigaction(SIGUSR2, &usract, NULL) < 0) {
    perror("sigaction");
    return 1;
  }
  // Store the pid of the parent
  pid_t ppid = getpid();
  srand(time(NULL));

  // spawn the children
  for (int i = 0; i < 2; i++) {
    // if this process is the parent make a fork and
    // add the child process to the pids array
    if (ppid == getpid()) {
      if ((pids[i] = fork()) < 0) {
        perror("fork");
      }
      if (ppid == getpid()) {
        printf("spawned child PID# %d\n", pids[i]);
      }
    }
  }
  if (pids[0] != 0 && pids[1] != 0) {
    struct sigaction shutdownAct;
    memset(&shutdownAct, '\0', sizeof(shutdownAct));
    // assign signal handler for shutdownAct
    shutdownAct.sa_sigaction = &shutdownHandler;
    // tell sigaction to use the sa_sigaction field
    shutdownAct.sa_flags = SA_SIGINFO;

    if (sigaction(SIGINT, &shutdownAct, NULL) < 0) {
      perror("sigaction");
      return 1;
    }
    while (1) {
      printf("waiting...  ");
      fflush(stdout);
      pause();
    }

  } else {
    // send random signal from child
    while (1) {
      int signalChoice = rand() % 2;
      int sleepTime = rand() % 5;
      sleep(sleepTime);
      if (signalChoice == 0) {
        kill(ppid, SIGUSR1);
      } else {
        kill(ppid, SIGUSR2);
      }
    }
  }

  return 0;
}

void shutdownHandler(int sigNum, siginfo_t *siginfo, void *context) {
  printf(" received\n");
  printf("That's it. I'm shutting you down...\n");
  for (int i = 0; i < 2; i++) {
    kill(pids[i], SIGKILL);
  }
  exit(0);
}

void sigUSRHandler(int sigNum, siginfo_t *siginfo, void *context) {
  char *signalName;
  long senderPID = (long)siginfo->si_pid;
  if (sigNum == 10) {
    signalName = "SIGUSR1";
  } else {
    signalName = "SIGUSR2";
  }
  printf(" received a %s signal, from PID %ld\n", signalName, senderPID);
}
