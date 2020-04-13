package com.project.APP;

import com.project.BL.CrudFacade;
import com.project.BL.DTOs.Details.FolderDetailDTO;
import com.project.BL.DTOs.Details.GeometryDetailDTO;
import com.project.BL.DTOs.Details.ImageDetailDTO;
import com.project.BL.DTOs.Lists.FolderListDTO;
import com.project.BL.DTOs.Lists.GeometryListDTO;
import com.project.BL.DTOs.Lists.ImageListDTO;
import com.project.BL.Exceptions.NotFoundException;
import javafx.embed.swing.SwingFXUtils;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.scene.Group;
import javafx.scene.Node;
import javafx.scene.control.*;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseButton;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.*;
import javafx.scene.paint.Color;
import javafx.scene.paint.ImagePattern;
import javafx.scene.shape.Circle;
import javafx.scene.shape.Rectangle;
import javafx.stage.FileChooser;
import oracle.spatial.geometry.JGeometry;

import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.*;
import java.nio.file.Paths;
import java.sql.SQLException;
import java.util.Optional;

public class Application {

    private final CrudFacade crudFacade = new CrudFacade();

    int imageID = 0;
    String moveID = new String();

    @FXML TextField x;
    @FXML TextField y;
    @FXML TextField w;
    @FXML TextField h;
    @FXML TextField r;

    @FXML TextField ac;
    @FXML TextField ch;
    @FXML TextField pc;
    @FXML TextField tx;

    @FXML
    private ImageView imageView;

    @FXML
    private ImageView imageViewRes;

    @FXML
    private Group mapGroup;

    @FXML
    private void initialize() throws NotFoundException, SQLException, FileNotFoundException {
        Rectangle rect = new Rectangle(0, 0, 600, 600);

        Image image = new Image("file:" + Paths.get(".").toAbsolutePath().normalize().toString() + "\\src\\com\\project\\BL\\Tests\\media\\Map.png");
        ImagePattern imagePattern = new ImagePattern(image);
        rect.setFill(imagePattern);

        rect.setOnMouseClicked(event ->
        {
            if (event.getButton() == MouseButton.SECONDARY)
            {
                if(moveID != "")
                {
                    Node n = mapGroup.lookup("#" + moveID);
                    n.setTranslateX(event.getSceneX());
                    n.setTranslateY(event.getSceneY());
                }
            }
        });
        mapGroup.getChildren().add(rect);

        loadFolderStructure();
        loadMap();
    }

    @FXML
    private void loadMap() throws SQLException, NotFoundException {
        var allGeometries = crudFacade.GetAllGeometries();

        for (var geometry : allGeometries)
        {
            if(geometry.getGeometry().getType() == JGeometry.GTYPE_POLYGON && geometry.getGeometry().getElemInfo()[2] == 1)
            {
                createNewRectangle(geometry);
            }
            else if(geometry.getGeometry().isPoint())
            {
                createNewPoint(geometry);
            }
            else if(geometry.getGeometry().getType() == JGeometry.GTYPE_POLYGON && geometry.getGeometry().getElemInfo()[2] == 4)
            {
                createNewCircle(geometry);
            }
        }
    }

    private void createNewRectangle(GeometryListDTO geometry)
    {
        var points = geometry.getGeometry().getOrdinatesArray();
        Rectangle rect = new Rectangle(points[0], points[1], points[2] - points[0], points[5] - points[1]);
        rect.setFill(Color.BLUE);
        rect.setId(Integer.toString(geometry.getId()));
        rect.toFront();

        rect.setOnMouseClicked(event ->
        {
            if (event.getButton() == MouseButton.SECONDARY)
            {
                rect.setFill(Color.ORANGE);
                moveID = rect.getId();
            }
        });

        mapGroup.getChildren().add(rect);
    }

    private void createNewCircle(GeometryListDTO geometry)
    {
        var points = geometry.getGeometry().getOrdinatesArray();

        Circle circle = new Circle(points[0], points[5], points[4] - points[0]);
        circle.setFill(Color.RED);
        circle.setId(Integer.toString(geometry.getId()));
        circle.toFront();

        circle.setOnMouseClicked(event ->
        {
            if (event.getButton() == MouseButton.SECONDARY)
            {
                circle.setFill(Color.ORANGE);
                moveID = circle.getId();
            }
        });

        mapGroup.getChildren().add(circle);
    }

