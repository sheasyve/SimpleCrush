[Setup]
AppName=SimpleCrush
AppVersion=1.0.3
AppPublisher=Syverson Audio
DefaultDirName={autopf}\Syverson Audio\SimpleCrush
DefaultGroupName=SimpleCrush
OutputBaseFilename=SimpleCrush_v1.0.3_Windows
PrivilegesRequired=admin
Compression=lzma
SolidCompression=yes
OutputDir=.\

[Files]
Source: "C:\Users\sheam\Documents\VST\SimpleCrush.vst3\Contents\x86_64-win"; DestDir: "{cf64}\VST3\SimpleCrush.vst3"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\SimpleCrush"; Filename: "{app}\SimpleCrush.exe"
Name: "{group}\Uninstall SimpleCrush"; Filename: "{uninstallexe}"