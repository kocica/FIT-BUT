package com.project.BL.Base;

import com.project.BL.DTOs.Lists.ImageListDTO;
import com.project.BL.Repositories.GetDetailByIdRepository;
import com.project.BL.Repositories.SaveDetailRepository;
import org.flywaydb.core.Flyway;

import java.nio.file.Paths;

public class Installer {

    public void Install() throws Exception {
        Flyway flyway = Flyway.configure().dataSource(Connector.url, Connector.user, Connector.password).load();
        flyway.repair();
        flyway.clean();
        flyway.baseline();
        flyway.migrate();

        InsertImageDB(1,"Charles Bridge","CharlesBridge.jpg");
        InsertImageDB(2,"Castle Karlstejn","Karlstejn.jpeg");
        InsertImageDB(3,"Castle Spilberk","Spilberk.jpg");
    }

    private void InsertImageDB(int ID, String name, String filename) throws Exception {
        GetDetailByIdRepository getDetailByIdRepository = new GetDetailByIdRepository();
        SaveDetailRepository saveDetailRepository = new SaveDetailRepository();

        var image = new ImageListDTO();
        var ordImage = getDetailByIdRepository.GetImage(ID);

        image.setId(ID);
        image.setName(name);
        image.setPosition(ID);
        image.setOrdImage(ordImage.getOrdImage());
        image.getOrdImage().loadDataFromFile(Paths.get(".").toAbsolutePath().normalize().toString() + "\\src\\com\\project\\BL\\Tests\\media\\" + filename);
        image.getOrdImage().setProperties();
        saveDetailRepository.Save(image);
    }
}
