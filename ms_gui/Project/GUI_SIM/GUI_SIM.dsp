# Microsoft Developer Studio Project File - Name="GUI_SIM" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=GUI_SIM - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GUI_SIM.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GUI_SIM.mak" CFG="GUI_SIM - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GUI_SIM - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "GUI_SIM - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GUI_SIM - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "GUI_SIM - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\..\\Source" /I "..\..\Resource" /I "..\..\Simulation" /I "..\..\PIC_Graphics" /I "..\..\Demo" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "GUI_SIM - Win32 Release"
# Name "GUI_SIM - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Simulation"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\Branding.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\emWin.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\GUI_X_SIM.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\LCD_X_SIM.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\LCDInfo0.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\LCDSIM.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Main.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SIM_X.c
# End Source File
# End Group
# Begin Group "ConvertColor"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\GUI\ConvertColor\LCDP111.c
# End Source File
# Begin Source File

SOURCE=..\..\GUI\ConvertColor\LCDP222.c
# End Source File
# Begin Source File

SOURCE=..\..\GUI\ConvertColor\LCDP233.c
# End Source File
# Begin Source File

SOURCE=..\..\GUI\ConvertColor\LCDP323.c
# End Source File
# Begin Source File

SOURCE=..\..\GUI\ConvertColor\LCDP332.c
# End Source File
# Begin Source File

SOURCE=..\..\GUI\ConvertColor\LCDP444.c
# End Source File
# Begin Source File

SOURCE=..\..\GUI\ConvertColor\LCDP555.c
# End Source File
# Begin Source File

SOURCE=..\..\GUI\ConvertColor\LCDP565.c
# End Source File
# Begin Source File

SOURCE=..\..\GUI\ConvertColor\LCDP8666.c
# End Source File
# Begin Source File

SOURCE=..\..\GUI\ConvertColor\LCDPM233.c
# End Source File
# Begin Source File

SOURCE=..\..\GUI\ConvertColor\LCDPM323.c
# End Source File
# Begin Source File

SOURCE=..\..\GUI\ConvertColor\LCDPM332.c
# End Source File
# Begin Source File

SOURCE=..\..\GUI\ConvertColor\LCDPM555.c
# End Source File
# Begin Source File

SOURCE=..\..\GUI\ConvertColor\LCDPM565.c
# End Source File
# End Group
# Begin Group "PIC_Graphics"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\PIC_Graphics\Button.c
# End Source File
# Begin Source File

SOURCE=..\..\PIC_Graphics\CheckBox.c
# End Source File
# Begin Source File

SOURCE=..\..\PIC_Graphics\CustomControlDemo.c
# End Source File
# Begin Source File

SOURCE=..\..\PIC_Graphics\DisplayL.c
# End Source File
# Begin Source File

SOURCE=..\..\PIC_Graphics\EditBox.c
# End Source File
# Begin Source File

SOURCE=..\..\PIC_Graphics\GOL.c
# End Source File
# Begin Source File

SOURCE=..\..\PIC_Graphics\GOLFontDefault.c
# End Source File
# Begin Source File

SOURCE=..\..\PIC_Graphics\GroupBox.c
# End Source File
# Begin Source File

SOURCE=..\..\PIC_Graphics\ListBox.c
# End Source File
# Begin Source File

SOURCE=..\..\PIC_Graphics\Meter.c
# End Source File
# Begin Source File

SOURCE=..\..\PIC_Graphics\Picture.c
# End Source File
# Begin Source File

SOURCE=..\..\PIC_Graphics\Primitive.c
# End Source File
# Begin Source File

SOURCE=..\..\PIC_Graphics\ProgressBar.c
# End Source File
# Begin Source File

SOURCE=..\..\PIC_Graphics\RadioButton.c
# End Source File
# Begin Source File

SOURCE=..\..\PIC_Graphics\RoundDial.c
# End Source File
# Begin Source File

SOURCE=..\..\PIC_Graphics\Slider.c
# End Source File
# Begin Source File

SOURCE=..\..\PIC_Graphics\StaticText.c
# End Source File
# Begin Source File

SOURCE=..\..\PIC_Graphics\Template.c
# End Source File
# Begin Source File

SOURCE=..\..\PIC_Graphics\Window.c
# End Source File
# End Group
# Begin Group "Demo"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Demo\Beep.c
# End Source File
# Begin Source File

SOURCE=..\..\Demo\Fonts.c
# End Source File
# Begin Source File

SOURCE=..\..\Demo\MainDemo.c
# End Source File
# Begin Source File

SOURCE=..\..\Demo\Pictures.c
# End Source File
# Begin Source File

SOURCE=..\..\Demo\rtcc.c
# End Source File
# Begin Source File

SOURCE=..\..\Demo\SideButtons.c
# End Source File
# Begin Source File

SOURCE=..\..\Demo\TouchScreen.c
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\Source\Common.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\..\Resource\emWin.rc
# End Source File
# End Group
# End Target
# End Project
