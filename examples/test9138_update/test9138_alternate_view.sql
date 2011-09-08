SELECT dropIfExists('VIEW', 'test9136_alternate_view', 'alternate');
CREATE VIEW alternate.test9138_alternate_view AS
  SELECT a FROM alternate.test9138_alternate_table;
REVOKE ALL ON alternate.test9138_alternate_view FROM public;
GRANT ALL ON alternate.test9138_alternate_view TO openmfg;
