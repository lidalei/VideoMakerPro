; VideoMakerPro.nsi
;
; This script is based on VideoMakerPro.nsi, but it remember the directory, 
; has uninstall support and (optionally) installs start menu shortcuts.
;
; It will install VideoMakerPro.nsi into a directory that the user selects,

;--------------------------------

; The name of the installer
Name "VideoMakerPro"

; The file to write
OutFile "VideoMakerPro.exe"

; The default installation directory
InstallDir $PROGRAMFILES\VideoMakerPro

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\VideoMakerPro" "Install_Dir"

; Request application privileges for Windows Vista
RequestExecutionLevel admin

;--------------------------------

; Pages

Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

; The stuff to install
Section "VideoMakerPro"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  File "VideoMakerPro\Qt5Widgets.dll"
  File "VideoMakerPro\Qt5OpenGL.dll"
  File "VideoMakerPro\Qt5Network.dll"
  File "VideoMakerPro\Qt5MultimediaWidgets.dll"
  File "VideoMakerPro\Qt5Multimedia.dll"
  File "VideoMakerPro\Qt5Gui.dll"
  File "VideoMakerPro\Qt5Core.dll"
  File "VideoMakerPro\opencv_videostab249.dll"
  File "VideoMakerPro\opencv_video249.dll"
  File "VideoMakerPro\opencv_superres249.dll"
  File "VideoMakerPro\opencv_stitching249.dll"
  File "VideoMakerPro\opencv_photo249.dll"
  File "VideoMakerPro\opencv_ocl249.dll"
  File "VideoMakerPro\opencv_objdetect249.dll"
  File "VideoMakerPro\opencv_nonfree249.dll"
  File "VideoMakerPro\opencv_ml249.dll"
  File "VideoMakerPro\opencv_legacy249.dll"
  File "VideoMakerPro\opencv_imgproc249.dll"
  File "VideoMakerPro\opencv_highgui249.dll"
  File "VideoMakerPro\opencv_gpu249.dll"
  File "VideoMakerPro\opencv_flann249.dll"
  File "VideoMakerPro\opencv_ffmpeg249_64.dll"
  File "VideoMakerPro\opencv_features2d249.dll"
  File "VideoMakerPro\opencv_core249.dll"
  File "VideoMakerPro\opencv_contrib249.dll"
  File "VideoMakerPro\opencv_calib3d249.dll"
  File "VideoMakerPro\npps64_65.dll"
  File "VideoMakerPro\nppi64_65.dll"
  File "VideoMakerPro\nppc64_65.dll"
  File "VideoMakerPro\libEGL.dll"
  File "VideoMakerPro\icuuc53.dll"
  File "VideoMakerPro\icuin53.dll"
  File "VideoMakerPro\icudt53.dll"
  File "VideoMakerPro\cufft64_65.dll"
  File "VideoMakerPro\cudart64_65.dll"
  File "VideoMakerPro\VideoMakerPro.exe"
  File "VideoMakerPro\VideoMakerPro.res"
  File "VideoMakerPro\icon.res"
  File "VideoMakerPro\VideoMakerPro.ico"
  
  SetOutPath $INSTDIR\platforms  
  File "VideoMakerPro\platforms\qminimal.dll"
  File "VideoMakerPro\platforms\qminimald.dll"
  File "VideoMakerPro\platforms\qoffscreen.dll"
  File "VideoMakerPro\platforms\qoffscreend.dll"
  File "VideoMakerPro\platforms\qwindows.dll"
  File "VideoMakerPro\platforms\qwindowsd.dll"
  SetOutPath $INSTDIR
  
  ; Write the installation path into the registry
  WriteRegStr HKLM "SOFTWARE\VideoMakerPro" "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\VideoMakerPro" "DisplayName" "VideoMakerPro"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\VideoMakerPro" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\VideoMakerPro" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\VideoMakerPro" "NoRepair" 1
  WriteUninstaller "$INSTDIR\uninstall.exe"
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\VideoMakerPro"
  CreateShortcut "$SMPROGRAMS\VideoMakerPro\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortcut "$SMPROGRAMS\VideoMakerPro\VideoMakerPro.lnk" "$INSTDIR\VideoMakerPro.exe" "" "$INSTDIR\VideoMakerPro.ico" 0
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\VideoMakerPro"
  DeleteRegKey HKLM "SOFTWARE\VideoMakerPro"

  ; Remove all files
  Delete "$INSTDIR\Qt5Widgets.dll"
  Delete "$INSTDIR\Qt5OpenGL.dll"
  Delete "$INSTDIR\Qt5Network.dll"
  Delete "$INSTDIR\Qt5MultimediaWidgets.dll"
  Delete "$INSTDIR\Qt5Multimedia.dll"
  Delete "$INSTDIR\Qt5Gui.dll"
  Delete "$INSTDIR\Qt5Core.dll"
  Delete "$INSTDIR\opencv_videostab249.dll"
  Delete "$INSTDIR\opencv_video249.dll"
  Delete "$INSTDIR\opencv_superres249.dll"
  Delete "$INSTDIR\opencv_stitching249.dll"
  Delete "$INSTDIR\opencv_photo249.dll"
  Delete "$INSTDIR\opencv_ocl249.dll"
  Delete "$INSTDIR\opencv_objdetect249.dll"
  Delete "$INSTDIR\opencv_nonfree249.dll"
  Delete "$INSTDIR\opencv_ml249.dll"
  Delete "$INSTDIR\opencv_legacy249.dll"
  Delete "$INSTDIR\opencv_imgproc249.dll"
  Delete "$INSTDIR\opencv_highgui249.dll"
  Delete "$INSTDIR\opencv_gpu249.dll"
  Delete "$INSTDIR\opencv_flann249.dll"
  Delete "$INSTDIR\opencv_ffmpeg249_64.dll"
  Delete "$INSTDIR\opencv_features2d249.dll"
  Delete "$INSTDIR\opencv_core249.dll"
  Delete "$INSTDIR\opencv_contrib249.dll"
  Delete "$INSTDIR\opencv_calib3d249.dll"
  Delete "$INSTDIR\npps64_65.dll"
  Delete "$INSTDIR\nppi64_65.dll"
  Delete "$INSTDIR\nppc64_65.dll"
  Delete "$INSTDIR\libEGL.dll"
  Delete "$INSTDIR\icuuc53.dll"
  Delete "$INSTDIR\icuin53.dll"
  Delete "$INSTDIR\icudt53.dll"
  Delete "$INSTDIR\cufft64_65.dll"
  Delete "$INSTDIR\cudart64_65.dll"
  Delete "$INSTDIR\VideoMakerPro.exe"
  Delete "$INSTDIR\VideoMakerPro.res"
  Delete "$INSTDIR\icon.res"
  Delete "$INSTDIR\VideoMakerPro.ico"
  Delete "$INSTDIR\platforms\qminimal.dll"
  Delete "$INSTDIR\platforms\qminimald.dll"
  Delete "$INSTDIR\platforms\qoffscreen.dll"
  Delete "$INSTDIR\platforms\qoffscreend.dll"
  Delete "$INSTDIR\platforms\qwindows.dll"
  Delete "$INSTDIR\platforms\qwindowsd.dll"
  RMDir "$INSTDIR\platforms"
  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\VideoMakerPro\*.*"
  ; Always delete uninstaller as the last action
  Delete "$INSTDIR\uninstall.exe"
  ; Remove directories used  
  RMDir "$INSTDIR"

SectionEnd
