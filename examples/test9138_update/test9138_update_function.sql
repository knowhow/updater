SELECT dropIfExists('TRIGGER',  'test9138_trigger',    'public');
SELECT dropIfExists('FUNCTION', 'test9138_function()', 'public');
CREATE OR REPLACE FUNCTION test9138_function() RETURNS TRIGGER AS $$
BEGIN
  RAISE NOTICE 'test9138_function() called';
  RETURN NEW;
END;
$$ LANGUAGE 'plpgsql';
