; pbbrowser nsis ��ġ��ũ��Ʈ
; 2019.11.08 kim,jk

Name "PowerBase Plus Browser"
OutFile "pbbrowser_setup.exe"
RequestExecutionLevel admin
SilentInstall silent
InstallDir $PROGRAMFILES\PBBrowser

!include FileFunc.nsh

var cmd

	VIProductVersion "1.0.0.5"
	VIAddVersionKey /LANG=0 "ProductName" "PowerBase Browser"	
	VIAddVersionKey /LANG=0 "CompanyName" "Yuri Solution"
	VIAddVersionKey /LANG=0 "LegalTrademarks" "PowerBase Browser Installer is a trademakr of Yuri Solution"
	VIAddVersionKey /LANG=0 "FileVersion" "1.0.0.5"
	VIAddVersionKey /LANG=0 "LegalCopyright" "�� Yuri Solution"
	
Section ""
	; ��ġ���
	SetOutPath $INSTDIR
	; ��ġ���� ���
	File /r dist\*.*
SectionEnd