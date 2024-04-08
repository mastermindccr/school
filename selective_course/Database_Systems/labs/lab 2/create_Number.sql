drop table if exists "Number";
create table "Number"(
	"three_letter_country_code" character varying(3) primary key,
	"country_number" integer
);
insert into "Number"
select distinct "three_letter_country_code", "country_number"
from "temporary";