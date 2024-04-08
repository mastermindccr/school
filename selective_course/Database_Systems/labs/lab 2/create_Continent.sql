drop table if exists "Continent";
create table "Continent"(
	"continent_code" character(2) primary key,
	"continent_name" character(20)
);
insert into "Continent"("continent_code", "continent_name")
select distinct "continent_code", "continent_name" from "temporary";