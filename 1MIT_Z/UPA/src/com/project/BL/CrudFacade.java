package com.project.BL;

import com.project.BL.Base.*;
import com.project.BL.DTOs.Details.*;
import com.project.BL.DTOs.Lists.*;
import com.project.BL.Exceptions.*;
import com.project.BL.Repositories.*;
import oracle.ord.im.*;
import oracle.spatial.geometry.*;

import java.sql.*;
import java.util.*;

public class CrudFacade {

    private final Connector connector = new Connector();
    private final Installer installer = new Installer();
    private final Mapper mapper = new Mapper();

    private final GetListByIdRepository getListByIdRepository = new GetListByIdRepository();
    private final GetDetailByIdRepository getDetailByIdRepository = new GetDetailByIdRepository();
    private final GetAllRepository getAllRepository = new GetAllRepository();
    private final InitializeRepository initializeRepository = new InitializeRepository();
    private final SaveDetailRepository saveDetailRepository = new SaveDetailRepository();
    private final SaveListRepository saveListRepository = new SaveListRepository();
    private final DeleteRepository deleteRepository = new DeleteRepository();
    private final AnalyticFunctionsRepository analyticFunctionsRepository = new AnalyticFunctionsRepository();
    private final MultimediaFunctionsRepository multimediaFunctionsRepository = new MultimediaFunctionsRepository();

    /**
     * Sets connection to the database, if not called, default value is used.
     * Connection will be open and used for all upcoming calls until set again.
     *
     * @param url      server url
     * @param user     user name
     * @param password user's password
     */
    public void SetConnection(String url, String user, String password) throws SQLException {
        connector.setConnection(url, user, password);
    }

    /**
     * Initialize database: creates tables, functions, applies seed...
     */
    public void InitializeDB() throws Exception {
        installer.Install();
    }

    /**
     * Find a folder with specified id in the database.
     *
     * @param id folder id
     * @return list object of found data
     * @throws SQLException      SQL error
     * @throws NotFoundException folder was not found in database
     */
    public FolderListDTO GetFolderListById(int id) throws NotFoundException, SQLException {
        return getListByIdRepository.GetFolder(id);
    }

    /**
     * Find a geometry with specified id in the database.
     *
     * @param id geometry id
     * @return list object of found data
     * @throws SQLException      SQL error
     * @throws NotFoundException geometry was not found in database
     */
    public GeometryListDTO GetGeometryListById(int id) throws NotFoundException, SQLException {
        return getListByIdRepository.GetGeometry(id);
    }

    /**
     * Find a image with specified id in the database.
     *
     * @param id image id
     * @return list object of found data
     * @throws SQLException      SQL error
     * @throws NotFoundException image was not found in database
     */
    public ImageListDTO GetImageListById(int id) throws NotFoundException, SQLException {
        return getListByIdRepository.GetImage(id);
    }

    /**
     * Find a folder with specified id in the database.
     *
     * @param id folder id
     * @return detail object of found data
     * @throws SQLException      SQL error
     * @throws NotFoundException folder was not found in database
     */
    public FolderDetailDTO GetFolderDetailById(int id) throws NotFoundException, SQLException {
        return getDetailByIdRepository.GetFolder(id);
    }

    /**
     * Find a geometry with specified id in the database.
     *
     * @param id geometry id
     * @return detail object of found data
     * @throws SQLException      SQL error
     * @throws NotFoundException geometry was not found in database
     */
    public GeometryDetailDTO GetGeometryDetailById(int id) throws NotFoundException, SQLException {
        return getDetailByIdRepository.GetGeometry(id);
    }

    /**
     * Find a image with specified id in the database.
     *
     * @param id image id
     * @return detail object of found data
     * @throws SQLException      SQL error
     * @throws NotFoundException image was not found in database
     */
    public ImageDetailDTO GetImageDetailById(int id) throws NotFoundException, SQLException {
        return getDetailByIdRepository.GetImage(id);
    }

    /**
     * Find a OrdImage with specified id in the database for update
     *
     * @param id image id
     * @return Ord image what can be used for update in database
     * @throws SQLException      SQL error
     * @throws NotFoundException image was not found in database
     */
    public OrdImage GetOrdImageForUpdate(int id) throws NotFoundException, SQLException {
        return getListByIdRepository.SelectOrdImageForUpdate(id);
    }

    /**
     * Get all folders from the database
     *
     * @return collection of all folders
     * @throws SQLException SQL error
     */
    public Collection<FolderListDTO> GetAllFolders() throws SQLException {
        return getAllRepository.GetFolders();
    }

