----------------------------------------------------------------------------
-- Function used to calculate distance of two images
-- @param image_1_id - ID of first image
-- @param image_2_id - ID of second image
-- @return Calculated distance or '-1' in case some error
--         has occured during processing
----------------------------------------------------------------------------
CREATE OR REPLACE
FUNCTION calc_img_distance( image_1_id IN table_image.ImageID%TYPE,
                            image_2_id IN table_image.ImageID%TYPE )
RETURN FLOAT
AS
    geometry_1_id table_geometry.GeometryID%TYPE;
    geometry_2_id table_geometry.GeometryID%TYPE;
    pt1 table_geometry.SpatialDataObject%TYPE;
    pt2 table_geometry.SpatialDataObject%TYPE;
BEGIN
   SELECT RepresentedByGeometryID INTO geometry_1_id FROM TABLE_IMAGE WHERE ImageID = image_1_id;
   SELECT RepresentedByGeometryID INTO geometry_2_id FROM TABLE_IMAGE WHERE ImageID = image_2_id;
   SELECT SpatialDataObject INTO pt1 FROM TABLE_GEOMETRY WHERE GeometryID = geometry_1_id;
   SELECT SpatialDataObject INTO pt2 FROM TABLE_GEOMETRY WHERE GeometryID = geometry_2_id;

   RETURN SDO_GEOM.SDO_DISTANCE( pt1, pt2, 0.01 );
EXCEPTION
   WHEN OTHERS THEN
      RETURN -1;
END calc_img_distance;
/

----------------------------------------------------------------------------
-- Function used to connect points where pictures were taken
-- @param folder_id - ID of folder
-- @return Connected points (line) or NULL (in case connection failed)
----------------------------------------------------------------------------
CREATE OR REPLACE
FUNCTION connect_img_points( folder_id IN table_folder.FolderID%TYPE )
RETURN SDO_GEOMETRY
AS
    TYPE image_ids_t IS TABLE OF table_image.ImageID%TYPE INDEX BY PLS_INTEGER;
    v_image_ids image_ids_t;
    connected_line SDO_GEOMETRY := NULL;
    geometry table_geometry.GeometryID%TYPE;
    pt1 table_geometry.SpatialDataObject%TYPE;
    pt2 table_geometry.SpatialDataObject%TYPE;
BEGIN
   SELECT imageid
     BULK COLLECT INTO v_image_ids
     FROM table_image
    WHERE belongstofolderid = folder_id;

   IF( v_image_ids.count() >= 2 ) THEN
     SELECT RepresentedByGeometryID INTO geometry FROM TABLE_IMAGE WHERE ImageID = v_image_ids( 1 );
     SELECT SpatialDataObject INTO pt1 FROM TABLE_GEOMETRY WHERE GeometryID = geometry;
     SELECT RepresentedByGeometryID INTO geometry FROM TABLE_IMAGE WHERE ImageID = v_image_ids( 2 );
     SELECT SpatialDataObject INTO pt2 FROM TABLE_GEOMETRY WHERE GeometryID = geometry;

     connected_line := SDO_UTIL.APPEND( SDO_GEOMETRY( 2001, NULL, SDO_POINT_TYPE( pt1.sdo_point.x, pt1.sdo_point.y, NULL ), NULL, NULL ),
                                        SDO_GEOMETRY( 2001, NULL, SDO_POINT_TYPE( pt2.sdo_point.x, pt2.sdo_point.y, NULL ), NULL, NULL ) );
   END IF;

   FOR i IN 3 .. v_image_ids.count() LOOP
      SELECT RepresentedByGeometryID INTO geometry FROM TABLE_IMAGE WHERE ImageID = v_image_ids( i );
      SELECT SpatialDataObject INTO pt1 FROM TABLE_GEOMETRY WHERE GeometryID = geometry;

      connected_line := SDO_UTIL.APPEND( connected_line,
                                         SDO_GEOMETRY( 2001, NULL, SDO_POINT_TYPE( pt1.sdo_point.x, pt1.sdo_point.y, NULL ), NULL, NULL ) );
   END LOOP;

   RETURN connected_line;
