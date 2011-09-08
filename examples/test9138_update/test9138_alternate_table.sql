SELECT dropIfExists('TABLE', 'test9138_table', 'alternate');
CREATE TABLE alternate.test9138_alternate_table (a INTEGER PRIMARY KEY, b INTEGER, c INTEGER);

REVOKE ALL ON TABLE alternate.test9138_alternate_table FROM PUBLIC;
GRANT  ALL ON TABLE alternate.test9138_alternate_table TO   admin;
GRANT  ALL ON TABLE alternate.test9138_alternate_table TO   GROUP openmfg;
