package com.project.APP;

import com.project.BL.*;
import javafx.event.*;
import javafx.fxml.*;
import javafx.scene.*;
import javafx.scene.control.*;
import javafx.stage.*;

import java.io.*;

public class Login {

    @FXML
    private TextField urlField;
    @FXML
    private TextField userField;
    @FXML
    private PasswordField passwordField;

    private CrudFacade crudFacade = new CrudFacade();

    @FXML
    private javafx.scene.control.Button loginButton;

    @FXML
    private void login(ActionEvent actionEvent) throws Exception {
        crudFacade.SetConnection(urlField.getText(), userField.getText(), passwordField.getText());
        switchWindow();
    }

    @FXML
    private void loginAndInitialize(ActionEvent actionEvent) throws Exception {
        crudFacade.SetConnection(urlField.getText(), userField.getText(), passwordField.getText());
        crudFacade.InitializeDB();
        switchWindow();
    }

    private void switchWindow() throws IOException {
        FXMLLoader fxmlLoader = new FXMLLoader();
        fxmlLoader.setLocation(getClass().getResource("application.fxml"));
        Scene scene = new Scene(fxmlLoader.load(), 1280, 720);
        Stage stage = new Stage();
        stage.setTitle("UPA application");
        stage.setScene(scene);
        stage.show();
        stage = (Stage) loginButton.getScene().getWindow();
        stage.close();
    }
}