EXCEPTION
   WHEN OTHERS THEN
      RETURN NULL;
END connect_img_points;
/

----------------------------------------------------------------------------
-- Function used to calculate convex hull used by calc_folder_area and
-- calc_folders_intersection
-- @param folder_id - ID of folder
-- @return Calculated convex hull or NULL in case some error
--         has occured during processing
----------------------------------------------------------------------------
CREATE OR REPLACE
FUNCTION calc_folder_convex_hull( folder_id IN table_folder.FolderID%TYPE )
RETURN SDO_GEOMETRY
AS
    convex_hull    SDO_GEOMETRY;
    connected_line SDO_GEOMETRY;
BEGIN
   connected_line := connect_img_points( folder_id );
   convex_hull    := SDO_GEOM.SDO_CONVEXHULL( connected_line, 2 );

   RETURN convex_hull;
EXCEPTION
   WHEN OTHERS THEN
      RETURN NULL;
END calc_folder_convex_hull;
/

----------------------------------------------------------------------------
-- Function used to calculate area from convex hull of images in folder
-- @param folder_id - ID of folder
-- @return Calculated area or '-1' in case some error
--         has occured during processing
----------------------------------------------------------------------------
CREATE OR REPLACE
FUNCTION calc_folder_area( folder_id IN table_folder.FolderID%TYPE )
RETURN FLOAT
AS
    area           FLOAT;
    convex_hull    SDO_GEOMETRY;
BEGIN
   convex_hull    := calc_folder_convex_hull( folder_id );
   area           := SDO_GEOM.SDO_AREA( convex_hull );

   RETURN area;
EXCEPTION
   WHEN OTHERS THEN
      RETURN -1;
END calc_folder_area;
/

----------------------------------------------------------------------------
-- Function used to calculate intersection of two folders convex hulls
-- @param folder_1_id - ID of first folder
-- @param folder_2_id - ID of second folder
-- @return Calculated area or '-1' in case some error
--         has occured during processing
----------------------------------------------------------------------------
CREATE OR REPLACE
FUNCTION calc_folders_intersection( folder_1_id IN table_folder.FolderID%TYPE,
                                    folder_2_id IN table_folder.FolderID%TYPE)
RETURN FLOAT
AS
    area_1         SDO_GEOMETRY;
    area_2         SDO_GEOMETRY;
    intersection   SDO_GEOMETRY;
    area_intersect FLOAT;
BEGIN
   area_1 := calc_folder_convex_hull( folder_1_id );
   area_2 := calc_folder_convex_hull( folder_2_id );
   intersection   := SDO_GEOM.SDO_INTERSECTION( area_1, area_2, 0.01 );
   area_intersect := SDO_GEOM.SDO_AREA( intersection );

   RETURN area_intersect;
EXCEPTION
   WHEN OTHERS THEN
      RETURN -1;
END calc_folders_intersection;
/

----------------------------------------------------------------------------
-- Function used to calculate avg distance between images in one folder
-- @param folder_id - ID of folder
-- @return Calculated avg distance or '-1' in case some error
--         has occured during processing
----------------------------------------------------------------------------
CREATE OR REPLACE
FUNCTION calc_avg_distance( folder_id IN table_folder.FolderID%TYPE )
RETURN FLOAT
AS
    TYPE image_ids_t IS TABLE OF table_image.ImageID%TYPE INDEX BY PLS_INTEGER; 
    v_image_ids image_ids_t;
    distance_sum FLOAT  := 0;
    num_records INTEGER := 0;
BEGIN
   SELECT imageid
     BULK COLLECT INTO v_image_ids
     FROM table_image
    WHERE belongstofolderid = folder_id;

   FOR i IN 1 .. ( v_image_ids.count() - 1 ) LOOP
      FOR j IN ( i + 1 ) .. v_image_ids.count() LOOP
         distance_sum := distance_sum + calc_img_distance( v_image_ids( i ), v_image_ids( j ) );
         num_records  := num_records + 1;
      END LOOP;
   END LOOP;

   RETURN distance_sum / num_records;
