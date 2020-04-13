package com.project.BL.Repositories;

import com.project.BL.Base.*;
import com.project.BL.DTOs.Lists.*;
import com.project.BL.Exceptions.*;
import oracle.jdbc.*;
import oracle.ord.im.*;
import oracle.spatial.geometry.*;

import java.sql.*;

public class GetListByIdRepository {

    private final String SQL_SELECT_FOLDER_BY_ID = "SELECT FolderName FROM TABLE_FOLDER WHERE FolderID = ?";
    private final String SQL_SELECT_GEOMETRY_BY_ID = "SELECT GeometryName, SpatialDataObject FROM TABLE_GEOMETRY WHERE GeometryId = ?";
    private final String SQL_SELECT_IMAGE_BY_ID = "SELECT ImageName, ImagePosition, Image FROM TABLE_IMAGE WHERE ImageId = ?";
    private final String SQL_SELECT_IMAGE_FOR_UPDATE = "SELECT image FROM TABLE_IMAGE WHERE imageid = ? FOR UPDATE";

    private final Connector connector = new Connector();

    public FolderListDTO GetFolder(int id) throws SQLException, NotFoundException {
        var connection = connector.getConnection();
        try (var preparedStatement = connection.prepareStatement(SQL_SELECT_FOLDER_BY_ID)) {
            preparedStatement.setInt(1, id);
            try (var resultSet = preparedStatement.executeQuery()) {
                if (resultSet.next()) {
                    return new FolderListDTO(id, resultSet.getString(1));
                } else {
                    throw new NotFoundException();
                }
            }
        }
    }

    public GeometryListDTO GetGeometry(int id) throws SQLException, NotFoundException {
        var connection = connector.getConnection();
        try (var preparedStatement = connection.prepareStatement(SQL_SELECT_GEOMETRY_BY_ID)) {
            preparedStatement.setInt(1, id);
            try (var resultSet = preparedStatement.executeQuery()) {
                if (resultSet.next()) {
                    var obj = (Struct) resultSet.getObject(2);
                    return new GeometryListDTO(id, resultSet.getString(1), JGeometry.loadJS(obj));
                } else {
                    throw new NotFoundException();
                }
            }
        }
    }

    public ImageListDTO GetImage(int id) throws SQLException, NotFoundException {
        var connection = connector.getConnection();
        try (var preparedStatement = connection.prepareStatement(SQL_SELECT_IMAGE_BY_ID)) {
            preparedStatement.setInt(1, id);
            try (var resultSet = preparedStatement.executeQuery()) {
                if (resultSet.next()) {
                    return new ImageListDTO(id, resultSet.getString(1), resultSet.getInt(2), SelectOrdImageForUpdate(id));
                } else {
                    throw new NotFoundException();
                }
            }
        }
    }

    public OrdImage SelectOrdImageForUpdate(int id) throws SQLException, NotFoundException {
        var connection = connector.getConnection();
        try (PreparedStatement preparedStatement = connection.prepareStatement(SQL_SELECT_IMAGE_FOR_UPDATE)) {
            preparedStatement.setInt(1, id);
            try (ResultSet resultSet = preparedStatement.executeQuery()) {
                if (resultSet.next()) {
                    final OracleResultSet oracleResultSet = (OracleResultSet) resultSet;
                    return (OrdImage) oracleResultSet.getORAData(1, OrdImage.getORADataFactory());
                } else {
                    throw new NotFoundException();
                }
            }
        }
    }
}
