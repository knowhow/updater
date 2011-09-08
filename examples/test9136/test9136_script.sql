SELECT setMetric('ServerPatchVersionPrevious',
                 fetchMetricText('ServerPatchVersion'));
SELECT setMetric('ServerPatchVersion',
                 fetchMetricText('ServerPatchVersionPrevious') || 'Z');
DELETE FROM metric WHERE (metric_name='ServerPatchVersionPrevious');
