; ��� ����������
#define   Name       "Chmod"
; ������ ����������
#define   Version    "0.0.1"
; �����-�����������
#define   Publisher  "Fomina Masha"
; ��� ������������ ������
#define   ExeName    "chmod.exe"

[Setup]
; ���������� ������������� ����������, 
;��������������� ����� Tools -> Generate GUID
AppId={{19B052CC-D440-49D8-8720-842711A7B3DF}

; ������ ����������, ������������ ��� ���������
AppName={#Name}
AppVersion={#Version}
AppPublisher={#Publisher}

; ���� ��������� ��-���������
DefaultDirName={pf}\{#Name}
; ��� ������ � ���� "����"
DefaultGroupName={#Name}

; �������, ���� ����� ������� ��������� setup � ��� ������������ �����
OutputDir="E:\��\5 ����\�������������� �� � �����������\������\��� 2, �����������\inno setup"
OutputBaseFileName="chmod-setup"

; ���� ������
SetupIconFile="E:\��\5 ����\�������������� �� � �����������\������\��� 2, �����������\inno setup\icon.ico"

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
Source: "E:\��\5 ����\�������������� �� � �����������\������\��� 2, �����������\inno setup\chmod.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\��\5 ����\�������������� �� � �����������\������\��� 2, �����������\inno setup\icon.ico"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\{#Name}"; Filename: "{app}\{#ExeName}"; IconFilename: "{app}\icon.ico"
Name: "{commondesktop}\{#Name}"; Filename: "{app}\{#ExeName}"; Tasks: desktopicon; IconFilename: "{app}\icon.ico"