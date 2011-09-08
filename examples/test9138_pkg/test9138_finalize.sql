INSERT INTO public.test9138_public_table SELECT sequence_value, ROUND(random() * 1000), ROUND(random() * 100)
FROM sequence
WHERE sequence_value <= 100;
INSERT INTO test9138_pkg.test9138_pkg_table SELECT sequence_value, ROUND(random() * 1000), ROUND(random() * 100)
FROM sequence
WHERE sequence_value <= 100;

CREATE OR REPLACE FUNCTION checkAlternate() RETURNS BOOL AS $$
BEGIN
  IF (NOT EXISTS(SELECT metasql_id
                 FROM ONLY metasql
                 WHERE metasql_group='test9138_public'
                   AND metasql_name='detail')) THEN
    RAISE EXCEPTION 'couldn''t find public metasql';
  END IF;
  IF (NOT EXISTS(SELECT metasql_id
                 FROM ONLY test9138_pkg.pkgmetasql
                 WHERE metasql_group='test9138_pkg'
                   AND metasql_name='detail')) THEN
    RAISE EXCEPTION 'couldn''t find test9138_pkg metasql';
  END IF;

  IF (NOT EXISTS(SELECT report_id
                 FROM ONLY report
                 WHERE report_name='test9138_public_report')) THEN
    RAISE EXCEPTION 'couldn''t find public report';
  END IF;
  IF (NOT EXISTS(SELECT report_id
                 FROM ONLY test9138_pkg.pkgreport
                 WHERE report_name='test9138_pkg_report')) THEN
    RAISE EXCEPTION 'couldn''t find test9138_pkg report';
  END IF;

  PERFORM * FROM public.test9138_public_table;
  PERFORM * FROM test9138_pkg.test9138_pkg_table;

  IF (NOT EXISTS(SELECT proname
                 FROM pg_proc JOIN pg_namespace n ON (pronamespace=n.oid)
                 WHERE proname='test9138_public_function'
                   AND nspname = 'public')) THEN
    RAISE EXCEPTION 'couldn''t find public function';
  END IF;
  IF (NOT EXISTS(SELECT proname
                 FROM pg_proc JOIN pg_namespace n ON (pronamespace=n.oid)
                 WHERE proname='test9138_pkg_function'
                   AND nspname = 'test9138_pkg')) THEN
    RAISE EXCEPTION 'couldn''t find test9138_pkg function';
  END IF;

  IF (NOT EXISTS(SELECT tgname
                 FROM pg_trigger, pg_class, pg_namespace
                 WHERE tgname='test9138_public_trigger'
                   AND tgrelid=pg_class.oid
                   AND relnamespace=pg_namespace.oid
                   AND nspname = 'public')) THEN
    RAISE EXCEPTION 'couldn''t find public trigger';
  END IF;
  IF (NOT EXISTS(SELECT tgname
                 FROM pg_trigger, pg_class, pg_namespace
                 WHERE tgname='test9138_pkg_trigger'
                   AND tgrelid=pg_class.oid
                   AND relnamespace=pg_namespace.oid
                   AND nspname = 'test9138_pkg')) THEN
    RAISE EXCEPTION 'couldn''t find test9138_pkg trigger';
  END IF;

  IF (NOT EXISTS(SELECT relname
                 FROM pg_class, pg_namespace
                 WHERE relname='test9138_public_view'
                   AND relnamespace=pg_namespace.oid
                   AND nspname = 'public')) THEN
    RAISE EXCEPTION 'couldn''t find public view';
  END IF;
  IF (NOT EXISTS(SELECT relname
                 FROM pg_class, pg_namespace
                 WHERE relname='test9138_pkg_view'
                   AND relnamespace=pg_namespace.oid
                   AND nspname = 'test9138_pkg')) THEN
    RAISE EXCEPTION 'couldn''t find test9138_pkg trigger';
  END IF;

  IF (NOT EXISTS(SELECT script_id
                 FROM ONLY script
                 WHERE script_name = 'test9138_public')) THEN
    RAISE EXCEPTION 'couldn''t find public script';
  END IF;
  IF (NOT EXISTS(SELECT script_id
                 FROM ONLY test9138_pkg.pkgscript
                 WHERE script_name='test9138_pkg')) THEN
    RAISE EXCEPTION 'couldn''t find test9138_pkg script';
  END IF;

  IF (NOT EXISTS(SELECT image_id
                 FROM ONLY image
                 WHERE image_name = 'test9138_public_image')) THEN
    RAISE EXCEPTION 'couldn''t find public image';
  END IF;
  IF (NOT EXISTS(SELECT image_id
                 FROM ONLY test9138_pkg.pkgimage
                 WHERE image_name='test9138_pkg_image')) THEN
    RAISE EXCEPTION 'couldn''t find test9138_pkg image';
  END IF;

  IF (NOT EXISTS(SELECT uiform_id
                 FROM ONLY uiform
                 WHERE uiform_name = 'test9138_public')) THEN
    RAISE EXCEPTION 'couldn''t find public uiform';
  END IF;
  IF (NOT EXISTS(SELECT uiform_id
                 FROM ONLY test9138_pkg.pkguiform
                 WHERE uiform_name='test9138_pkg')) THEN
    RAISE EXCEPTION 'couldn''t find test9138_pkg uiform';
  END IF;

  IF (NOT EXISTS(SELECT priv_id
                 FROM ONLY priv
                 WHERE priv_name = 'test9138_public_priv')) THEN
    RAISE EXCEPTION 'couldn''t find public priv';
  END IF;
  IF (NOT EXISTS(SELECT priv_id
                 FROM ONLY test9138_pkg.pkgpriv
                 WHERE priv_name = 'test9138_pkg_priv')) THEN
    RAISE EXCEPTION 'couldn''t find test9138_pkg priv';
  END IF;

  IF (NOT EXISTS(SELECT cmd_id
                 FROM ONLY cmd
                 WHERE cmd_name = 'test9138_public_cmd')) THEN
    RAISE EXCEPTION 'couldn''t find public cmd';
  END IF;
  IF (NOT EXISTS(SELECT cmd_id
                 FROM ONLY test9138_pkg.pkgcmd
                 WHERE cmd_name = 'test9138_pkg_cmd')) THEN
    RAISE EXCEPTION 'couldn''t find test9138_pkg cmd';
  END IF;

  IF (NOT EXISTS(SELECT cmdarg_id
                 FROM ONLY cmdarg
                 WHERE cmdarg_arg = 'uiform=test9138_public_ui')) THEN
    RAISE EXCEPTION 'couldn''t find public cmdarg';
  END IF;
  IF (NOT EXISTS(SELECT cmdarg_id
                 FROM ONLY test9138_pkg.pkgcmdarg
                 WHERE cmdarg_arg = 'uiform=test9138_pkg_ui')) THEN
    RAISE EXCEPTION 'couldn''t find test9138_pkg cmdarg';
  END IF;

  RETURN 0;
END;
$$
LANGUAGE 'plpgsql';

SELECT checkAlternate();
DROP FUNCTION checkAlternate();
