drop table if exists "Policy";
create table "Policy"(
	"country_code" character (3),
	"date" date,
	"region_code" character (20),
	"confirmed_cases" float,
	"confirmed_deaths" float,
	"stringency_index" float,
	"stringency_index_for_display" float,
	"stringency_legacy_index" float,
	"stringency_legacy_index_for_display" float,
	"government_response_index" float,
	"government_response_index_for_display" float,
	"containment_health_index" float,
	"containment_health_index_for_display" float,
	"economic_support_index" float,
	"economic_support_index_for_display" varchar
);
alter table "Policy" add primary key("country_code", "date", "region_code");
insert into "Policy"(
	"country_code",
	"date",
	"region_code",
	"confirmed_cases",
	"confirmed_deaths",
	"stringency_index",
	"stringency_index_for_display",
	"stringency_legacy_index",
	"stringency_legacy_index_for_display",
	"government_response_index",
	"government_response_index_for_display",
	"containment_health_index",
	"containment_health_index_for_display",
	"economic_support_index",
	"economic_support_index_for_display")
select distinct 
	"countrycode",
	"date",
	"regioncode",
	"confirmedcases",
	"confirmeddeaths",
	"stringencyindex",
	"stringencyindexfordisplay",
	"stringencylegacyindex",
	"stringencylegacyindexfordisplay",
	"governmentresponseindex",
	"governmentresponseindexfordisplay",
	"containmenthealthindex",
	"containmenthealthindexfordisplay",
	"economicsupportindex",
	"economicsupportindexfordisplay"
from "temporary";