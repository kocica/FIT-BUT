
CREATE TABLE TABLE_FOLDER (
                              FolderID INTEGER NOT NULL,
                              FolderName VARCHAR(20) NOT NULL,
                              ParentFolderID INTEGER
);

CREATE TABLE TABLE_IMAGE (
                             ImageID INTEGER NOT NULL,
                             ImageName VARCHAR(20) NOT NULL,
                             ImagePosition INTEGER,
                             BelongsToFolderID INTEGER NOT NULL,
                             RepresentedByGeometryID INTEGER,
                             Image ORDSYS.ORDImage,
                             Image_si ORDSYS.SI_StillImage,
                             Image_ac ORDSYS.SI_AverageColor,
                             Image_ch ORDSYS.SI_ColorHistogram,
                             Image_pc ORDSYS.SI_PositionalColor,
                             Image_tx ORDSYS.SI_Texture,
                             Signature ORDSYS.ORDImageSignature
);

CREATE TABLE TABLE_GEOMETRY (
                                GeometryID INTEGER NOT NULL,
                                GeometryName VARCHAR(20) NOT NULL,
                                SpatialDataObject SDO_Geometry,
                                RepresentsImageID INTEGER
);

--CREATE OR REPLACE TRIGGER IMAGE_GEN_FEATURES
--    AFTER INSERT OR UPDATE OF Image ON TABLE_IMAGE
--    FOR EACH ROW
--DECLARE
--    si  ORDSYS.SI_StillImage;
--    sig ORDSYS.ORDImageSignature;
--BEGIN
--    si := new SI_StillImage( :NEW.Image.getContent() );
--    sig.generateSignature( :NEW.Image );
--
--    UPDATE TABLE_IMAGE ti
--    SET image_si  = si,
--        image_ac  = SI_AverageColor( si ),
--        image_ch  = SI_ColorHistogram( si ),
--        image_pc  = SI_PositionalColor( si ),
--        image_tx  = SI_Texture( si ),
--        signature = sig
--    WHERE ti.ImageID = :NEW.ImageID;
--END;
--/

-----------------------------------------------
-- Create sequences for automatic ID
-- incrementation
-----------------------------------------------
CREATE SEQUENCE FOLDER_SEQ_ID;
CREATE OR REPLACE TRIGGER AI_FOLDER
    BEFORE INSERT ON TABLE_FOLDER
    FOR EACH ROW
BEGIN
    :new.FolderID := FOLDER_SEQ_ID.nextval;
END AI_FOLDER;
/

CREATE SEQUENCE IMAGE_SEQ_ID;
CREATE OR REPLACE TRIGGER AI_IMAGE
    BEFORE INSERT ON TABLE_IMAGE
    FOR EACH ROW
BEGIN
    :new.ImageID := IMAGE_SEQ_ID.nextval;
END AI_IMAGE;
/

CREATE SEQUENCE GEOMETRY_SEQ_ID;
CREATE OR REPLACE TRIGGER AI_GEOMETRY
    BEFORE INSERT ON TABLE_GEOMETRY
    FOR EACH ROW
BEGIN
    :new.GeometryID := GEOMETRY_SEQ_ID.nextval;
END AI_GEOMETRY;
/

-----------------------------------------------
-- Grant rights for modification of tables
-- to users (TODO: fill in user logins)
-----------------------------------------------
GRANT ALL ON TABLE_FOLDER TO xkocic01;
GRANT ALL ON TABLE_IMAGE TO xkocic01;
GRANT ALL ON TABLE_GEOMETRY TO xkocic01;

-----------------------------------------------
-- Assign primary keys to the tables
-----------------------------------------------
ALTER TABLE TABLE_FOLDER ADD CONSTRAINT PK_Folder PRIMARY KEY ( FolderID );
ALTER TABLE TABLE_IMAGE ADD CONSTRAINT PK_Image PRIMARY KEY ( ImageID );
ALTER TABLE TABLE_GEOMETRY ADD CONSTRAINT PK_Geometry PRIMARY KEY ( GeometryID );

-----------------------------------------------
-- Assign foreign keys to the tables
-----------------------------------------------
ALTER TABLE TABLE_FOLDER ADD CONSTRAINT FK_ParentFolder FOREIGN KEY ( ParentFolderID ) REFERENCES TABLE_FOLDER ON DELETE CASCADE;
ALTER TABLE TABLE_IMAGE ADD CONSTRAINT FK_BelongsToFolder FOREIGN KEY ( BelongsToFolderID ) REFERENCES TABLE_FOLDER ON DELETE CASCADE;
ALTER TABLE TABLE_IMAGE ADD CONSTRAINT FK_RepresentedByGeometry FOREIGN KEY ( RepresentedByGeometryID ) REFERENCES TABLE_GEOMETRY ON DELETE CASCADE;
ALTER TABLE TABLE_GEOMETRY ADD CONSTRAINT FK_RepresentsImage FOREIGN KEY ( RepresentsImageID ) REFERENCES TABLE_IMAGE ON DELETE CASCADE;

-----------------------------------------------
-- Create mesh (X/Y axes) where spatial data
-- are stored
-----------------------------------------------
DELETE FROM USER_SDO_GEOM_METADATA WHERE TABLE_NAME = 'TABLE_GEOMETRY';
INSERT INTO USER_SDO_GEOM_METADATA VALUES ( 'TABLE_GEOMETRY', 'SpatialDataObject', SDO_DIM_ARRAY( SDO_DIM_ELEMENT( 'X', 0, 600, 0.1 ), SDO_DIM_ELEMENT( 'Y', 0, 600, 0.1 ) ), NULL );


COMMIT;