    /**
     * Get all geometries from the database
     *
     * @return collection of all geometries
     * @throws SQLException SQL error
     */
    public Collection<GeometryListDTO> GetAllGeometries() throws SQLException {
        return getAllRepository.GetGeometries();
    }

    /**
     * Get all images from the database
     *
     * @return collection of all images
     * @throws SQLException SQL error
     */
    public Collection<ImageListDTO> GetAllImages() throws SQLException {
        return getAllRepository.GetImages();
    }

    /**
     * Get detail version of list folder from database
     *
     * @param folderList list version of folder
     * @return detail version of folder
     * @throws SQLException      SQL error
     * @throws NotFoundException folder was not found in database
     */
    public FolderDetailDTO ToDetail(FolderListDTO folderList) throws NotFoundException, SQLException {
        return getDetailByIdRepository.GetFolder(folderList.getId());
    }

    /**
     * Get detail version of list geometry from database
     *
     * @param geometryList list version of geometry
     * @return detail version of geometry
     * @throws SQLException      SQL error
     * @throws NotFoundException geometry was not found in database
     */
    public GeometryDetailDTO ToDetail(GeometryListDTO geometryList) throws NotFoundException, SQLException {
        return getDetailByIdRepository.GetGeometry(geometryList.getId());
    }

    /**
     * Get detail version of list image from database
     *
     * @param imageList list version of image
     * @return detail version of image
     * @throws SQLException      SQL error
     * @throws NotFoundException image was not found in database
     */
    public ImageDetailDTO ToDetail(ImageListDTO imageList) throws NotFoundException, SQLException {
        return getDetailByIdRepository.GetImage(imageList.getId());
    }

    /**
     * Get List version of detail folder using mapper
     *
     * @param folderDetail detail version of folder
     * @return list version of folder
     */
    public FolderListDTO ToList(FolderDetailDTO folderDetail) {
        return mapper.MapTo(folderDetail);
    }

    /**
     * Get List version of detail geometry using mapper
     *
     * @param geometryDetail detail version of geometry
     * @return list version of geometry
     */
    public GeometryListDTO ToList(GeometryDetailDTO geometryDetail) {
        return mapper.MapTo(geometryDetail);
    }

    /**
     * Get List version of detail image using mapper
     *
     * @param imageDetail detail version of image
     * @return list version of image
     */
    public ImageListDTO ToList(ImageDetailDTO imageDetail) {
        return mapper.MapTo(imageDetail);
    }

    /**
     * Initializes folder and inserts it into database
     *
     * @param parentFolderList parent of inserted folder
     * @return initialized folder detail
     * @throws SQLException SQL error
     */
    public FolderDetailDTO InitializeFolder(FolderListDTO parentFolderList) throws SQLException {
        return initializeRepository.InitializeFolder(parentFolderList);
    }

    /**
     * Initializes geometry and inserts it into database
     *
     * @return initialized geometry detail
     * @throws SQLException SQL error
     */
    public GeometryDetailDTO InitializeGeometry() throws SQLException {
        return initializeRepository.InitializeGeometry();
    }

    /**
     * Initializes image and inserts it into database
     *
     * @param folderList folder containing initialized image
     * @return initialized image detail
     * @throws SQLException SQL error
     */
    public ImageDetailDTO InitializeImage(FolderListDTO folderList) throws SQLException {
        return initializeRepository.InitializeImage(folderList);
    }

    /**
     * Saves changes on folder to the database
     *
     * @param folderDetail saved folder detail
     * @throws SQLException SQL error
     */
    public void Save(FolderDetailDTO folderDetail) throws SQLException {
        saveDetailRepository.Save(folderDetail);
    }

    /**
     * Saves changes on geometry to the database
     *
     * @param geometryDetail saved geometry detail
     * @throws SQLException SQL error
     * @throws Exception    Parsing geometry error
     */
    public void Save(GeometryDetailDTO geometryDetail) throws Exception {
        saveDetailRepository.Save(geometryDetail);
    }

    /**
     *
     * Create rectangle from given coords and proportions
     *
     * @param geometryDetail Geometry infos
     * @param x X coord
     * @param y Y coord
     * @param w Width of rectangle
     * @param h Height of rectangle
     */
    public void SavePolygon(GeometryDetailDTO geometryDetail, double x, double y, double w, double h) throws Exception {
        JGeometry polygon = new JGeometry(
                3, 0,
                new int[]{1, 1003, 1},
                new double[]{
                        x, y,
                        x + w, y,
                        x + w, y + h,
                        x, y + h,
                        x, y
                }
        );

        geometryDetail.setGeometry(polygon);
        saveDetailRepository.Save(geometryDetail);
    }

