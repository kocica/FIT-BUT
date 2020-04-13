package com.project.BL.Repositories;

import com.project.BL.Base.*;
import com.project.BL.DTOs.Lists.*;
import oracle.jdbc.*;
import oracle.ord.im.*;
import oracle.spatial.geometry.*;

import java.sql.*;
import java.util.*;

public class GetAllRepository {

    private final String SQL_SELECT_ALL_FOLDERS = "SELECT FolderId, FolderName FROM TABLE_FOLDER";
    private final String SQL_SELECT_ALL_GEOMETRIES = "SELECT GeometryId, GeometryName, SpatialDataObject FROM TABLE_GEOMETRY";
    private final String SQL_SELECT_ALL_IMAGES = "SELECT ImageId, ImageName, ImagePosition, Image FROM TABLE_IMAGE";

    private final Connector connector = new Connector();

    public Collection<FolderListDTO> GetFolders() throws SQLException {
        var connection = connector.getConnection();
        try (var preparedStatement = connection.prepareStatement(SQL_SELECT_ALL_FOLDERS)) {
            try (var resultSet = preparedStatement.executeQuery()) {
                var folderLists = new ArrayList<FolderListDTO>();
                while (resultSet.next()) {
                    folderLists.add(new FolderListDTO(resultSet.getInt(1), resultSet.getString(2)));
                }
                return folderLists;
            }
        }
    }

    public Collection<GeometryListDTO> GetGeometries() throws SQLException {
        var connection = connector.getConnection();
        try (var preparedStatement = connection.prepareStatement(SQL_SELECT_ALL_GEOMETRIES)) {
            try (var resultSet = preparedStatement.executeQuery()) {
                var geometryLists = new ArrayList<GeometryListDTO>();
                while (resultSet.next()) {
                    var obj = (Struct) resultSet.getObject(3);
                    var geometryList = new GeometryListDTO(resultSet.getInt(1), resultSet.getString(2), JGeometry.loadJS(obj));
                    geometryLists.add(geometryList);
                }
                return geometryLists;
            }
        }
    }

    public Collection<ImageListDTO> GetImages() throws SQLException {
        var connection = connector.getConnection();
        try (var preparedStatement = connection.prepareStatement(SQL_SELECT_ALL_IMAGES)) {
            try (var resultSet = preparedStatement.executeQuery()) {
                var imageLists = new ArrayList<ImageListDTO>();
                while (resultSet.next()) {
                    final var oracleResultSet = (OracleResultSet) resultSet;
                    final var ordImage = (OrdImage) oracleResultSet.getORAData(4, OrdImage.getORADataFactory());
                    var imageList = new ImageListDTO(resultSet.getInt(1), resultSet.getString(2), resultSet.getInt(3), ordImage);
                    imageLists.add(imageList);
                }
                return imageLists;
            }
        }
    }
}
