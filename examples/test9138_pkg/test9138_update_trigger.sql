SELECT dropIfExists('TRIGGER', 'test9138_public_trigger');
CREATE TRIGGER test9138_public_trigger
BEFORE INSERT OR UPDATE ON public.test9138_public_table FOR EACH ROW
  EXECUTE PROCEDURE test9138_public_function();
