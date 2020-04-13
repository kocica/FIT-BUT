package com.project.BL.DTOs.Details;

import com.project.BL.DTOs.Base.*;
import com.project.BL.DTOs.Lists.*;
import oracle.ord.im.*;

public class ImageDetailDTO extends BaseDTO implements IDetailDTO, IImageDTO {

    private String name;
    private int position;
    private OrdImage ordImage;
    private FolderListDTO parent;
    private GeometryListDTO geometry;

    public ImageDetailDTO() {
    }

    public ImageDetailDTO(int id, String name, int position, OrdImage ordImage, FolderListDTO parent, GeometryListDTO geometry) {
        super.setId(id);
        this.name = name;
        this.position = position;
        this.ordImage = ordImage;
        this.parent = parent;
        this.geometry = geometry;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getPosition() {
        return position;
    }

    public void setPosition(int position) {
        this.position = position;
    }

    public OrdImage getOrdImage() {
        return ordImage;
    }

    public void setOrdImage(OrdImage ordImage) {
        this.ordImage = ordImage;
    }

    public FolderListDTO getParent() {
        return parent;
    }

    public void setParent(FolderListDTO parent) {
        this.parent = parent;
    }

    public GeometryListDTO getGeometry() {
        return geometry;
    }

    public void setGeometry(GeometryListDTO geometry) {
        this.geometry = geometry;
    }
}