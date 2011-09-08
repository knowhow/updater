SELECT dropIfExists('TABLE', 'test9138_pkg_table', 'test9138_pkg');
CREATE TABLE test9138_pkg.test9138_pkg_table (a INTEGER PRIMARY KEY, b INTEGER, c INTEGER);

REVOKE ALL ON TABLE test9138_pkg.test9138_pkg_table FROM PUBLIC;
GRANT  ALL ON TABLE test9138_pkg.test9138_pkg_table TO   admin;
GRANT  ALL ON TABLE test9138_pkg.test9138_pkg_table TO   GROUP openmfg;
