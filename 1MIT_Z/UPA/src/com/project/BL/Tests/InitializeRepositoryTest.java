package com.project.BL.Tests;

import com.project.BL.Exceptions.*;
import com.project.BL.Repositories.*;
import org.junit.jupiter.api.*;

import java.sql.*;

class InitializeRepositoryTest {

    private final InitializeRepository initializeRepository = new InitializeRepository();
    private final GetListByIdRepository getListByIdRepository = new GetListByIdRepository();

    @Test
    void initializeFolder() throws NotFoundException, SQLException {
        var parent = getListByIdRepository.GetFolder(1);
        var folder = initializeRepository.InitializeFolder(parent);
        System.out.println(folder.getId());
    }

    @Test
    void initializeGeometry() throws SQLException {
        var geometry = initializeRepository.InitializeGeometry();
        System.out.println(geometry.getId());
    }

    @Test
    void initializeImage() throws NotFoundException, SQLException {
        var image = initializeRepository.InitializeImage(getListByIdRepository.GetFolder(1));
        System.out.println(image.getId());
    }
}