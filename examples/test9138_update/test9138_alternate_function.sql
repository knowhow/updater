SELECT dropIfExists('TRIGGER',  'test9138_alternate_trigger',    'alternate');
SELECT dropIfExists('FUNCTION', 'test9138_alternate_function()', 'alternate');
CREATE OR REPLACE FUNCTION alternate.test9138_alternate_function() RETURNS TRIGGER AS $$
BEGIN
  RAISE NOTICE 'test9138_alternate_function() called';
  RETURN NEW;
END;
$$ LANGUAGE 'plpgsql';
