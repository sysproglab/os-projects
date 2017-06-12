int monitorProcess() {
    int      pid;
    int      status;    
    sigset_t sigset;
    siginfo_t siginfo;
    // настраиваем сигналы которые будем обрабатывать
    sigemptyset(&sigset);
    // сигнал остановки процесса пользователем
    sigaddset(&sigset, SIGQUIT);
    // сигнал для остановки процесса пользователем с терминала
    sigaddset(&sigset, SIGINT);
    // сигнал запроса завершения процесса
    sigaddset(&sigset, SIGTERM);
    // сигнал посылаемый при изменении статуса дочернего процесса
    sigaddset(&sigset, SIGCHLD);
    sigprocmask(SIG_BLOCK, &sigset, NULL);

    // данная функция создаст файл с нашим PID'ом
    setPidFile(PID_FILE);
    // бесконечный цикл работы
    for (;;) {
        // создаём потомка
        pid = fork();
        if (pid == -1) {
            break;
        } else if (!pid) {// если мы потомок
            // данный код выполняется в потомке
            // запустим функцию отвечающую за работу демона
            status = workingProcess();
            // завершим процесс
            exit(status);
        } else {// если мы родитель
            // данный код выполняется в родителе
            // ожидаем поступление сигнала
            sigwaitinfo(&sigset, &siginfo);
            // если пришел какой-либо ожидаемый сигнал
            // убьем потомка
            kill(pid, SIGKILL);
            status = 0;
            break;
        }
    }
    // удалим файл с PID'ом
    unlink(PID_FILE.c_str());
    return status;
} 


int workingProcess() {
    for(;;) {
        std::ofstream file;
        file.open("/tmp/Debugger.txt");
        file << "Debugger\n";
        file.close();
        sleep(10);
    }
    return 0;
} 

int main(int argc, char** argv) {
    int status;
    int pid;

    // создаем потомка
    pid = fork();

    if (pid == -1) 
        std::cout << "Error” << std::endl;
        return -1;

    } else if (!pid) {// если это потомок
        // данный код уже выполняется в процессе потомка
        // разрешаем выставлять все биты прав на создаваемые файлы,
        // иначе у нас могут быть проблемы с правами доступа
        umask(0);
        // создаём новый сеанс, чтобы не зависеть от родителя
        setsid();
        // переходим в корень диска, если мы этого не сделаем, то могут быть проблемы.
        // к примеру с размантированием дисков
        chdir("/");
        // закрываем дискрипторы ввода/вывода/ошибок, так как нам они больше не понадобятся
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);

        // Данная функция будет осуществлять слежение за процессом
        status = monitorProcess();
        return status;
    }
    else {// если это родитель
        // завершим процес, т.к. основную свою задачу (запуск демона) мы выполнили
        return 0;
    }
}
