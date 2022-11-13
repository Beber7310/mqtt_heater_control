#C:/Users/Bertrand/workspace/HomeControl/utils/deploy.bat  "${ProjDirPath}/Debug/${ProjName}.a" /home/pi/projects/${ProjName}.a
#ECHO OFF

ECHO Start deploy

rem Check for a valid filename
IF "%1"=="" (
	ECHO You have to provide a valid source file.
	EXIT /b
)

IF "%2"=="" (
	ECHO You have to provide a valid destination path.
	EXIT /b
)

SET PUTTYSCP_BIN="C:\Program Files\PuTTY\pscp.exe"
SET PUTTY_BIN="C:\Program Files (x86)\WinSCP\putty.exe"
SET RASPBERRYPI_ADDR=192.168.1.101
SET USERNAME=pi
SET PASSWORD=q27krtrq

rem SET RASPBERRYPI_ADDR=emonpi
rem SET USERNAME=pi
rem SET PASSWORD=emonpi2016


SET CMD_FILENAME=commands.sh

rem Upload the file to raspberry pi
%PUTTYSCP_BIN% -pw %PASSWORD% "%1" %USERNAME%@%RASPBERRYPI_ADDR%:"%2" 

rem Build a list of actions to do on the pi (chmod, execute GDB server)
rem if exist %~dp0%CMD_FILENAME% del %~dp0%CMD_FILENAME%
rem echo rm "%2" >> %~dp0%CMD_FILENAME%
rem echo chmod +x "%2" >> %~dp0%CMD_FILENAME%
rem echo gdbserver :3785 "%2" >> %~dp0%CMD_FILENAME%

rem Execute the action list on the raspberry pi
rem %PUTTY_BIN% -pw %PASSWORD% -P 22 -m %~dp0%CMD_FILENAME% %USERNAME%@%RASPBERRYPI_ADDR%
 
rem exit /b %ERRORLEVEL%

