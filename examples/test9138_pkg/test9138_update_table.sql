SELECT dropIfExists('VIEW',    'test9138_public_view');
SELECT dropIfExists('TRIGGER', 'test9138_public_trigger');
SELECT dropIfExists('TABLE',   'test9138_public_table');
CREATE TABLE public.test9138_public_table (a INTEGER PRIMARY KEY, b INTEGER, c INTEGER);

REVOKE ALL ON TABLE public.test9138_public_table FROM PUBLIC;
GRANT  ALL ON TABLE public.test9138_public_table TO   admin;
GRANT  ALL ON TABLE public.test9138_public_table TO   GROUP openmfg;
