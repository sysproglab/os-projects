; Имя приложения
#define   Name       "Chmod"
; Версия приложения
#define   Version    "0.0.1"
; Фирма-разработчик
#define   Publisher  "Fomina Masha"
; Имя исполняемого модуля
#define   ExeName    "chmod.exe"

[Setup]
; Уникальный идентификатор приложения, 
;сгенерированный через Tools -> Generate GUID
AppId={{19B052CC-D440-49D8-8720-842711A7B3DF}

; Прочая информация, отображаемая при установке
AppName={#Name}
AppVersion={#Version}
AppPublisher={#Publisher}

; Путь установки по-умолчанию
DefaultDirName={pf}\{#Name}
; Имя группы в меню "Пуск"
DefaultGroupName={#Name}

; Каталог, куда будет записан собранный setup и имя исполняемого файла
OutputDir="E:\всё\5 курс\Проектирование ОС и компонентов\отчеты\лаб 2, инсталляция\inno setup"
OutputBaseFileName="chmod-setup"

; Файл иконки
SetupIconFile="E:\всё\5 курс\Проектирование ОС и компонентов\отчеты\лаб 2, инсталляция\inno setup\icon.ico"

; Параметры сжатия
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"

[Tasks]
; Создание иконки на рабочем столе
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
; Исполняемый файл
Source: "E:\всё\5 курс\Проектирование ОС и компонентов\отчеты\лаб 2, инсталляция\inno setup\chmod.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\всё\5 курс\Проектирование ОС и компонентов\отчеты\лаб 2, инсталляция\inno setup\icon.ico"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\{#Name}"; Filename: "{app}\{#ExeName}"; IconFilename: "{app}\icon.ico"
Name: "{commondesktop}\{#Name}"; Filename: "{app}\{#ExeName}"; Tasks: desktopicon; IconFilename: "{app}\icon.ico"