    private void createNewPoint(GeometryListDTO geometry) throws NotFoundException, SQLException {
        var point = geometry.getGeometry().getPoint();
        var geomDetail = crudFacade.ToDetail(geometry);

        Circle circle = new Circle(point[0], point[1], 20);

        circle.setId(Integer.toString(geomDetail.getId()));
        circle.toFront();

        Image image = new Image("file:" + Paths.get(".").toAbsolutePath().normalize().toString() + "\\src\\com\\project\\BL\\Tests\\media\\MapMarker.png");
        ImagePattern imagePattern = new ImagePattern(image);
        circle.setFill(imagePattern);

        circle.setOnMouseClicked(new EventHandler<MouseEvent>()
        {
            @Override
            public void handle(MouseEvent t)
            {
                if (t.getButton() == MouseButton.SECONDARY) {
                    circle.setStyle("-fx-stroke: #ff6a0f; -fx-stroke-width: 5;");
                    moveID = circle.getId();
                }
                else {
                    try {
                        if(geomDetail.getImage() != null)
                        {
                            var img = crudFacade.GetImageListById(geomDetail.getImage().getId());

                            imageID = img.getId();

                            BufferedImage bufferedImg = ImageIO.read(new ByteArrayInputStream(img.getOrdImage().getDataInByteArray()));
                            Image imageToShow = SwingFXUtils.toFXImage(bufferedImg, null);

                            imageView.setImage(imageToShow);
                            imageViewRes.setImage(null);
                        }
                        else
                        {
                            // Get into block below
                            throw new Exception();
                        }
                    } catch (Exception e) {
                        e.printStackTrace();
                        imageView.setImage(null);
                        imageViewRes.setImage(null);
                        imageID = 0;
                    }
                }
            }
        });

        mapGroup.getChildren().add(circle);
    }

    @FXML
    private void deleteGeometryObject() throws NotFoundException, SQLException
    {
        GeometryListDTO geom;

        if (moveID != "")
        {
            imageID = 0;

            // Delete from the DB
            geom = crudFacade.GetGeometryListById(Integer.valueOf(moveID));
            crudFacade.Delete(geom);

            // Remove from UI
            Node n = mapGroup.lookup("#" + moveID);
            mapGroup.getChildren().remove(n);
            moveID = "";
        }
    }

    @FXML
    private void createNewGeometryAction() throws Exception {
        int xCoord  = x.getText().equals("") ?  0 : Integer.valueOf(x.getText());
        int yCoord  = y.getText().equals("") ?  0 : Integer.valueOf(y.getText());
        int width   = w.getText().equals("") ?  0 : Integer.valueOf(w.getText());
        int height  = h.getText().equals("") ?  0 : Integer.valueOf(h.getText());
        int radius  = r.getText().equals("") ?  0 : Integer.valueOf(r.getText());
        String pathToImage = "";

        GeometryDetailDTO geom = crudFacade.InitializeGeometry();

        if(width != 0 && height != 0) // Rectangular
        {
            crudFacade.SavePolygon(geom, xCoord, yCoord, width, height);
            createNewRectangle(crudFacade.ToList(geom));
        }
        else if(radius != 0)          // Circle
        {
            crudFacade.SaveCircle(geom, xCoord, yCoord, radius);
            createNewCircle(crudFacade.ToList(geom));
        }
        else                          // Point
        {
            crudFacade.SavePoint(geom, xCoord, yCoord);

            FileChooser fileChooser = new FileChooser();
            File selectedFile = fileChooser.showOpenDialog(mapGroup.getScene().getWindow());

            FolderListDTO fodlerListDTO = new FolderListDTO();
            fodlerListDTO.setId(openedFolder.getId());

            var image = crudFacade.InitializeImage(fodlerListDTO);
            image.setGeometry(crudFacade.ToList(geom));
            image.getOrdImage().loadDataFromFile(selectedFile.getAbsolutePath());
            image.setName(selectedFile.getName());

            crudFacade.Save(image);
            createNewPoint(crudFacade.ToList(geom));

            geom.setImage(crudFacade.ToList(image));
            crudFacade.Save(geom);

            // Show in GUI
            createNewPoint(crudFacade.ToList(geom));
        }
    }

    @FXML
    private void deleteGeometryImage() throws Exception {
        ImageListDTO image;
        GeometryDetailDTO geom;

        if (moveID != "")
        {
            imageID = 0;

            // Retrieve geometry detail from the DB
            geom = crudFacade.GetGeometryDetailById(Integer.valueOf(moveID));

            // In case there is no image to delete, skip
            if(geom.getImage() == null)
                return;

            // Retrieve image list from the DB
            image = crudFacade.GetImageListById(geom.getImage().getId());

            // Delete image from the DB
            crudFacade.Delete(image);

            // Set represented image to NULL for given geometry
            geom.setImage(null);
            crudFacade.Save(geom);

            // Remove marked from UI
            Circle c = (Circle)mapGroup.lookup("#" + moveID);
            c.setStyle("-fx-stroke-width: 0;");
            moveID = "";
        }
    }

