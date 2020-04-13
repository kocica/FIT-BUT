package com.project.BL.Repositories;

import com.project.BL.Base.*;
import com.project.BL.DTOs.Lists.*;
import oracle.jdbc.*;
import oracle.spatial.geometry.*;

import java.sql.*;

public class AnalyticFunctionsRepository {

    private final Connector connector = new Connector();

    public float CalculateImagesDistance(ImageListDTO firtImageList, ImageListDTO secondImageList) throws SQLException {
        var connection = connector.getConnection();
        var sql = "{ ? = call calc_img_distance(?,?) }";
        var statement = connection.prepareCall(sql);
        statement.setString(2, String.valueOf(firtImageList.getId()));
        statement.setString(3, String.valueOf(secondImageList.getId()));
        statement.registerOutParameter(1, Types.FLOAT);
        statement.execute();
        var result = statement.getFloat(1);
        if (result == -1) throw new SQLException("SQL exception occurred during function execution.");
        return result;
    }

    public JGeometry ConnectImagesPoints(FolderListDTO folderList) throws SQLException {
        var connection = connector.getConnection();
        var sql = "{ ? = call connect_img_points(?) }";
        var statement = connection.prepareCall(sql);
        statement.setString(2, String.valueOf(folderList.getId()));
        statement.registerOutParameter(1, OracleTypes.STRUCT, "MDSYS.SDO_GEOMETRY");
        statement.execute();
        var obj = (Struct) statement.getObject(1);
        if (obj == null) return null;
        return JGeometry.loadJS(obj);
    }

    public float CalculateFolderArea(FolderListDTO folderList) throws SQLException {
        var connection = connector.getConnection();
        var sql = "{ ? = call calc_folder_area(?) }";
        var statement = connection.prepareCall(sql);
        statement.setString(2, String.valueOf(folderList.getId()));
        statement.registerOutParameter(1, Types.FLOAT);
        statement.execute();
        var result = statement.getFloat(1);
        if (result == -1) throw new SQLException("SQL exception occurred during function execution.");
        return result;
    }

    public JGeometry CalculateFoldersConvexHull(FolderListDTO folderList) throws SQLException {
        var connection = connector.getConnection();
        var sql = "{ ? = call calc_folder_convex_hull(?) }";
        var statement = connection.prepareCall(sql);
        statement.setString(2, String.valueOf(folderList.getId()));
        statement.registerOutParameter(1, OracleTypes.STRUCT, "MDSYS.SDO_GEOMETRY");
        statement.execute();
        var obj = (Struct) statement.getObject(1);
        if (obj == null) return null;
        return JGeometry.loadJS(obj);
    }

    public Float CalculateFoldersIntersection(FolderListDTO firstFolderList, FolderListDTO secondFolderList) throws SQLException {
        var connection = connector.getConnection();
        var sql = "{ ? = call calc_folders_intersection(?, ?) }";
        var statement = connection.prepareCall(sql);
        statement.setString(2, String.valueOf(firstFolderList.getId()));
        statement.setString(3, String.valueOf(secondFolderList.getId()));
        statement.registerOutParameter(1, Types.JAVA_OBJECT);
        statement.execute();
        var result = statement.getFloat(1);
        if (result == -1) throw new SQLException("SQL exception occurred during function execution.");
        return result;
    }

    public Float CalculateAverageDistance(FolderListDTO folderList) throws SQLException {
        var connection = connector.getConnection();
        var sql = "{ ? = call calc_avg_distance(?) }";
        var statement = connection.prepareCall(sql);
        statement.setString(2, String.valueOf(folderList.getId()));
        statement.registerOutParameter(1, Types.FLOAT);
        statement.execute();
        var result = statement.getFloat(1);
        if (result == -1) throw new SQLException("SQL exception occurred during function execution.");
        return result;
    }

    public Float CalculateMaxDistance(FolderListDTO folderList) throws SQLException {
        var connection = connector.getConnection();
        var sql = "{ ? = call calc_max_distance(?) }";
        var statement = connection.prepareCall(sql);
        statement.setString(2, String.valueOf(folderList.getId()));
        statement.registerOutParameter(1, Types.FLOAT);
        statement.execute();
        var result = statement.getFloat(1);
        if (result == -1) throw new SQLException("SQL exception occurred during function execution.");
        return result;
    }

    public Float CalculateMinDistance(FolderListDTO folderList) throws SQLException {
        var connection = connector.getConnection();
        var sql = "{ ? = call calc_min_distance(?) }";
        var statement = connection.prepareCall(sql);
        statement.setString(2, String.valueOf(folderList.getId()));
        statement.registerOutParameter(1, Types.FLOAT);
        statement.execute();
        var result = statement.getFloat(1);
        if (result == -1) throw new SQLException("SQL exception occurred during function execution.");
        return result;
    }

    public Float CalculateOverallDistance(FolderListDTO folderList) throws SQLException {
        var connection = connector.getConnection();
        var sql = "{ ? = call calc_overall_distance(?) }";
        var statement = connection.prepareCall(sql);
        statement.setString(2, String.valueOf(folderList.getId()));
        statement.registerOutParameter(1, Types.JAVA_OBJECT);
        statement.execute();
        var result = statement.getFloat(1);
        if (result == -1) throw new SQLException("SQL exception occurred during function execution.");
        return result;
    }
}