EXCEPTION
   WHEN OTHERS THEN
      RETURN -1;
END calc_avg_distance;
/

----------------------------------------------------------------------------
-- Function used to calculate max distance between images in one folder
-- @param folder_id - ID of folder
-- @return Calculated max distance or '-1' in case some error
--         has occured during processing
----------------------------------------------------------------------------
CREATE OR REPLACE
FUNCTION calc_max_distance( folder_id  IN table_folder.FolderID%TYPE )
RETURN FLOAT
AS
    TYPE image_ids_t IS TABLE OF table_image.ImageID%TYPE INDEX BY PLS_INTEGER; 
    v_image_ids image_ids_t;
    max_distance FLOAT  := 0;
    calc_distance FLOAT := 0;
BEGIN
   SELECT imageid
     BULK COLLECT INTO v_image_ids
     FROM table_image
    WHERE belongstofolderid = folder_id
    ORDER BY imageposition;

   FOR i IN 1 .. ( v_image_ids.count() - 1 ) LOOP
      calc_distance := calc_img_distance( v_image_ids( i ), v_image_ids( i + 1 ) );
      
      IF( calc_distance > max_distance ) THEN
         max_distance := calc_distance;
      END IF;
   END LOOP;

   RETURN max_distance;
EXCEPTION
   WHEN OTHERS THEN
      RETURN -1;
END calc_max_distance;
/

----------------------------------------------------------------------------
-- Function used to calculate min distance between images in one folder
-- @param folder_id - ID of folder
-- @return Calculated min distance or '-1' in case some error
--         has occured during processing or there are not at least 2 images.
----------------------------------------------------------------------------
CREATE OR REPLACE
FUNCTION calc_min_distance( folder_id  IN table_folder.FolderID%TYPE )
RETURN FLOAT
AS
    TYPE image_ids_t IS TABLE OF table_image.ImageID%TYPE INDEX BY PLS_INTEGER; 
    v_image_ids image_ids_t;
    min_distance FLOAT  := 0;
    calc_distance FLOAT := 0;
BEGIN
   SELECT imageid
     BULK COLLECT INTO v_image_ids
     FROM table_image
    WHERE belongstofolderid = folder_id
    ORDER BY imageposition;
    
   IF( v_image_ids.count() >= 2 ) THEN
     min_distance := calc_img_distance( v_image_ids( 1 ), v_image_ids( 2 ) );
   ELSE
     RETURN -1;
   END IF;

   FOR i IN 2 .. ( v_image_ids.count() - 1 ) LOOP
      calc_distance := calc_img_distance( v_image_ids( i ), v_image_ids( i + 1 ) );
      
      IF( calc_distance < min_distance ) THEN
         min_distance := calc_distance;
      END IF;
   END LOOP;

   RETURN min_distance;
EXCEPTION
   WHEN OTHERS THEN
      RETURN -1;
END calc_min_distance;
/

----------------------------------------------------------------------------
-- Function used to calculate overall distance between images in one folder
-- @param folder_id - ID of folder
-- @return Calculated overall distance or '-1' in case some error
--         has occured during processing
----------------------------------------------------------------------------
CREATE OR REPLACE
FUNCTION calc_overall_distance( folder_id IN table_folder.FolderID%TYPE )
RETURN FLOAT
AS
    TYPE image_ids_t IS TABLE OF table_image.ImageID%TYPE INDEX BY PLS_INTEGER; 
    v_image_ids image_ids_t;
    distance_sum FLOAT  := 0;
BEGIN
   SELECT imageid
     BULK COLLECT INTO v_image_ids
     FROM table_image
    WHERE belongstofolderid = folder_id
    ORDER BY imageposition;

   FOR i IN 1 .. ( v_image_ids.count() - 1 ) LOOP
      distance_sum := distance_sum + calc_img_distance( v_image_ids( i ), v_image_ids( i + 1 ) );
   END LOOP;

   RETURN distance_sum;
EXCEPTION
   WHEN OTHERS THEN
      RETURN -1;
END calc_overall_distance;
/
