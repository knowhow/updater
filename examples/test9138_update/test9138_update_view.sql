SELECT dropIfExists('VIEW', 'test9138_view');
CREATE VIEW test9138_view AS SELECT a FROM test9138_table;
REVOKE ALL ON test9138_view FROM public;
GRANT ALL ON test9138_view TO openmfg;
