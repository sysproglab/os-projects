; ��� ����������
#define   Name       "FullInfoFile"
; ������ ����������
#define   Version    "0.0.1"
; �����-�����������
#define   Publisher  "Nikita Shalyapin"
; ��� ������������ ������
#define   ExeName    "FullInfoFile.exe"
[Setup]
; ���������� ������������� ����������, 
;��������������� ����� Tools -> Generate GUID
AppId={{A4BEA521-F1E2-4167-8019-0CF1ECDB91C8}

; ������ ����������, ������������ ��� ���������
AppName={#Name}
AppVersion={#Version}
AppPublisher={#Publisher}

; ���� ��������� ��-���������
DefaultDirName={pf}\{#Name}
; ��� ������ � ���� "����"
DefaultGroupName={#Name}

; �������, ���� ����� ������� ��������� setup � ��� ������������ �����
OutputDir="D:\!\13541_3.2\�� (�������� ����������������)\Nikita\Lab3\Setup"
OutputBaseFileName="FullInfoFile-setup"

; ���� ������
SetupIconFile="D:\!\13541_3.2\�� (�������� ����������������)\Nikita\Lab3\Setup\icon.ico"

; ��������� ������
Compression=lzma
SolidCompression=yes


[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"

[Tasks]
; �������� ������ �� ������� �����
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
; ����������� ����
Source: "D:\!\13541_3.2\�� (�������� ����������������)\Nikita\Lab3\Setup\FullInfoFile2.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\!\13541_3.2\�� (�������� ����������������)\Nikita\Lab3\Setup\icon.ico"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\{#Name}"; Filename: "{app}\{#ExeName}"; IconFilename: "{app}\icon.ico"
Name: "{commondesktop}\{#Name}"; Filename: "{app}\{#ExeName}"; Tasks: desktopicon; IconFilename: "{app}\icon.ico"