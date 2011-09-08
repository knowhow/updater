@echo off
REM #########################################
REM xTuple Updater database restore script.

REM First create your upgrade test database
REM  to restore your backup to. Once created
REM  this script will clean the database and 
REM  restore the backup to it.

REM Use this restore script in conjunction with
REM  the updater-backup.bat script that is setup
REM  to create the backup file with the correct
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

REM Conneciton info.
SET server=192.168.1.1
SET port=5432
SET pguser=admin
SET PGPASSWORD=admin
SET pgadmin_path=C:\"Program Files"\"pgAdmin III"\1.10\

REM Database names.
SET production_database=xtuple_production
SET upgrade_test_database_name=upgrade_test

REM Path to production database backup file.
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
REM Run the pg_restore command.

REM Add the -v flag if you want to see the progress.
REM #########################################
echo on
%pgadmin_path%pg_restore -i -h %server% -p %port% -U %pguser% -d %upgrade_test_database_name% %BACKUP_FILE%
