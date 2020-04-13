-- Create folder hierarchy:
--
--            Root
--           /    \
--    Italy         Czechia
--                 /       \
--             Prague      Brno
--
INSERT INTO TABLE_FOLDER( FolderName, ParentFolderID ) VALUES( 'Root',    NULL );
INSERT INTO TABLE_FOLDER( FolderName, ParentFolderID ) VALUES( 'Italy',   1 );
INSERT INTO TABLE_FOLDER( FolderName, ParentFolderID ) VALUES( 'Czechia', 1 );
INSERT INTO TABLE_FOLDER( FolderName, ParentFolderID ) VALUES( 'Prague',  3 );
INSERT INTO TABLE_FOLDER( FolderName, ParentFolderID ) VALUES( 'Brno',    3 );

-- Two images to prague folder, one to brno folder
INSERT INTO TABLE_IMAGE( ImageName, ImagePosition, BelongsToFolderID, RepresentedByGeometryID, Image ) VALUES( 'Charles bridge', 1, 4, NULL, ORDSYS.ORDImage.init() );
INSERT INTO TABLE_IMAGE( ImageName, ImagePosition, BelongsToFolderID, RepresentedByGeometryID, Image ) VALUES( 'Charles castle', 2, 4, NULL, ORDSYS.ORDImage.init() );
INSERT INTO TABLE_IMAGE( ImageName, ImagePosition, BelongsToFolderID, RepresentedByGeometryID, Image ) VALUES( 'Park Luzanky',   1, 5, NULL, ORDSYS.ORDImage.init() );

-- Assignment of positions of images (point for each image)
INSERT INTO TABLE_GEOMETRY( GeometryName, SpatialDataObject, RepresentsImageID ) VALUES( 'Geometry 1', SDO_GEOMETRY( 2001, NULL, SDO_POINT_TYPE( 100, 200, NULL ), NULL, NULL ), 1 );
INSERT INTO TABLE_GEOMETRY( GeometryName, SpatialDataObject, RepresentsImageID ) VALUES( 'Geometry 2', SDO_GEOMETRY( 2001, NULL, SDO_POINT_TYPE( 200, 300, NULL ), NULL, NULL ), 2 );
INSERT INTO TABLE_GEOMETRY( GeometryName, SpatialDataObject, RepresentsImageID ) VALUES( 'Geometry 3', SDO_GEOMETRY( 2003, NULL, SDO_POINT_TYPE( 300, 400, NULL ), NULL, NULL ), 3 );
INSERT INTO TABLE_GEOMETRY( GeometryName, SpatialDataObject, RepresentsImageID ) VALUES( 'Rect 1',     SDO_GEOMETRY( 2003, NULL, NULL, SDO_ELEM_INFO_ARRAY( 1, 1003, 1 ), SDO_ORDINATE_ARRAY( 500, 300, 550, 300, 550, 370, 500, 370, 500, 300 ) ), NULL );
INSERT INTO TABLE_GEOMETRY( GeometryName, SpatialDataObject, RepresentsImageID ) VALUES( 'Rect 2',     SDO_GEOMETRY( 2003, NULL, NULL, SDO_ELEM_INFO_ARRAY( 1, 1003, 1 ), SDO_ORDINATE_ARRAY( 320, 320, 400, 320, 400, 400, 320, 400, 320, 320 ) ), NULL );
INSERT INTO TABLE_GEOMETRY( GeometryName, SpatialDataObject, RepresentsImageID ) VALUES( 'Circle 1',   SDO_GEOMETRY( 2003, NULL, NULL, SDO_ELEM_INFO_ARRAY( 1, 1003, 4 ), SDO_ORDINATE_ARRAY( 50, 20, 50, 80, 80, 50 ) ), NULL );
INSERT INTO TABLE_GEOMETRY( GeometryName, SpatialDataObject, RepresentsImageID ) VALUES( 'Circle 2',   SDO_GEOMETRY( 2003, NULL, NULL, SDO_ELEM_INFO_ARRAY( 1, 1003, 4 ), SDO_ORDINATE_ARRAY( 250, 200, 250, 300, 300, 250 ) ), NULL );

-- Assign geometries to the images
UPDATE TABLE_IMAGE SET RepresentedByGeometryID = 1 WHERE ImageID = 1;
UPDATE TABLE_IMAGE SET RepresentedByGeometryID = 2 WHERE ImageID = 2;
UPDATE TABLE_IMAGE SET RepresentedByGeometryID = 3 WHERE ImageID = 3;

-- Check validity of spatial data
SELECT GeometryName, SDO_GEOM.VALIDATE_GEOMETRY_WITH_CONTEXT( SpatialDataObject, 0.1 ) valid FROM TABLE_GEOMETRY;
SELECT t_geo.GeometryName, t_geo.SpatialDataObject.ST_IsValid() FROM TABLE_GEOMETRY t_geo;