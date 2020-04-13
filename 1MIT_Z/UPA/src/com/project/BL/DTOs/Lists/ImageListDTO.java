package com.project.BL.DTOs.Lists;

import com.project.BL.DTOs.Base.*;
import oracle.ord.im.*;

public class ImageListDTO extends BaseDTO implements IListDTO, IImageDTO {

    private String name;
    private int position;
    private OrdImage ordImage;

    public ImageListDTO() {
    }

    public ImageListDTO(int id, String name, int position, OrdImage ordImage) {
        super.setId(id);
        this.name = name;
        this.position = position;
        this.ordImage = ordImage;
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
}
