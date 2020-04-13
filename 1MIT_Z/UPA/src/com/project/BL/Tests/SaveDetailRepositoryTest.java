package com.project.BL.Tests;

import com.project.BL.Base.*;
import com.project.BL.DTOs.Details.*;
import com.project.BL.DTOs.Lists.*;
import com.project.BL.Exceptions.*;
import com.project.BL.Repositories.*;
import oracle.spatial.geometry.*;
import org.junit.jupiter.api.*;

import java.io.*;
import java.nio.file.*;
import java.sql.*;

public class SaveDetailRepositoryTest {
    private final SaveDetailRepository saveDetailRepository = new SaveDetailRepository();
    private final GetDetailByIdRepository getDetailByIdRepository = new GetDetailByIdRepository();
    private final GetListByIdRepository getListByIdRepository = new GetListByIdRepository();
    private final Connector connector = new Connector();

    @Test
    void saveFolder() throws NotFoundException, SQLException {
        FolderDetailDTO folder = new FolderDetailDTO();

        var folderId = 5;
        var newFolderName = "TestingFolder";

        // Set folder keys
        folder.setId(folderId);
        folder.setName(newFolderName);
        folder.setParentFolder(new FolderListDTO(1, "Root"));

        // Store (update) folder to the database
        try {
            saveDetailRepository.Save(folder);
        } catch (Exception ex) {
            assert false : "SaveDetailRepository has thrown exception: " + ex.getMessage();
        }

        // Retrieve newly updated folder from the DB
        folder = getDetailByIdRepository.GetFolder(folderId);

        // Compare keys of retrieved folder
        assert folder.getId() == folderId : "Folder IDs doesnt match";
        assert folder.getName().equals(newFolderName) : "Folder names doesnt match";
        assert folder.getParentFolder().getId() == 1 : "Parent folder ID doesnt match";
        assert folder.getParentFolder().getName().equals("Root") : "Parent folder name doesnt match";
        assert folder.getImages().size() == 0 : "Folder image count doesnt match";
        assert folder.getChildFolders().size() == 0 : "Folder child count doesnt match";
    }

    @Test
    void saveGeometry() throws NotFoundException, SQLException {
        GeometryDetailDTO geometry = new GeometryDetailDTO();

        var geometryId = 1;
        var newGeometryName = "TestingGeometry";
        var representsImageId = new ImageListDTO();
        var jGeometry = new JGeometry(40.0, 20.0, 1);

        representsImageId.setId(1);

        // Set geometry keys
        geometry.setId(geometryId);
        geometry.setName(newGeometryName);
        geometry.setGeometry(jGeometry);
        geometry.setImage(representsImageId);

        // Store (update) geometry to the database
        try {
            saveDetailRepository.Save(geometry);
        } catch (Exception ex) {
            assert false : "SaveDetailRepository has thrown exception: " + ex.getMessage();
        }

        // Retrieve newly updated geometry from the DB
        geometry = getDetailByIdRepository.GetGeometry(geometryId);

        // Compare keys of retrieved geometry
        assert geometry.getId() == geometryId : "Geometry ID doesnt match";
        assert geometry.getName().equals(newGeometryName) : "Geometry name doesnt match";
        assert geometry.getImage().getId() == representsImageId.getId() : "Geometry image doesnt match" + geometry.getImage().getId() + " " + representsImageId.getId();
        assert geometry.getGeometry().getType() == JGeometry.GTYPE_POINT : "Geometry has incorrect type";
        assert geometry.getGeometry().getPoint()[0] == 40 : "Geometry X coordinate doesnt match";
        assert geometry.getGeometry().getPoint()[1] == 20 : "Geometry Y coordinate doesnt match";
    }

    @Test
    void saveImage() throws NotFoundException, SQLException, IOException {
        ImageDetailDTO image = new ImageDetailDTO();

        var imageId = 1;
        var newImageName = "TestingImage";
        var newImagePosition = 100;
        var newFolder = new FolderListDTO(5, "TestingFolder");
        var newGeometry = getDetailByIdRepository.GetGeometry(1);
        var ordImage = getDetailByIdRepository.GetImage(imageId);

        // Set image keys
        image.setId(imageId);
        image.setName(newImageName);
        image.setPosition(newImagePosition);
        image.setParent(newFolder);
        image.setGeometry(new GeometryListDTO(newGeometry.getId(), newGeometry.getName(), newGeometry.getGeometry()));
        image.setOrdImage(ordImage.getOrdImage());
        image.getOrdImage().loadDataFromFile(Paths.get(".").toAbsolutePath().normalize().toString() + "\\src\\com\\project\\BL\\Tests\\media\\testingImage.jpg");
        image.getOrdImage().setProperties();

        // Store (update) image to the database
        try {
            saveDetailRepository.Save(image);
        } catch (Exception ex) {
            assert false : "SaveDetailRepository has thrown exception: " + ex.getMessage();
        }

        // Retrieve newly updated image from the DB
        image = getDetailByIdRepository.GetImage(imageId);

        // Compare keys of retrieved image
        assert image.getId() == imageId : "Image ID doesnt match";
        assert image.getName().equals(newImageName) : "Image name doesnt match";
        assert image.getPosition() == newImagePosition : "Image position doesnt match";
        assert image.getGeometry().getId() == newGeometry.getId() : "Image geometry doesnt match";
        assert image.getParent().getId() == newFolder.getId() : "Image folder doesnt match";
        assert image.getOrdImage().getWidth() == 720 : "Image is malformed";
        assert image.getOrdImage().getHeight() == 266 : "Image is malformed";

        // Store image to media folder
        image.getOrdImage().getDataInFile(Paths.get(".").toAbsolutePath().normalize().toString() + "\\src\\com\\project\\BL\\Tests\\media\\testingImage.jpg");
    }
}
