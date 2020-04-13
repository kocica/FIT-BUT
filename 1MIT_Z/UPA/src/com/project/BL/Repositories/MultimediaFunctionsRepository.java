package com.project.BL.Repositories;

import com.project.BL.Base.*;
import com.project.BL.DTOs.Lists.*;
import com.project.BL.Exceptions.*;
import oracle.jdbc.*;
import oracle.ord.im.*;

import java.sql.*;

public class MultimediaFunctionsRepository {

    private final Connector connector = new Connector();

    private static final String SQL_SIMILAR_IMAGE = "SELECT dst.ImageId, dst.ImageName, dst.ImagePosition, dst.Image, SI_ScoreByFtrList(new SI_FeatureList(src.image_ac,?,src.image_ch,?,src.image_pc,?,src.image_tx,?),dst.image_si) AS similarity FROM table_image src, table_image dst WHERE (src.imageid = ?) AND (src.imageid <> dst.imageid) ORDER BY similarity";

    public ImageListDTO FindMostSimilarImage(ImageListDTO imageListDTO, double weightAC, double weightCH, double weightPC, double weightTX) throws SQLException, NotFoundException {
        var connection = connector.getConnection();
        try (var preparedStatement = connection.prepareStatement(SQL_SIMILAR_IMAGE)) {
            preparedStatement.setDouble(1, weightAC);
            preparedStatement.setDouble(2, weightCH);
            preparedStatement.setDouble(3, weightPC);
            preparedStatement.setDouble(4, weightTX);
            preparedStatement.setInt(5, imageListDTO.getId());
            try (var resultSet = preparedStatement.executeQuery()) {
                if (resultSet.next()) {
                    final var oracleResultSet = (OracleResultSet) resultSet;
                    final var ordImage = (OrdImage) oracleResultSet.getORAData(4, OrdImage.getORADataFactory());
                    return new ImageListDTO(resultSet.getInt(1), resultSet.getString(2), resultSet.getInt(3), ordImage);
                } else {
                    throw new NotFoundException();
                }
            }
        }
    }

    public void RotateImage(ImageListDTO imageListDTO, int degrees) throws SQLException {
        var connection = connector.getConnection();
        var sql = "{ call rotate_image(?, ?) }";
        var statement = connection.prepareCall(sql);
        statement.setInt(1, imageListDTO.getId());
        statement.setInt(2, degrees);
        statement.execute();
    }

}
