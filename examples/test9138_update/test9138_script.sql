SELECT setMetric('ServerPatchVersionPrevious',
                 fetchMetricText('ServerPatchVersion'));
SELECT setMetric('ServerPatchVersion',
                 fetchMetricText('ServerPatchVersionPrevious') || 'Z');
DELETE FROM metric WHERE (metric_name='ServerPatchVersionPrevious');
SELECT createPkgSchema('alternate', 'testing 9138 for main app updates');
INSERT INTO pkghead VALUES (DEFAULT, 'alternate',
          'test 9138 for main app updates',
          '0.0.1wip', 'xTuple', 'manually created schema and pkghead',
          now(), now(), true);
