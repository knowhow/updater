INSERT INTO test9138_table SELECT sequence_value, ROUND(random() * 1000), ROUND(random() * 100)
FROM sequence
WHERE sequence_value <= 100;
INSERT INTO alternate.test9138_alternate_table SELECT sequence_value, ROUND(random() * 1000), ROUND(random() * 100)
FROM sequence
WHERE sequence_value <= 100;

CREATE OR REPLACE FUNCTION checkAlternate() RETURNS BOOL AS $$
BEGIN
  IF (NOT EXISTS(SELECT metasql_id
                 FROM ONLY metasql
                 WHERE metasql_group='test9138'
                   AND metasql_name='detail')) THEN
    RAISE EXCEPTION 'couldn''t find public metasql';
  END IF;
  IF (NOT EXISTS(SELECT metasql_id
                 FROM ONLY alternate.pkgmetasql
                 WHERE metasql_group='test9138_alternate'
                   AND metasql_name='detail')) THEN
    RAISE EXCEPTION 'couldn''t find alternate metasql';
  END IF;

  IF (NOT EXISTS(SELECT report_id
                 FROM ONLY report
                 WHERE report_name='test9138_report')) THEN
    RAISE EXCEPTION 'couldn''t find public report';
  END IF;
  IF (NOT EXISTS(SELECT report_id
                 FROM ONLY alternate.pkgreport
                 WHERE report_name='test9138_alternate_report')) THEN
    RAISE EXCEPTION 'couldn''t find alternate report';
  END IF;

  PERFORM * FROM public.test9138_table;
  PERFORM * FROM alternate.test9138_alternate_table;

  IF (NOT EXISTS(SELECT proname
                 FROM pg_proc JOIN pg_namespace n ON (pronamespace=n.oid)
                 WHERE proname='test9138_function'
                   AND nspname = 'public')) THEN
    RAISE EXCEPTION 'couldn''t find public function';
  END IF;
  IF (NOT EXISTS(SELECT proname
                 FROM pg_proc JOIN pg_namespace n ON (pronamespace=n.oid)
                 WHERE proname='test9138_alternate_function'
                   AND nspname = 'alternate')) THEN
    RAISE EXCEPTION 'couldn''t find alternate function';
  END IF;

  IF (NOT EXISTS(SELECT tgname
                 FROM pg_trigger, pg_class, pg_namespace
                 WHERE tgname='test9138_trigger'
                   AND tgrelid=pg_class.oid
                   AND relnamespace=pg_namespace.oid
                   AND nspname = 'public')) THEN
    RAISE EXCEPTION 'couldn''t find public trigger';
  END IF;
  IF (NOT EXISTS(SELECT tgname
                 FROM pg_trigger, pg_class, pg_namespace
                 WHERE tgname='test9138_alternate_trigger'
                   AND tgrelid=pg_class.oid
                   AND relnamespace=pg_namespace.oid
                   AND nspname = 'alternate')) THEN
    RAISE EXCEPTION 'couldn''t find alternate trigger';
  END IF;

  IF (NOT EXISTS(SELECT relname
                 FROM pg_class, pg_namespace
                 WHERE relname='test9138_view'
                   AND relnamespace=pg_namespace.oid
                   AND nspname = 'public')) THEN
    RAISE EXCEPTION 'couldn''t find public view';
  END IF;
  IF (NOT EXISTS(SELECT relname
                 FROM pg_class, pg_namespace
                 WHERE relname='test9138_alternate_view'
                   AND relnamespace=pg_namespace.oid
                   AND nspname = 'alternate')) THEN
    RAISE EXCEPTION 'couldn''t find alternate trigger';
  END IF;

  IF (NOT EXISTS(SELECT script_id
                 FROM ONLY script
                 WHERE script_name = 'test9138')) THEN
    RAISE EXCEPTION 'couldn''t find public script';
  END IF;
  IF (NOT EXISTS(SELECT script_id
                 FROM ONLY alternate.pkgscript
                 WHERE script_name='test9138_alternate')) THEN
    RAISE EXCEPTION 'couldn''t find alternate script';
  END IF;

  IF (NOT EXISTS(SELECT image_id
                 FROM ONLY image
                 WHERE image_name = 'test9138_update_image')) THEN
    RAISE EXCEPTION 'couldn''t find public image';
  END IF;
  IF (NOT EXISTS(SELECT image_id
                 FROM ONLY alternate.pkgimage
                 WHERE image_name='test9138_alternate_image')) THEN
    RAISE EXCEPTION 'couldn''t find alternate image';
  END IF;

  IF (NOT EXISTS(SELECT uiform_id
                 FROM ONLY uiform
                 WHERE uiform_name = 'test9138')) THEN
    RAISE EXCEPTION 'couldn''t find public uiform';
  END IF;
  IF (NOT EXISTS(SELECT uiform_id
                 FROM ONLY alternate.pkguiform
                 WHERE uiform_name='test9138_alternate')) THEN
    RAISE EXCEPTION 'couldn''t find alternate uiform';
  END IF;

  IF (NOT EXISTS(SELECT priv_id
                 FROM ONLY priv
                 WHERE priv_name = 'test9138_update_priv')) THEN
    RAISE EXCEPTION 'couldn''t find public priv';
  END IF;
  IF (NOT EXISTS(SELECT priv_id
                 FROM ONLY alternate.pkgpriv
                 WHERE priv_name = 'test9138_alternate_priv')) THEN
    RAISE EXCEPTION 'couldn''t find alternate priv';
  END IF;

  IF (NOT EXISTS(SELECT cmd_id
                 FROM ONLY cmd
                 WHERE cmd_name = 'test9138_update_cmd')) THEN
    RAISE EXCEPTION 'couldn''t find public cmd';
  END IF;
  IF (NOT EXISTS(SELECT cmd_id
                 FROM ONLY alternate.pkgcmd
                 WHERE cmd_name = 'test9138_alternate_cmd')) THEN
    RAISE EXCEPTION 'couldn''t find alternate cmd';
  END IF;

  IF (NOT EXISTS(SELECT cmdarg_id
                 FROM ONLY cmdarg
                 WHERE cmdarg_arg = 'uiform=test9138_update_ui')) THEN
    RAISE EXCEPTION 'couldn''t find public cmdarg';
  END IF;
  IF (NOT EXISTS(SELECT cmdarg_id
                 FROM ONLY alternate.pkgcmdarg
                 WHERE cmdarg_arg = 'uiform=test9138_alternate_ui')) THEN
    RAISE EXCEPTION 'couldn''t find alternate cmdarg';
  END IF;

  RETURN 0;
END;
$$
LANGUAGE 'plpgsql';

SELECT checkAlternate();
DROP FUNCTION checkAlternate();
