# Microsoft Developer Studio Project File - Name="Iperlogo" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Iperlogo - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Iperlogo.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Iperlogo.mak" CFG="Iperlogo - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Iperlogo - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Iperlogo - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Iperlogo - Win32 Release"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /I "Inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x410 /d "NDEBUG"
# ADD RSC /l 0x410 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 oldnames.lib /nologo /stack:0x2800 /subsystem:windows /map:"FULL" /machine:IX86
# ADD LINK32 oldnames.lib winmm.lib vfw32.lib ilconfig.lib cProtect\iperlogo.lib CxImage\CxImageDLL\Release\cximage.lib /nologo /subsystem:windows /map:"FULL" /machine:IX86

!ELSEIF  "$(CFG)" == "Iperlogo - Win32 Debug"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /I "Inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x410 /d "_DEBUG"
# ADD RSC /l 0x410 /i "inc" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 oldnames.lib ddeml.lib /nologo /stack:0x2800 /subsystem:windows /debug /machine:IX86 /pdbtype:sept
# ADD LINK32 oldnames.lib winmm.lib vfw32.lib ilconfig.lib cProtect\iperlogo.lib CxImage\CxImageDLL\Release\cximage.lib /nologo /subsystem:windows /debug /machine:IX86 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Iperlogo - Win32 Release"
# Name "Iperlogo - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat"
# Begin Source File

SOURCE=.\Src\Activear.cpp
# ADD BASE CPP /Yu"STDAFX.H"
# ADD CPP /Yu"STDAFX.H"
# End Source File
# Begin Source File

SOURCE=.\Src\aiuto.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\CDib.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\crack.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\dao.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Editorfr.cpp
# ADD BASE CPP /Yu"STDAFX.H"
# ADD CPP /Yu"STDAFX.H"
# End Source File
# Begin Source File

SOURCE=.\Src\Editorvw.cpp
# ADD BASE CPP /Yu"STDAFX.H"
# ADD CPP /Yu"STDAFX.H"
# End Source File
# Begin Source File

SOURCE=.\Src\Ilanalis.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Ilargvar.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Ilcatene.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Ilcodici.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Ilcomune.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Ilcontro.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Ildde.cpp
# ADD BASE CPP /Yu"STDAFX.H"
# ADD CPP /Yu"STDAFX.H"
# End Source File
# Begin Source File

SOURCE=.\Src\Ildef.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Ildialog.cpp
# ADD BASE CPP /Yu"STDAFX.H"
# ADD CPP /Yu"STDAFX.H"
# End Source File
# Begin Source File

SOURCE=.\Src\Ildizion.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Ilflussi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Ilgui.cpp
# ADD BASE CPP /Yu"STDAFX.H"
# ADD CPP /Yu"STDAFX.H"
# End Source File
# Begin Source File

SOURCE=.\Src\Ilinic.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Illogica.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Ilmain.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Ilmm.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Ilmondo.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Ilnodi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Ilnumeri.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Ilparlis.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Ilscrivi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Ilsis.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Ilspazio.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Ilstack.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Iltesto.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Iltimer.cpp
# ADD BASE CPP /Yu"STDAFX.H"
# ADD CPP /Yu"STDAFX.H"
# End Source File
# Begin Source File

SOURCE=.\Src\Ilutil.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\internet.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Iperldoc.cpp
# ADD BASE CPP /Yu"STDAFX.H"
# ADD CPP /Yu"STDAFX.H"
# End Source File
# Begin Source File

SOURCE=.\Src\Iperlogo.cpp
# ADD BASE CPP /Yu"STDAFX.H"
# ADD CPP /Yu"STDAFX.H"
# End Source File
# Begin Source File

SOURCE=.\Iperlogo.def
# End Source File
# Begin Source File

SOURCE=.\Hlp\Iperlogo.hpj
# End Source File
# Begin Source File

SOURCE=.\Iperlogo.rc
# End Source File
# Begin Source File

SOURCE=.\Src\Iperlvw.cpp
# ADD BASE CPP /Yu"STDAFX.H"
# ADD CPP /Yu"STDAFX.H"
# End Source File
# Begin Source File

SOURCE=.\Src\Kbdialog.cpp
# ADD BASE CPP /Yu"STDAFX.H"
# ADD CPP /Yu"STDAFX.H"
# End Source File
# Begin Source File

SOURCE=.\Src\Lineedit.cpp
# ADD BASE CPP /Yu"STDAFX.H"
# ADD CPP /Yu"STDAFX.H"
# End Source File
# Begin Source File

SOURCE=.\Src\Mainfrm.cpp
# ADD BASE CPP /Yu"STDAFX.H"
# ADD CPP /Yu"STDAFX.H"
# End Source File
# Begin Source File

SOURCE=.\Src\Mainstat.cpp
# ADD BASE CPP /Yu"STDAFX.H"
# ADD CPP /Yu"STDAFX.H"
# End Source File
# Begin Source File

