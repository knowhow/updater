SELECT dropIfExists('TRIGGER', 'pkgtestbefore', 'telephonelookup');
SELECT dropIfExists('FUNCTION', '_pkgtestbefore()', 'telephonelookup');
CREATE OR REPLACE FUNCTION _pkgtestbefore() RETURNS TRIGGER AS '
BEGIN
  NEW.b = NEW.a;
  RETURN NEW;
END;
' LANGUAGE 'plpgsql';

