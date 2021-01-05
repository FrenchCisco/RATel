
CREATE TABLE IF NOT EXISTS "table_ratel"(
  "session" INTEGER PRIMARY KEY NOT NULL DEFAULT CURRENT_TIMESTAMP,
  "ip" CHAR(256),
  "port" INTEGER(16),
  "is_he_alive" VARCHAR(8),
  "is_he_admin" VARCHAR(8),
  "path_RAT" VARCHAR(256),
  "username" VARCHAR(256),
  "token" VARCHAR(256)
  /*1,"192.168.0.13",90,"False"," False"," Z:\client\src\12.exe","cisco","token"*/
);

