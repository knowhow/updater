SELECT dropIfExists('VIEW',    'test9138_view');
SELECT dropIfExists('TRIGGER', 'test9138_trigger');
SELECT dropIfExists('TABLE',   'test9138_table');
CREATE TABLE test9138_table (a INTEGER PRIMARY KEY, b INTEGER, c INTEGER);

REVOKE ALL ON TABLE test9138_table FROM PUBLIC;
GRANT  ALL ON TABLE test9138_table TO   admin;
GRANT  ALL ON TABLE test9138_table TO   GROUP openmfg;
