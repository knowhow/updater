@echo off
REM #########################################
REM xTuple Updater database initialize script.

REM This script will run the file updater-initialize.sql
REM  The SQL file can be setup to make any changes to 
REM  your upgrade_test databse after it was restored from
REM  backup and before you start the upgrade. 

REM Use this initialize script in conjunction with
REM  the updater-restore.bat script that is setup
REM  to restore the backup file with the correct
REM  name and location.
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
SET psql_path=C:\"Program Files"\PostgreSQL\8.4\bin\

REM Database name.
SET upgrade_test_database_name=upgrade_test

REM Script file to run.
SET filename=updater-initialize.sql

REM Log output file name.
SET outputfile=update-log.txt

REM #########################################

REM STOP EDITS

REM #########################################

REM #########################################
REM Run the script.
REM #########################################
echo on
%psql_path%psql -h %server% -p %port% -U %pguser% -d %upgrade_test_database_name% -f %filename% -o %outputfile% -q