    private void loadFolderStructure() throws SQLException, NotFoundException, FileNotFoundException {
        var allFolders = crudFacade.GetAllFolders();
        for (var folder: allFolders) {
            var detail = crudFacade.GetFolderDetailById(folder.getId());
            if(detail.getParentFolder() == null){
                setOpenedFolder(detail);
                break;
            }
        }
    }

    @FXML
    private void rotateImage() throws SQLException, NotFoundException, IOException
    {
        if(imageID == 0)
        {
            return;
        }

        ImageListDTO imageListDTO = new ImageListDTO();
        imageListDTO.setId(imageID);

        crudFacade.RotateImage(imageListDTO, 90);

        ImageDetailDTO imageDetailDTO = crudFacade.GetImageDetailById(imageID);

        BufferedImage bufferedImg = ImageIO.read(new ByteArrayInputStream(imageDetailDTO.getOrdImage().getDataInByteArray()));
        Image imageToShow = SwingFXUtils.toFXImage(bufferedImg, null);

        imageViewRes.setImage(imageToShow);
    }

    @FXML
    private void findSimilar() throws Exception
    {
        if(imageID == 0)
        {
            return;
        }

        ImageListDTO imageListDTO = new ImageListDTO();
        imageListDTO.setId(imageID);

        var similarImg = crudFacade.FindMostSimilarImage(imageListDTO, Double.parseDouble(ac.getText()),
                Double.parseDouble(ch.getText()), Double.parseDouble(pc.getText()), Double.parseDouble(tx.getText()));

        ImageDetailDTO imageDetailDTO = crudFacade.ToDetail(similarImg);

        BufferedImage bufferedImg = ImageIO.read(new ByteArrayInputStream(imageDetailDTO.getOrdImage().getDataInByteArray()));
        Image imageToShow = SwingFXUtils.toFXImage(bufferedImg, null);

        imageViewRes.setImage(imageToShow);
    }

    @FXML
    private void loadImage() throws Exception
    {
        FileChooser fileChooser = new FileChooser();
        File selectedFile = fileChooser.showOpenDialog(mapGroup.getScene().getWindow());

        var image = crudFacade.GetImageListById(1);
        image.getOrdImage().loadDataFromFile(selectedFile.getAbsolutePath());

        BufferedImage bufferedImg = ImageIO.read(new ByteArrayInputStream(image.getOrdImage().getDataInByteArray()));
        Image imageToShow = SwingFXUtils.toFXImage(bufferedImg, null);

        imageView.setImage(imageToShow);
    }

    @FXML
    private Label areaLabel;

    @FXML
    private Label averageLabel;

    @FXML
    private Label maxLabel;

    @FXML
    private Label minLabel;

    @FXML
    private ListView folderStructureListView;

    private FolderDetailDTO openedFolder;

    public void setOpenedFolder(FolderDetailDTO openedFolder) throws FileNotFoundException, SQLException, NotFoundException {
        this.openedFolder = openedFolder;
        folderStructureListView.getItems().clear();
        for (var folder: openedFolder.getChildFolders()) {
            GridPane grid = CreateFolderPane(folder);
            folderStructureListView.getItems().add(grid);
        }

        for (var image: openedFolder.getImages()) {
            GridPane grid = CreateImagePane(image);
            folderStructureListView.getItems().add(grid);

            ImageDetailDTO imgDetail = crudFacade.ToDetail(image);
            Circle c = (Circle)mapGroup.lookup("#" + Integer.toString(imgDetail.getGeometry().getId()));
            c.setStyle("-fx-stroke: #ff3deb; -fx-stroke-width: 5;");
        }

        var folderList = crudFacade.ToList(openedFolder);
        try {
            areaLabel.setText("Area: "+ crudFacade.CalculateFolderArea(folderList));
        }
        catch (SQLException e){
            areaLabel.setText("Area: UNDEFINED");
        }
        try {
            averageLabel.setText("Average: "+ crudFacade.CalculateAverageDistance(folderList));
        }
        catch (SQLException e){
            averageLabel.setText("Average: UNDEFINED");
        }
        try {
            maxLabel.setText("Max: "+ crudFacade.CalculateMaxDistance(folderList));
        }
        catch (SQLException e){
            maxLabel.setText("Max: UNDEFINED");
        }
        try {
            minLabel.setText("Min: "+ crudFacade.CalculateMinDistance(folderList));
        }
        catch (SQLException e){
            minLabel.setText("Min: UNDEFINED");
        }
    }