    /**
     *
     * Create point from given coords
     *
     * @param geometryDetail Geometry infos
     * @param x X coord
     * @param y Y coord
     */
    public void SavePoint(GeometryDetailDTO geometryDetail, double x, double y) throws Exception {
        JGeometry point = new JGeometry(x, y, 0);

        geometryDetail.setGeometry(point);
        saveDetailRepository.Save(geometryDetail);
    }

    /**
     *
     * Create circle from given coords and radius
     *
     * @param geometryDetail Geometry infos
     * @param x X coord
     * @param y Y coord
     * @param radius Radius of the circle
     */
    public void SaveCircle(GeometryDetailDTO geometryDetail, double x, double y, double radius) throws Exception {
        JGeometry circle = new JGeometry(
                3, 0,
                new int[]{1, 1003, 4},
                new double[]{
                        x, y - radius,
                        x, y + radius,
                        x + radius, y
                }
        );

        geometryDetail.setGeometry(circle);
        saveDetailRepository.Save(geometryDetail);
    }

    /**
     * Saves changes on image to the database
     *
     * @param imageDetail saved image detail
     * @throws SQLException SQL error
     * @throws Exception    Parsing geometry error
     */
    public void Save(ImageDetailDTO imageDetail) throws Exception {
        saveDetailRepository.Save(imageDetail);
    }

    /**
     * Saves changes on folder to the database
     *
     * @param folderList saved folder list
     * @throws SQLException SQL error
     */
    public void Save(FolderListDTO folderList) throws SQLException {
        saveListRepository.Save(folderList);
    }

    /**
     * Saves changes on geometry to the database
     *
     * @param geometryList saved geometry list
     * @throws SQLException SQL error
     * @throws Exception    Parsing geometry error
     */
    public void Save(GeometryListDTO geometryList) throws Exception {
        saveListRepository.Save(geometryList);
    }

    /**
     * Saves changes on image to the database
     *
     * @param imageList saved image list
     * @throws SQLException SQL error
     */
    public void Save(ImageListDTO imageList) throws SQLException {
        saveListRepository.Save(imageList);
    }

    /**
     * Deletes folder from the database.
     *
     * @param folderDetail deleted folder
     * @throws SQLException      SQL error
     * @throws NotFoundException folder was not found in database
     */
    public void Delete(FolderDetailDTO folderDetail) throws SQLException, NotFoundException {
        deleteRepository.DeleteFolderById(folderDetail.getId());
    }

    /**
     * Deletes geometry from the database.
     *
     * @param geometryDetail deleted geometry
     * @throws SQLException      SQL error
     * @throws NotFoundException geometry was not found in database
     */
    public void Delete(GeometryDetailDTO geometryDetail) throws SQLException, NotFoundException {
        deleteRepository.DeleteGeometryById(geometryDetail.getId());
    }

    /**
     * Deletes image from the database.
     *
     * @param imageDetail deleted image
     * @throws SQLException      SQL error
     * @throws NotFoundException image was not found in database
     */
    public void Delete(ImageDetailDTO imageDetail) throws SQLException, NotFoundException {
        deleteRepository.DeleteImageById(imageDetail.getId());
    }

    /**
     * Deletes folder from the database.
     *
     * @param folderList deleted folder
     * @throws SQLException      SQL error
     * @throws NotFoundException folder was not found in database
     */
    public void Delete(FolderListDTO folderList) throws SQLException, NotFoundException {
        deleteRepository.DeleteFolderById(folderList.getId());
    }

    /**
     * Deletes geometry from the database.
     *
     * @param geometryList deleted geometry
     * @throws SQLException      SQL error
     * @throws NotFoundException geometry was not found in database
     */
    public void Delete(GeometryListDTO geometryList) throws SQLException, NotFoundException {
        deleteRepository.DeleteGeometryById(geometryList.getId());
    }

    /**
     * Deletes image from the database.
     *
     * @param imageList deleted image
     * @throws SQLException      SQL error
     * @throws NotFoundException image was not found in database
     */
    public void Delete(ImageListDTO imageList) throws SQLException, NotFoundException {
        deleteRepository.DeleteImageById(imageList.getId());
    }

