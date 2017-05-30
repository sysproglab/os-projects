using System;
using System.Security.Permissions;
using System.ServiceProcess;
using DynamicLibraryFSW;
using StaticLibraryFSW;
using System.Threading;

namespace FileWatcherService
{
    public partial class Service1 : ServiceBase
    {
        Watch watch;
        public Service1()
        {
            InitializeComponent();
            this.CanStop = true;
            this.CanPauseAndContinue = true;
            this.AutoLog = true;
        }

        protected override void OnStart(string[] args)
        {
            watch = new Watch();
            Thread watchThread = new Thread(new ThreadStart(watch.Start));
            watchThread.Start();
        }

        protected override void OnStop()
        {
            watch.Stop();
            Thread.Sleep(10000);
        }
    }

    class Watch
    {
        bool enabled = true;
        [PermissionSet(SecurityAction.Demand, Name = "FullTrust")]

        public Watch()
        {
            AllFiles("D:\\DirForWatching");
        }

        public void Start()
        {
            while (enabled)
            {
                Thread.Sleep(10000);
            }
        }

        public void Stop()
        {
            enabled = false;
        }



        private static bool AllFiles(string path)
        {
            while (true)
            {
                //Ключи утилиты для выхода и для ввода новой директории
                Console.WriteLine("There will be changes of all files in the directory you entered");
                Console.WriteLine();
                Console.WriteLine("Press 'q' to quit this util");
                Console.WriteLine("Press 'd' to enter new directory");
                Console.WriteLine();

                //Метод задания всех свойств объекта класса
                //+Метод из динамической библиотеки
                WatcherEven.WatcherEvents(Watcher.GetFolderWatcher(path));

                var typedValue = Console.ReadLine();
                Console.WriteLine();

                //Выход из метода
                if ("q".Equals(typedValue.ToLower())) return false;
                if ("d".Equals(typedValue.ToLower())) return true;
            }
        }

        private static bool OneFile(string path)
        {
            while (true)
            {
                //Ввод имени файла
                //Console.WriteLine("Enter file name for monitoring:");
                Console.WriteLine();
                Console.WriteLine("Press 'q' to quit this util");
                Console.WriteLine("Press 'd' to enter new directory");

                //Метод задания всех свойств объекта класса
                WatcherEven.WatcherEvents(Watcher.GetFileWatcher(path));

                var typedValue = Console.ReadLine();
                Console.WriteLine();

                //Выход из метода
                if ("q".Equals(typedValue.ToLower())) return false;
                if ("d".Equals(typedValue.ToLower())) return true;
            }
        }
    }
}
