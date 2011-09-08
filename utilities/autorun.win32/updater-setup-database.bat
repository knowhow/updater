@echo off
REM #########################################
REM xTuple Updater database setup script.

REM This script will run the file updater-setup-database.sql
REM  The SQL file is setup to make any changes to DROP upgrade_test
REM  and recreate it as an empty database.
REM Edit the updater-setup-database.sql file to use your database.
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

REM The updater-setup-database.sql will DROP and CREATE the upgrade_test_database database.
REM Edit the updater-setup-database.sql file to set the upgrade_test_database name.
REM We are connecting to the postgres database to run the script.
SET database=postgres

REM Name a SQL file to run.
SET filename=updater-setup-database.sql

REM Log output file
SET outputfile=update-log.txt

REM #########################################

REM STOP EDITS

REM #########################################

REM #########################################
REM Run the script.
REM #########################################
echo on
%psql_path%psql -h %server% -p %port% -U %pguser% -d %database% -f %filename% -o %outputfile% -q