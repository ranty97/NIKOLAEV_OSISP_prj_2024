#include "grep.h"

// void execute_command_and_grep(char *command, int argc, char **argv, const char *pattern) {
//     int stdout_pipe[2], stderr_pipe[2];
//     char buffer[128];
//     pid_t pid;

//     // Создаем пайпы для stdout и stderr
//     if (pipe(stdout_pipe) == -1) {
//         perror("pipe");
//         exit(EXIT_FAILURE);
//     }
//     if (pipe(stderr_pipe) == -1) {
//         perror("pipe");
//         exit(EXIT_FAILURE);
//     }

//     // Создаем дочерний процесс
//     if ((pid = fork()) == -1) {
//         perror("fork");
//         exit(EXIT_FAILURE);
//     }

//     if (pid == 0) { // Дочерний процесс
//         // Закрываем каналы чтения пайпов
//         close(stdout_pipe[0]);
//         close(stderr_pipe[0]);

//         // Перенаправляем stdout и stderr на пайпы
//         dup2(stdout_pipe[1], STDOUT_FILENO);
//         dup2(stderr_pipe[1], STDERR_FILENO);

//         // Закрываем записи пайпов, которые больше не нужны
//         close(stdout_pipe[1]);
//         close(stderr_pipe[1]);

//         // Выполняем команду
//         execute_m(command, argc, argv);

//         exit(EXIT_SUCCESS);
//     } else { // Родительский процесс
//         FILE *grep_pipe;
//         char grep_command[256];

//         // Закрываем каналы записи пайпов
//         close(stdout_pipe[1]);
//         close(stderr_pipe[1]);

//         // Формируем команду grep
//         snprintf(grep_command, sizeof(grep_command), "grep %s", pattern);
//         grep_pipe = popen(grep_command, "w");
//         if (grep_pipe == NULL) {
//             perror("popen");
//             exit(EXIT_FAILURE);
//         }

//         // Читаем из пайпов и пишем в grep
//         while (read(stdout_pipe[0], buffer, sizeof(buffer)) > 0) {
//             fputs(buffer, grep_pipe);
//         }
//         while (read(stderr_pipe[0], buffer, sizeof(buffer)) > 0) {
//             fputs(buffer, grep_pipe);
//         }

//         // Закрываем пайпы и ждем завершения дочернего процесса
//         close(stdout_pipe[0]);
//         close(stderr_pipe[0]);
//         pclose(grep_pipe);
//         wait(NULL);
//     }
// }