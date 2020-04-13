package com.project.BL.DTOs.Lists;

import com.project.BL.DTOs.Base.BaseDTO;
import com.project.BL.DTOs.Base.IFolderDTO;
import com.project.BL.DTOs.Base.IListDTO;

public class FolderListDTO extends BaseDTO implements IListDTO, IFolderDTO {

    private String name;

    public FolderListDTO() {
    }

    public FolderListDTO(int id, String name) {
        super.setId(id);
        this.name = name;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }
}
