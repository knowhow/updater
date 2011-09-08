SELECT dropIfExists('VIEW', 'test9136_pkg_view', 'test9138_pkg');
CREATE VIEW test9138_pkg.test9138_pkg_view AS SELECT a FROM test9138_pkg.test9138_pkg_table;
REVOKE ALL ON test9138_pkg.test9138_pkg_view FROM public;
GRANT ALL ON test9138_pkg.test9138_pkg_view TO openmfg;