    private GridPane CreateImagePane(ImageListDTO image) throws FileNotFoundException {
        var grid = new GridPane();
        grid.addColumn(1);
        grid.addColumn(2);
        grid.addColumn(3);
        grid.addColumn(4);

        var input = new FileInputStream("src/com/project/APP/media/file-image.png");
        var icon = new Image(input);
        ImageView imageView = new ImageView(icon);

        var label = new Label(image.getName());
        label.setPrefWidth(260);

        var renameButton = new Button("Rename");
        renameButton.setPrefWidth(100);
        renameButton.setOnAction(click -> {
            TextInputDialog dialog = new TextInputDialog(image.getName());
            dialog.setTitle("Rename Dialog");
            dialog.setHeaderText("Image rename");
            dialog.setContentText("Please enter new name:");
            Optional<String> result = dialog.showAndWait();
            if (result.isPresent()){
                image.setName(result.get());
                label.setText(image.getName());
                try {
                    crudFacade.Save(image);
                } catch (SQLException e) {
                    e.printStackTrace();
                }
            }
        });

        var deleteButton = new Button("Delete");
        deleteButton.setPrefWidth(100);
        deleteButton.setOnAction(click -> {
            try {
                crudFacade.Delete(image);
            } catch (SQLException e) {
                e.printStackTrace();
            } catch (NotFoundException e) {
                e.printStackTrace();
            }
            folderStructureListView.getItems().remove(grid);
        });

        grid.add(imageView, 1, 0);
        grid.add(label, 2, 0);
        grid.add(renameButton, 3,0);
        grid.add(deleteButton, 4,0);
        return grid;
    }

    private GridPane CreateFolderPane(FolderListDTO folder) throws FileNotFoundException {
        var grid = new GridPane();
        grid.addColumn(1);
        grid.addColumn(2);
        grid.addColumn(3);
        grid.addColumn(4);
        grid.addColumn(5);

        var input = new FileInputStream("src/com/project/APP/media/folder-image.png");
        Image image = new Image(input);
        ImageView imageView = new ImageView(image);

        var label = new Label(folder.getName());
        label.setPrefWidth(160);

        var openButton = new Button("Open");
        openButton.setUserData(folder);
        openButton.setOnAction(click -> {
            var list = (FolderListDTO) openButton.getUserData();
            FolderDetailDTO detail = null;
            try {
                detail = crudFacade.ToDetail(list);
                setOpenedFolder(detail);
            } catch (Exception e) {
                e.printStackTrace();
            }
        });
        openButton.setPrefWidth(100);

        var renameButton = new Button("Rename");
        renameButton.setPrefWidth(100);
        renameButton.setOnAction(click -> {
            TextInputDialog dialog = new TextInputDialog(folder.getName());
            dialog.setTitle("Rename Dialog");
            dialog.setHeaderText("Folder rename");
            dialog.setContentText("Please enter new name:");
            Optional<String> result = dialog.showAndWait();
            if (result.isPresent()){
                folder.setName(result.get());
                label.setText(folder.getName());
                try {
                    crudFacade.Save(folder);
                } catch (SQLException e) {
                    e.printStackTrace();
                }
            }
        });

        var deleteButton = new Button("Delete");
        deleteButton.setPrefWidth(100);
        deleteButton.setOnAction(click -> {
            try {
                crudFacade.Delete(folder);
            } catch (SQLException e) {
                e.printStackTrace();
            } catch (NotFoundException e) {
                e.printStackTrace();
            }
            folderStructureListView.getItems().remove(grid);
        });

        grid.add(imageView, 1, 0);
        grid.add(label, 2, 0);
        grid.add(openButton, 3,0);
        grid.add(renameButton, 4,0);
        grid.add(deleteButton, 5,0);
        return grid;
    }

    @FXML
    private void folderStructureBackClick(ActionEvent actionEvent) throws NotFoundException, SQLException, FileNotFoundException {
        var parentFolder = openedFolder.getParentFolder();
        if(parentFolder != null) setOpenedFolder(crudFacade.GetFolderDetailById(parentFolder.getId()));

        for(var image: crudFacade.GetAllImages())
        {
            ImageDetailDTO imgDetail = crudFacade.ToDetail(image);
            Circle c = (Circle)mapGroup.lookup("#" + Integer.toString(imgDetail.getGeometry().getId()));
            c.setStyle("-fx-stroke-width: 0;");
        }
    }

    @FXML
    private void folderStructureAddFolder(ActionEvent actionEvent) throws SQLException, FileNotFoundException {
        var folder = crudFacade.InitializeFolder(crudFacade.ToList(openedFolder));
        var grid = CreateFolderPane(crudFacade.ToList(folder));
        folderStructureListView.getItems().add(grid);
    }

    @FXML
    public void folderStructureAddImage(ActionEvent actionEvent) throws SQLException, FileNotFoundException {
        var image = crudFacade.InitializeImage(crudFacade.ToList(openedFolder));
        var grid = CreateImagePane(crudFacade.ToList(image));
        folderStructureListView.getItems().add(grid);
    }

}