SOURCE=.\Src\MciMidi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\MciPlay.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\menu.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Midi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Modeless.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Paletted.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Register.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\SelglossD.cpp
# ADD CPP /I ".."
# End Source File
# Begin Source File

SOURCE=.\Src\Selprocd.cpp
# ADD BASE CPP /Yu"STDAFX.H"
# ADD CPP /Yu"STDAFX.H"
# End Source File
# Begin Source File

SOURCE=.\SRC\shell.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Statdial.cpp
# ADD BASE CPP /Yu"STDAFX.H"
# ADD CPP /Yu"STDAFX.H"
# End Source File
# Begin Source File

SOURCE=.\Src\Stdafx.cpp
# ADD BASE CPP /Yc"STDAFX.H"
# ADD CPP /Yc"STDAFX.H"
# End Source File
# Begin Source File

SOURCE=.\Src\ToolFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\turtleCtlr.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\TurtleDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Turtlefr.cpp
# ADD BASE CPP /Yu"STDAFX.H"
# ADD CPP /Yu"STDAFX.H"
# End Source File
# Begin Source File

SOURCE=.\Src\turtleMdl.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Turtlenw.cpp
# ADD BASE CPP /Yu"STDAFX.H"
# ADD CPP /Yu"STDAFX.H"
# End Source File
# Begin Source File

SOURCE=.\Src\turtleVp.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Turtlevw.cpp
# ADD BASE CPP /Yu"STDAFX.H"
# ADD CPP /Yu"STDAFX.H"
# End Source File
# Begin Source File

SOURCE=.\Src\Usage.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\webframe.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\webview.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Welcomed.cpp
# ADD BASE CPP /Yu"STDAFX.H"
# ADD CPP /Yu"STDAFX.H"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\Inc\CDib.h
# End Source File
# Begin Source File

SOURCE=.\INC\Contesti.h
# End Source File
# Begin Source File

SOURCE=.\Inc\Cprot30.h
# End Source File
# Begin Source File

SOURCE=.\Inc\dib.h
# End Source File
# Begin Source File

SOURCE=.\Inc\Editorfr.h
# End Source File
# Begin Source File

SOURCE=.\Inc\Editorvw.h
# End Source File
# Begin Source File

SOURCE=.\Inc\Gif.h
# End Source File
# Begin Source File

SOURCE=.\Inc\Gifhash.h
# End Source File
# Begin Source File

SOURCE=.\Inc\Heckbert.h
# End Source File
# Begin Source File

SOURCE=.\Inc\Iperlogo.h
# End Source File
# Begin Source File

SOURCE=.\Inc\Mainfrm.h
# End Source File
# Begin Source File

SOURCE=.\Inc\MciMidi.h
# End Source File
# Begin Source File

SOURCE=.\Inc\Modeless.h
# End Source File
# Begin Source File

SOURCE=.\Inc\Paletted.h
# End Source File
# Begin Source File

SOURCE=.\Inc\Quant.h
# End Source File
# Begin Source File

SOURCE=.\Inc\Register.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\Inc\SelglossD.h
# End Source File
# Begin Source File

SOURCE=.\Inc\ToolFrame.h
# End Source File
# Begin Source File

SOURCE=.\Inc\turtleCtlr.h
# End Source File
# Begin Source File

SOURCE=.\Inc\TurtleDoc.h
# End Source File
# Begin Source File

SOURCE=.\Inc\Turtlefr.h
# End Source File
# Begin Source File

SOURCE=.\Inc\turtleMdl.h
# End Source File
# Begin Source File

SOURCE=.\Inc\Turtlenw.h
# End Source File
# Begin Source File

SOURCE=.\Inc\turtleVp.h
# End Source File
# Begin Source File

SOURCE=.\Inc\Turtlevw.h
# End Source File
# Begin Source File

SOURCE=.\Inc\Usage.h
# End Source File
# Begin Source File

SOURCE=.\Inc\webframe.h
# End Source File
# Begin Source File

SOURCE=.\Inc\webview.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\RES\16C.BMP
# End Source File
# Begin Source File

SOURCE=.\RES\16M.BMP
# End Source File
# Begin Source File

SOURCE=.\RES\256.BMP
# End Source File
# Begin Source File

SOURCE=.\Res\browser.ico
# End Source File
# Begin Source File

SOURCE=.\Res\editor.ico
# End Source File
# Begin Source File

SOURCE=.\Res\idr_edit.ico
# End Source File
# Begin Source File

SOURCE=.\Res\idr_turt.ico
# End Source File
# Begin Source File

SOURCE=.\RES\IPERLOGO.ICO
# End Source File
# Begin Source File

SOURCE=.\res\iperlogo.rc2
# End Source File
# Begin Source File

SOURCE=.\Res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\turtle.ico
# End Source File
# End Group
# End Target
# End Project
