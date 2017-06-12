int monitorProcess() {
    int      pid;
    int      status;    
    sigset_t sigset;
    siginfo_t siginfo;
    // ����������� ������� ������� ����� ������������
    sigemptyset(&sigset);
    // ������ ��������� �������� �������������
    sigaddset(&sigset, SIGQUIT);
    // ������ ��� ��������� �������� ������������� � ���������
    sigaddset(&sigset, SIGINT);
    // ������ ������� ���������� ��������
    sigaddset(&sigset, SIGTERM);
    // ������ ���������� ��� ��������� ������� ��������� ��������
    sigaddset(&sigset, SIGCHLD);
    sigprocmask(SIG_BLOCK, &sigset, NULL);

    // ������ ������� ������� ���� � ����� PID'��
    setPidFile(PID_FILE);
    // ����������� ���� ������
    for (;;) {
        // ������ �������
        pid = fork();
        if (pid == -1) {
            break;
        } else if (!pid) {// ���� �� �������
            // ������ ��� ����������� � �������
            // �������� ������� ���������� �� ������ ������
            status = workingProcess();
            // �������� �������
            exit(status);
        } else {// ���� �� ��������
            // ������ ��� ����������� � ��������
            // ������� ����������� �������
            sigwaitinfo(&sigset, &siginfo);
            // ���� ������ �����-���� ��������� ������
            // ����� �������
            kill(pid, SIGKILL);
            status = 0;
            break;
        }
    }
    // ������ ���� � PID'��
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

    // ������� �������
    pid = fork();

    if (pid == -1) 
        std::cout << "Error� << std::endl;
        return -1;

    } else if (!pid) {// ���� ��� �������
        // ������ ��� ��� ����������� � �������� �������
        // ��������� ���������� ��� ���� ���� �� ����������� �����,
        // ����� � ��� ����� ���� �������� � ������� �������
        umask(0);
        // ������ ����� �����, ����� �� �������� �� ��������
        setsid();
        // ��������� � ������ �����, ���� �� ����� �� �������, �� ����� ���� ��������.
        // � ������� � ���������������� ������
        chdir("/");
        // ��������� ����������� �����/������/������, ��� ��� ��� ��� ������ �� �����������
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);

        // ������ ������� ����� ������������ �������� �� ���������
        status = monitorProcess();
        return status;
    }
    else {// ���� ��� ��������
        // �������� ������, �.�. �������� ���� ������ (������ ������) �� ���������
        return 0;
    }
}
