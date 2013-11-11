# This example tests the compile time aspect of the Library macros
# more than the runtime aspect. It is more of a syntax example,
# rather than a usage example.

#!include "Library.nsh"


# All the other settings can be tweaked by editing the !defines at the top of this script
!define APPNAME "Tireless Worker"
!define COMPANYNAME "TWInspect"
!define DESCRIPTION "Experimet automation and data acquisition"
# These three must be integers
!define VERSIONMAJOR 0
!define VERSIONMINOR 8
!define VERSIONBUILD 2
# These will be displayed by the "Click here for support information" link in "Add/Remove Programs"
# It is possible to use "mailto:" links in here to open the email client
!define HELPURL "http://..." # "Support Information" link
!define UPDATEURL "http://..." # "Product Updates" link
!define ABOUTURL "http://..." # "Publisher" link
# This is the size (in kB) of all the files copied into "Program Files"
!define INSTALLSIZE 8180
OutFile "..\tw-${VERSIONMAJOR}.${VERSIONMINOR}.exe"
#RequestExecutionLevel admin ;Require admin rights on NT6+ (When UAC is turned on)
 
InstallDir "$PROGRAMFILES\${COMPANYNAME}\${APPNAME}"
Page directory
Page instfiles

XPStyle on

#RequestExecutionLevel user

!define wxbase28u_wat_custom '"${NSISDIR}\wxbase28u_wat_custom.dll"'
!define wxbase28u_xml_wat_custom '"${NSISDIR}\wxbase28u_xml_wat_custom.dll"'
!define wxmsw28u_adv_wat_custom '"${NSISDIR}\wxmsw28u_adv_wat_custom.dll"'
!define wxmsw28u_core_wat_custom '"${NSISDIR}\wxmsw28u_core_wat_custom.dll"'

#!define MainEXE '"${NSISDIR}\tirelessworker.exe"'

Section "Installer Section"


#!insertmacro InstallLib DLL       NOTSHARED REBOOT_PROTECTED      ${TestDLL} $INSTDIR\test.dll $INSTDIR
#!insertmacro InstallLib DLL       NOTSHARED NOREBOOT_PROTECTED    ${TestDLL} $INSTDIR\test.dll $INSTDIR
#!insertmacro InstallLib DLL       NOTSHARED REBOOT_NOTPROTECTED   ${TestDLL} $INSTDIR\test.dll $INSTDIR
#!insertmacro InstallLib DLL       NOTSHARED NOREBOOT_NOTPROTECTED ${wxbase28u_wat_custom} $INSTDIR\wxbase28u_wat_custom.dll $INSTDIR
#!insertmacro InstallLib DLL       NOTSHARED NOREBOOT_NOTPROTECTED ${wxbase28u_xml_wat_custom} $INSTDIR\wxbase28u_xml_wat_custom.dll $INSTDIR
#!insertmacro InstallLib DLL       NOTSHARED NOREBOOT_NOTPROTECTED ${wxmsw28u_adv_wat_custom} $INSTDIR\wxmsw28u_adv_wat_custom.dll $INSTDIR
#!insertmacro InstallLib DLL       NOTSHARED NOREBOOT_NOTPROTECTED ${wxmsw28u_core_wat_custom} $INSTDIR\wxmsw28u_core_wat_custom.dll $INSTDIR
SetOutPath "$INSTDIR"
File ..\wx\*.*
File ..\watcom\*.*
File ..\wat_mswudll\exe\*.*

SetOutPath "$INSTDIR\handlers"
File ..\wat_mswudll\dll\*.*

SetOutPath "$APPDATA\tmp"
#WriteRegStr HKLM "Software\${COMPANYNAME}\${APPNAME}\handlers" "generic" "$INSTDIR\handlers\generic.dll"

createDirectory "$SMPROGRAMS\${COMPANYNAME}"
	createShortCut "$SMPROGRAMS\${COMPANYNAME}\${APPNAME}.lnk" "$INSTDIR\tirelessworker.exe" "" ""
 
	# Registry information for add/remove programs
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "DisplayName" "${APPNAME} - ${DESCRIPTION}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "UninstallString" "$\"$INSTDIR\uninstall.exe$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "QuietUninstallString" "$\"$INSTDIR\uninstall.exe$\" /S"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "InstallLocation" "$\"$INSTDIR$\""
#	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "DisplayIcon" "$\"$INSTDIR\logo.ico$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "Publisher" "$\"${COMPANYNAME}$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "HelpLink" "$\"${HELPURL}$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "URLUpdateInfo" "$\"${UPDATEURL}$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "URLInfoAbout" "$\"${ABOUTURL}$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "DisplayVersion" "$\"${VERSIONMAJOR}.${VERSIONMINOR}.${VERSIONBUILD}$\""
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "VersionMajor" ${VERSIONMAJOR}
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "VersionMinor" ${VERSIONMINOR}
	# There is no option for modifying or repairing the install
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "NoModify" 1
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "NoRepair" 1
	# Set the INSTALLSIZE constant (!defined at the top of this script) so Add/Remove Programs can accurately report the size
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "EstimatedSize" ${INSTALLSIZE}


WriteUninstaller $INSTDIR\uninstall.exe

SectionEnd

function un.onInit
	SetShellVarContext all
 
	#Verify the uninstaller - last chance to back out
	MessageBox MB_OKCANCEL "Permanantly remove ${APPNAME}?" IDOK next
		Abort
	next:
#	!insertmacro VerifyUserIsAdmin
functionEnd

Section "un.Uninstaller"
# Remove Start Menu launcher
	delete "$SMPROGRAMS\${COMPANYNAME}\${APPNAME}.lnk"
	# Try to remove the Start Menu folder - this will only happen if it is empty
	rmDir "$SMPROGRAMS\${COMPANYNAME}"
 
	# Remove files
	delete $INSTDIR\*.*
	delete $INSTDIR\handlers\*.*
	rmDir $INSTDIR\handlers
	delete $INSTDIR\res\*.*
	rmDir $INSTDIR\res

	# Always delete uninstaller as the last action
	delete $INSTDIR\uninstall.exe
 
	# Try to remove the install directory - this will only happen if it is empty
	rmDir $INSTDIR
 
	# Remove uninstaller information from the registry
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}"

SectionEnd
