SELECT dropIfExists('TRIGGER',  'test9136_trigger',    'public');
SELECT dropIfExists('FUNCTION', 'test9136_function()', 'public');
CREATE OR REPLACE FUNCTION test9136_function() RETURNS TRIGGER AS $$
BEGIN
  RAISE NOTICE 'test9136_function() called';
  RETURN NEW;
END;
$$ LANGUAGE 'plpgsql';
