drop table if exists "Name";
create table "Name"(
	"three_letter_country_code" character(3) primary key,
	"country_name" character varying (100)
);
alter table "Name" add foreign key("three_letter_country_code");
insert into "Name"("three_letter_country_code", "country_name")
select distinct "three_letter_country_code", "country_name"
from "temporary";