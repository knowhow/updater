SELECT dropIfExists('TRIGGER', 'pkgtestbefore', 'package');
CREATE TRIGGER pkgtestbefore
BEFORE INSERT OR UPDATE ON pkgtest FOR EACH ROW
  EXECUTE PROCEDURE _pkgtestbefore();
