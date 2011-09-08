SELECT dropIfExists('VIEW', 'test9138_public_view', 'public');
CREATE VIEW public.test9138_public_view AS SELECT a FROM public.test9138_public_table;
REVOKE ALL ON public.test9138_public_view FROM public;
GRANT ALL ON public.test9138_public_view TO openmfg;
