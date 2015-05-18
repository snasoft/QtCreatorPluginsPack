;NSIS Modern User Interface
;Header Bitmap Example Script
;Written by Joost Verburg

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"
  !define MUI_ICON "Graphics\Icon\icon.ico"

;--------------------------------
;General

  ;Properly display all languages (Installer will not work on Windows 95, 98 or ME!)
  Unicode true
  
  ;Name and file
  Name "Qt Creator Plugins Pack 3.4.0"
  OutFile "../Distr/Qt Creator Plugins Pack 3.4.0.exe"

  ;Default installation folder
  InstallDir "C:\Qt\qtcreator-3.4.0"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\QtCreatorPluginsPack" ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel user

;--------------------------------
;Interface Configuration

  !define MUI_HEADERIMAGE "Graphics\Icon\orange-install.ico"
  !define MUI_HEADERIMAGE_BITMAP "Graphics\Header\header.bmp"
  !define MUI_ABORTWARNING
  
    ;Show all languages, despite user's codepage
  !define MUI_LANGDLL_ALLLANGUAGES

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "..\LICENSE"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English" ;first language is the default language
;  !insertmacro MUI_LANGUAGE "Russian"

;--------------------------------
;Reserve Files
  
  ;If you are using solid compression, files that are required before
  ;the actual installation should be stored first in the data block,
  ;because this will make your installer start faster.
  
  !insertmacro MUI_RESERVEFILE_LANGDLL
  
;--------------------------------
;Installer Sections
SectionGroup "Plugins" SecPlugins

Section "Qt Project Tools" SecQpt
  SetOutPath "$INSTDIR\tools\qpt"
  File "..\Bin\Plugins\qpt\qpt.exe"
  WriteRegStr HKCU "Software\Modern UI Test" "" "$INSTDIR\tools\qpt"
  WriteUninstaller "$INSTDIR\tools\qpt\Uninstall.exe"
SectionEnd

Section "CppCheck" SecQtcCppcheck
  SetOutPath "$INSTDIR\lib\qtcreator\plugins\pluginspack\cppcheck"
  File "..\Bin\Plugins\QtcCppcheck\QtcCppcheck.dll"
  WriteRegStr HKCU "Software\Modern UI Test" "" "$INSTDIR\lib\qtcreator\plugins\pluginspack\cppcheck"
  WriteUninstaller "$INSTDIR\lib\qtcreator\plugins\pluginspack\cppcheck\Uninstall.exe"
SectionEnd

Section "Duplicated code finder" SecSimianPlugin
  SetOutPath "$INSTDIR\lib\qtcreator\plugins\pluginspack\mediacontrol"
  File "..\Bin\Plugins\MediaControl\MediaControl.dll"
  WriteRegStr HKCU "Software\Modern UI Test" "" "$INSTDIR\lib\qtcreator\plugins\pluginspack\mediacontrol"
  WriteUninstaller "$INSTDIR\lib\qtcreator\plugins\pluginspack\mediacontrol\Uninstall.exe"
SectionEnd

Section "Doxygen" SecDoxygen
  SetOutPath "$INSTDIR\lib\qtcreator\plugins\pluginspack\doxygen"
  File "..\Bin\Plugins\Doxygen\Doxygen.dll"
  WriteRegStr HKCU "Software\Modern UI Test" "" "$INSTDIR\lib\qtcreator\plugins\pluginspack\doxygen"
  WriteUninstaller "$INSTDIR\lib\qtcreator\plugins\pluginspack\doxygen\Uninstall.exe"
SectionEnd

Section "Tabbed Editor" SecTabbedEditor
  SetOutPath "$INSTDIR\lib\qtcreator\plugins\pluginspack\tabbededitor"
  File "..\Bin\Plugins\TabbedEditor\TabbedEditor.dll"
  WriteRegStr HKCU "Software\Modern UI Test" "" "$INSTDIR\lib\qtcreator\plugins\pluginspack\tabbededitor"
  WriteUninstaller "$INSTDIR\lib\qtcreator\plugins\pluginspack\tabbededitor\Uninstall.exe"
SectionEnd

Section "Markdown Viewer" SecQtcMarkview
  SetOutPath "$INSTDIR\lib\qtcreator\plugins\pluginspack\markview"
  File "..\Bin\Plugins\QtcMarkview\QtcMarkview.dll"
  WriteRegStr HKCU "Software\Modern UI Test" "" "$INSTDIR\lib\qtcreator\plugins\pluginspack\markview"
  WriteUninstaller "$INSTDIR\lib\qtcreator\plugins\pluginspack\markview\Uninstall.exe"
