using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using System.Threading.Tasks;

namespace ProcService
{
    public partial class ProcService : ServiceBase
    {
        string name = "TaskManager.exe";
        string path = "D:\\Study\\10 семестр\\Проектирование ОС\\WinUtil\\TaskManager\\TaskManager\\";
        string log = "C:\\log.txt";

        public ProcService()
        {
            InitializeComponent();
        }

        protected override void OnStart(string[] args)
        {
            Process.Start(path + name, log);
        }

        protected override void OnStop()
        {
            System.Diagnostics.Process[] etc = System.Diagnostics.Process.GetProcesses();//получим процессы
            foreach (System.Diagnostics.Process anti in etc)//обойдем каждый процесс
                if (anti.ProcessName.ToLower().Contains(name.ToLower())) anti.Kill();
        }
    }
}
