drop table if exists "In";
create table "In"(
	"three_letter_country_code" character varying(3),
	"continent_code" character varying (20)
);
alter table "In" add primary key("three_letter_country_code", "continent_code");
insert into "In"
select distinct "three_letter_country_code", "continent_code"
from "temporary";