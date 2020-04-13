----------------------------------------------------------------------------
-- Create new FOLDER using given keys
----------------------------------------------------------------------------
CREATE OR REPLACE
PROCEDURE create_folder( folder_name      IN table_folder.FolderName%TYPE,
                         parent_folder_id IN table_folder.ParentFolderID%TYPE )
AS
BEGIN
   INSERT INTO TABLE_FOLDER( FolderName, ParentFolderID ) VALUES( folder_name, parent_folder_id );
END create_folder;
/

----------------------------------------------------------------------------
-- Create new FOLDER using given keys (find parent folder using its name)
----------------------------------------------------------------------------
CREATE OR REPLACE
PROCEDURE create_folder_lookup( folder_name        IN table_folder.FolderName%TYPE,
                                parent_folder_name IN table_folder.FolderName%TYPE,
                                error_code         OUT INTEGER )
AS
   ParentFolderID table_folder.ParentFolderID%TYPE;
BEGIN
   error_code := 0;
   SELECT FolderID INTO ParentFolderID FROM TABLE_FOLDER WHERE FolderName = parent_folder_name;
   INSERT INTO TABLE_FOLDER( FolderName, ParentFolderID ) VALUES( folder_name, ParentFolderID );
EXCEPTION
    WHEN NO_DATA_FOUND THEN
      error_code := 1;
    WHEN OTHERS THEN
      error_code := 2;
END create_folder_lookup;
/
