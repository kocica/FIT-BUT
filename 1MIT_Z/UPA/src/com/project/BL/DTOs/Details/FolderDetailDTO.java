package com.project.BL.DTOs.Details;

import com.project.BL.DTOs.Base.*;
import com.project.BL.DTOs.Lists.*;

import java.util.*;

public class FolderDetailDTO extends BaseDTO implements IDetailDTO, IFolderDTO {

    private String name;
    private FolderListDTO parentFolder;
    private Collection<FolderListDTO> childFolders = new ArrayList<>();
    private Collection<ImageListDTO> images = new ArrayList<>();

    public FolderDetailDTO() {
    }

    public FolderDetailDTO(int id, String name, FolderListDTO parentFolder) {
        super.setId(id);
        this.name = name;
        this.parentFolder = parentFolder;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public FolderListDTO getParentFolder() {
        return parentFolder;
    }

    public void setParentFolder(FolderListDTO parentFolder) {
        this.parentFolder = parentFolder;
    }

    public Collection<FolderListDTO> getChildFolders() {
        return childFolders;
    }

    public Collection<ImageListDTO> getImages() {
        return images;
    }
}
