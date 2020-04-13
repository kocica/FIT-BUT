package com.project.BL.Tests;

import com.project.BL.Exceptions.*;
import com.project.BL.Repositories.*;
import org.junit.jupiter.api.*;

import java.sql.*;

import static org.junit.jupiter.api.Assertions.*;

class GetListByIdRepositoryTest {

    private final GetListByIdRepository getListByIdRepositorySUT = new GetListByIdRepository();

    @Test
    void getListById() throws NotFoundException, SQLException {
        var folderListDTO = getListByIdRepositorySUT.GetFolder(1);
        assertEquals("Root", folderListDTO.getName(), "Id 1 should be root folder");
    }
}