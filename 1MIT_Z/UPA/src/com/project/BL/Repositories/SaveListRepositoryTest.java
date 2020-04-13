package com.project.BL.Repositories;

import com.project.BL.DTOs.Lists.*;
import org.junit.jupiter.api.*;

import java.sql.*;

class SaveListRepositoryTest {

    private final SaveListRepository saveListRepositorySUT = new SaveListRepository();

    @Test
    void SaveFolder() throws SQLException {
        saveListRepositorySUT.Save(new FolderListDTO(2, "Slovakia"));
    }

    @Test
    void SaveGeometry() {
    }

    @Test
    void SaveImage() {
    }
}