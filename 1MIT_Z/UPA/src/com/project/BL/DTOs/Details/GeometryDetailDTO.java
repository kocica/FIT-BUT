package com.project.BL.DTOs.Details;

import com.project.BL.DTOs.Base.*;
import com.project.BL.DTOs.Lists.*;
import oracle.spatial.geometry.*;

public class GeometryDetailDTO extends BaseDTO implements IDetailDTO, IGeometryDTO {

    private String name;
    private JGeometry geometry;
    private ImageListDTO image;

    public GeometryDetailDTO() {
    }

    public GeometryDetailDTO(int id, String name, JGeometry geometry, ImageListDTO image) {
        super.setId(id);
        this.name = name;
        this.geometry = geometry;
        this.image = image;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public JGeometry getGeometry() {
        return geometry;
    }

    public void setGeometry(JGeometry geometry) {
        this.geometry = geometry;
    }

    public ImageListDTO getImage() {
        return image;
    }

    public void setImage(ImageListDTO image) {
        this.image = image;
    }
}