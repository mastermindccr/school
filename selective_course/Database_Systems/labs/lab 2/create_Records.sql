drop table if exists "Records";
create table "Records"(
	"countrycode" character(3),
	"date" date,
	"regioncode" varchar(20),
	"jurisdiction" varchar(20),
	"C1_School_closing" bigint,
	"C1_Flag" bigint,
	"C2_Workplace_closing" bigint,
	"C2_Flag" bigint,
	"C3_Cancel_public_events" bigint,
	"C3_Flag" bigint,
	"C4_Restrictions_on_gatherings" bigint,
	"C4_Flag" bigint,
	"C5_Close_public_transport" bigint,
	"C5_Flag" bigint,
	"C6_Stay_at_home_requirements" bigint,
	"C6_Flag" bigint,
	"C7_Restrictions_on_internal_movement" bigint,
	"C7_Flag" bigint,
	"C8_International_travel_controls" bigint,
	"E1_Income_support" bigint,
	"E1_Flag" bigint,
	"E2_Debt_contract_relief" bigint,
	"E3_Fiscal_measures" bigint,
	"E4_International_support" bigint,
	"H1_Public_information_campaigns" bigint,
	"H1_Flag" bigint,
	"H2_Testing_policy" bigint,
	"H3_Contact_tracing" bigint,
	"H4_Emergency_investment_in_healthcare" bigint,
	"H5_Investment_in_vaccines" bigint,
	"H6_Facial_Coverings" bigint,
	"H6_Flag" bigint,
	"H7_Vaccination_policy" bigint,
	"H7_Flag" bigint,
	"H8_Protection_of_elderly_people" bigint,
	"H8_Flag" bigint,
	"M1_Wildcard" bigint,
	"V1_Vaccine_Prioritisation_summary" bigint,
	"V2A_Vaccine_Availability_summary" bigint,
	"V2B_Vaccine_age_eligibility_availability_age_floor_general_population_summary" varchar(20),
	"V2C_Vaccine_age_eligibility_availability_age_floor_at_risk_summary" varchar(20),
	"V2D_Medically__clinically_vulnerable_Non_elderly" bigint,
	"V2E_Education" bigint,
	"V2F_Frontline_workers__non_healthcare" bigint,
	"V2G_Frontline_workers__healthcare" bigint,
	"V3_Vaccine_Financial_Support_summary" bigint,
	"V4_Mandatory_Vaccination_summary" bigint
);
insert into "Records"(
	"countrycode",
	"date",
	"regioncode",
	"jurisdiction",
	"C1_School_closing",
	"C1_Flag",
	"C2_Workplace_closing",
	"C2_Flag",
	"C3_Cancel_public_events",
	"C3_Flag",
	"C4_Restrictions_on_gatherings",
	"C4_Flag",
	"C5_Close_public_transport",
	"C5_Flag",
	"C6_Stay_at_home_requirements",
	"C6_Flag",
	"C7_Restrictions_on_internal_movement",
	"C7_Flag",
	"C8_International_travel_controls",
	"E1_Income_support",
	"E1_Flag",
	"E2_Debt_contract_relief",
	"E3_Fiscal_measures",
	"E4_International_support",
	"H1_Public_information_campaigns",
	"H1_Flag",
	"H2_Testing_policy",
	"H3_Contact_tracing",
	"H4_Emergency_investment_in_healthcare",
	"H5_Investment_in_vaccines",
	"H6_Facial_Coverings",
	"H6_Flag",
	"H7_Vaccination_policy",
	"H7_Flag",
	"H8_Protection_of_elderly_people",
	"H8_Flag",
	"M1_Wildcard",
	"V1_Vaccine_Prioritisation_summary",
	"V2A_Vaccine_Availability_summary",
	"V2B_Vaccine_age_eligibility_availability_age_floor_general_population_summary",
	"V2C_Vaccine_age_eligibility_availability_age_floor_at_risk_summary",
	"V2D_Medically__clinically_vulnerable_Non_elderly",
	"V2E_Education",
	"V2F_Frontline_workers__non_healthcare",
	"V2G_Frontline_workers__healthcare",
	"V3_Vaccine_Financial_Support_summary",
	"V4_Mandatory_Vaccination_summary"
)
select "countrycode",
	"date",
	regioncode,
	jurisdiction,
	C1_School_closing,
	C1_Flag,
	C2_Workplace_closing,
	C2_Flag,
	C3_Cancel_public_events,
	C3_Flag,
	C4_Restrictions_on_gatherings,
	C4_Flag,
	C5_Close_public_transport,
	C5_Flag,
	C6_Stay_at_home_requirements,
	C6_Flag,
	C7_Restrictions_on_internal_movement,
	C7_Flag,
	C8_International_travel_controls,
	E1_Income_support,
	E1_Flag,
	E2_Debt_contract_relief,
	E3_Fiscal_measures,
	E4_International_support,
	H1_Public_information_campaigns,
	H1_Flag,
	H2_Testing_policy,
	H3_Contact_tracing,
	H4_Emergency_investment_in_healthcare,
	H5_Investment_in_vaccines,
	H6_Facial_Coverings,
	H6_Flag,
	H7_Vaccination_policy,
	H7_Flag,
	H8_Protection_of_elderly_people,
	H8_Flag,
	M1_Wildcard,
	V1_Vaccine_Prioritisation_summary,
	V2A_Vaccine_Availability_summary,
	V2B_Vaccine_age_eligibility_availability_age_floor_general_population_summary,
	V2C_Vaccine_age_eligibility_availability_age_floor_at_risk_summary,
	V2D_Medically__clinically_vulnerable_Non_elderly,
	V2E_Education,
	V2F_Frontline_workers__non_healthcare,
	V2G_Frontline_workers__healthcare,
	V3_Vaccine_Financial_Support_summary,
	V4_Mandatory_Vaccination_summary
from "temporary"