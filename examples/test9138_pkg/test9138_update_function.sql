SELECT dropIfExists('TRIGGER',  'test9138_public_trigger',    'public');
SELECT dropIfExists('FUNCTION', 'test9138_public_function()', 'public');
CREATE OR REPLACE FUNCTION public.test9138_public_function() RETURNS TRIGGER AS $$
BEGIN
  RAISE NOTICE 'test9138_public_function() called';
  RETURN NEW;
END;
$$ LANGUAGE 'plpgsql';
