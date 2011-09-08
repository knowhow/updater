SELECT dropIfExists('TRIGGER', 'test9138_pkg_trigger', 'test9138_pkg');
CREATE TRIGGER test9138_pkg_trigger
BEFORE INSERT OR UPDATE ON test9138_pkg.test9138_pkg_table FOR EACH ROW
  EXECUTE PROCEDURE test9138_pkg.test9138_pkg_function();
