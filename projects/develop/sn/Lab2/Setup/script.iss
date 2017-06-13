; Имя приложения
#define   Name       "FullInfoFile"
; Версия приложения
#define   Version    "0.0.1"
; Фирма-разработчик
#define   Publisher  "Nikita Shalyapin"
; Имя исполняемого модуля
#define   ExeName    "FullInfoFile.exe"
[Setup]
; Уникальный идентификатор приложения, 
;сгенерированный через Tools -> Generate GUID
AppId={{A4BEA521-F1E2-4167-8019-0CF1ECDB91C8}

; Прочая информация, отображаемая при установке
AppName={#Name}
AppVersion={#Version}
AppPublisher={#Publisher}

; Путь установки по-умолчанию
DefaultDirName={pf}\{#Name}
; Имя группы в меню "Пуск"
DefaultGroupName={#Name}

; Каталог, куда будет записан собранный setup и имя исполняемого файла
OutputDir="D:\!\13541_3.2\ОС (агентное программирование)\Nikita\Lab3\Setup"
OutputBaseFileName="FullInfoFile-setup"

; Файл иконки
SetupIconFile="D:\!\13541_3.2\ОС (агентное программирование)\Nikita\Lab3\Setup\icon.ico"

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
Source: "D:\!\13541_3.2\ОС (агентное программирование)\Nikita\Lab3\Setup\FullInfoFile2.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\!\13541_3.2\ОС (агентное программирование)\Nikita\Lab3\Setup\icon.ico"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\{#Name}"; Filename: "{app}\{#ExeName}"; IconFilename: "{app}\icon.ico"
Name: "{commondesktop}\{#Name}"; Filename: "{app}\{#ExeName}"; Tasks: desktopicon; IconFilename: "{app}\icon.ico"