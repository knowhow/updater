SELECT dropIfExists('TABLE', 'pkgtest');
CREATE TABLE pkgtest (a INTEGER PRIMARY KEY, b INTEGER);

REVOKE ALL ON TABLE pkgtest FROM PUBLIC;
GRANT  ALL ON TABLE pkgtest TO   admin;
GRANT  ALL ON TABLE pkgtest TO   xtrole;