SectionEnd

Section "Google Tests" SecQtcGtest
  SetOutPath "$INSTDIR\lib\qtcreator\plugins\pluginspack\googletest"
  File "..\Bin\Plugins\QtcGtest\QtcGtest.dll"
  WriteRegStr HKCU "Software\Modern UI Test" "" "$INSTDIR\lib\qtcreator\plugins\pluginspack\googletest"
  WriteUninstaller "$INSTDIR\lib\qtcreator\plugins\pluginspack\googletest\Uninstall.exe"
SectionEnd

Section "Database Viewer" SecQtcDbViewer
  SetOutPath "$INSTDIR\lib\qtcreator\plugins\pluginspack\dbviewer"
  File "..\Bin\Plugins\QtcDbViewer\QtcDbViewer.dll"
  WriteRegStr HKCU "Software\Modern UI Test" "" "$INSTDIR\lib\qtcreator\plugins\pluginspack\dbviewer"
  WriteUninstaller "$INSTDIR\lib\qtcreator\plugins\pluginspack\dbviewer\Uninstall.exe"
SectionEnd

Section "Gist" SecGist
  SetOutPath "$INSTDIR\lib\qtcreator\plugins\pluginspack\gist"
  File "..\Bin\Plugins\GistPlugin\GistPlugin.dll"
  WriteRegStr HKCU "Software\Modern UI Test" "" "$INSTDIR\lib\qtcreator\plugins\pluginspack\gist"
  WriteUninstaller "$INSTDIR\lib\qtcreator\plugins\pluginspack\gist\Uninstall.exe"
SectionEnd

Section "Jenkins" SecJenkins
  SetOutPath "$INSTDIR\lib\qtcreator\plugins\pluginspack\jenkins"
  File "..\Bin\Plugins\Jenkins\Jenkins.dll"
  WriteRegStr HKCU "Software\Modern UI Test" "" "$INSTDIR\lib\qtcreator\plugins\pluginspack\jenkins"
  WriteUninstaller "$INSTDIR\lib\qtcreator\plugins\pluginspack\jenkins\Uninstall.exe"
SectionEnd

Section "Project Line Counter" SecProjectLineCounter
  SetOutPath "$INSTDIR\lib\qtcreator\plugins\pluginspack\projectlinecounter"
  File "..\Bin\Plugins\ProjectLineCounter\ProjectLineCounter.dll"
  WriteRegStr HKCU "Software\Modern UI Test" "" "$INSTDIR\lib\qtcreator\plugins\pluginspack\projectlinecounter"
  WriteUninstaller "$INSTDIR\lib\qtcreator\plugins\pluginspack\projectlinecounter\Uninstall.exe"
SectionEnd

Section "Habrahabr.ru display" SecHabraStata
  SetOutPath "$INSTDIR\lib\qtcreator\plugins\pluginspack\habrastata"
  File "..\Bin\Plugins\HabraStata\HabraStata.dll"
  WriteRegStr HKCU "Software\Modern UI Test" "" "$INSTDIR\lib\qtcreator\plugins\pluginspack\habrastata"
  WriteUninstaller "$INSTDIR\lib\qtcreator\plugins\pluginspack\habrastata\Uninstall.exe"
SectionEnd

Section "Audio Player Control" SecMediaControl
  SetOutPath "$INSTDIR\lib\qtcreator\plugins\pluginspack\mediacontrol"
  File "..\Bin\Plugins\MediaControl\MediaControl.dll"
  WriteRegStr HKCU "Software\Modern UI Test" "" "$INSTDIR\lib\qtcreator\plugins\pluginspack\mediacontrol"
  WriteUninstaller "$INSTDIR\lib\qtcreator\plugins\pluginspack\mediacontrol\Uninstall.exe"
SectionEnd

SectionGroupEnd


SectionGroup "Developer Tools" SecTools

Section "CppCheck" SecCppCheck
  SetOutPath "$INSTDIR\tools\cppcheck"
  File /r "..\Bin\3rdParty\cppcheck"
  WriteRegStr HKCU "Software\Modern UI Test" "" "$INSTDIR\tools\cppcheck"
  WriteUninstaller "$INSTDIR\tools\cppcheck\Uninstall.exe"
SectionEnd

Section "Simian" SecSimian
  SetOutPath "$INSTDIR\tools\simian"
  File /r "..\Bin\3rdParty\simian"
  WriteRegStr HKCU "Software\Modern UI Test" "" "$INSTDIR\tools\simian"
  WriteUninstaller "$INSTDIR\tools\simian\Uninstall.exe"
