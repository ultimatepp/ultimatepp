TABLE(TEST_TABLE) // Note that column names definition is simply shared between schemas
	INT     (ID)       PRIMARY_KEY AUTO_INCREMENT
	STRING  (VALUE, 32)
	STRING_ (NAME, 200)
END_TABLE
