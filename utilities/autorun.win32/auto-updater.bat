@echo off & setLocal EnableDELAYedExpansion
REM #########################################
REM xTuple Updater autorun batch file to 
REM  upgrade a database through multiple versions.

REM Put all upgrade scripts in a directory called
REM  scripts

REM The directory structure should look like this:
REM - Path_to_dir
REM     auto-updater.bat
REM     - scripts
REM         pb353to354.gz
REM         pb354to355.gz
REM         pb355to360.gz

REM Note that scripts will be ran in alphabetical order.
REM You may need to prepend numbers to the file names
REM  to get the proper sequence e.g.

REM 01pb353to354.gz, 02pb354to355.gz, 03pb355to360.gz
REM #########################################

REM #########################################

REM START EDITS 

REM #########################################

REM #########################################
REM Edit these variables to your environment.
REM #########################################

REM Path to directory containing updater.exe
REM EDIT BELOW
SET updater_path=C:\xTuple\Updater\bin\

REM Name of xTuple updater.exe. 
REM This script will check if the program is running.
REM You should change this to something like xtuple-updater.exe.
REM This will prevent conflicts with other updater.exe 
REM  from different programs that exist.
REM EDIT BELOW
SET process=updater.exe

REM Setup connection info.
REM EDIT BELOW
SET server=192.168.1.1
SET port=5432
SET upgrade_test_database_name=upgrade_test
SET username=admin
SET password=admin

REM #########################################
REM Edit this section for your initial setup script files.
REM Defaults should be fine here if you have setup the files.
REM #########################################
:Start
echo ----------------------------------------
echo ----------------------------------------
echo xTuple Database auto backup, create copy, and upgrade script.
echo ----------------------------------------
echo ----------------------------------------
echo Creating backup file.
echo ----------------------------------------
call updater-backup.bat
@echo off
echo ----------------------------------------
echo Setting up upgrade test database.
echo ----------------------------------------
call updater-setup-database.bat
@echo off
echo ----------------------------------------
echo Restoring backup to upgrade test database.
echo ----------------------------------------
call updater-restore.bat
@echo off
echo ----------------------------------------
echo Restore finished.
echo ----------------------------------------
call updater-initialize.bat
@echo off
echo ----------------------------------------
echo Initialize finished.
echo ----------------------------------------

REM #########################################

REM STOP EDITS

REM #########################################

REM #########################################
REM Set some variables.
REM #########################################
SET arguments=-databaseURL=psql://%server%/%upgrade_test_database_name%:%port% -username=%username% -passwd=%password% -autorun -file=
SET result=INFO:
SET message1=No Updater running, lauching first update.
SET message2=Updater finished, lauching next update.
SET cur_message=%message1%

SET index=1
SET /A counter=0
SET /A filenum=0
SET /A script_counter=0

REM #########################################
REM Get script count.
REM #########################################
FOR %%i IN (scripts\*.gz) DO (
	IF !script_counter!==0 (
		SET current_script=%%i
	)
SET /A script_counter=%index%+!script_counter!
)

echo Total of !script_counter! upgrade scripts to run.

REM #########################################
REM Start update.
REM #########################################
SET /P var=Checking for Updater... <NUL

:Check_Process
for /f "usebackq" %%A in (`tasklist /nh /fi "imagename eq %process%"`) do if not %%A==%result% GOTO Process_Found

:Process_NotFound
IF !counter! LSS !script_counter! (
	echo %cur_message%
	SET /A filenum=%index%+%filenum%
	echo Using %current_script% update !filenum! of !script_counter! running...
	start %updater_path%%process% %arguments%%current_script%
	SET /A counter=0
	GOTO Next
)
IF !counter!==!script_counter! (
	echo All upgrade scripts finished.
	GOTO Finish
	)

:Process_Found
GOTO Check_Process

REM #########################################
REM Switch to next file and start update.
REM #########################################
:Next
FOR %%i IN (scripts\*.gz) DO (
	IF !counter!==%filenum% (
		SET current_script=%%i
		SET cur_message=%message2%
		GOTO Check_Process
	)

	SET /A counter=%index%+!counter!

	IF !counter!==!script_counter! (
		GOTO Check_Process
	)
)

REM #########################################
REM EDIT this sections for your final setup scripts.
REM #########################################
:Finish
echo ----------------------------------------
echo More stuff here.
echo ----------------------------------------