package com.project.BL.Repositories;

import com.project.BL.Base.*;
import com.project.BL.Exceptions.*;

import java.sql.*;

public class DeleteRepository {

    private final Connector connector = new Connector();

    private final String SQL_DELETE_FOLDER_BY_ID = "DELETE FROM TABLE_FOLDER WHERE FolderID = ?";
    private final String SQL_DELETE_GEOMETRY_BY_ID = "DELETE FROM TABLE_GEOMETRY WHERE GeometryID = ?";
    private final String SQL_DELETE_IMAGE_BY_ID = "DELETE FROM TABLE_IMAGE WHERE ImageID = ?";

    public void DeleteFolderById(int id) throws SQLException, NotFoundException {
        var connection = connector.getConnection();
        try (var preparedStatement = connection.prepareStatement(SQL_DELETE_FOLDER_BY_ID)) {
            preparedStatement.setInt(1, id);
            if (preparedStatement.executeUpdate() == 0) {
                throw new NotFoundException();
            }
        }
        connection.commit();
    }

    public void DeleteGeometryById(int id) throws SQLException, NotFoundException {
        var connection = connector.getConnection();
        try (var preparedStatement = connection.prepareStatement(SQL_DELETE_GEOMETRY_BY_ID)) {
            preparedStatement.setInt(1, id);
            if (preparedStatement.executeUpdate() == 0) {
                throw new NotFoundException();
            }
        }
        connection.commit();
    }

    public void DeleteImageById(int id) throws SQLException, NotFoundException {
        var connection = connector.getConnection();
        try (var preparedStatement = connection.prepareStatement(SQL_DELETE_IMAGE_BY_ID)) {
            preparedStatement.setInt(1, id);
            if (preparedStatement.executeUpdate() == 0) {
                throw new NotFoundException();
            }
        }
        connection.commit();
    }
}
