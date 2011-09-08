-- Database: upgrade_test
-- DROP the upgrade_test database from the server before the backup restore.
-- Then CREATE the upgrade_test database as an empty database for the restore to use.

DROP DATABASE upgrade_test;

CREATE DATABASE upgrade_test
  WITH OWNER = "admin"
       ENCODING = 'UTF8'
       CONNECTION LIMIT = -1;
