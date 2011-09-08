INSERT INTO test9136_table SELECT sequence_value, ROUND(random() * 1000), ROUND(random() * 100)
FROM sequence
WHERE sequence_value <= 100;
