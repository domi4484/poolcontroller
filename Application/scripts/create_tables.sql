
-- This script is used by initializePostgreeSQL.sh.

CREATE LANGUAGE plpgsql;	-- only needed once after database creation
CREATE FUNCTION auto_date() RETURNS trigger AS $$
  BEGIN
    NEW.timestamp := current_timestamp;
    RETURN NEW;
  END;
$$ LANGUAGE plpgsql;


--------------
-- Settings --
--------------
--
-- Create a table for the setting and add a few settings of different types.
--
CREATE TABLE settings(name TEXT, type INTEGER, value TEXT);
--INSERT INTO settings (name, type, value) VALUES ('speed', 0, 'slow');


---------
-- Log --
---------
--
-- Create a table for the log entries.
-- This table has an auto incrementing integer(id) as primary key.
-- The timestamp is auto inserted via trigger.
-- This table will be on another partition, to be sure the logger continues working when the disk is full
--
CREATE TABLE log (
		id serial primary key,
		timeStamp TIMESTAMP WITH TIME ZONE,
		severity INTEGER,
		thread TEXT,
		text TEXT)
TABLESPACE logspace;
CREATE TRIGGER auto_date
  BEFORE INSERT
  ON log 
  FOR EACH ROW
  EXECUTE PROCEDURE auto_date();


-------------
-- Analyse --
-------------
--
-- Create a table for the analyse. The table name is analysis because analyse is a reserved word.
--
CREATE TABLE analysis (
		id serial primary key,
		timeStamp TIMESTAMP WITH TIME ZONE,
		param1 TEXT
);
CREATE TRIGGER auto_date
  BEFORE INSERT
  ON analysis 
  FOR EACH ROW
  EXECUTE PROCEDURE auto_date();


-------------
-- Results --
-------------
--
-- Create a table for the results.
--
CREATE TABLE results (
		id serial primary key,
		timeStamp TIMESTAMP WITH TIME ZONE,
		image bytea,
		analysis INTEGER REFERENCES analysis(id)
);
CREATE TRIGGER auto_date
  BEFORE INSERT
  ON results
  FOR EACH ROW
  EXECUTE PROCEDURE auto_date();


