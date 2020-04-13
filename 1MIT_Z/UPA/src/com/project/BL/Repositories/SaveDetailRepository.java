package com.project.BL.Repositories;

import com.project.BL.Base.*;
import com.project.BL.DTOs.Details.*;
import com.project.BL.DTOs.Lists.ImageListDTO;
import oracle.jdbc.*;
import oracle.spatial.geometry.*;

import java.sql.*;

public class SaveDetailRepository {

    private final String SQL_UPDATE_DETAIL_FOLDER = "UPDATE TABLE_FOLDER SET FolderName = ?, ParentFolderID = ? WHERE FolderId = ?";
    private final String SQL_UPDATE_DETAIL_GEOMETRY = "UPDATE TABLE_GEOMETRY SET GeometryName = ?, SpatialDataObject = ?, RepresentsImageId = ? WHERE GeometryId = ?";
    private final String SQL_UPDATE_DETAIL_IMAGE = "UPDATE TABLE_IMAGE SET ImageName = ?, ImagePosition = ?, Image = ?, BelongsToFolderId = ?, RepresentedByGeometryId = ? Where ImageId = ?";
    private final String SQL_UPDATE_LIST_IMAGE = "UPDATE TABLE_IMAGE SET ImageName = ?, ImagePosition = ?, Image = ? Where ImageId = ?";

    private final Connector connector = new Connector();
    private final SaveListRepository saveListRepository = new SaveListRepository();

    public void Save(FolderDetailDTO folderDetailDTO) throws SQLException {
        var connection = connector.getConnection();
        try (var preparedStatement = (OraclePreparedStatement) connection.prepareStatement(SQL_UPDATE_DETAIL_FOLDER)) {
            preparedStatement.setString(1, folderDetailDTO.getName());
            if( folderDetailDTO.getParentFolder() == null){
                preparedStatement.setNull(2, java.sql.Types.INTEGER);
            }
            else {
                preparedStatement.setInt(2, folderDetailDTO.getParentFolder().getId());
            }
            preparedStatement.setInt(3, folderDetailDTO.getId());
            preparedStatement.execute();
        }
        if (folderDetailDTO.getParentFolder() != null) saveListRepository.Save(folderDetailDTO.getParentFolder());
        for (var image : folderDetailDTO.getImages()) saveListRepository.Save(image);
        connection.commit();
    }

    public void Save(GeometryDetailDTO geometryDetailDTO) throws Exception {
        var connection = connector.getConnection();
        try (var preparedStatement = (OraclePreparedStatement) connection.prepareStatement(SQL_UPDATE_DETAIL_GEOMETRY)) {
            preparedStatement.setString(1, geometryDetailDTO.getName());
            var obj = JGeometry.storeJS(connection, geometryDetailDTO.getGeometry());
            preparedStatement.setObject(2, obj);
            if(geometryDetailDTO.getImage() == null) {
                preparedStatement.setNull(3, java.sql.Types.INTEGER);
            }
            else {
                preparedStatement.setInt(3, geometryDetailDTO.getImage().getId());
            }
            preparedStatement.setInt(4, geometryDetailDTO.getId());
            preparedStatement.execute();
        }
        if (geometryDetailDTO.getImage() != null) saveListRepository.Save(geometryDetailDTO.getImage());
        connection.commit();
    }

    public void Save(ImageDetailDTO imageDetailDTO) throws Exception {
        var connection = connector.getConnection();
        try (var preparedStatement = (OraclePreparedStatement) connection.prepareStatement(SQL_UPDATE_DETAIL_IMAGE)) {
            preparedStatement.setString(1, imageDetailDTO.getName());
            preparedStatement.setInt(2, imageDetailDTO.getPosition());
            preparedStatement.setORAData(3, imageDetailDTO.getOrdImage());
            preparedStatement.setInt(4, imageDetailDTO.getParent() == null ? null : imageDetailDTO.getParent().getId());
            if(imageDetailDTO.getGeometry() == null){
                preparedStatement.setNull(5, java.sql.Types.INTEGER);
            }
            else {
                preparedStatement.setInt(5, imageDetailDTO.getGeometry().getId());
            }
            preparedStatement.setInt(6, imageDetailDTO.getId());
            preparedStatement.execute();
        }
        saveListRepository.UpdateStillImageData(connection, imageDetailDTO.getId());
        if (imageDetailDTO.getParent() != null) saveListRepository.Save(imageDetailDTO.getParent());
        if (imageDetailDTO.getGeometry() != null) saveListRepository.Save(imageDetailDTO.getGeometry());
        connection.commit();
    }

    public void Save(ImageListDTO imageListDTO) throws Exception {
        var connection = connector.getConnection();
        try (var preparedStatement = (OraclePreparedStatement) connection.prepareStatement(SQL_UPDATE_LIST_IMAGE)) {
            preparedStatement.setString(1, imageListDTO.getName());
            preparedStatement.setInt(2, imageListDTO.getPosition());
            preparedStatement.setORAData(3, imageListDTO.getOrdImage());
            preparedStatement.setInt(4, imageListDTO.getId());
            preparedStatement.execute();
        }
        saveListRepository.UpdateStillImageData(connection, imageListDTO.getId());
        connection.commit();
    }
}
