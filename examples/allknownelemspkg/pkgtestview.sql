SELECT dropIfExists('VIEW', 'pkgtestview', 'telephonelookup');
CREATE VIEW pkgtestview AS SELECT a FROM pkgtest;
REVOKE ALL ON pkgtestview FROM public;
GRANT ALL ON pkgtestview TO xtrole;
