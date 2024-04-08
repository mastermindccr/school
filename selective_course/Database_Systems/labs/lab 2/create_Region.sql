drop table if exists "Region";
create table "Region"(
	"country_code" character(3),
	"region_code" character varying (20),
	"region_name" character varying (100)
);
alter table "Region" add primary key ("country_code", "region_code");
insert into "Region"("country_code",  "region_code", "region_name")
select distinct "countrycode", "regioncode", "regionname"
from "temporary";