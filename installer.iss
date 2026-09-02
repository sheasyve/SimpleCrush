[Setup]
AppName=SimpleCrush
AppVersion=1.3.1
AppPublisher=Syverson Audio
DefaultDirName={autopf}\Syverson Audio\SimpleCrush
; CRITICAL: Required for {cf64} (Common Files 64-bit) to resolve correctly for VST3
ArchitecturesInstallIn64BitMode=x64
OutputBaseFilename=SimpleCrush_v1.3.1_Windows
PrivilegesRequired=admin
Compression=lzma
SolidCompression=yes
OutputDir=.\
DisableDirPage=yes 

[Files]
; 1. VST3 Plugin - Copying the whole root folder catches Resources/desktop.ini
Source: "C:\Users\sheam\Documents\VST\SimpleCrush.vst3\*"; DestDir: "{code:GetVST3Dir}\SimpleCrush.vst3"; Flags: ignoreversion recursesubdirs createallsubdirs

; 2. Presets - Removed skipifsourcedoesntexist so compiler catches missing files
Source: "C:\Users\sheam\Documents\Presets\*"; DestDir: "{code:GetDataDir}\Presets"; Flags: ignoreversion recursesubdirs createallsubdirs

; 3. Themes - Removed skipifsourcedoesntexist so compiler catches missing files
Source: "C:\Users\sheam\Documents\Themes\*.json"; DestDir: "{code:GetDataDir}\Themes"; Flags: ignoreversion recursesubdirs createallsubdirs

[Registry]
; HKLM (Local Machine) ensures the path is accessible to all users
Root: HKLM; Subkey: "Software\Syverson Audio\SimpleCrush"; ValueType: string; ValueName: "DataPath"; ValueData: "{code:GetDataDir}"; Flags: uninsdeletekey

[Code]
var
  AdvancedPage: TInputOptionWizardPage;
  CustomDirsPage: TInputDirWizardPage;

procedure InitializeWizard;
begin
  AdvancedPage := CreateInputOptionPage(wpWelcome,
    'Installation Options', 'How would you like to install SimpleCrush?',
    'Standard installation is recommended for most users. Installs the plugin and presets to the standard directories automatically detected by your DAW.',
    True, False);
  
  AdvancedPage.Add('Standard Installation (Recommended)');
  AdvancedPage.Add('Advanced Installation (Choose custom folder paths)');
  AdvancedPage.Values[0] := True;
  
  CustomDirsPage := CreateInputDirPage(AdvancedPage.ID,
    'Custom Installation Locations', 'Choose where SimpleCrush will be installed.',
    'Select your preferred destination folders. Note: Changing the default VST3 path may require you to manually update your DAW''s plugin scanner.',
    False, '');
  
  CustomDirsPage.Add('VST3 Plugin Folder:');
  CustomDirsPage.Add('Data Folder (Presets & Themes):'); 
  
  CustomDirsPage.Values[0] := ExpandConstant('{cf64}\VST3');
  // Uses C:\ProgramData\Syverson Audio\SimpleCrush
  CustomDirsPage.Values[1] := ExpandConstant('{commonappdata}\Syverson Audio\SimpleCrush');
end;

function ShouldSkipPage(PageID: Integer): Boolean;
begin
  Result := False;
  if (PageID = CustomDirsPage.ID) and (AdvancedPage.Values[0] = True) then
    Result := True;
end;

function GetVST3Dir(Param: String): String;
begin
  if AdvancedPage.Values[1] = True then
    Result := CustomDirsPage.Values[0]
  else
    Result := ExpandConstant('{cf64}\VST3');
end;

function GetDataDir(Param: String): String; 
begin
  if AdvancedPage.Values[1] = True then
    Result := CustomDirsPage.Values[1]
  else
    Result := ExpandConstant('{commonappdata}\Syverson Audio\SimpleCrush');
end;