[Version]
Class=IEXPRESS
SEDVersion=3
[Options]
PackagePurpose=InstallApp
ShowInstallProgramWindow=1
HideExtractAnimation=1
UseLongFileName=1
InsideCompressed=0
CAB_FixedSize=0
CAB_ResvCodeSigning=0
RebootMode=N
InstallPrompt=%InstallPrompt%
DisplayLicense=%DisplayLicense%
FinishMessage=%FinishMessage%
TargetName=%TargetName%
FriendlyName=%FriendlyName%
AppLaunched=%AppLaunched%
PostInstallCmd=%PostInstallCmd%
AdminQuietInstCmd=%AdminQuietInstCmd%
UserQuietInstCmd=%UserQuietInstCmd%
SourceFiles=SourceFiles
[Strings]
InstallPrompt=
DisplayLicense=
FinishMessage=
TargetName=E:\Windows Desktop\Рабочий стол\ssh-keygen.EXE
FriendlyName=ssh-keygen
AppLaunched=powershell.exe -ExecutionPolicy Bypass -File open_ssh_key_gen.ps1 
PostInstallCmd=<None>
AdminQuietInstCmd=
UserQuietInstCmd=
FILE0="open_ssh_key_gen.ps1"
[SourceFiles]
SourceFiles0=E:\Windows Desktop\Рабочий стол\
[SourceFiles0]
%FILE0%=
