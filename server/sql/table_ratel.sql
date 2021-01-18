CREATE TABLE IF NOT EXISTS "table_ratel"(
    "session" INTEGER PRIMARY KEY NOT NULL DEFAULT CURRENT_TIMESTAMP,
    "ip" CHAR(256),
    "port" INTEGER(45),
    "is_he_alive" INTEGER,
    "is_he_admin" INTEGER,
    "path_RAT" VARCHAR(256),
    "username" VARCHAR(256),
    "token" VARCHAR(256)
);