    /**
     * Calculates distance between 2 images
     *
     * @param firtImageList   first image for calculation
     * @param secondImageList second image for calculation
     * @return distance between two images
     * @throws SQLException SQL error
     */
    public float CalculateImageDistance(ImageListDTO firtImageList, ImageListDTO secondImageList) throws SQLException {
        return analyticFunctionsRepository.CalculateImagesDistance(firtImageList, secondImageList);
    }

    /**
     * Connect points where pictures were taken
     *
     * @param folderList folder which images will be connected
     * @return connected points (line) or null if there are no point to connect
     * @throws SQLException SQL error
     */
    public JGeometry ConnectImagesPoints(FolderListDTO folderList) throws SQLException {
        return analyticFunctionsRepository.ConnectImagesPoints(folderList);
    }

    /**
     * Calculate area from convex hull of image points in folder
     *
     * @param folderList folder which images will used
     * @return calculated area
     * @throws SQLException SQL error
     */
    public Float CalculateFolderArea(FolderListDTO folderList) throws SQLException {
        return analyticFunctionsRepository.CalculateFolderArea(folderList);
    }

    /**
     * Calculate convex hull of image points in folder
     *
     * @param folderList folder which image points will used
     * @return calculated convex hull or null, if there are no points for calculation
     * @throws SQLException SQL error
     */
    public JGeometry CalculateFoldersConvexHull(FolderListDTO folderList) throws SQLException {
        return analyticFunctionsRepository.CalculateFoldersConvexHull(folderList);
    }

    /**
     * Calculate intersection of two folder image points
     *
     * @param firstFolderList  first folder for calculation
     * @param secondFolderList second folder for calculation
     * @return calculated area
     * @throws SQLException SQL error
     */
    public Float CalculateFoldersIntersection(FolderListDTO firstFolderList, FolderListDTO secondFolderList) throws SQLException {
        return analyticFunctionsRepository.CalculateFoldersIntersection(firstFolderList, secondFolderList);
    }

    /**
     * Calculate average distance between images in one folder
     *
     * @param folderList folder which image points will used
     * @return calculated average distance
     * @throws SQLException SQL error
     */
    public Float CalculateAverageDistance(FolderListDTO folderList) throws SQLException {
        return analyticFunctionsRepository.CalculateAverageDistance(folderList);
    }

    /**
     * Calculate maximal distance between images in one folder
     *
     * @param folderList folder which image points will used
     * @return calculated maximal distance
     * @throws SQLException SQL error
     */
    public Float CalculateMaxDistance(FolderListDTO folderList) throws SQLException {
        return analyticFunctionsRepository.CalculateMaxDistance(folderList);
    }

    /**
     * Calculate minimal distance between images in one folder
     *
     * @param folderList folder which image points will used
     * @return calculated minimal distance
     * @throws SQLException SQL error
     */
    public Float CalculateMinDistance(FolderListDTO folderList) throws SQLException {
        return analyticFunctionsRepository.CalculateMinDistance(folderList);
    }

    /**
     * Calculate overall distance between images in one folder
     *
     * @param folderList folder which image points will used
     * @return calculated overall distance
     * @throws SQLException SQL error
     */
    public Float CalculateOverallDistance(FolderListDTO folderList) throws SQLException {
        return analyticFunctionsRepository.CalculateOverallDistance(folderList);
    }

    /**
     * Find image which is the most similar based on several criteria
     *
     * @param imageListDTO image used for finding
     * @param weightAC     average color criteria
     * @param weightCH     color histogram criteria
     * @param weightPC     positional color criteria
     * @param weightTX     texture criteria
     * @return most similar image
     * @throws SQLException      SQL error
     * @throws NotFoundException the suitable image is not in the database
     */
    public ImageListDTO FindMostSimilarImage(ImageListDTO imageListDTO, double weightAC, double weightCH, double weightPC, double weightTX) throws SQLException, NotFoundException {
        return multimediaFunctionsRepository.FindMostSimilarImage(imageListDTO, weightAC, weightCH, weightPC, weightTX);
    }

    /**
     *
     * Rotate image for given angle
     *
     * @param imageListDTO image to be rotated
     * @param angle how many degrees to rotate
     */
    public void RotateImage(ImageListDTO imageListDTO, int angle) throws SQLException {
        multimediaFunctionsRepository.RotateImage(imageListDTO, angle);

        // TODO: Redraw image in GUI?
    }
}