SectionEnd

Section "Artistic Style" SecAstyle
  SetOutPath "$INSTDIR\tools\astyle"
  File /r "..\Bin\3rdParty\astyle"
  WriteRegStr HKCU "Software\Modern UI Test" "" "$INSTDIR\tools\astyle"
  WriteUninstaller "$INSTDIR\tools\astyle\Uninstall.exe"
SectionEnd

Section "CLOC" SecCloc
  SetOutPath "$INSTDIR\tools\cloc"
  File /r "..\Bin\3rdParty\cloc"
  WriteRegStr HKCU "Software\Modern UI Test" "" "$INSTDIR\tools\cloc"
  WriteUninstaller "$INSTDIR\tools\cloc\Uninstall.exe"
SectionEnd

Section "Vera++" SecVera
  SetOutPath "$INSTDIR\tools\vera"
  File /r "..\Bin\3rdParty\vera"
  WriteRegStr HKCU "Software\Modern UI Test" "" "$INSTDIR\tools\vera"
  WriteUninstaller "$INSTDIR\tools\vera\Uninstall.exe"
SectionEnd

SectionGroupEnd

;--------------------------------
;Descriptions

  ;Language strings
;ENGLISH
  LangString DESC_SecPlugins ${LANG_ENGLISH} "QtCreator plugins."
  LangString DESC_SecTools ${LANG_ENGLISH} "3rdParty developers tools, used in plugins."
  
  LangString DESC_SecQpt ${LANG_ENGLISH} "Qt Project Tool allow integrate 3rd Party tools to Qt Creator."
  LangString DESC_SecProjectLineCounter ${LANG_ENGLISH} "This plugin adds info string to the Qt Creator Editor toolbar that counts lines in the current file and whole project if pro-file opened."
  LangString DESC_SecDoxygen ${LANG_ENGLISH} "Simple QtCreator plugin that inserts Doxygen tags in source code and handles interaction with Doxygen binaries."
  LangString DESC_SecTabbedEditor ${LANG_ENGLISH} "Plugin add tabs for QtCreator source editor. It is very helpful when you edit less than 10 files simultaneously."
  LangString DESC_SecMediaControl ${LANG_ENGLISH} "Plugin allow you to control media player playback from QtCreator."
  LangString DESC_SecHabraStata ${LANG_ENGLISH} "Plugin regularly update and display user karma and rating on habrahabr.ru in QtCreator left panel."
  LangString DESC_SecQtcCppcheck ${LANG_ENGLISH} "Plugin integrates some Cppcheck (http://cppcheck.sourceforge.net/) functionality into Qt Creator."
  LangString DESC_SecQtcMarkview ${LANG_ENGLISH} "Viewer of markup documentation languages"
  LangString DESC_SecQtcGtest ${LANG_ENGLISH} "Integrates Google test framework into Qt Creator."
  LangString DESC_SecQtcDbViewer ${LANG_ENGLISH} "Viewer of markup documentation languages."
  LangString DESC_SecJenkins ${LANG_ENGLISH} "Jenkins plugin to show the status of the builds."
  
  LangString DESC_SecGist ${LANG_ENGLISH} "Plugin integrates some Gist functionality into Qt Creator IDE."
	
;Developer tools
   LangString DESC_SecSimian ${LANG_ENGLISH} "Similarity Analyser - required for Simian Plugin."
   LangString DESC_SecAstyle ${LANG_ENGLISH} "Artistic Style - required for Beautifier Plugin."
   LangString DESC_SecCloc ${LANG_ENGLISH} "CLOC - source code line counter."
   LangString DESC_SecCppCheck ${LANG_ENGLISH} "Static code analyzer."
   LangString DESC_SecVera ${LANG_ENGLISH} "Static code analyzer."
   
