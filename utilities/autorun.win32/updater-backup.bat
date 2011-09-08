@echo off
REM #########################################
REM xTuple Updater database backup script.

REM Use this backup script in conjunction with
REM  the updater-restore.bat script that is setup
REM  to restore the backup file with the correct
REM  name and location.

REM Make sure to schedule the backup and restore
REM  for a time that will not span midnight or
REM  the naming convention will be off.
REM #########################################

REM #########################################

REM START EDITS 

REM #########################################

REM #########################################
REM EDIT these variables to your environment.
REM #########################################

REM Connection info.
SET server=192.168.1.1
SET port=5432
SET pguser=admin
SET PGPASSWORD=admin
SET pgadmin_path=C:\"Program Files"\"pgAdmin III"\1.10\

REM Database name.
SET production_database=xtuple_production

REM Path to save backup to.
SET backup_path=backups\

REM #########################################

REM STOP EDITS

REM #########################################

REM #########################################
REM Create the correct date string for the file name.
REM #########################################
   for /f "tokens=1-4 delims=/ " %%i in ("%date%") do (
     set dow=%%i
     set month=%%j
     set day=%%k
     set year=%%l
   )
   set datestr=%month%_%day%_%year%
   echo datestr is %datestr%

   set BACKUP_FILE=%backup_path%%production_database%_%datestr%.backup
   set logFile=EmproDatabase_%datestr%.log
   echo backup file name is %BACKUP_FILE%

REM #########################################
REM Run the pg_dump command

REM Add the -v flag if you want to see the progress.
REM #########################################
echo on
%pgadmin_path%pg_dump -i -h %server% -p %port% -U %pguser% -F c -b -f %BACKUP_FILE% %production_database%
