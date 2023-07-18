-- Advanced Database Program:
-- The stored procedure "update_medal_count" takes in three parameters: Ath_Name, NOC, and medal_type. It checks if the athlete exists in the Athletes table and if the medal type is valid. If the athlete does not exist or the medal type is invalid, it returns an error message.

-- If the athlete exists and the medal type is valid, the procedure checks if there is already a row for the athlete and NOC combination in the Medals table. If there is no row, it inserts a new row with the specified medal count. If there is a row, it updates the row with the new medal count.

-- The stored procedure uses cursors to iterate over the rows in the Medals table and update the correct row. It also uses IF statements to check the medal type and to handle errors.

-- The trigger "update_medal_count_trigger" is set to fire after an insert on the Medals table. It checks if the athlete exists in the Athletes table. If the athlete does not exist, it deletes the corresponding row from the Medals table. This ensures that the Medals table only contains entries for existing athletes.

-- Overall, these stored procedure and trigger ensure that the Medals table only contains valid entries for existing athletes and updates the medal counts for athletes as new medals are earned.



-- Create the stored procedure:
DELIMITER //
CREATE PROCEDURE update_medal_count(IN Ath_Name VARCHAR(512), IN NOC VARCHAR(512), IN medal_type VARCHAR(512))
BEGIN
   DECLARE medal_count INT;
   DECLARE total_count INT;
   DECLARE athlete_exists INT;
   DECLARE medal_exists INT;
   DECLARE done INT DEFAULT FALSE;
   DECLARE noc_total_medals INT;
   DECLARE sport_total_medals INT;
   DECLARE cur CURSOR FOR SELECT Num_total FROM Medals WHERE Ath_Name = Ath_Name AND NOC = NOC;
   DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;
   SET athlete_exists = (SELECT COUNT(*) FROM Athletes WHERE Ath_Name = Ath_Name LIMIT 1);

   -- add two advanced queries here
   SET noc_total_medals = (SELECT SUM(Num_total) FROM Medals WHERE NOC = NOC);
   SET sport_total_medals = (SELECT SUM(Num_total) FROM Medals INNER JOIN Athletes ON Medals.Ath_Name = Athletes.Ath_Name WHERE Athletes.Discipline = Discipline);


   IF athlete_exists = 0 THEN
      SELECT 'Athlete does not exist';
   ELSE
      IF medal_type = 'gold' THEN
         SET medal_count = (SELECT Num_Gold FROM Medals WHERE Ath_Name = Ath_Name AND NOC = NOC LIMIT 1);
         SET total_count = (SELECT Num_total FROM Medals WHERE Ath_Name = Ath_Name AND NOC = NOC LIMIT 1);
         IF medal_count IS NULL THEN
            INSERT INTO Medals (Ath_Name, NOC, Num_total, Num_Gold) VALUES (Ath_Name, NOC, 1, 1);
         ELSE
            UPDATE Medals SET Num_Gold = Num_Gold + 1, Num_total = Num_total + 1 WHERE Ath_Name = Ath_Name AND NOC = NOC;
         END IF;
      ELSEIF medal_type = 'silver' THEN
         SET medal_count = (SELECT Num_Silver FROM Medals WHERE Ath_Name = Ath_Name AND NOC = NOC LIMIT 1);
         SET total_count = (SELECT Num_total FROM Medals WHERE Ath_Name = Ath_Name AND NOC = NOC LIMIT 1);
         IF medal_count IS NULL THEN
            INSERT INTO Medals (Ath_Name, NOC, Num_total, Num_Silver) VALUES (Ath_Name, NOC, 1, 1);
         ELSE
            UPDATE Medals SET Num_Silver = Num_Silver + 1, Num_total = Num_total + 1 WHERE Ath_Name = Ath_Name AND NOC = NOC;
         END IF;
      ELSEIF medal_type = 'bronze' THEN
         SET medal_count = (SELECT Num_Bronze FROM Medals WHERE Ath_Name = Ath_Name AND NOC = NOC LIMIT 1);
         SET total_count = (SELECT Num_total FROM Medals WHERE Ath_Name = Ath_Name AND NOC = NOC LIMIT 1);
         IF medal_count IS NULL THEN
            INSERT INTO Medals (Ath_Name, NOC, Num_total, Num_Bronze) VALUES (Ath_Name, NOC, 1, 1);
         ELSE
            UPDATE Medals SET Num_Bronze = Num_Bronze + 1, Num_total = Num_total + 1 WHERE Ath_Name = Ath_Name AND NOC = NOC;
         END IF;
      ELSE
         SELECT 'Invalid medal type';
      END IF;
   END IF;
   OPEN cur;
   read_loop: LOOP
      FETCH cur INTO total_count;
      IF done THEN
         LEAVE read_loop;
      END IF;
   END LOOP;
   CLOSE cur;

END //



-- Create the trigger:
DELIMITER //
CREATE TRIGGER update_medal_count_trigger AFTER INSERT ON Medals
FOR EACH ROW
BEGIN
DECLARE athlete_exists INT;
SET athlete_exists = (SELECT COUNT(*) FROM Athletes WHERE Ath_Name = NEW.Ath_Name LIMIT 1);
IF athlete_exists = 0 THEN
DELETE FROM Medals WHERE Ath_Name = NEW.Ath_Name AND NOC = NEW.NOC;
END IF;
END //


-- Steps in GCS Console to see the procdeures: same as above
-- show databases;

-- use olympicstokyo;

-- You can use the ROUTINES information schema view to get a list of all user-defined stored procedures in a database.

-- You can use the TRIGGERS information schema view to get a list of all triggers in a database.

-- Write the SQL statement to get a list of all user-defined stored procedures in the database.
SELECT ROUTINE_NAME FROM INFORMATION_SCHEMA.ROUTINES WHERE ROUTINE_TYPE = 'PROCEDURE';

-- Write the SQL statement to get a list of all triggers in the database.
SELECT TRIGGER_NAME FROM INFORMATION_SCHEMA.TRIGGERS;

 -- See contents of update_medal_count stored procedure:
SHOW CREATE PROCEDURE update_medal_count;

-- See contents of update_medal_count_trigger trigger:
SHOW CREATE TRIGGER update_medal_count_trigger;

DROP PROCEDURE update_medal_count;

DROP TRIGGER update_medal_count_trigger;