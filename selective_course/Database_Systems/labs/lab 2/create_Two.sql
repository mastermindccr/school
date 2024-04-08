drop table if exists "Two";
create table "Two"(
	"three_letter_country_code" character(3) primary key,
	"two_letter_country_code" character(2)
);

insert into "Two"("three_letter_country_code", "two_letter_country_code")
select distinct "three_letter_country_code", "two_letter_country_code"
from "temporary";