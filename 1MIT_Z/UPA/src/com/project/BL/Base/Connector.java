package com.project.BL.Base;

import oracle.jdbc.pool.*;

import java.sql.*;

public class Connector {

    public static String url = "jdbc:oracle:thin:@//gort.fit.vutbr.cz:1521/orclpdb";
    public static String user = "";
    public static String password = "";

    public static Connection connection = null;

    public Connection getConnection() throws SQLException {
        if (connection == null) setConnection(url, user, password);
        return connection;
    }

    public Connection setConnection(String url, String user, String password) throws SQLException {
        if (connection != null) connection.close();
        Connector.url = url;
        Connector.user = user;
        Connector.password = password;
        var oracleDataSource = new OracleDataSource();
        oracleDataSource.setURL(url);
        oracleDataSource.setUser(user);
        oracleDataSource.setPassword(password);
        connection = oracleDataSource.getConnection();
        connection.setAutoCommit(false);
        return connection;
    }
}
