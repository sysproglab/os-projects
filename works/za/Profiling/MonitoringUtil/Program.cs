using System;
using System.IO;
using DynamicLibraryFSW;
using StaticLibraryFSW;
using System.Security.Permissions;

namespace MonitoringUtil
{
    class Program
    {
        [PermissionSet(SecurityAction.Demand, Name = "FullTrust")]
        private static void Main()
        {
            while (true)
            {
                //Ввод отслеживаемой директории
                Console.WriteLine("Enter directory name for monitoring:");
                var directory = Console.ReadLine();

                //Проверка введенной директории на корректность
                try
                {
                    if ("".Equals(directory)) throw new FileNotFoundException();
                    if ("q".Equals(directory))
                    {
                        Console.WriteLine("Exit from util");
                        return;
                    }
                }
                catch (FileNotFoundException)
                {
                    Console.WriteLine("Error: Incorrect directory name");
                    continue;
                }
                catch (Exception e)
                {
                    Console.WriteLine(e.Message);
                    continue;
                }

                //Выбор дальнейшей работы утилиты
                //Будут отслеживаться изменения всей директории или одного конкретного файла
                Console.WriteLine("Press 'f' for selecting file for monitoring changes");
                Console.WriteLine("Press any key for monitoring changes of all files in selected direcory");

                var typedValue = Console.ReadLine();
                Console.WriteLine();
                //Ключ утилиты соответствующий отслеживанию конкретного файла
                if ("f".Equals(typedValue.ToLower()))
                {
                    //Метод отслеживания конкретного файла
                    if (!OneFile(directory)) return;
                    continue;
                }
                //Отслеживание всей директории
                if (!AllFiles(directory)) return;
            }
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

                //Метод из динамической библиотеки
                //var fsWatcher = ;

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

        #region Comment
        /*
        private static void WatcherEvents(FileSystemWatcher fsWatcher)
        {
            //Задание отслеживаемых изменений
            fsWatcher.NotifyFilter = NotifyFilters.Attributes
                                     | NotifyFilters.CreationTime
                                     | NotifyFilters.Security
                                     | NotifyFilters.Size
                                     | NotifyFilters.DirectoryName
                                     | NotifyFilters.FileName
                                     | NotifyFilters.LastAccess
                                     | NotifyFilters.LastWrite;

            //Подпись на события изменений
            fsWatcher.Created += OnCreated;
            fsWatcher.Changed += OnChanged;
            fsWatcher.Deleted += OnDeleted;
            fsWatcher.Renamed += OnRenamed;
            fsWatcher.Error += OnError;
            
            //Обеспечение отслеживания изменений
            fsWatcher.EnableRaisingEvents = true;
        }

        private static void OnCreated(object source, FileSystemEventArgs e)
        {
            Console.WriteLine("{0}\nFile {1} was created",
                DateTime.Now, e.FullPath, e.ChangeType);
        }


        private static void OnChanged(object source, FileSystemEventArgs e)
        {
            Console.WriteLine("{0}\nFile {1}\tChange type: {2}", 
                DateTime.Now, e.FullPath, e.ChangeType);
        }

        private static void OnDeleted(object source, FileSystemEventArgs e)
        {
            Console.WriteLine("{0}\nFile {1} was deleted",
                DateTime.Now, e.FullPath);
        }

        private static void OnRenamed(object source, RenamedEventArgs e)
        {
            Console.WriteLine("{0}\nFile {1} was renamed to {2}", 
                DateTime.Now, e.OldFullPath, e.FullPath);
        }

        private static void OnError(object source, ErrorEventArgs e)
        {
            Console.WriteLine("There was an error {0}", e);
        }*/
        #endregion

    }
}
