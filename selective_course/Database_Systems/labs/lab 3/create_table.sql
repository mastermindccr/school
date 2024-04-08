create table "recommendation" (
    title character varying(200),
    genre character varying(200),
    premiere date,
    runningtime bigint,
    rating double precision,
    language, character varying(50),
    primary key("title")
);
create index rating_index on "recommendation"("rating");