package com.project.BL.Base;

import com.project.BL.DTOs.Details.*;
import com.project.BL.DTOs.Lists.*;

public class Mapper {

    public FolderListDTO MapTo(FolderDetailDTO folderDetail) {
        var folderList = new FolderListDTO();
        folderList.setId(folderDetail.getId());
        folderList.setName(folderDetail.getName());
        return folderList;
    }

    public GeometryListDTO MapTo(GeometryDetailDTO geometryDetail) {
        var geometryList = new GeometryListDTO();
        geometryList.setId(geometryDetail.getId());
        geometryList.setName(geometryDetail.getName());
        geometryList.setGeometry(geometryDetail.getGeometry());
        return geometryList;
    }

    public ImageListDTO MapTo(ImageDetailDTO imageDetail) {
        var imageList = new ImageListDTO();
        imageList.setId(imageDetail.getId());
        imageList.setName(imageDetail.getName());
        imageList.setPosition(imageDetail.getPosition());
        imageList.setOrdImage(imageDetail.getOrdImage());
        return imageList;
    }
}
