----------------------------------------------------------------------------
-- Rotate image for given number of degrees
-- @param in_image_id - Image to be rotated
-- @param in_degrees - Number of degrees
----------------------------------------------------------------------------
CREATE OR REPLACE
    PROCEDURE rotate_image( in_image_id IN table_image.imageID%TYPE,
                            in_degrees IN INTEGER := 90 )
AS
    rotated_image ORDSYS.ORDImage;
BEGIN
    SELECT ti.image INTO rotated_image FROM table_image ti WHERE ti.imageID = in_image_id FOR UPDATE;

    -- Rotate image and store back to the DB
    rotated_image.process('rotate=' || in_degrees);

    UPDATE table_image SET image = rotated_image WHERE imageID = in_image_id;
    COMMIT;
END rotate_image;
/


----------------------------------------------------------------------------
-- Find image similar to image passed as parameter
-- @param image - Image to which similar image we are looking for
-- @return Similar image ID or '-1' in case there is not any
----------------------------------------------------------------------------
CREATE OR REPLACE
FUNCTION find_similar_img( image IN ORDSYS.ORDImage )
RETURN table_image.imageID%TYPE
AS
    image_id  table_image.imageID%TYPE;
    image_sig ORDSYS.ORDImageSignature;
BEGIN

   image_sig.generateSignature( image );

   SELECT ti.imageID
     INTO image_id
     FROM table_image ti
    WHERE ORDSYS.ORDImageSignature.isSimilar( image_sig, ti.signature, 'color=1.0,texture=0,shape=0,location=0', 10 ) = 1;

   RETURN image_id;
EXCEPTION
   WHEN OTHERS THEN
      RETURN -1;
END find_similar_img;
/