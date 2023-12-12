#include <stdio.h>
#include <unistd.h>

int main(void) {
    pid_t pid;

    printf("Avant fork\n");
    printf("Je suis le processus initial (getpid %d)\n", getpid());
    pid = fork(); // Crée un nouveau processus
    printf("pid = fork() = %d\n", pid);

    if (pid == -1) {
        perror("Erreur lors de l'appel à fork");
        return 1;
    }

    if (pid > 0) {
        // Code du processus parent
        printf("Je suis le processus parent (getpid %d), pid : %d\n", getpid(), pid);
    } else if (pid == 0) {
        // Code du processus enfant
        printf("Je suis le processus enfant (getpid %d), pid : %d\n", getpid(), pid);
    }
    
    else printf("toto");

    printf("Après fork\n");

    return 0;
}
