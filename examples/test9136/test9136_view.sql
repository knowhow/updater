SELECT dropIfExists('VIEW', 'test9136_view');
CREATE VIEW test9136_view AS SELECT a FROM test9136_table;
REVOKE ALL ON test9136_view FROM public;
GRANT ALL ON test9136_view TO openmfg;
