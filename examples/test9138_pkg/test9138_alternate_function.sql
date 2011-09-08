SELECT dropIfExists('TRIGGER',  'test9138_pkg_trigger',    'test9138_pkg');
SELECT dropIfExists('FUNCTION', 'test9138_pkg_function()', 'test9138_pkg');
CREATE OR REPLACE FUNCTION test9138_pkg.test9138_pkg_function() RETURNS TRIGGER AS $$
BEGIN
  RAISE NOTICE 'test9138_pkg_function() called';
  RETURN NEW;
END;
$$ LANGUAGE 'plpgsql';
