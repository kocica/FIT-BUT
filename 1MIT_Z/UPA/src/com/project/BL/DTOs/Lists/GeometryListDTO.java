package com.project.BL.DTOs.Lists;

import com.project.BL.DTOs.Base.BaseDTO;
import com.project.BL.DTOs.Base.IGeometryDTO;
import com.project.BL.DTOs.Base.IListDTO;
import oracle.spatial.geometry.JGeometry;

public class GeometryListDTO extends BaseDTO implements IListDTO, IGeometryDTO {

    private String name;
    private JGeometry geometry;

    public GeometryListDTO() {
    }

    public GeometryListDTO(int id, String name, JGeometry geometry) {
        super.setId(id);
        this.name = name;
        this.geometry = geometry;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public JGeometry  getGeometry() {
        return geometry;
    }

    public void setGeometry(JGeometry  geometry) {
        this.geometry = geometry;
    }
}
