;имя приложения
!define PRODUCT_NAME "Debugger"
;версия приложения
!define PRODUCT_VERSION "1.0"
;папка, где будут храниться исходные файлы, подлежащие сжатию.
!define pkgdir "e:\lab3_files"
!include "MUI.nsh"
SetCompressor /SOLID lzma
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_LANGUAGE "Russian"
Name "${PRODUCT_NAME}"
Caption "Установка ${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "${PRODUCT_NAME}-${PRODUCT_VERSION}.exe"

Section
SetOutPath "${pkgdir}"
File "${pkgdir}\Debugger.exe"
SectionEnd