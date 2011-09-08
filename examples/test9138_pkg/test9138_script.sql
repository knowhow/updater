SELECT setMetric('ServerPatchVersionPrevious',
                 fetchMetricText('ServerPatchVersion'));
SELECT setMetric('ServerPatchVersion',
                 fetchMetricText('ServerPatchVersionPrevious') || 'Y');
DELETE FROM metric WHERE (metric_name='ServerPatchVersionPrevious');