; RUSSIAN
	;Root sections
  LangString DESC_SecPlugins ${LANG_RUSSIAN} "Плагины QtCreator"
  LangString DESC_SecTools ${LANG_RUSSIAN} "Инструменты разработчика, используемые в плагинах."
	;Plugins
  LangString DESC_SecQpt ${LANG_RUSSIAN} "Позволяет интегрировать статические анализаторы кода и другие консольные утилиты в QtCreator."
  LangString DESC_SecProjectLineCounter ${LANG_RUSSIAN} "Отображает количество строк в текущем файле и проекте."
  LangString DESC_SecDoxygen ${LANG_RUSSIAN} "Вставляет теги Doxygen в исходном коде файла."
  LangString DESC_SecTabbedEditor ${LANG_RUSSIAN} "Добавляет табы в QtCreator."
  LangString DESC_SecMediaControl ${LANG_RUSSIAN} "Управление аудиоплеером из QtCreator."
  LangString DESC_SecHabraStata ${LANG_RUSSIAN} "Отображает карму и рейтинг пользователя habrahabr.ru."
  LangString DESC_SecQtcCppcheck ${LANG_RUSSIAN} "Интеграция CppCheck в QtCreator."
  LangString DESC_SecQtcMarkview ${LANG_RUSSIAN} "Просмотр Markview файлов."
  LangString DESC_SecQtcGtest ${LANG_RUSSIAN} "Результаты Google Tests в QtCreator."
  LangString DESC_SecQtcDbViewer ${LANG_RUSSIAN} "Просмотр баз данных."
  LangString DESC_SecJenkins ${LANG_RUSSIAN} "Статус сборки сервера CI Jenkins."
  LangString DESC_SecGist ${LANG_RUSSIAN} "Взаимодействие с Gist."
		
;Developer tools
   LangString DESC_SecSimian ${LANG_RUSSIAN} "Поиск дублирующегося кода - используется в плагине Simian."
   LangString DESC_SecAstyle ${LANG_RUSSIAN} "Форматирование кода - используется в плагине Beautifier."
   LangString DESC_SecCloc ${LANG_RUSSIAN} "Подсчет строк кода, комментариев и пустых строк."
   LangString DESC_SecCppCheck ${LANG_RUSSIAN} "Статический анализатор кода."
   LangString DESC_SecVera ${LANG_RUSSIAN} "Статический анализатор кода."

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
	;Root Sections 
	!insertmacro MUI_DESCRIPTION_TEXT ${SecPlugins} $(DESC_SecPlugins)
	!insertmacro MUI_DESCRIPTION_TEXT ${SecTools} $(DESC_SecTools)
	
		
	;3rdParty Developers Tools
	!insertmacro MUI_DESCRIPTION_TEXT ${SecSimian} $(DESC_SecSimian)
  	!insertmacro MUI_DESCRIPTION_TEXT ${SecAstyle} $(DESC_SecAstyle)
	!insertmacro MUI_DESCRIPTION_TEXT ${SecCloc} $(DESC_SecCloc)
	!insertmacro MUI_DESCRIPTION_TEXT ${SecCppCheck} $(DESC_SecCppCheck)
	!insertmacro MUI_DESCRIPTION_TEXT ${SecVera} $(DESC_SecVera)
	
	;Plugins
  	!insertmacro MUI_DESCRIPTION_TEXT ${SecQpt} $(DESC_SecQpt)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecProjectLineCounter} $(DESC_SecProjectLineCounter)
	!insertmacro MUI_DESCRIPTION_TEXT ${SecDoxygen} $(DESC_SecDoxygen)
	
	;Shikalev Andrei
	!insertmacro MUI_DESCRIPTION_TEXT ${SecTabbedEditor} $(DESC_SecTabbedEditor)
	!insertmacro MUI_DESCRIPTION_TEXT ${SecSimian} $(DESC_SecSimian)
	!insertmacro MUI_DESCRIPTION_TEXT ${SecHabraStata} $(DESC_SecHabraStata)
	!insertmacro MUI_DESCRIPTION_TEXT ${SecMediaControl} $(DESC_SecMediaControl)	

	;Gres
	!insertmacro MUI_DESCRIPTION_TEXT ${SecQtcCppcheck} $(DESC_SecQtcCppcheck)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecQtcGtest} $(DESC_SecQtcGtest)
	!insertmacro MUI_DESCRIPTION_TEXT ${SecQtcMarkview} $(DESC_SecQtcMarkview)
	!insertmacro MUI_DESCRIPTION_TEXT ${SecQtcDbViewer} $(DESC_SecQtcDbViewer)
	
	!insertmacro MUI_DESCRIPTION_TEXT ${SecJenkins} $(DESC_SecJenkins)
	
	!insertmacro MUI_DESCRIPTION_TEXT ${SecGist} $(DESC_SecGist)
	

  !insertmacro MUI_FUNCTION_DESCRIPTION_END
 
 
;--------------------------------
;Uninstaller Section

Section "Uninstall"

  ;ADD YOUR OWN FILES HERE...

  Delete "$INSTDIR\Uninstall.exe"

  RMDir "$INSTDIR"

  DeleteRegKey /ifempty HKCU "Software\Modern UI Test"

SectionEnd

;--------------------------------
;Uninstaller Functions

Function un.onInit

  !insertmacro MUI_UNGETLANGUAGE
  
FunctionEnd