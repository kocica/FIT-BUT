package com.project.BL.Tests;

import com.project.BL.Exceptions.*;
import com.project.BL.Repositories.*;
import org.junit.jupiter.api.*;

import java.sql.*;

class AnalyticFunctionsRepositoryTest {

    private final GetListByIdRepository getListByIdRepository = new GetListByIdRepository();
    private final AnalyticFunctionsRepository analyticFunctionsRepositorySUT = new AnalyticFunctionsRepository();

    @Test
    void calculateDistance() throws NotFoundException, SQLException {
        var firstImageList = getListByIdRepository.GetImage(1);
        var secondImageList = getListByIdRepository.GetImage(2);
        analyticFunctionsRepositorySUT.CalculateImagesDistance(firstImageList, secondImageList);
    }

    @Test
    void connectImagesPoints() throws NotFoundException, SQLException {
        var folderList = getListByIdRepository.GetFolder(1);
        analyticFunctionsRepositorySUT.ConnectImagesPoints(folderList);
    }
}