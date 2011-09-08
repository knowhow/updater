SELECT dropIfExists('TRIGGER', 'test9138_alternate_trigger', 'alternate');
CREATE TRIGGER test9138_alternate_trigger
BEFORE INSERT OR UPDATE ON alternate.test9138_alternate_table FOR EACH ROW
  EXECUTE PROCEDURE alternate.test9138_alternate_function();
