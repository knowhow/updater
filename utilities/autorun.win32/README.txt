xTuple Updater.exe auto-run scripts.
====================================

contributed by xikar

These batch files work with a modified updater.exe program that
accepts the -autorun command line argument.

What these batch files do:

0. Make a directory called "scripts" to hold the upgrade .gz files
   and another directory called "backups" to hold database backup
   files created by the auto-run batch files.
1. Backup your production database to the "backup" folder.
2. Drop your "upgrade_test" database and recreate it as an empty
   database.
3. Restore your production backup file to your "upgrade_test"
   database.
4. Run an initialize script on your restored "upgrade_test" database.
   This is optional and currently does nothing, but is useful if
   you need to make any edits before you upgrade.  I have a data
   type mismatch on a custom view in my database, so I drop the
   view and recreate it after the upgrades are finished.
5. Run through each upgrade script .gz file in the "scripts" folder.
   When an upgrade script is finished, the updater.exe will exit
   and be re-ran with the next upgrade script until all have
   completed.

When finished, you will have a fully upgraded test database.  The
main batch file that controls all of this is "auto-updater.bat".
It can be scheduled to run every night so you have a fresh upgraded
test database each morning. All the other batch files are called
from this script.

BEFORE RUNNING:

1. Backup your production database manually and make sure you have
   a copy saved somewhere safe.

2. Perform a complete upgrade on a copy of your production database
   manually to make sure it will upgrade through all versions without
   any errors.

3. Test this on a test server first!

HOW TO SET UP:

1. You need to have pgAdmin III and PostgreSQL server installed on
   your Windows computer.  The PostgreSQL version on windows does
   not need to host any databases, but the scripts use the binary
   to run SQL commands in Windows against your actual PostgreSQL
   server.

2. Create a new database called "upgrade_test".

3. Edit each batch filstd::string to your environment.  Each batch
   file is commented with a section where you should make edits.
   The comments say:

 REM #########################################

 REM START EDITS 

 REM #########################################

 edits go here...

 REM #########################################

 REM STOP EDITS

 REM #########################################

Make your changes in between these comments.  You will need to set
your server IP, port, user, pass, production database name, upgrade
database name, path to pgadmin and PostgreSQL.

4. Download each xTuple database upgrade script that you need to
   go from your current production version to the latest released
   version. See the note below about file naming conventions.
   Download your upgrade scripts here:
   http://sourceforge.net/projects/postbooks/files/

5. OPTIONAL - Edit "updater-setup-database.sql" file and change the
   "upgrade_test" database name if you are using a different name
   for your test database.

6. OPTIONAL - Edit "updater-initialize.sql" file and put in any
   special SQL statements you might need before you upgrade.

7. OPTIONAL - You may want to change the "updater.exe" file name
   to something different and then change it in the batch file.
   I've noticed some other Windows programs also occasionally run
   "updater.exe" programs that will stall this process. The
   "auto-updater.bat" file checks to make sure there are no
   "updater.exe" processes running before it starts an upgrade.

BEWARE:

1. The backup file is saved with the current date in the file name.
   Don't schedule the "auto-updater.bat" file to run before midnight,
   or you might get a backup file name for the previous day when
   the restore script starts running at 12:02 AM and is looking for
   a file with a date of today.

2. Make sure all of your xTuple database upgrade scripts sort
   alphabetically in the proper order.  They will be ran in
   alphabetical order, so you may need to prepend numbers to the
   file names to get the proper sequence e.g. (01pb353to354.gz,
   02pb354to355.gz, 03pb355to360.gz)
