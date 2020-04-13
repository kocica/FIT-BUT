package com.project.BL.Repositories;

import com.project.BL.Base.*;
import com.project.BL.DTOs.Details.*;
import com.project.BL.DTOs.Lists.*;
import com.project.BL.Exceptions.*;
import oracle.jdbc.*;

import java.sql.*;

public class InitializeRepository {

    private final String SQL_INSERT_FOLDER = "INSERT INTO TABLE_FOLDER (FolderName, ParentFolderId) VALUES('New Folder', ?) RETURNING FolderId INTO ?";
    private final String SQL_INSERT_GEOMETRY = "INSERT INTO TABLE_GEOMETRY (GeometryName, SpatialDataObject) VALUES('New Geometry', SDO_GEOMETRY( 2001, NULL, SDO_POINT_TYPE( 0, 0, NULL ), NULL, NULL )) RETURNING GeometryId INTO ?";
    private final String SQL_INSERT_IMAGE = "INSERT INTO TABLE_IMAGE (ImageName, BelongsToFolderId, Image) VALUES ('New Image', ?, ordsys.ordimage.init()) RETURNING ImageId INTO ?";

    private final Connector connector = new Connector();
    private final GetDetailByIdRepository getDetailByIdRepository = new GetDetailByIdRepository();

    public FolderDetailDTO InitializeFolder(FolderListDTO parentFolder) throws SQLException {
        var connection = connector.getConnection();
        try (var preparedStatement = (OraclePreparedStatement) connection.prepareStatement(SQL_INSERT_FOLDER)) {
            preparedStatement.setInt(1, parentFolder.getId());
            preparedStatement.registerReturnParameter(2, Types.INTEGER);
            preparedStatement.execute();
            var resultSet = preparedStatement.getReturnResultSet();
            resultSet.next();
            connection.commit();
            return new FolderDetailDTO(resultSet.getInt(1), "New Folder", parentFolder);
        }
    }

    public GeometryDetailDTO InitializeGeometry() throws SQLException {
        var connection = connector.getConnection();
        try (var preparedStatement = (OraclePreparedStatement) connection.prepareStatement(SQL_INSERT_GEOMETRY)) {
            preparedStatement.registerReturnParameter(1, Types.INTEGER);
            preparedStatement.execute();
            var resultSet = preparedStatement.getReturnResultSet();
            resultSet.next();
            var geometry = new GeometryDetailDTO(resultSet.getInt(1), "New Geometry", null, null);
            connection.commit();
            return geometry;
        }
    }

    public ImageDetailDTO InitializeImage(FolderListDTO parentFolder) throws SQLException {
        var connection = connector.getConnection();
        try (var preparedStatement = (OraclePreparedStatement) connection.prepareStatement(SQL_INSERT_IMAGE)) {
            preparedStatement.setInt(1, parentFolder.getId());
            preparedStatement.registerReturnParameter(2, Types.INTEGER);
            preparedStatement.execute();
            var resultSet = (OracleResultSet) preparedStatement.getReturnResultSet();
            resultSet.next();
            try {
                return getDetailByIdRepository.GetImage(resultSet.getInt(1));
            } catch (NotFoundException ignored) {
            }
            return null;
        }
    }
}
