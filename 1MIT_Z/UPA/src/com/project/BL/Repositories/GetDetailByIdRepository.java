package com.project.BL.Repositories;

import com.project.BL.Base.*;
import com.project.BL.DTOs.Details.*;
import com.project.BL.DTOs.Lists.*;
import com.project.BL.Exceptions.*;
import oracle.spatial.geometry.*;

import java.sql.*;

public class GetDetailByIdRepository {

    private final String SQL_SELECT_FOLDER_WITH_PARENT_BY_ID = "SELECT folder.FolderName,\n" +
            "       parent.FolderId,\n" +
            "       parent.FolderName\n" +
            "FROM TABLE_FOLDER parent\n" +
            "RIGHT JOIN TABLE_FOLDER folder \n" +
            "  ON folder.ParentFolderId = parent.FolderId\n" +
            "WHERE folder.FolderId = ?";

    private final String SQL_SELECT_CHILD_FOLDERS_BY_FOLDER_ID = "SELECT child.FolderId,\n" +
            "       child.FolderName\n" +
            "FROM TABLE_FOLDER folder\n" +
            "JOIN TABLE_FOLDER child \n" +
            "  ON child.ParentFolderId = folder.FolderId\n" +
            "WHERE child.ParentFolderId = ?";

    private final String SQL_SELECT_CHILD_IMAGES_BY_FOLDER_ID = "SELECT ImageId, ImageName, ImagePosition FROM TABLE_IMAGE WHERE BelongsToFolderId = ?";

    private final String SQL_SELECT_GEOMETRY_WITH_IMAGE_BY_ID = "SELECT GeometryName,\n" +
            "       SpatialDataObject,\n" +
            "       ImageId,\n" +
            "       ImageName,\n" +
            "       ImagePosition\n" +
            "FROM TABLE_GEOMETRY \n" +
            "LEFT JOIN TABLE_IMAGE  \n" +
            "  ON RepresentsImageId = ImageId\n" +
            "WHERE GeometryId = ?";

    private final String SQL_SELECT_IMAGE_WITH_FOLDER_AND_GEOMETRY_BY_ID = "SELECT ImageName,\n" +
            "       ImagePosition,\n" +
            "       FolderId,\n" +
            "       FolderName,\n" +
            "       GeometryID,\n" +
            "       GeometryName,\n" +
            "       SpatialDataObject\n" +
            "FROM TABLE_IMAGE \n" +
            "LEFT JOIN TABLE_FOLDER ON FolderId = BelongsToFolderId\n" +
            "LEFT JOIN TABLE_GEOMETRY ON GeometryId = RepresentedByGeometryId\n" +
            "WHERE ImageId = ?";

    private final Connector connector = new Connector();
    private final GetListByIdRepository getListByIdRepository = new GetListByIdRepository();

    public FolderDetailDTO GetFolder(int id) throws SQLException, NotFoundException {
        var connection = connector.getConnection();
        FolderDetailDTO folderDetail;

        //Get folder and his parent
        try (var preparedStatement = connection.prepareStatement(SQL_SELECT_FOLDER_WITH_PARENT_BY_ID)) {
            preparedStatement.setInt(1, id);
            try (var resultSet = preparedStatement.executeQuery()) {
                if (resultSet.next()) {
                    FolderListDTO parentFolderList = null;
                    if (resultSet.getInt(2) != 0) {
                        parentFolderList = new FolderListDTO(resultSet.getInt(2), resultSet.getString(3));
                    }
                    folderDetail = new FolderDetailDTO(id, resultSet.getString(1), parentFolderList);
                } else {
                    throw new NotFoundException();
                }
            }
        }

        //Get folder's child folders
        try (var preparedStatement = connection.prepareStatement(SQL_SELECT_CHILD_FOLDERS_BY_FOLDER_ID)) {
            preparedStatement.setInt(1, id);
            try (var resultSet = preparedStatement.executeQuery()) {
                while (resultSet.next()) {
                    var childFolderList = new FolderListDTO(resultSet.getInt(1), resultSet.getString(2));
                    folderDetail.getChildFolders().add(childFolderList);
                }
            }
        }

        //Get images in folder
        try (var preparedStatement = connection.prepareStatement(SQL_SELECT_CHILD_IMAGES_BY_FOLDER_ID)) {
            preparedStatement.setInt(1, id);
            try (var resultSet = preparedStatement.executeQuery()) {
                while (resultSet.next()) {
                    var childImageList = new ImageListDTO(resultSet.getInt(1), resultSet.getString(2),
                            resultSet.getInt(3), getListByIdRepository.SelectOrdImageForUpdate(resultSet.getInt(1)));
                    folderDetail.getImages().add(childImageList);
                }
            }
        }
        return folderDetail;
    }

    public GeometryDetailDTO GetGeometry(int id) throws SQLException, NotFoundException {
        var connection = connector.getConnection();
        try (var preparedStatement = connection.prepareStatement(SQL_SELECT_GEOMETRY_WITH_IMAGE_BY_ID)) {
            preparedStatement.setInt(1, id);
            try (var resultSet = preparedStatement.executeQuery()) {
                if (resultSet.next()) {
                    ImageListDTO childImageList = null;
                    if (resultSet.getInt(3) != 0) {
                        childImageList = new ImageListDTO(resultSet.getInt(3), resultSet.getString(4),
                                resultSet.getInt(5), getListByIdRepository.SelectOrdImageForUpdate(resultSet.getInt(3)));
                    }
                    var obj = (Struct) resultSet.getObject(2);
                    return new GeometryDetailDTO(id, resultSet.getString(1), JGeometry.loadJS(obj), childImageList);
                } else {
                    throw new NotFoundException();
                }
            }
        }
    }

    public ImageDetailDTO GetImage(int id) throws SQLException, NotFoundException {
        var connection = connector.getConnection();
        try (var preparedStatement = connection.prepareStatement(SQL_SELECT_IMAGE_WITH_FOLDER_AND_GEOMETRY_BY_ID)) {
            preparedStatement.setInt(1, id);
            try (var resultSet = preparedStatement.executeQuery()) {
                if (resultSet.next()) {
                    GeometryListDTO geometryList = null;
                    if (resultSet.getInt(5) != 0) {
                        Struct obj = (Struct) resultSet.getObject(7);
                        geometryList = new GeometryListDTO(resultSet.getInt(5), resultSet.getString(6), JGeometry.loadJS(obj));
                    }
                    var folderList = new FolderListDTO(resultSet.getInt(3), resultSet.getString(4));
                    return new ImageDetailDTO(id, resultSet.getString(1), resultSet.getInt(2), getListByIdRepository.SelectOrdImageForUpdate(id), folderList, geometryList);
                } else {
                    throw new NotFoundException();
                }
            }
        }
    }
}
