package com.project.BL.Tests;

import com.project.BL.Exceptions.*;
import com.project.BL.Repositories.*;
import org.junit.jupiter.api.*;

import java.io.*;
import java.nio.file.*;
import java.sql.*;

public class MultimediaFunctionsRepositoryTest {

    private final GetListByIdRepository getListByIdRepository = new GetListByIdRepository();
    private final GetDetailByIdRepository getDetailByIdRepository = new GetDetailByIdRepository();
    private final MultimediaFunctionsRepository multimediaFunctionsRepository = new MultimediaFunctionsRepository();

    @Test
    void rotateImage() throws NotFoundException, SQLException, IOException {
        var firstImageList = getListByIdRepository.GetImage(1);
        var degrees = 90;

        // Rotate image in the DB
        multimediaFunctionsRepository.RotateImage(firstImageList, degrees);

        // Get rotated image
        var rotatedImage = getDetailByIdRepository.GetImage(firstImageList.getId());

        // Check its rotated
        assert rotatedImage.getOrdImage().getWidth() == 266 : "Image is NOT rotated";
        assert rotatedImage.getOrdImage().getHeight() == 720 : "Image is NOT rotated";

        // Save rotated image
        rotatedImage.getOrdImage().getDataInFile(Paths.get(".").toAbsolutePath().normalize().toString() + "\\src\\main\\src\\com\\project\\BL\\Tests\\media\\rotatedImage.jpg");
    }
}
