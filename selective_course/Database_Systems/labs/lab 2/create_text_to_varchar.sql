alter table "temporary"
	alter column v4_combined type varchar(20)
		using v4_combined::varchar(20);