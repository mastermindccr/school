alter table "temporary"
	alter column "stringencyindex" type float
		using cast(coalesce(nullif("stringencyindex", ''), '0') as float);