package com.project.BL.Repositories;

import com.project.BL.Base.*;
import com.project.BL.DTOs.Lists.*;
import oracle.jdbc.*;
import oracle.spatial.geometry.*;

import java.sql.*;

public class SaveListRepository {

    private final String SQL_UPDATE_LIST_FOLDER = "UPDATE TABLE_FOLDER SET FolderName = ? WHERE FolderId = ?";
    private final String SQL_UPDATE_LIST_GEOMETRY = "UPDATE TABLE_GEOMETRY SET GeometryName = ?, SpatialDataObject = ? WHERE GeometryId = ?";
    private final String SQL_UPDATE_LIST_IMAGE = "UPDATE TABLE_IMAGE SET ImageName = ?, ImagePosition = ?, Image = ? Where ImageId = ?";
    private final String SQL_UPDATE_STILLIMAGE = "UPDATE TABLE_IMAGE i SET i.image_si = SI_StillImage(i.image.getContent()) WHERE i.ImageId = ?";
    private final String SQL_UPDATE_STILLIMAGE_META = "UPDATE TABLE_IMAGE SET image_ac = SI_AverageColor(image_si), image_ch = SI_ColorHistogram(image_si), image_pc = SI_PositionalColor(image_si), image_tx = SI_Texture(image_si) WHERE ImageId = ?";


    private final Connector connector = new Connector();

    public void Save(FolderListDTO folderListDTO) throws SQLException {
        var connection = connector.getConnection();
        try (var preparedStatement = (OraclePreparedStatement) connection.prepareStatement(SQL_UPDATE_LIST_FOLDER)) {
            preparedStatement.setString(1, folderListDTO.getName());
            preparedStatement.setInt(2, folderListDTO.getId());
            preparedStatement.execute();
        }
        connection.commit();
    }

    public void Save(GeometryListDTO geometryListDTO) throws Exception {
        var connection = connector.getConnection();
        try (var preparedStatement = (OraclePreparedStatement) connection.prepareStatement(SQL_UPDATE_LIST_GEOMETRY)) {
            preparedStatement.setString(1, geometryListDTO.getName());
            var obj = JGeometry.storeJS(connection, geometryListDTO.getGeometry());
            preparedStatement.setObject(2, obj);
            preparedStatement.setInt(3, geometryListDTO.getId());
            preparedStatement.execute();
            connection.commit();
        }
    }

    public void Save(ImageListDTO imageList) throws SQLException {
        var connection = connector.getConnection();
        try (var preparedStatement = (OraclePreparedStatement) connection.prepareStatement(SQL_UPDATE_LIST_IMAGE)) {
            preparedStatement.setString(1, imageList.getName());
            preparedStatement.setInt(2, imageList.getPosition());
            preparedStatement.setORAData(3, imageList.getOrdImage());
            preparedStatement.setInt(4, imageList.getId());
            preparedStatement.execute();
        }
        UpdateStillImageData(connection, imageList.getId());
        connection.commit();
    }

    public void UpdateStillImageData(Connection connection, int imageId) throws SQLException {
        try {
            try (var preparedStatementSi = connection.prepareStatement(SQL_UPDATE_STILLIMAGE)) {
                preparedStatementSi.setInt(1, imageId);
                preparedStatementSi.executeUpdate();
            }
            try (var preparedStatementSiMeta = connection.prepareStatement(SQL_UPDATE_STILLIMAGE_META)) {
                preparedStatementSiMeta.setInt(1, imageId);
                preparedStatementSiMeta.executeUpdate();
            }
        }
        catch (Exception ignored){
        }
    }
}
