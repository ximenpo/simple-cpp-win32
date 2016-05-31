pushd	%~dp0
cd	..

;;
set	test_files=%1
if "%test_files%" == "" (
set	test_files=*
)

;;rem	clean prvious temp files.
if exist *.exe		@del /f /q	*.exe
if exist *.obj		@del /f /q	*.obj
if exist *.lib		@del /f /q	*.lib
if exist *.exp		@del /f /q	*.exp
if exist *.module	@del /f /q	*.module

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;rem	compile and run the tests.
if "%OSTYPE%"=="cygwin"	(
	gcc	inc/simple/*.cpp inc/simple/*.c tests/main.cpp tests/%test_files%.cpp	-o _test.exe	-D NDEBUG	-D _WIN32_WINNT=0x0501	-I inc	-I tests	-I ../simple-cpp/inc	-lstdc++
) else (
	cl	/nologo		/EP		tools\__cl-param.txt 2>&1 | findstr "@" > .\cl_param.cmd
	CALL	.\cl_param.cmd
	DEL	/Q	.\cl_param.cmd
	cl	inc\simple\*.cpp inc\simple\*.c ..\simple-cpp\inc\simple\*.cpp ..\simple-cpp\inc\simple\*.c tests\main.cpp tests\%test_files%.cpp	kernel32.lib	user32.lib	/Fe_test.exe	/DNDEBUG	/D_WIN32_WINNT=0x0501	/Iinc				/I..\simple-cpp\inc	/EHa /wd4819	%MSVC_CL_PARAM%
)
if not	exist _test.exe			goto ERROR
if exist *.obj		@del /f /q	*.obj
echo	==========================================
echo		run tests now ...
_test.exe	--version
echo	==========================================
if "%OSTYPE%"=="cygwin"	(
	_test.exe	--output=color	||	goto ERROR
) else (
	_test.exe	--output=vs	||	goto ERROR
)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;rem	end clean
goto	END
:ERROR

;;rem	alert.
if "%OSTYPE%"=="cygwin"	(
	pause
) else (
	color 4f
	pause
	color
)

:END

;;rem	clean temp files.
if exist *.exe		@del /f /q	*.exe
if exist *.obj		@del /f /q	*.obj
if exist *.lib		@del /f /q	*.lib
if exist *.exp		@del /f /q	*.exp
if exist *.module	@del /f /q	*.module

popd
